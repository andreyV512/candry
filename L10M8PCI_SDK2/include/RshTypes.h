/*!
* \copyright JSC "Rudnev-Shilyaev"
*
* \file RshTypes.h
* \date 11.04.2014
* \version 1.0 [SDK 2.00]
*
* \~english
* \brief
* This file includes RSH SDK types and definitions.
*
* \~russian
* \brief
* В этом файле содержатся определения типов данных, используемых в RSH SDK.
*
*/

#ifndef __RSH_TYPES_H__
#define __RSH_TYPES_H__

#include "Rsh.h"
#include "RshDefChk.h"
#include "RshStatusCodes.h"
#include "RshConsts.h"

#include <limits>
#include <math.h>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <sstream>

/********************************************\
*              Definitions                   *
\********************************************/
#if defined(RSH_MSWINDOWS)

#define  NOMINMAX // fix problem with QDataTime -> windows.h in Qt; must be defined before include of windows.h

#include "windows.h"
#include <process.h>

#define __RSHCALLCONV       __stdcall
#define __rshisfinite(a)    _finite(a)
#define __rshmssleep(a)     Sleep(a)

#ifdef __BORLANDC__
	#define __rshgetpid() getpid()
#else
	#define __rshgetpid() _getpid()
#endif

#elif defined(RSH_LINUX)

#include <cstdarg>
#include <float.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>
#include <linux/types.h>    // Linux types
#include <linux/stddef.h>	// size_t, ptrdiff_t
#include <sys/time.h>

#define __RSHCALLCONV
#define __rshisfinite(a)     isfinite(a)
#define __rshmssleep(a)      usleep(a * 1000)
#define __rshgetpid()        getpid()
#endif

#if !defined(PLX_LINUX) && !defined(PLX_MSWINDOWS)
#if defined(RSH_LINUX)
typedef __s8 S8;
typedef __u8 U8;
typedef __s16 S16;
typedef __u16 U16;
typedef __s32 S32;
typedef __u32 U32;
typedef __s64 S64;
typedef __u64 U64;
#elif defined(RSH_MSWINDOWS)
typedef signed char S8;
typedef unsigned char U8;
typedef signed short S16;
typedef unsigned short U16;
typedef signed long S32;
typedef unsigned long U32;
typedef signed __int64 S64;
typedef unsigned __int64 U64;
#endif
#endif

#define	IN   // input parameter marker
#define OUT  // output parameter marker

#if !defined(VOID)
typedef void VOID;
#endif

#if !defined(NULL)
#define NULL              ((VOID *) 0x0)
#endif


/**********************************************\
*                 Types                      *
\**********************************************/
#pragma pack(push, 8)	// 8 byte alignment as default
//Data types

/*!
* 
* \~english
* \brief
* Base class for all other SDK classes.
* 
* All SDK classes, data types and data
* structures are deriverd from this one.\n
* Every deriverd class has its own ID
* (stored in _type field) and can be
* identified. It allows easily implement 
* polymophic behaviour for classes and methods.
* 
* \~russian
* \brief
* Базовый класс для всех классов SDK.
* 
* Все остальные классы, структуры и типы данных
* SDK унаследованы от этого базового класса.\n
* Каждый класс-потомок имеет свой уникальный 
* идентификатор (хранящийся в поле _type), по которому
* можно определить тип данных. Использование
* базового класса позволяет легко реализовать полиморфизм
* для методов и классов.
* 
*/
struct RshBaseType {

	/*!
	* 
	* \~english
	* \brief
	* Size of the data structure or class.
	* 
	* Size in bytes of data structure or class.
	* 
	* \warning
	* Size of complex structures depends on
	* used <b>#pragma pack</b> preprocessor definition.
	* If you change this in your source code,
	* compilled libraries may be not compatible with
	* your program, because most data structure transfer
	* method check this field to determine is structure 
	* corrupted or not.\n
	* Do not change this field manually on any derived class!
	* Each derived class implements its size calculation in
	* constructor.
	* 
	* \~russian
	* \brief
	* Размер структуры данных или класса.
	* 
	* Размер в байтах структуры данных или класса.
	* 
	* \warning
	* Размер структур зависит от используемой
	* директивы препроцессора <b>#pragma pack</b>.
	* Если изменить эту директиву в своем коде,
	* скомпилированные ранее
	* библиотеки могут стать несовместимыми с вашей
	* программой, т.к. все методы, в которых передаются
	* структуры данных, используют это поле для проверки 
	* переданной структуры.\n
	* Не меняйте это поле вручную в созданных объектах
	* унаследованных классов!
	* Каждый создаваемый объект автоматически заполняет это поле
	* в конструкторе.
	*
	*/
	size_t _typeSize;

	/*!
	* 
	* \~english
	* \brief
	* Data type ID.
	* 
	* One of the RshDataTypes enum constants.
	* This field (and _typeSize field too) is
	* used to determine data type of passed data
	* structure.
	* 
	* \warning
	* Do not change this field manually on any derived class!
	* Each derived class implements its size calculation in
	* constructor.
	* 
	* \~russian
	* \brief
	* Идентификатор типа данных.
	* 
	* Одна из констант перечисления RshDataTypes.
	* Это поле (наряду с _typeSize) исползуется
	* для определения типа данных переданного объекта
	* в различных методах SDK.
	* 
	* \warning
	* Не меняйте это поле вручную в созданных объектах
	* унаследованных классов!
	* Каждый создаваемый объект автоматически заполняет это поле
	* в конструкторе.
	* 
	*/
	RshDataTypes _type;

	//!Constructor
	RshBaseType();

	/*!
	* 
	* \~english
	* \brief
	* Get type name as string literal
	* 
	* \returns
	* Pointer to const char* string with type name.
	* 
	* This method can be used to get type name as 
	* constant char* string. It can be useful for debug
	* or log tasks.
	*
	* \~russian
	* \brief
	* Получение названия типа данных.
	* 
	* \returns
	* Указатель на строку с названием в виде const char*.
	* 
	* Этот метод может быть использован для получения
	* названия типа данных в виде строки (const char*),
	* что может быть удобно для вывода отладочных сообщений
	* или записи в логи.
	* \todo
	* Дефолтное имя типа (в скрипте генерации хедеров)
	* 
	*/
	const char* GetTypeName();	
};


/*!
* 
* \~english
* \brief
* Simple type template class.
* 
* This template is used to simlify
* construction of simple data types (such as int,
* float and so on) to be derived from RshBaseType.\n
* Constucted classes (such as ::RSH_DOUBLE or ::RSH_U32)
* contain one data field of type, specified in template.
* 
* \remarks
* Usually, classes constructed with this template
* are used as data container to pass data in
* polymorphic methods such as IRshDevice::Get().
*
* \see
* RshBaseType
* 
* \~russian
* \brief
* Шаблонный класс для простых типов данных 
* 
* Данный шаблон используется для создания 
* типов данных (как правило, базовых типов вроде int
* или double) унаследованных от RshBaseType.\n
* Сконструированные с помощью этого шаблона классы
* (такие как ::RSH_DOUBLE или ::RSH_U32) имеют
* одно поле данных нужного (базового) типа, 
* определенного в шаблоне. Идентификатор типа данных
* тоже задается через параметр шаблона.
* 
* \remarks
* Как правило, созданные с помощью этого шаблона
* классы используются как контейнер для данных
* при передаче параметров в полиморфные методы,
* такие как IRshDevice::Get().
* 
* \see
* RshBaseType
* 
*/
template <class T, RshDataTypes dataCode>
struct RshType : public RshBaseType {
	T data;

	RshType();	

	RshType(T value);

	RshType(RshType const& obj);

	RshType& operator=(RshType & obj);

	RshType& operator=(T value);
};


