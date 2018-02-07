/*!
 * \copyright JSC "Rudnev-Shilyaev"
 *
 * \file RshEventManager.cpp
 * \date 14.04.2014
 * \version 1.0 [SDK 2.00]
 *
 * \~english
 * \brief
 * Cross platform realisation of event manager class.
 *
 * \~russian
 * \brief
 * Кроссплатформенная реализация класса для синхронизации потоков и событий.
 *
 */

#ifndef __RSHEVENTMANAGER_CPP__
#define __RSHEVENTMANAGER_CPP__

#include "RshEventManager.h"

RshEventObject::RshEventObject(U32 timeToWait, bool autoReset)
{

#if defined(__BOOST_EVENT_MODE__)
	if( timeToWait == 0 ) timeToWait = 1;
	this->eventState = RSH_EVENT_STATE_NONSIGNALED;
	this->timeToWait = timeToWait;
	this->autoReset  = autoReset;
	this->waitState  = false;
#else
        #if defined(RSH_MSWINDOWS)
                this->hEvent	 = NULL;
                this->autoReset  = autoReset;
                this->timeToWait = timeToWait;

                hEvent = CreateEvent(
                        NULL,               // default security attributes
                        !autoReset,         // manual-reset event
                        FALSE,              // initial state is nonsignaled
                        NULL  // object name
                        );

        #elif defined(RSH_LINUX)
                hEvent = neosmart::CreateEvent(!autoReset);
                neosmart::ResetEvent(hEvent); // initial state is nonsignaled
                this->timeToWait = timeToWait;
                this->autoReset  = autoReset;
        #else
                #error undefinedMode
        #endif
#endif

}

RshCriticalSectionObject::RshCriticalSectionObject()
{
#if defined(__BOOST_EVENT_MODE__)

#else
    #if defined(RSH_MSWINDOWS)
	InitializeCriticalSection(&critSection);
    #elif defined(RSH_LINUX)
        pthread_mutex_init(&critSection, NULL);
    #else
        #error undefinedMode
    #endif
#endif

}

RshCriticalSectionObject::~RshCriticalSectionObject()
{
#if defined(__BOOST_EVENT_MODE__)

#else
    #if defined(RSH_MSWINDOWS)
	DeleteCriticalSection(&critSection);
    #elif defined(RSH_LINUX)
        pthread_mutex_destroy(&critSection);
    #else
        #error undefinedMode
    #endif
#endif

}

//------------------------------------------------------------------------------
U32 RshEventObject::WaitForEvent() {

#if defined(__BOOST_EVENT_MODE__)
        boost::mutex::scoped_lock e_lock(mutex); // захватываем наш mutex для дальнейшей обработки

	if ( timeToWait == RSH_INFINITE_WAIT_TIME) // бесконечное ожидание события
	{
	waitState = true; // показываем, что mutex захвачен
	// ожидаем пока не изменится состояние события на RSH_EVENT_STATE_NONSIGNALED
	condition.wait(e_lock,
	RSH_EVENT_STATE_NONSIGNALED != boost::lambda::var(eventState));

	if( autoReset )
	eventState = RSH_EVENT_STATE_NONSIGNALED; // сбрасываем событие в несигнальное состояние

	waitState = false; // показываем, что mutex свободен
	return RSH_API_SUCCESS;
	}
#if (BOOST_VERSION <= 104900)
	boost::xtime_get(&time, boost::TIME_UTC); // set current time as start point for timed_wait
#else
	boost::xtime_get(&time, boost::TIME_UTC_); // set current time as start point for timed_wait
#endif
	time.sec  += timeToWait / 1000; // set user defined wait time
	time.nsec += timeToWait % 1000 * 1000 * 1000;


	bool st;
	waitState = true; // показываем, что mutex захвачен
	st = condition.timed_wait(e_lock,
	time,
	RSH_EVENT_STATE_NONSIGNALED != boost::lambda::var(eventState)); // wait until buffer complete event
	waitState = false; // показываем, что mutex свободен

	if ( st == true)
	{
	if( autoReset )
	eventState = RSH_EVENT_STATE_NONSIGNALED; // сбрасываем событие в несигнальное состояние

	return RSH_API_SUCCESS;
	}
	else // timeout
	return RSH_API_EVENT_WAITTIMEOUT;
#else
    #if defined(RSH_MSWINDOWS)
	if(hEvent == 0)
                return RSH_API_EVENT_NOTINITIALIZED;

	DWORD value;
	value = WaitForSingleObject(hEvent, timeToWait);
	if( value == WAIT_OBJECT_0 )
		return RSH_API_SUCCESS;
	else
	{
		if( value == WAIT_TIMEOUT )
			return RSH_API_EVENT_WAITTIMEOUT;
		else
			return RSH_API_EVENT_WAITFAILED;
	}
    #elif defined(RSH_LINUX)
        int st = neosmart::WaitForEvent(hEvent,timeToWait);
        if( st == ETIMEDOUT ) return RSH_API_EVENT_WAITTIMEOUT;

        return RSH_API_SUCCESS;
    #else
        #error undefinedMode
    #endif
#endif

}

