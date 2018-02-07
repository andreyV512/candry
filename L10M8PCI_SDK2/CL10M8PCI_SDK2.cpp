#pragma hdrstop
#include "CL10M8PCI_SDK2.h"
#define BOARD_NAME "LAn10M8PCI"

// ---------------------------------------------------------------------------
CL10M8PCI_SDK2* CL10M8PCI_SDK2::Create(int _baseAddress,OnPrDef _OnPr)
{
	CL10M8PCI_SDK2* ret=new CL10M8PCI_SDK2(_baseAddress,_OnPr);
	if(ret->error)
	{
		delete ret;
		return (NULL);
	}
	return (ret);
}
// ---------------------------------------------------------------------------
CL10M8PCI_SDK2::CL10M8PCI_SDK2(int _baseAddress,OnPrDef _OnPr)
{
//	pgbuf=gbuf;
//	ibuf=0;
	OnPr=_OnPr;
	baseAddress=_baseAddress;
	error=true;
	hIRQEvent=CreateEvent(NULL,FALSE,FALSE,NULL);
	U32 st;

	RshDllInterfaceKey DEV_KEY(BOARD_NAME,(void**)&device);
	// Получаем экземляр класса устройства.
	// Client.Free();
	Client=new RshDllClient();
	st=Client->GetDeviceInterface(DEV_KEY);
	if(st!=RSH_API_SUCCESS)
	{
		Client->Free();
		pr(GetErr(st));
		return;
	}
	RSH_S8P libname;
	RSH_S8P libVersion;
	RSH_S8P libCoreName;
	RSH_S8P libCoreVersion;

	st=device->Get(RSH_GET_LIBRARY_FILENAME,&libname);
	if(st!=RSH_API_SUCCESS)
	{
		Client->Free();
		pr(GetErr(st));
		return;
	}
	pr(AnsiString("Library Name: ")+(char*)(libname.data));

	st=device->Get(RSH_GET_LIBRARY_VERSION_STR,&libVersion);
	if(st!=RSH_API_SUCCESS)
	{
		Client->Free();
		pr(GetErr(st));
		return;
	}
	pr(AnsiString("Library Version: ")+(char*)(libVersion.data));

	st=device->Get(RSH_GET_CORELIB_FILENAME,&libCoreName);
	if(st!=RSH_API_SUCCESS)
	{
		Client->Free();
		pr(GetErr(st));
		return;
	}
	pr(AnsiString("Core Library Name: ")+(char*)(libCoreName.data));

	st=device->Get(RSH_GET_CORELIB_VERSION_STR,&libCoreVersion);
	if(st!=RSH_API_SUCCESS)
	{
		Client->Free();
		pr(GetErr(st));
		return;
	}
	pr(AnsiString("Core Library Version: ")+(char*)(libCoreVersion.data));

	// ============================= ПРОВЕРКА СОВМЕСТИМОСТИ =============================
	// Проверим, поддерживает ли устройство режим работы "Старт-Стоп".
	RSH_U32 caps=RSH_CAPS_SOFT_GATHERING_IS_AVAILABLE;
	st=device->Get(RSH_GET_DEVICE_IS_CAPABLE,&caps);
	if(st!=RSH_API_SUCCESS)
	{
		Client->Free();
		pr(GetErr(st));
		return;
	}
	pr("Поддержка Старт-Стоп: Да");

	// ========================= ПОДКЛЮЧЕНИЕ К УСТРОЙСТВУ ====================================
	// Создаем ключ, по которому будет выполняться подключение (в данном случае используется базовый адрес)
	RshDeviceKey key(_baseAddress);

	// Подключаемся к устройству
	st=device->Connect(&key);
	if(st!=RSH_API_SUCCESS)
	{
		Client->Free();
		pr(GetErr(st));
		return;
	}
	pr(AnsiString("L10M8PCI: Подключено устройство: ")+_baseAddress);
	st=device->Get(RSH_GET_DEVICE_NAME_VERBOSE,&libname);
	if(st!=RSH_API_SUCCESS)
	{
		Client->Free();
		pr(GetErr(st));
		return;
	}
	pr(AnsiString("Библиотека: ")+(char*)(libname.data));
	RSH_U32 activeChanNumber;
	RSH_U32 serialNumber;

	st=device->Get(RSH_GET_DEVICE_SERIAL_NUMBER,&serialNumber);
	if(st!=RSH_API_SUCCESS)
	{
		Client->Free();
		pr(GetErr(st));
		return;
	}
	pr(AnsiString("Серийный номер: ")+(serialNumber.data));

	st=device->Get(RSH_GET_DEVICE_ACTIVE_CHANNELS_NUMBER,&activeChanNumber);
	if(st!=RSH_API_SUCCESS)
	{
		Client->Free();
		pr(GetErr(st));
		return;
	}
	pr(AnsiString("Активных каналов: ")+(activeChanNumber.data));

	RSH_U32 mem_size;
	st=device->Get(RSH_GET_DEVICE_MEMORY_SIZE,&mem_size);
	if(st!=RSH_API_SUCCESS)
	{
		Client->Free();
		pr(GetErr(st));
		return;
	}
	pr(AnsiString("Размер памяти: ")+(mem_size.data));
	RSH_BUFFER_U32 sizes;
	st=device->Get(RSH_GET_DEVICE_SIZE_LIST,&sizes);
	if(st!=RSH_API_SUCCESS)
	{
		Client->Free();
		pr(GetErr(st));
		return;
	}
	for(int i=0;i<sizes.PSize();i++)
	{
		pr(AnsiString("Размер блока может быть: ")+sizes[i]);
	}
	error=false;
}
// ---------------------------------------------------------------------------
__fastcall CL10M8PCI_SDK2::~CL10M8PCI_SDK2(void)
{
	if(error)
		return;
//	device->Stop();
//	Client->Free();
}
// ---------------------------------------------------------------------------
AnsiString CL10M8PCI_SDK2::GetErr(unsigned int _error)
{
	wchar_t ws[1024];
	if(RshError::GetErrorDescriptionUTF16(_error,ws,1024,RSH_LANGUAGE_RUSSIAN)!=RSH_API_SUCCESS)
		return ("CL10M8PCI_SDK2::GetErr: не могу получить код ошибки");
	WideString w=ws;
	AnsiString ret=w;
	return (ret);
}
// ---------------------------------------------------------------------------
void CL10M8PCI_SDK2::pr(AnsiString _msg)
{
	AnsiString a="CL10M8PCI_SDK2: ";
	a+=baseAddress;
	a+=": ";
	a+=_msg;
	if(OnPr!=NULL)
		OnPr(a);
}
// ---------------------------------------------------------------------------
bool CL10M8PCI_SDK2::initSettings(AcquireSettings* _settings)
{
	pr("*** CL10M8PCI_SDK2::initSettings");
	U32 st;
	RshInitMemory p;
	p.startType=RshInitMemory::External;
	p.frequency=_settings->quantFrequency*1.0e6;
	p.bufferSize=_settings->measureSize;
	p.packetNumber=_settings->sensorCount*_settings->strobesPerPacket;
	p.preHistory=0;
	p.channels[0].control=RshChannel::Used;
	p.channels[0].gain=_settings->ampGain;
	p.startDelay=_settings->startDelay;
	p.threshold=_settings->syncLevel;
	if(_settings->syncFront)
		p.controlSynchro|=RshInitMemory::SlopeFront;
	else
		p.controlSynchro|=RshInitMemory::SlopeDecline;
	p.channelSynchro.gain=1;
	p.channelSynchro.control=RshSynchroChannel::DC;
	st=device->Init(&p);
	if(st!=RSH_API_SUCCESS)
	{
		pr(GetErr(st));
		return (false);
	}
	RSH_U32 activeChanNumber;
	st=device->Get(RSH_GET_DEVICE_ACTIVE_CHANNELS_NUMBER,&activeChanNumber);
	if(st!=RSH_API_SUCCESS)
	{
		Client->Free();
		pr(GetErr(st));
		return (false);
	}
	pr(AnsiString("Активных каналов: ")+(activeChanNumber.data));

	pr(AnsiString("ЧАСТОТА: ")+p.frequency);
	pr(AnsiString("РАЗМЕР ПАКЕТА: ")+p.bufferSize);
	pr(AnsiString("КОЛИЧЕСТВО ПАЧЕК: ")+p.packetNumber);
	st=data.Allocate(_settings->measureSize*_settings->sensorCount*_settings->strobesPerPacket);
	if(st!=RSH_API_SUCCESS)
	{
		Client->Free();
		pr(GetErr(st));
		return (false);
	}
	pr(AnsiString("РАЗМЕР БУФЕРА ПЛАТЫ(буф): ")+data.PSize());
	pr(AnsiString("РАЗМЕР БУФЕРА ПЛАТЫ(пар): ")+p.bufferSize*p.packetNumber);
	return (true);
}
// ---------------------------------------------------------------------------
bool CL10M8PCI_SDK2::Read(char* _buf)
{
	RSH_U32 waitTime=50000;
	U32 st;
	st=device->Start();
	if(st!=RSH_API_SUCCESS)
	{
		Client->Free();
		pr(GetErr(st));
		return (false);
	}
	st=device->Get(RSH_GET_WAIT_BUFFER_READY_EVENT,&waitTime);
	if(st!=RSH_API_SUCCESS)
	{
		device->Stop();
		pr(GetErr(st));
		return (false);
	}
	device->Stop();

	char* ptr_save=data.ptr;
	data.ptr=_buf;
	st=device->GetData(&data);
/*
	int j=0;
	for(int i=0;i<data.PSize();i++)
	{
		if(ibuf==10)
		{
			*(pgbuf++)=data[i];
		}
		_buf[i]=data[i];
		if(j<20)
			_buf[i]=100;
		j++;
		if(j==484)
			j=0;
	}
	ibuf++;
*/
	data.ptr=ptr_save;

	if(st!=RSH_API_SUCCESS)
	{
		Client->Free();
		pr(GetErr(st));
		return (false);
	}
	pr("Read Ok");
	return (true);
}
// ---------------------------------------------------------------------------
int CL10M8PCI_SDK2::GetBufSize(void)
{
	return(data.PSize());
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
ADCBoards* ADCBoards::Create(int _baseAddress0,int _baseAddress1,OnPrDef _OnPr)
{
	ADCBoards* ret=new ADCBoards(_baseAddress0,_baseAddress1,_OnPr);
	if(ret->error)
	{
		delete ret;
		return (NULL);
	}
	return (ret);
}
// ---------------------------------------------------------------------------
ADCBoards::ADCBoards(int _baseAddress0,int _baseAddress1,OnPrDef _OnPr)
{
	error=false;
	board0=NULL;
	board1=NULL;
	board0=CL10M8PCI_SDK2::Create(_baseAddress0,_OnPr);
	if(board0==NULL)
	{
		error=true;
		return;
	}
	board1=CL10M8PCI_SDK2::Create(_baseAddress1,_OnPr);
	if(board1==NULL)
		error=true;
}
// ---------------------------------------------------------------------------
__fastcall ADCBoards::~ADCBoards(void)
{
	if(board0!=NULL)
		delete board0;
	if(board1!=NULL)
		delete board1;
}
// ---------------------------------------------------------------------------
bool ADCBoards::initSettings(AcquireSettings* _settings)
{
	if(!board0->initSettings(_settings))
		return(false);
	if(!board1->initSettings(_settings))
		return(false);
	return(true);
}
// ---------------------------------------------------------------------------
int ADCBoards::GetBufSize(void)
{
	return(board0->GetBufSize());
}
// ---------------------------------------------------------------------------
void ADCBoards::SetPr(OnPrDef _OnPr)
{
	board0->OnPr=_OnPr;
	board1->OnPr=_OnPr;
}
// ---------------------------------------------------------------------------
/*
void CL10M8PCI_SDK2::ToFile(FILE* df)
{
	fprintf(df,"*** CL10M8PCI_SDK2::ToFile ***\n");
	for(int i=0;i<100*3*484;i++)
	{
		char c=gbuf[i];
		int ic=c;
		fprintf(df,"[%05d]=%d\n",i,ic);
	}
	fprintf(df,"--- CL10M8PCI_SDK2::ToFile ---\n");
}
*/