/*!
* 
* \~english
* \brief
* Template class for buffer construction.
* 
* This template is used to construct
* data buffer container classes derived from RshBaseType.\n
* Also contains some useful methods to work with data,
* such as sorting and saving to file.
* 
* \remarks
* Data buffers created are used to pass
* data to polymorfic methods of SDK, such as
* IRshDevice::GetData() and so on.
* 
* \see
* RshBaseType
* 
* \~russian
* \brief
* Шаблонный класс для конструирования буферов данных.
* 
* Данный шаблон используется для создания
* классов-контейнеров (массивов данных), унаследованных
* от RshBaseType.\n
* Также содержит полезные методы для работы
* с массивами данных - сортировка, запись в файл и др.
* 
* \remarks
* Созданные буферы данных использутся, как правило,
* для передачи массивов данных при вызове полиморфных
* методов SDK, таких как IRshDevice::GetData() и др.
* 
* \see
* RshBaseType
* 
*/
template <class T, RshDataTypes dataCode>
class RshBufferType : public RshBaseType {

public:
	/*!
	* 
	* \~english
	* \brief
	* Direct pointer to allocated memory.
	* 
	* One can use this pointer to access to
	* buffer data (there is also overloaded []
	* operator, but this method is faster - 
	* for long buffers its preferable).
	* 
	* \warning
	* Do not delete or change this pointer!
	* All operations with memory allocation
	* and release must be performed using class
	* methods. This pointer is set as public
	* only because performance issuses.\n
	* Also, always refer to RshBufferType::PSize() when indexing.
	* 
	* \~russian
	* \brief
	* Указатель на выделнную для данных память.
	* 
	* Можно использовать данный указатель для доступа
	* к данным, хранящимся в буфере (также, есть
	* перегруженный оператор [], но прямой доступ быстрее -
	* для больших массивов данных индексирование лучше
	* производить, используя указатель ptr).
	* 
	* \warning
	* Используйте данных указатель только для доступа к данным -
	* попытки выделить или удалить память или поменять адрес,
	* на который ссылается указатель могут быть фатальными.
	* Для управления памятью используйте методы класса.
	* Данный указатель имеет публичный доступ только из соображений
	* производительности и удобства.\n
	* Также, всегда используйте RshBufferType::PSize() в качестве референса
	* при индексировании.
	* 
	*/
	T* ptr;

	//! Constructor
	RshBufferType(size_t bufferSize = RSH_MAX_LIST_SIZE);
	
	//! Copy constructor
	RshBufferType(const RshBufferType<T, dataCode>& obj);
	
	/*!
	* 
	* \~english
	* \brief
	* Allocate memory for buffer data
	* 
	* \param[in] bufferSize
	* Desired size of buffer (elements number).
	* If system will fail to allocate memory,
	* error code will be returned and RshBufferType::PSize()
	* field will be equal to zero.
	* 
	* \returns
	* ::RSH_API_SUCCESS or error code.
	* 
	* Use this method to set buffer size you need and 
	* allocate memory for data. After successfull
	* call of this method RshBufferType::PSize() will be equal
	* to buffer size (number of elements), and
	* RshBufferType::ptr will point to allocated memory.
	* 
	* \remarks
	* If buffer size before call of this method was
	* different from bufferSize parameter passes,
	* old memory block will be delited, and all data
	* will be lost.
	* 
	* \~russian
	* \brief
	* Выделение памяти для данных буфера
	* 
	* \param[in] bufferSize
	* Желаемый размер буфера (количество элементов).
	* Если система не сможет выделить необходимый
	* размер памяти, будет возвращен соответствующий код
	* ошибки, а значение поля RshBufferType::PSize()
	* станет равным нулю.
	* 
	* \returns
	* ::RSH_API_SUCCESS или код ошибки.
	* 
	* Используйте этот метод для установки
	* необходимого размера буфера и выделения
	* памяти для данных. После успешного
	* вызова данного метода в поле RshBufferType::PSize()
	* будет содержаться размер (количество элементов) буфера,
	* а указатель RshBufferType::ptr будет
	* указывать на выделенный блок памяти.
	* 
	* \remarks
	* Если метод вызывается для буфера, который уже содержит
	* какие-то данные, и новый размер буфера отличается
	* от старого, старый блок памяти будет удален, и все
	* данные потеряны.
	* 
	*/
	U32 Allocate(size_t bufferSize = RSH_MAX_LIST_SIZE);	

	/*!
	* 
	* \~english
	* \brief
	* Copy data to buffer from memory location
	* 
	* \param[in] iptr
	* Pointer to memory block from where data
	* will be copied to buffer.
	* 
	* \param[in] psize
	* Physical size (in samples) of memory block
	* pointed by <b>iptr</b> parameter.
	* 
	* \param[in] size
	* How many samples need to be copied
	* from <b>iptr</b> memory location.\n
	* If zero, maximum avaliable samples
	* will be copied.
	* 
	* Use this method to fill buffer with data.
	* If allocated buffer size is less than size,
	* only part of data will be copied to buffer.
	* Always check RshBufferType::Size() after operation
	* complition to check how many elements buffer contain.
	* 
	* \~russian
	* \brief
	* Копирование данных в буфер из указанной области памяти
	* 
	* \param[in] iptr
	* Указатель на блок памяти, из которого
	* будут скопированы данные в буфер.
	* 
	* \param[in] psize
	* Размер (в отсчетах) блока памяти, на который
	* ссылается указатель, переданный в параметре <b>iptr</b>.\n
	* Если, например, копируются значения из массива, то 
	* размер psize должен быть равен размеру массива.
	* 
	* \param[in] size
	* Сколько всего элементов нужно скопировать в буфер из
	* области памяти, на которую указывает <b>iptr</b>\n
	* Если передать в качестве этого параметра ноль (дефолтное значение),
	* будет скопировано максимально возможное количество элементов.
	*
	* Этот метод можно использовать для заполнения буфера данными.
	* Если размер буфера (RshBufferType::PSize() ) меньше, чем заданное
	* для копирования количество элементов, только часть данных
	* будет скопирована в буфер. Всегда проверяйте состояние поля
	* RshBufferType::Size() после выполнения операции копирования,
	* чтобы определить актуальное количество элементов в буфере.
	* 	
	* \todo Возможно, следует исправить неоднозначность установки параметра this->size
	* при копировании из "пустого" буфера (т.е.  size = 0)
	*/
	void Copy(T* iptr, size_t psize, size_t size = 0);

	/*!
	* 
	* \~english
	* \brief
	* Copy data from buffer to memory location.
	* 
	* \param[in, out] iptr
	* Address of memory block where data 
	* from buffer will be placed.
	* 
	* \param[in] iptrSize
	* Maximum size (in samples) of memory
	* available for data.
	* 
	* \returns
	* Number of buffer elements copied to destination memory.
	* 
	* Copy data block from buffer to memory
	* block with address specified in <b>iptr</b> parameter.\n
	* If buffer size is smaller than <b>iptrSize</b>,
	* only RshBufferType::PSize() elements will be copied.
	*
	* \remarks
	* This method always try to copy maximum elements
	* availiable. If you need copy only, say, ten
	* elements to destination memory, specify
	* parameter iptrSize=10.
	* 
	* \~russian
	* \brief
	* Копирование данных из буфера в область памяти.
	* 
	* \param[in, out] iptr
	* Адрес (указатель на блок памяти),
	* куда нужно поместить данные из буфера.
	* 
	* \param[in] iptrSize
	* Максимальный размер (в отсчетах) блока памяти,
	* доступный для копирования.
	* 
	* \returns
	* Количество скопированных элементов буфера.
	* 
	* Данный метод копирует данные из буфера в заданную
	* параметром <b>iptr</b> область памяти.\n
	* Если размер буфера меньше, чем запрашиваемый 
	* размер (<b>iptrSize</b>), будет скопировано
	* только RshBufferType::PSize() элементов буфера.
	* 
	* \remarks
	* Данный метод всегда пытается скопировать максимально
	* возможное количество элементов из буфера. Если нужно,
	* к примеру, взять первые десять элементов, можно задать
	* параметр iptrSize=10.
	*
	*/
	size_t CopyTo(T* iptr, size_t iptrSize);