void RshEventObject::SetSignaled()
{

#if defined(__BOOST_EVENT_MODE__)
    eventState = RSH_EVENT_STATE_SIGNALED;
    condition.notify_all();
#else
    #if defined(RSH_MSWINDOWS)
	SetEvent(hEvent);
    #elif defined(RSH_LINUX)
        neosmart::SetEvent(hEvent);
    #else
        #error undefinedMode
    #endif
#endif

}
//------------------------------------------------------------------------------
void RshEventObject::SetNonSignaled()
{

#if defined(__BOOST_EVENT_MODE__)
    eventState = RSH_EVENT_STATE_NONSIGNALED;
#else
    #if defined(RSH_MSWINDOWS)
        ResetEvent(hEvent);
    #elif defined(RSH_LINUX)
        neosmart::ResetEvent(hEvent);
    #else
        #error undefinedMode
    #endif
#endif

}
//------------------------------------------------------------------------------
void RshEventObject::WaitForMutex()
{

#if defined(__BOOST_EVENT_MODE__)
    while(waitState)
    boost::this_thread::sleep(boost::posix_time::millisec(10));
#else
    #if defined(RSH_MSWINDOWS)
        WaitForSingleObject(hEvent,INFINITE);
        CloseHandle(hEvent);
    #elif defined(RSH_LINUX)
        neosmart::WaitForEvent(hEvent,RSH_INFINITE_WAIT_TIME);
        neosmart::DestroyEvent(hEvent);
    #else
        #error undefinedMode
    #endif
#endif

}
//==============================================================================
RshEventManager::RshEventManager() {

}
//------------------------------------------------------------------------------
RshEventManager::~RshEventManager() {

	DeInitializeResources();
}
//------------------------------------------------------------------------------
void RshEventManager::DeInitializeResources()
 {
 //#ifdef RSH_MSWINDOWS
 	std::map<U32, RshEventObject*>::iterator it;
	while (!userEvents.empty()) {
		it = userEvents.begin();
		it->second->SetSignaled();
		it->second->WaitForMutex();
        delete (RshEventObject*)it->second;
        userEvents.erase(it);
	}
//#endif
// #TODO in Linux we have issues with above code; won't use it till better times
}
//------------------------------------------------------------------------------
U32 RshEventManager::InitializeEvent(U32 eventCode, bool autoReset) {


	if ( userEvents.find( eventCode ) == userEvents.end() ) // not found
	{
		userEvents[eventCode] = new RshEventObject(RSH_INFINITE_WAIT_TIME,autoReset);
	}
	else  // found
	{
		SetEvent(eventCode);
		userEvents[eventCode]->autoReset =  autoReset;
		userEvents[eventCode]->timeToWait = RSH_INFINITE_WAIT_TIME;
	}

	return RSH_API_SUCCESS;
}
//------------------------------------------------------------------------------
U32 RshEventManager::SetEvent(U32 eventCode) {

    if( userEvents.find(eventCode) != userEvents.end())
    {
       userEvents[eventCode]->SetSignaled();
       return RSH_API_SUCCESS;
    }

    return RSH_API_OBJECT_NOTFOUND;
}