	/*!
	* 
	* \~english
	* \brief
	* Copy data from another buffer of the same type
	* 
	* \param[in] srcBuffer
	* Buffer of the same type from which
	* data will be copied to caller buffer.
	* 
	* Using this method one can fill buffer
	* with data from another buffer.
	* 
	* \remarks
	* No memory allocation is performed!
	* If destination buffer psize is smaller
	* than source buffer size, only
	* psize first elements will be copied.\n
	* If you want create identical buffer,
	* use copy constructor (dstBuffer = srcBuffer).
	* 
	* \~russian
	* \brief
	* Копирование данных из другого буфера того же типа
	* 
	* \param[in] srcBuffer
	* Буфер того же самого типа, что и вызывающий 
	* данный метод буфер.
	* 
	* Используя данный метод, можно заполнить
	* буфер данными из другого буфера.
	* 
	* \remarks
	* При вызове этого метода не производится
	* выделения памяти! Если буфер имеет 
	* размер (psize) меньший, чем количество 
	* элементов (size) в другом буфере, будут скопированы
	* только первые psize элементов.\n
	* Если требуется создать точную копию буфера, можно
	* использовать конструктор копий (dstBuffer = srcBuffer).
	* 	
	*/
	void Copy(RshBufferType<T, dataCode> const& srcBuffer);

	/*!
	* 
	* \~english
	* \brief
	* Sort buffer elements in assending order.
	* 
	* \returns
	* ::RSH_API_SUCCESS or error code.
	* 
	* This method use quick sort algorithm
	* to sort elements in assending orfer.
	*
	* \~russian
	* \brief
	* Сортировка буфера по возрастанию.
	* 
	* \returns
	* ::RSH_API_SUCCESS или код ошибки.
	* 
	* Данный метод использует алгоритм
	* быстрой сортировки для сортировки
	* элементов буфера в порядке возрастания.
	* 
	*/
	U32 SortAscending();

	/*!
	* 
	* \~english
	* \brief
	* Sort buffer elements in descending order.
	* 
	* \returns
	* ::RSH_API_SUCCESS or error code.
	* 
	* This method use quick sort algorithm
	* to sort elements in descending orfer.
	*
	* \~russian
	* \brief
	* Сортировка буфера по убыванию.
	* 
	* \returns
	* ::RSH_API_SUCCESS или код ошибки.
	* 
	* Данный метод использует алгоритм
	* быстрой сортировки для сортировки
	* элементов буфера в порядке убывания.
	* 
	*/
	U32 SortDescending();
		
	/*!
	* 
	* \~english
	* \brief
	* Print buffer values in standart output
	* 
	* \param[in] size
	* Number of elements to print. If zero (default) value
	* is passed, all available elements will be printed.
	* 
	* Output buffer content to console. If <b>size</b>
	* parameters is specified, only first size elements will be printed.
	* Otherwise, RshBufferType::Size() elements will be printed
	* (for initialized buffer with size>0). If RshBufferType::Size() is equal
	* to zero too, all data (RshBufferType::PSize()) will be printed.
	* 
	* \~russian
	* \brief
	* Печать содержимого буфер в стандартный поток вывода.
	* 
	* \param[in] size
	* Количество элементов, которые нужно напечатать. Если это число
	* равно нулю (дефолт), будут напечатаны все доступные элементы.
	* 
	* Вывод содержимого буфера на консоль. Если задан параметр
	* <b>size</b>, только первые size элементов буфера будут
	* напечатаны. Иначе, будет выведено RshBufferType::Size()
	* элементов (для буферов с RshBufferType::size>0). Если
	* RshBufferType::Size() тоже равен нулю, будут напечатаны все
	* данные буфера (RshBufferType::PSize()).
	*
	*/	
	void WriteDataToConsole(unsigned size=0);	

	/*!
	* 
	* \~english
	* \brief
	* Save buffer content to file
	* 
	* \param[in] fileName
	* Path to file where buffer must be written.
	* 
	* \returns
	* ::RSH_API_SUCCESS or error code.
	* 
	* Using this method, one can write RshBufferType::Size()
	* elements to file specified by <b>fileName</b>.
	* 
	* \~russian
	* \brief
	* Запись содержимого буфера в файл (актуальный размер)
	* 
	* \param[in] fileName
	* Путь к файлу, в который нужно записать данные.
	* 
	* \returns
	* ::RSH_API_SUCCESS или код ошибки.
	* 
	* Используя данный метод, можно записать RshBufferType::Size()
	* элементов буфера в файл, заданный с помощью параметра <b>fileName</b>.
	* 
	*/
	U32 WriteDataToFile(const wchar_t* fileName);

	/*!
	* 
	* \~english
	* \brief
	* Save all buffer content to file
	* 
	* \param[in] fileName
	* Path to file where buffer must be written.
	* 
	* \returns
	* ::RSH_API_SUCCESS or error code.
	* 
	* Using this method, one can write RshBufferType::PSize()
	* elements to file specified by <b>fileName</b>.
	* 
	* \~russian
	* \brief
	* Запись содержимого буфера в файл (вся выделенная память)
	* 
	* \param[in] fileName
	* Путь к файлу, в который нужно записать данные.
	* 
	* \returns
	* ::RSH_API_SUCCESS или код ошибки.
	* 
	* Используя данный метод, можно записать RshBufferType::PSize()
	* элементов буфера в файл, заданный с помощью параметра <b>fileName</b>.
	* 
	*/
	U32 WriteBufferToFile(const wchar_t* fileName);

	/*!
	* 
	* \~english
	* \brief
	* Fill buffer with data from file
	* 
	* \param[in] fileName
	* Path to the file which will be used
	* as data source to fill the buffer.
	* 
	* \returns
	* ::RSH_API_SUCCESS or error code.
	* 
	* Using this method, one can fill buffer
	* with data stored in binary file. 
	* 
	* \remarks
	* Memory will be allocated automaticaly to 
	* store all file content in buffer. If not empty
	* buffer is used for this operation, its content will
	* be lost.
	* 
	* \~russian
	* \brief
	* Заполнение буфера данными из файла
	* 
	* \param[in] fileName
	* Путь к файлу, который будет использован
	* в качестве источника данных для заполнения буфера.
	* 
	* \returns
	* ::RSH_API_SUCCESS или код ошибки.
	* 
	* Используя данный метод, можно прочитать данные
	* из указанного файла и заполнить ими буфер.
	* 
	* \remarks
	* Память под данные выделяется автоматически. Всегда читаются
	* все данные, которые содержатся в файле. Если в буфере
	* содержались какие-либо данные, они будут утеряны
	* после выполнения данного метода.
	* 
	*/
	U32 ReadFromFile(const wchar_t* fileName);

	 //! Destructror
	~RshBufferType();

	/*!
	* 
	* \~english
	* \brief
	* Get actual buffer data size.
	* 
	* \returns
	* Size of actual data in buffer in bytes.
	* 
	* Using this method one can obtain size
	* of actual data (RshBufferType::Size() elements)
	* in bytes.
	* 
	* \~russian
	* \brief
	* Получение размера актуальной части данных в буфере.
	* 
	* \returns
	* Размер актуальной части данных в байтах.
	* 
	* Используя данный метод, можно получить размер 
	* актуальной части буфера (RshBufferType::Size() элементов)
	* в байтах.
	* 
	*/
	size_t ByteSize();

	/*!
	* 
	* \~english
	* \brief
	* Get real buffer size in bytes.
	* 
	* \returns
	* Physical size of a buffer in bytes.
	* 
	* Using this method one can obtain physical size
	* of a buffer (RshBufferType::BytePSize() elements)
	* in bytes.
	* 
	* \~russian
	* \brief
	* Получение реального размера буфера в байтах. 
	* 
	* \returns
	* Физический размер буфера в байтах.
	* 
	* Используя данный метод, можно получить физический  
	* размер буфера (RshBufferType::BytePSize элементов)
	* в байтах.
	* 
	*/
	size_t BytePSize();

	/*!
	* 
	* \~english
	* \brief
	* Get buffer element size.
	* 
	* \returns
	* Size of buffer element in bytes.
	* 
	* Using this method one can obtain size
	* of buffer element in bytes.
	* 
	* \~russian
	* \brief
	* Получение размера элемента в буфере
	* 
	* \returns
	* Размер элеметна буфера в байтах
	* 
	* Используя данный метод, можно получить размер 
	* элемента буфера в байтах.
	* 
	*/
	size_t TypeSize();

	/*!
	* 
	* \~english
	* \brief
	* Get actual buffer size in data elements.
	* 
	* \returns
	* Size of actual data in buffer in elements.
	* 
	* Using this method one can obtain size
	* of actual data ( field RshBufferType::size )
	* in elements.
	* 
	* \~russian
	* \brief
	* Количество элементов в буфере (актуальный размер).
	* 
	* \returns
	* Размер актуальной части буфера данных в элементах.
	* 
	* Используя данный метод, можно получить размер 
	* актуальной части буфера ( поле RshBufferType::size)
	* в элементах.
	* 
	*/
	size_t Size();

	/*!
	* 
	* \~english
	* \brief
	* Get real buffer size in elements.
	* 
	* \returns
	* Physical size of a buffer in elements.
	* 
	* Using this method one can obtain physical size
	* of a buffer (RshBufferType::psize field)
	* in elements.
	* 
	* \~russian
	* \brief
	* Получение реального размера буфера в элементах. 
	* 
	* \returns
	* Физический размер буфера в элементах.
	* 
	* Используя данный метод, можно получить физический  
	* размер буфера (поле RshBufferType::psize)
	* в элементах.
	* 
	*/
	size_t PSize();

	T& operator[](size_t i);

	/*!
	* 
	* \~english
	* \brief
	* Set actual buffer size to zero value.
	* 
	* \remarks
	* Good to use before an invoke of IRshDevice::GetData().
	*
	* \~russian
	* \brief
	* Присваивает полю RshBufferType::size нулевое значение.
	* 
	* \remarks
	* Рекомендуется использовать этот метод перед вызовом
	* IRshDevice::GetData().
	* 
	*/
	void Clear();

	/// \cond SHOW_ALL_CLASSES
	void SetSize(size_t size);
	/// \endcond

	RshBufferType<T, dataCode>&	operator=(RshBufferType<T, dataCode>& obj);

	bool operator==(RshBufferType<T, dataCode>& obj);

	bool operator!=(RshBufferType<T, dataCode>& obj);

	/*!
	* 
	* \~english
	* \brief
	* Add new element to buffer.
	* 
	* Adds new element to buffer end and increase its
	* size by one. If there is no more space left in buffer
	* (see RshBufferType::PSize()), error code will be returned
	*
	* \returns
	* ::RSH_API_SUCCESS or error code
	*
	* \param[in] obj
	* Value to push in buffer.
	*
	* \~russian
	* \brief
	* Добавление элемента в буфер.
	*
	* Данный метод добавляет элемент в конец буфера и увеличивает
	* его размер. Если в буфере не осталось места (см. RshBufferType::PSize()),
	* будет возвращен код ошибки.
	* 
	*
	* \returns
	* ::RSH_API_SUCCESS или код ошибки
	*
	* \param[in] obj
	* Значение, которое нужно поместить в буфер
	* 
	*/
	U32 PushBack(T obj);

private:
	/*!
	* 
	* \~english
	* \brief
	* Valid data samples in buffer.
	* 
	* This field determine, how many samples
	* are actually copied (or initialized) in buffer
	* data. For example, if buffer with size <b>psize</b>=1024
	* passed to IRshDevice::GetData() method, and GetData
	* have only 512 samples to store in buffer, after method call
	* <b>size</b> will be equal to 512.
	* 
	* \remarks
	* Good practice is to set buffer size to zero before any
	* method call, and check if this parameter changed after.
	* If not, no data is placed in buffer and some error occured
	* in method.
	* 
	* \~russian
	* \brief
	* Актуальное количество элеменов в буфере.
	* 
	* Данное поле определяет, какое количество данных
	* было фактически помещено в буфер. Например, если 
	* буфер с размером <b>psize</b>=1024 , был 
	* передан в метод IRshDevice::GetData(), и в него 
	* было помещено 512 элементов, после вызова метода
	* значение <b>size</b> будет равно 512.
	* 
	* \remarks
	* Хорошая практика - установить значение size равным нулю
	* перед вызовом метода, который должен заполнить буфер, 
	* и проверить это значение после. Если значение не изменилось,
	* значит произошла какая-либо ошибка при обработке  и заполнении
	* буфера.
	* 
	*/
	size_t size;

	/*!
	* 
	* \~english
	* \brief
	* Physical size of buffer in samples.
	* 
	* This field determine, how many samples
	* can buffer contain. Usually, this size
	* is defined in buffer constructor or
	* by call to RshBufferType::Allocate() method.
	* 
	* \warning 
	* Newer change this field manually! It can lead
	* memory leaks and undetermined behaviour.
	*
	* \~russian
	* \brief
	* Физический размер (выделенная память) буфера в элементах.
	* 
	* Данное поле определяет максимальное количество элементов,
	* которые могут быть помещены в буфер. Обычно, этот
	* размер задается в конструкторе, или при вызове
	* метода RshBufferType::Allocate().
	* 
	* \warning
	* Никогда не меняйте это значение вручную! Это может 
	* привести к утечкам памяти или сбоям в работе программ
	* и библиотек.
	* 
	*/
	size_t psize;

	void QuickSortAscending(S64 left, S64 right);

	void QuickSortDescending(S64 left, S64 right);

	U32 WriteToFile(const wchar_t* fileName, U32 dataSize);


//=== forRSH cycle support code block ====
	/* \todo Actually implement this iteration feature	
	T* it; // iterator

	size_t GetCurrentIndex() // index of an element pointed by iterator
	{
	if ((it - ptr) == psize) return 0;
	return (it - ptr);
	}

	bool SetItToTheFirstElement() {
	it = ptr;
	if (ptr == 0 || psize == 0) return false;
	else
	return true;
	}

	bool SetItToTheNextElement() {
	if ((it - ptr) != psize) {
	++it;
	}

	return (it - ptr) != psize;
	}
	*/
};

//----------------------------------------------------------------------------------------------------
/// \cond SHOW_ALL_CLASSES
//! RSH 2 - Dimensional Buffer Type
template <class T, RshDataTypes dataCode>
class RshDBufferType : public RshBaseType {
	size_t size; // stored data units in buffer; usually used in GetData() to inform user about actual number of elements been copied
	size_t psize; // buffer physical size;
	RshBufferType<T, dataCode>* ptr; // buffer pointer

	RshDBufferType(size_t bufferSize = RSH_MAX_LIST_SIZE);

	void Allocate(size_t bufferSize);

	void Copy(RshBufferType<T, dataCode>* iptr, size_t size);

	void Copy(RshDBufferType<T, dataCode> const& obj);

	/*
	void FillWithRandomNumbers() {
		std::srand(RshMix());
		for (size_t i = 0; i < psize; i++)
			this->ptr[i].FillWithRandomNumbers(false);
	}
	*/

	~RshDBufferType();

	size_t ByteSize();

	RshBufferType<T, dataCode> &operator[](size_t i);