//------------------------------------------------------------------------------
U32 RshEventManager::ResetEvent(U32 eventCode) {

    if( userEvents.find(eventCode) != userEvents.end())
    {
        userEvents[eventCode]->SetNonSignaled();
        return RSH_API_SUCCESS;
	}

    return RSH_API_OBJECT_NOTFOUND;
}
//------------------------------------------------------------------------------
U32 RshEventManager::WaitForEvent(U32 eventCode, U32 timeToWait) {

    if( userEvents.find(eventCode) == userEvents.end())
        return RSH_API_OBJECT_NOTFOUND;

	userEvents[eventCode]->timeToWait = timeToWait;
	return  userEvents[eventCode]->WaitForEvent();
}
//------------------------------------------------------------------------------
U32 RshEventManager::InitializeCriticalSection(U32 sectionCode)
{
	if ( userCriticalSections.find( sectionCode ) == userCriticalSections.end() ) // not found
	{
		userCriticalSections[sectionCode] = new RshCriticalSectionObject();
	}
	else  // found
	{
		//do nothing
	}

	return RSH_API_SUCCESS;
}
//------------------------------------------------------------------------------
U32 RshEventManager::EnterCriticalSection(U32 sectionCode)
{
#if defined(__BOOST_CRITSECTION_MODE__)
    userCriticalSections[sectionCode]->critSection.lock();
#else
    #if defined(RSH_MSWINDOWS)
        ::EnterCriticalSection(&userCriticalSections[sectionCode]->critSection);
    #elif defined(RSH_LINUX)
        pthread_mutex_lock(&userCriticalSections[sectionCode]->critSection);
    #else
        #error undefinedMode
    #endif
#endif
    return RSH_API_SUCCESS;
}
//------------------------------------------------------------------------------
U32 RshEventManager::TryEnterCriticalSection(U32 sectionCode)
{

#if defined(__BOOST_CRITSECTION_MODE__)
    bool res = userCriticalSections[sectionCode]->critSection.try_lock();
    if(!res)
      return RSH_API_CRITSECTION_ALREADY_OWNED;
#else
    #if defined(RSH_MSWINDOWS)
        int res = ::TryEnterCriticalSection(&userCriticalSections[sectionCode]->critSection);
        if(res == 0)//section is owned by some else thread
                return RSH_API_CRITSECTION_ALREADY_OWNED;
    #elif defined(RSH_LINUX)
        int res = pthread_mutex_trylock(&userCriticalSections[sectionCode]->critSection);
        if( res == 0 ) return RSH_API_SUCCESS;
        else
                return RSH_API_CRITSECTION_ALREADY_OWNED;
    #else
        #error undefinedMode
    #endif
#endif

    return RSH_API_SUCCESS;
}
//------------------------------------------------------------------------------
U32 RshEventManager::LeaveCriticalSection(U32 sectionCode)
{

#if defined(__BOOST_CRITSECTION_MODE__)
    userCriticalSections[sectionCode]->critSection.unlock();
#else
    #if defined(RSH_MSWINDOWS)
        ::LeaveCriticalSection(&userCriticalSections[sectionCode]->critSection);
    #elif defined(RSH_LINUX)
        pthread_mutex_unlock(&userCriticalSections[sectionCode]->critSection);
    #else
        #error undefinedMode
    #endif
#endif

    return RSH_API_SUCCESS;
}

#if defined(__PEVENT_MODE__)
#include "pevents.cpp"
#endif

#endif