	RshDBufferType<T, dataCode> &operator=(RshDBufferType<T, dataCode>& obj);

	bool operator==(RshDBufferType<T, dataCode>& obj);

	bool operator!=(RshDBufferType<T, dataCode>& obj);

	/*
	//=== forRSH cycle support code block ====
	RshBufferType<T, dataCode>* it; // iterator

	bool SetItToTheFirstElement() {
		it = ptr;
		if (ptr == 0 || psize == 0) return false;
		else
			return true;
	}

	bool SetItToTheNextElement() {
		if ((it - ptr) != psize) {
			++it;
		}

		return (it - ptr) != psize;
	}
	*/
};
/// \endcond
//----------------------------------------------------------------------------------------------------
//! Device interface type
typedef enum _RSH_BOARD_ITYPE {
	RSH_ITYPE_UNDEFINED = 0, //!< default (undefined value)
	RSH_ITYPE_PCI = 1 << 0, //!< PCI device
	RSH_ITYPE_PCIe = 1 << 1, //!< PCI Express device
	RSH_ITYPE_USB_11 = 1 << 2, //!< USB ver 1.1 device
	RSH_ITYPE_USB_20 = 1 << 3, //!< USB ver 2.0 device
	RSH_ITYPE_USB_30 = 1 << 4, //!< USB ver 3.0 device

	RSH_ITYPE_ANY = 0xffffffff,
} RSH_BOARD_ITYPE;


/*! Коды для задания режима непрерывного сбора 
 *
 *  (поле ADCParametersDMA::m_nDMAMode )
 *  Persistent Gathering Mode Codes
 *
 * \todo Возможно, данный enum уже не нужен (или нужно перенести в файлы библиотеки DM)
 */
typedef enum _RSH_PG_MODE_CODES {
	RSH_PGMODE_DATAQUANTITY_DEFINED = 0,
	RSH_PGMODE_DATAQUANTITY_INFINITY = 1, //!< данные будут собираться пока устройство не будет остановлено
	RSH_PGMODE_SAVEDATATODISK = 2, //!< данные будут сохраняться на диск по внутреннему алгоритму
	RSH_PGMODE_AUTOBUFSIZE = 4 //!< будут выбраны автоматически оптимальные размеры буфера для корректного сбора данных
} RSH_PG_MODE_CODES;



/*!
* 
* \~english
* \brief
* Internal buffer parameters structure.
* 
* This structure contains pointer to internal
* data buffer (allocated in base library for data
* acquisition) according with that buffer size and
* element number.
* 
* \remarks
* This structure usually not used in user applications,
* but there is hidden ::RSH_GET code that allows to 
* get this structure from IRshDevice object.
* 
* \~russian
* \brief
* Структура с параметрами внунтреннего буфера
* 
* В данной структуре содерижится указатель на внутренний
* буфер данный (выделяемый в базовой библиотеке абстракции
* для сбора данных), а также количество элементов и размер
* элемента буфера.
* 
* \remarks
* Эта структура, как правило, не используется в пользовательских
* приложениях, но существует специальный скрытый код
* ::RSH_GET, с помощью которого можно получить эту структуру
* из объекта с интерфейсом IRshDevice.
* 
*/
struct RshDeviceBuffer : public RshBaseType {

	/*!
	* 
	* \~english
	* \brief
	* Number of elements in buffer
	* 
	* Size (in samples) of allocated memory
	* 
	* \~russian
	* \brief
	* Количество элементов во внутреннем буфере данных
	* 
	* Размер (в отсчетах) выделенной под данные памяти
	* 
	* 
	*/
	U64 sampleQuantity;

	/*!
	* 
	* \~english
	* \brief
	* Size of buffer element in bytes
	* 
	* \~russian
	* \brief
	* Размер одного элемента буфера в байтах
	* 
	*/
	U8 sampleSize; 

	/*!
	* 
	* \~english
	* \brief
	* Pointer to allocated memory
	* 
	* \~russian
	* \brief
	* Указатель на выделенную память
	* 
	*/
	void* buffer;

	//!Constructor
	RshDeviceBuffer();
};

/*!
* 
* \~english
* \brief
* Register data structure.
* 
* This structure is used to pass 
* parameters to or from device registers.
* 
* \see
* RSH_GET_DEVICE_REGISTER_BOARD | RSH_GET_DEVICE_REGISTER_BOARD_SET  
* 
* \~russian
* \brief
* Структура с данными регистра
* 
* Данная структура используется для передачи
* параметров в регистры устройтсва.
*
* \see
* RSH_GET_DEVICE_REGISTER_BOARD | RSH_GET_DEVICE_REGISTER_BOARD_SET  
* 
*/
struct RshRegister : public RshBaseType {
	/*!
	* 
	* \~english
	* \brief
	* Size of register in bytes
	*
	* Possible values are 1, 2 and 4.
	* Register size depends on device type,
	* see device documentation for more info.
	* 
	* \~russian
	* \brief
	* Размер регистра в байтах
	*
	* Допустимые значения: 1, 2 и 4 байта.
	* Размер регистра зависит от устройства,
	* смотрите документацию к устройтсву для
	* получения дополнительной информации.
	* 
	*/
	U8 size;
	/*!
	* 
	* \~english
	* \brief
	* Register address
	*
	* Register address (offset) for memory
	* mapped registers or actual register number.
	* See device documentation for more info.
	* 
	* \~russian
	* \brief
	* Адрес регистра
	*
	* Адрес регистра (смещение) для регистров,
	* отображенных в память, либо фактический
	* номер регистра.
	* Смотрите документацию к устройству для 
	* получения дополнительной информации.
	* 
	*/
	U32 offset; 
	/*!
	* 
	* \~english
	* \brief
	* Value passed to (or from) register
	* 
	* \~russian
	* \brief
	* Значение, которое нужно записать (или прочитанное из регистра)
	*
	*/
	U32 value; 

	RshRegister();
};

/*
\todo is this structure actually used somewhere?
struct RshPort : public RshBaseType {
U32 address;
U32 value;

RshPort() {
this->_type = rshPort;
this->_typeSize = sizeof (RshPort);
address = 0;
value = 0;
}
};
*/

/// \cond SHOW_ALL_CLASSES

/*!
	Параметры сбора данных для DM
	\todo перенести в заголовочный файл библиотеки DM
*/
struct RshGatheringParameters : public RshBaseType {
	U32 userBufferSize; // buffer size may be corrected internally while allocation process takes place(size in bytes)
	U8 measureSize; // 1 byte , 2 bytes or 4 bytes for setting local bus size
	U8 transferDirection; // from user - 0 , to user - 1

	//    float  frequency; // adc frequency for choosing correct internal buffer size ( to increase efficiency/the larger size the less possible number of interrupts )

	//	RSH_PG_MODE_CODES    mode; // persistent gathering mode

	void** userBuffer; // internal data buffer
	U8* evenPart; //  which part of two in userBuffer in persistent acquisition mode has filled completely

	RshGatheringParameters();
};


/***********************************************\
*        Device Flash Memory Structures       *
\***********************************************/
#pragma pack(push, 1)

/*! 
	Блок данных флеш памяти
	\todo перенести в Private хедеры
*/
struct RshFlashBlock 
{
	union
	{
		struct
		{
			unsigned char       m_ucRType;
			unsigned char       m_ucRSize;
		};
		unsigned short      m_uwRTypeSize;
	};

	union
	{
		float               frequency;        // if((m_nMode & FRT_MODE_FLAG) == 0) then freq
		int                 m_nMode;        // else mode
		struct
		{
			short               m_nLoMode;
			short               m_nHiMode;
		};
	};
	unsigned char       flashBlock[1];  // указатель на блок данных флеш памяти

	RshFlashBlock()
	{
		m_ucRType = 0xDF; //FRT_DACTUNE;
		m_ucRSize = sizeof(RshFlashBlock);
	}
};
#pragma pack(pop)

/*! 
	Структура отображает флеш память платы
	\todo перенести в Private хедеры
*/
struct RshFRDACTune : public RshBaseType
{
	RshFlashBlock**		flashBlocks;

	RshFRDACTune();
};
/// \endcond

/**********************************************\
*        Device Information Structures       *
\**********************************************/

/*!
* 
* \~english
* \brief
* Device information structure
* 
* Extended information about device. 
* Contains device base address, serial number,
* slot number and so on.\n
* One can get list of such structures using
* IRshDevice::Get() method with
* RSH_GET_DEVICE_BASE_LIST_EXT parameter.
* 
* \see
* RSH_GET_DEVICE_BASE_LIST_EXT
* 
* \~russian
* \brief
* Структура с информацие об устройстве
* 
* Расширенная информация об устройстве.
* Содержит базовый адрес устройства, его
* заводской номер, код чипа PLX и т.д.\n
* Получить заполненный список таких структур
* можно с помощью метода IRshDevice::Get() 
* с параметром RSH_GET_DEVICE_BASE_LIST_EXT.
*
* \see
* RSH_GET_DEVICE_BASE_LIST_EXT
* 
*/
struct RshDeviceBaseInfo  : public RshBaseType  {

	/*!
	* 
	* \~english
	* \brief
	* Chip ID
	* 
	* Chip ID (PLX 9050, 9054, 8311, USB-specific code)
	* 
	* \~russian
	* \brief
	* Идентификатор чипа
	* 
	* Идентификатор чипа (модули PLX  9050, 9054, 8311, или коды USB)
	* 
	* \todo Структура с кодами чипов - описание
	*/
	U16 chip;

	/*!
	* 
	* \~english
	* \brief
	* Vendor ID (VID) of the device
	*
	* Each peace of hardware connected to PC has
	* its unique (for device class) vendor ID and 
	* device ID numbers. Using this numbers operation
	* system can chose driver to use to work with
	* this device.
	* 
	* \~russian
	* \brief
	* Идентификатор производителя (VID) устройства
	*
	* Каждое устройство расширения, подключаемое к компьютеру,
	* имеет уникальный (для данной модели устройства) код устройства (product ID или PID)
	* и код производителя (vendor ID или VID). Операционная система
	* использует данные коды для идентифиакции устройства и выбора драйвера
	* для него.
	* 
	*/
	U16 vid; 
	/*!
	* 
	* \~english
	* \brief
	* Product ID (PID) of the device
	*
	* Each peace of hardware connected to PC has
	* its unique (for device class) vendor ID and 
	* device ID numbers. Using this numbers operation
	* system can chose driver to use to work with
	* this device.
	* 
	* \~russian
	* \brief
	* Идентификатор продукта (PID)
	*
	* Каждое устройство расширения, подключаемое к компьютеру,
	* имеет уникальный (для данной модели устройства) код устройства (product ID или PID)
	* и код производителя (vendor ID или VID). Операционная система
	* использует данные коды для идентифиакции устройства и выбора драйвера
	* для него.
	* 
	*/
	U16 pid;

	/*!
	* 
	* \~english
	* \brief
	* Device revision
	* 
	* Device revision.\n
	* Some revisions only different in firmware used,
	* some have diffierent hardware features, such
	* as memory size.
	* 
	* \~russian
	* \brief
	* Ревизия устройства
	* 
	* Номер ревизии устройства.\n
	* Некоторые ревизии отличаются только версией прошивки,
	* другие имеют более существенные отличия, такие
	* как размер установленной в устройстве памяти
	* или разрядность АЦП.
	* 
	*/
	U16 rev; 

	/*!
	* 
	* \~english
	* \brief
	* PCI slot number
	* 
	* Number of PCI slot where board is located.
	* Not actual for USB devices.
	* 
	* \~russian
	* \brief
	* Номер слота PCI
	* 
	* Номер слота PCI, в который установлена данная плата.
	* Не актуально для USB устройств.
	* 
	* \todo Добавить реализацию для USB устройств тоже, например по номеру хаба.
	*/
	U16 slot;

	/*!
	* 
	* \~english
	* \brief
	* Device base address
	* 
	* Device base address (index).
	* If there are two or more devices
	* of the same type, they will be enumerated
	* by the operation system.\n
	* Enumeration starts from 1.
	*
	* \see
	* IRshDevice::Connect()
	* 
	* \~russian
	* \brief
	* Базовый адрес устройства
	* 
	* Базовый адрес (индекс) устройства.
	* Если в системе одновременно установлено несколько
	* устройство одного типа, они будут пронумерованы 
	* операционной системой.\n
	* Нумерация начинается с 1.
	* 
	* \see
	* IRshDevice::Connect()
	*
	*/
	U16 base;

	/*!
	* 
	* \~english
	* \brief
	* Serial number of the device	
	* 
	* \~russian
	* \brief
	* Заводской номер устройства
	* 
	*/
	U32 serialNumber;

	RshDeviceBaseInfo(U16 vid, U16 pid, U16 rev, U16 chip, U16 slot, U16 base, U32 serialNumber = 0);

	/*!
	* 
	* \~english
	* \brief
	* Copy constructor
	* 
	* \~russian
	* \brief
	* Конструктор копий
	* 
	*/
	RshDeviceBaseInfo(RshDeviceBaseInfo const& pi);

	RshDeviceBaseInfo();

	bool operator==(RshDeviceBaseInfo& obj);

	bool operator!=(RshDeviceBaseInfo& obj);

	bool operator<(const RshDeviceBaseInfo& obj);

	bool operator>(const RshDeviceBaseInfo& obj);

	friend std::ostream& operator<< (std::ostream &out,  RshDeviceBaseInfo& obj);
};

/*!
* 
* \~english
* \brief
* RshBufferType of RshDeviceBaseInfo structures
* 
* Default size is ::RSH_MAX_LIST_SIZE
* 
* \~russian
* \brief
* Буфер RshBufferType из структур RshDeviceBaseInfo
* 
* Размер по умолчанию - ::RSH_MAX_LIST_SIZE
* 
*/
typedef RshBufferType< RshDeviceBaseInfo, rshBufferTypeDeviceBaseInfo> RSH_BUFFER_DEVICE_BASE_INFO;


//! Full Device Information Structure ( Full INFO )
struct RshDeviceFullInfo : public RshDeviceBaseInfo {
	RSH_BOARD_ITYPE itype; //!< physical interface type
	S8* libraryName; //!< in English to find a record in registry
	S8* libraryInterfaceName; //!< corresponding library interface name
	U16* deviceOriginalName_RU; //!< in Russian for user
	U16* deviceOriginalName_EN; //!< in English for user
	//    U32 CAPSEX[4]; //!< board capabilities (filled inside DM.dll)

	RshDeviceFullInfo();

	RshDeviceFullInfo(RshDeviceFullInfo const& obj);
};

/*!
* 
* \~english
* \brief
* RshBufferType of RshDeviceFullInfo structures
* 
* Default size is ::RSH_MAX_LIST_SIZE
* 
* \~russian
* \brief
* Буфер RshBufferType из структур RshDeviceFullInfo
* 
* Размер по умолчанию - ::RSH_MAX_LIST_SIZE
* 
*/
typedef RshBufferType< RshDeviceFullInfo, rshBufferTypeDeviceFullInfo> RSH_BUFFER_DEVICE_FULL_INFO;


/*!
* 
* \~english
* \brief
* Information about IRshDevice interface based devices represented in your system
* 
* This structure is used as a container
* for obtaining device related information.\n
* One need pass this structure to 
* IRshDevice::Get() method  with
* parameter ::RSH_GET_DEVICE_ACTIVE_LIST.
* 
* \~russian
* \brief
* Информация о подключенных к системе платах с интерфейсом IRshDevice
* 
* Данная структура используется как контейнер
* для получения информации о платах, обнаруженных в системе.\n
* После вызова метода IRshDevice::Get() с параметром
* ::RSH_GET_DEVICE_ACTIVE_LIST, переданная структура
* будет содержать информацию о подключенных к компьютеру
* платах.
* 
*/
struct RshDeviceActiveList : public RshBaseType {
public:
	RSH_BUFFER_DEVICE_BASE_INFO deviceList;

	RshDeviceActiveList();

	/*! \brief Set capability list for a device

	bind capability list obtained from abstraction object
	to device in list

	\param[in] devIndex divice index in list
	\param[in] caps capability array (must contain at least 4 numbers!)
	*/

	/*
	//! function allows to determine possible changes in devicelist when comparing with other RSH_DEV_FINFO _ALL structure

	U64 GetDeviceListHASH() {
	U64 hash = 0;

	for (U8 i = 0; i < deviceList.size; i++)
	hash += (((U64) deviceList[i].base) << 32) | deviceList[i].pid;

	return hash;
	}

	void SetCAPSEX(U8 devIndex, U32 * caps) {
	if (devIndex >= deviceList.size()) return;

	deviceList[devIndex].CAPSEX[0] = caps[0];
	deviceList[devIndex].CAPSEX[1] = caps[1];
	deviceList[devIndex].CAPSEX[2] = caps[2];
	deviceList[devIndex].CAPSEX[3] = caps[3];
	}*/
};

/*!
* 
* \~english
* \brief
* Information about digital port
* 
* This structure contains information
* about digital port.\n
* Usually, one can obtain list of such structures
* with information about device digital ports
* using IRshDevice::Get() method with 
* ::RSH_GET_DEVICE_PORT_INFO parameter.
*
* \see
* RSH_GET_DEVICE_PORT_INFO | RSH_CAPS_SOFT_DIGITAL_PORT_IS_AVAILABLE 
* 
* \~russian
* \brief
* Информация о цифровом порте
* 
* В данной структуре содержится информация
* о цифровом порте.\n
* Как правило, данная структура используется
* для получения полной информации о цифровых
* портах устройства. Вызвав метод 
* IRshDevice::Get() с параметром ::RSH_GET_DEVICE_PORT_INFO,
* можно получить список таких структур для данного устройства.
* 
* \see
* RSH_GET_DEVICE_PORT_INFO | RSH_CAPS_SOFT_DIGITAL_PORT_IS_AVAILABLE 
* 
*/
struct RshPortInfo : public RshBaseType {

	/*!
	* 
	* \~english
	* \brief
	* Port address
	* 
	* Usually just index that can be used
	* to access port, sometimes it is actual
	* address of mapped device register.\n
	* One can use addres, obtained by  IRshDevice::Get()
	* with ::RSH_GET_DEVICE_PORT_INFO parameter call,
	* to access digital port via IRshDevice::Init() with
	* RshInitPort structure.
	* 
	* \see
	* RshInitPort
	* 
	* \~russian
	* \brief
	* Адрес порта
	* 
	* Зачастую просто индекс (идентификатор) порта,
	* который должен использоваться для обращения к нему,
	* иногда действительный адрес отображаемого в память
	* регистра устройства.\n
	* Адрес порта, полученный при вызове метода IRshDevice::Get()
	* с параметром ::RSH_GET_DEVICE_PORT_INFO, можно использовать
	* для доступа к порту через метод IRshDevice::Init()
	* со структурой инициализации RshInitPort.
	* 
	* \see
	* RshInitPort
	* 
	*/
	U32 address;

	/*!
	* 
	* \~english
	* \brief
	* Port bits number
	* 
	* Number of bits in digital port
	* 
	* \~russian
	* \brief
	* Разрядность порта
	* 
	* Количество линий (разрядность) цифрового порта
	*
	*/
	U8 bitSize;

	/*!
	* 
	* \~english
	* \brief
	* Port name
	* 
	* Port name, usually corresponds to connector name
	* as it described in device manual.
	* 
	* \~russian
	* \brief
	* Имя порта
	* 
	* Имя порта, обычно соответствует названию разъема,
	* как оно прописано в руководстве пользователся для данного
	* устройства.
	*
	*/
	wchar_t* name; 

	RshPortInfo();

	RshPortInfo(U32 address, U8 bitsize, wchar_t* name);

	bool operator==(RshPortInfo& obj);

	bool operator!=(RshPortInfo& obj);

	bool operator< (RshPortInfo& obj);

	bool operator> (RshPortInfo& obj);

	friend std::ostream& operator<< (std::ostream &out,  RshPortInfo& obj);

};

/*!
* 
* \~english
* \brief
* RshBufferType of RshPortInfo structures
* 
* Default size is ::RSH_MAX_CHANNEL_NUMBER
* 
* \~russian
* \brief
* Буфер RshBufferType из структур RshPortInfo
* 
* Размер по умолчанию - ::RSH_MAX_CHANNEL_NUMBER
* 
*/
typedef RshBufferType< RshPortInfo, rshBufferTypePortInfo > RSH_BUFFER_PORTINFO;



/*!
* 
* \~english
* \brief
* Information about device digital ports
* 
* This structure is used as container
* for obtaining port information.\n
* One need pass this structure to 
* IRshDevice::Get() method  with
* parameter ::RSH_GET_DEVICE_PORT_INFO.
* 
* \~russian
* \brief
* Информация о цифровых портах устройства
* 
* Данная структура используется как контейнер
* для получения информации о цифровых портах.\n
* После вызова метода IRshDevice::Get() с параметром
* ::RSH_GET_DEVICE_PORT_INFO, переданная структура
* будет содержать информацию о цифровых портах
* данного устройтсва.
* 
*/
struct RshBoardPortInfo : public RshBaseType {
public:

	RshBoardPortInfo();

	/*!
	* 
	* \~english
	* \brief
	* Configuration ports
	* 
	* List of device configuration ports.
	* Use RshBufferType::Size() field to determine,
	* how many configuration ports device have.
	* 
	* \remarks
	* Most devices do not have configuration ports,
	* only data ports.
	* 
	* \~russian
	* \brief
	* Конфигурационные порты
	* 
	* Список конфигурационных портов устройства.
	* Используйте поле RshBufferType::Size() для того, 
	* чтобы определить количество конфигурационных портов
	* после заполнения структуры.
	* 
	* \remarks
	* Большинство устройств не имеет конфигурационных портов,
	* только порты данных.
	* 
	*/
	RSH_BUFFER_PORTINFO confs;

	/*!
	* 
	* \~english
	* \brief
	* Data ports
	* 
	* Actual digital ports, that can be used
	* to input and output data.
	* Use RshBufferType::Size() field to determine,
	* how many data ports device have.
	* 
	* \~russian
	* \brief
	* Порты данных
	* 
	* Цифровые порты устройства, которые могут быть 
	* использованы для ввода и вывода цифровых данных.
	* Используйте поле RshBufferType::Size() для того, 
	* чтобы определить количество портов данных
	* после заполнения структуры.
	* 
	*/
	RSH_BUFFER_PORTINFO ports;
};



// RshDBufferType buffers (2 dimensional)
typedef RshDBufferType< U8    , rshDBufferTypeU8>      RSH_DBUFFER_U8;
typedef RshDBufferType< S8    , rshDBufferTypeS8>      RSH_DBUFFER_S8;
typedef RshDBufferType< U16   , rshDBufferTypeU16>     RSH_DBUFFER_U16;
typedef RshDBufferType< S16   , rshDBufferTypeS16>     RSH_DBUFFER_S16;
typedef RshDBufferType< U32   , rshDBufferTypeU32>     RSH_DBUFFER_U32;
typedef RshDBufferType< S32   , rshDBufferTypeS32>     RSH_DBUFFER_S32;
typedef RshDBufferType< U64   , rshDBufferTypeU64>     RSH_DBUFFER_U64;
typedef RshDBufferType< S64   , rshDBufferTypeS64>     RSH_DBUFFER_S64;
typedef RshDBufferType< float , rshDBufferTypeFloat>   RSH_DBUFFER_FLOAT;
typedef RshDBufferType< double, rshDBufferTypeDouble>  RSH_DBUFFER_DOUBLE;
typedef RshDBufferType< bool  , rshDBufferTypeBool>    RSH_DBUFFER_BOOL;

// RshBufferType buffers
typedef RshBufferType< U8    , rshBufferTypeU8>      RSH_BUFFER_U8;
typedef RshBufferType< S8    , rshBufferTypeS8>      RSH_BUFFER_S8;
typedef RshBufferType< U16   , rshBufferTypeU16>     RSH_BUFFER_U16;
typedef RshBufferType< S16   , rshBufferTypeS16>     RSH_BUFFER_S16;
typedef RshBufferType< U32   , rshBufferTypeU32>     RSH_BUFFER_U32;
typedef RshBufferType< S32   , rshBufferTypeS32>     RSH_BUFFER_S32;
typedef RshBufferType< U64   , rshBufferTypeU64>     RSH_BUFFER_U64;
typedef RshBufferType< S64   , rshBufferTypeS64>     RSH_BUFFER_S64;
typedef RshBufferType< float , rshBufferTypeFloat>   RSH_BUFFER_FLOAT;
typedef RshBufferType< double, rshBufferTypeDouble>  RSH_BUFFER_DOUBLE;
typedef RshBufferType< bool  , rshBufferTypeBool>    RSH_BUFFER_BOOL;

typedef RshBufferType< U8*   , rshBufferTypeU8P>    RSH_BUFFER_U8P;

/*!
 * 
 * \~english
 * \brief
 * Unsigned 8 bit integer.
 * 
 * \~russian
 * \brief
 * Беззнаковое целое 8 бит.
 * 
 */
typedef RshType< U8, rshU8 > RSH_U8;

/*!
 * 
 * \~english
 * \brief
 * Signed 8 bit integer.
 * 
 * \~russian
 * \brief
 * Знаковое целое 8 бит.
 * 
 */
typedef RshType< S8, rshS8 > RSH_S8;

/*!
 * 
 * \~english
 * \brief
 * Unsigned 16 bit integer.
 * 
 * \~russian
 * \brief
 * Беззнаковое целое 16 бит.
 * 
 */
typedef RshType< U16, rshU16 > RSH_U16;

/*!
 * 
 * \~english
 * \brief
 * Signed 16 bit integer.
 * 
 * \~russian
 * \brief
 * Знаковое целое 16 бит.
 * 
 */
typedef RshType< S16, rshS16 > RSH_S16;

/*!
 * 
 * \~english
 * \brief
 * Unsigned 32 bit integer.
 * 
 * \~russian
 * \brief
 * Беззнаковое целое 32 бита.
 * 
 */
typedef RshType< U32, rshU32 > RSH_U32;

/*!
 * 
 * \~english
 * \brief
 * Signed 32 bit integer.
 * 
 * \~russian
 * \brief
 * Знаковое целое 32 бита.
 * 
 */
typedef RshType< S32, rshS32 > RSH_S32;

/*!
 * 
 * \~english
 * \brief
 * Unsigned 64 bit integer.
 * 
 * \~russian
 * \brief
 * Беззнаковое целое 64 бита.
 * 
 */
typedef RshType< U64, rshU64 > RSH_U64;

/*!
 * 
 * \~english
 * \brief
 * Signed 64 bit integer.
 * 
 * \~russian
 * \brief
 * Знаковое целое 64 бита.
 * 
 */
typedef RshType< S64, rshS64 > RSH_S64;

/*!
 * 
 * \~english
 * \brief
 * Floating point number (4 bytes).
 * 
 * \~russian
 * \brief
 * Число с плавающей точкой (4 байта).
 * 
 */
typedef RshType< float, rshFloat >  RSH_FLOAT;

/*!
 * 
 * \~english
 * \brief
 * Floating point number (8 bytes).
 * 
 * \~russian
 * \brief
 * Число с плавающей точкой (8 байт).
 * 
 */
typedef RshType< double, rshDouble >  RSH_DOUBLE;

/*!
 * 
 * \~english
 * \brief
 * Boolean.
 * 
 * \~russian
 * \brief
 * Логический тип.
 * 
 */
typedef RshType< bool,   rshBool >     RSH_BOOL;

typedef RshType< void*   , rshVoidP >    RSH_VOIDP;
typedef RshType< U8*     , rshU8P  >     RSH_U8P;
typedef RshType< S8*     , rshS8P  >     RSH_S8P;
typedef RshType< U16*    , rshU16P >     RSH_U16P;
typedef RshType< S16*    , rshS16P  >    RSH_S16P;
typedef RshType< U32*    , rshU32P  >    RSH_U32P;
typedef RshType< S32*    , rshS32P  >    RSH_S32P;
typedef RshType< U64*    , rshU64P  >    RSH_U64P;
typedef RshType< S64*    , rshS64P  >    RSH_S64P;
typedef RshType< float*  , rshFloatP  >  RSH_FLOATP;
typedef RshType< double* , rshDoubleP >  RSH_DOUBLEP;
typedef RshType< bool*   , rshBoolP >    RSH_BOOLP;

typedef RshType< U8**     , rshU8PP  >     RSH_U8PP;
typedef RshType< S8**     , rshS8PP  >     RSH_S8PP;
typedef RshType< U16**    , rshU16PP >     RSH_U16PP;
typedef RshType< S16**    , rshS16PP  >    RSH_S16PP;
typedef RshType< U32**    , rshU32PP  >    RSH_U32PP;
typedef RshType< S32**    , rshS32PP  >    RSH_S32PP;
typedef RshType< U64**    , rshU64PP  >    RSH_U64PP;
typedef RshType< S64**    , rshS64PP  >    RSH_S64PP;
typedef RshType< float**  , rshFloatPP  >  RSH_FLOATPP;
typedef RshType< double** , rshDoublePP >  RSH_DOUBLEPP;
typedef RshType< bool**   , rshBoolPP >    RSH_BOOLPP;


#pragma pack(pop)

struct RshTime{
    U32 h;
    U32 m;
    U32 s;
    U32 ms;
};

/**********************************************\
*      Common used RshFunction prototypes    *
\**********************************************/
U64 RshMix(); //!< Tries to get a unique number
U32 RshCRC32(U8 *buf, size_t len); //!< Calculates crc32 hash code
double RshTimeCurrent(); //!< Retrieves the current system date and time
void RshTimeReal(RshTime&);
void RshTimeElapsed(double timeSpent, U32& hours, U32& minutes, U32& seconds, U32& milliseconds, U32& microseconds); //!< Calculates elapsed time in minutes,seconds,milliseconds,microseconds
void RshTimeElapsedString(double timeSpent, std::string& str); //!< Calculates elapsed time in minutes,seconds,milliseconds,microseconds and retrieves std::string
void RshTimeElapsedWString(double timeSpent, std::wstring& str); //!< Calculates elapsed time in minutes,seconds,milliseconds,microseconds and retrieves std::string
double RshLsbToVoltCoef(U32 gain, double range, U8 bitSize); //!< Calculates coefficient intended for conversion ADC code to voltage
double RshVoltToLsbCoef(U32 gain, double range, U8 bitSize); //!< Calculates coefficient intended for conversion voltage to ADC code
double RshRoundD(double value, double precision); //!< Rounds a double value with predefined precision
const wchar_t* RshConvertToUTF16(const char* pStr);
U32 RshCopyStringTo(const char*, RshBaseType*); //!< Copies string to RshBuffer 


#endif //__RSH_TYPES_H__
