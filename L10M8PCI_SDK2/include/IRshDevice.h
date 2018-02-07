/*!
 * \copyright JSC "Rudnev-Shilyaev"
 *
 * \file IRshDevice.h
 * \date 11.04.2014
 * \version 1.0 [SDK 2.00]
 *
 * \~english
 * \brief
 * IRshDevice interface and dependent types declaration.
 *
 * \~russian
 * \brief
 * Интерфейс IRshDevice и связанные с ним типы данных.
 *
 */

#ifndef __RSH_DEVICE_H__
#define __RSH_DEVICE_H__

#include "RshTypes.h"

#if defined(RSH_LINUX)
    #include <string.h> // for memset function
#endif

//----------------------------------------------------------------------

#pragma pack(push, 8)

/*!
 *
 * \~english
 * \brief
 * Interface is used to control almost all RSH ADC/DAC devices.
 *
 * Main interface to handle ADC and DAC devices produced by CJSC "Rudnev-Shilyaev"
 * and some third-party devices by other companies. \n
 * Typical scenario is when user gets object with IRshDevice interface from
 * device abstraction library, which can be loaded
 * using RshDllClient class.
 * Then user can get access to all features of the device without knowing
 * low level architecture of device and device drivers.
 *
 * \remarks
 * Some RSh devices do not use this interface, for example, charge
 * amplifiers that are connected to PC using seral port and accessed
 * via RS-232 interface.
 *
 * \see
 * RshDllClient
 *
 * \~russian
 * \brief
 * Интерфейс используется для управления практически всеми
 * АЦП/ЦАП устройствами ЗАО "Руднев-Шиляев".
 *
 * Основной интерфейс для управления устройствами, выпускаемыми фирмой ЗАО "Руднев-Шиляев",
 * а также некоторыми устройствами других фирм.\n
 * Обычный сценарий работы - получение объекта, реализующего интерфейс IRshDevice,
 * из библиотеки абстракции, которая может быть загружена используя класс RshDllClient.
 * После этого пользователь получает доступ ко всем возможностям устройства, при этом
 * ему не нужно знать низкоуровневых деталей реализации аппаратной структуры устройства и драйвера.
 *
 * \remarks
 * Некоторые устройства ЗАО "Руднев-Шиляев" не используют этот интерфейс, например,
 * усилители заряда подключаются к компьютеру через последовательный порт и
 * управляются по протоколу RS-232.
 *
 * \see
 * RshDllClient.
 */
struct  IRshDevice
{
	/*!
	 *
	 * \~english
	 * \brief
	 * Connect to certain device.
	 *
	 * \param[in] key
	 * Identification key for a device. It might be an index of the device, a serial number of the device or a connection string to the device.
	 *
	 * \param[in] mode
	 * Connection mode using a device index, a serial number or a connection string.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS or error code.
	 *
	 * This method must be executed first before any other
	 * device access methods.\n
	 * After successfull call of this method
	 * device is ready to use and can be configured
	 * for data acquisition.\n
	 * Most other methods of interface (such as Init(), Start() etc.)
	 * will return error if device was not connected using Connect().\n
	 * One can, however, get some static library and device information
	 * using Get() method.
	 *
	 *
	 * \remarks
	 * If you use connection mode ::RSH_CONNECT_MODE_BASE devices are enumerated from 1.
	 * Order of devices is determined by operating system and
	 * depends on used slot and plug order.
	 *
	 * \see
	 * Get() | RSH_CONNECT_MODES |  RshDeviceKey
	 *
	 * \~russian
	 * \brief
	 * Подключение к заданному устройству.
	 *
	 * \param[in] key
	 * Идентификационный ключ устройства.
	 * В зависимости от выбранного режима подключения может быть индексом устройства, его серийным номером или строкой подключения.\n
	 * В качестве ключа используется структура RshDeviceKey (тип параметра RshBaseType оставлен для большей универсальности).
	 *
	 * \param[in] mode
	 * Режим подключения к устройству с использованием серийного номера устройства, его индекса или строки подключения.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS или код ошибки.
	 *
	 * Данный метод нужно вызывать первым, до вызова любых других
	 * методов доступа к устройству.\n
	 * После успешного вызова этого метода устройство полностью готово
	 * к использованию и может быть настроено для сбора данных.
	 * Большинство других методов интерфейса (такие как Init(), Start())
	 * вернут ошибку, если вызвать их до вызова Connect().\n
	 * Можно, однако, получить некоторые статические параметры библиотеки абстракции
	 * и устройства, используя метод Get().
	 *
	 * \remarks
	 * Если вы используете режим подключения ::RSH_CONNECT_MODE_BASE, нумерация устройств начинается с 1.
	 * Порядок устройств определяется операционной системой, и зависит
	 * от используемого разъема (слота), а так же очередности подключения устройств к компьютеру.
	 *
	 * \see
	 * Get() | RSH_CONNECT_MODES | RshDeviceKey
	 */
	virtual U32    __RSHCALLCONV  Connect( IN RshBaseType* key, IN U32 mode = RSH_CONNECT_MODE_BASE ) = 0;

	/*!
	 *
	 * \~english
	 * \brief
	 * Setup data acquisition parameters.
	 *
	 * \param[in,out] structure
	 * Initialization structure derived from RshBaseType.\n
	 * Each device can support one or more of initialization
	 * structures, such as RshInitDMA or RshInitMemory.
	 *
	 * \param[in] mode
	 * Initialization mode:\n
	 * ::RSH_INIT_MODE_CHECK - check and change wrong
	 * parameters in structure without any hardware access.\n
	 * ::RSH_INIT_MODE_INIT - real initialization, check and change
	 * wrong parameters and send them to hardware.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS or error code.
	 *
	 * This method is used to setup device and set data acquisition parameters.
	 * Special parameter structures are used:\n
	 * RshInitDMA: for low frequency (less than 10MHz) devices with no integrated flash memory.\n
	 * RshInitMemory: for high frequency devices with on board memory.\n
	 * RshInitGSPF: for digital generator devices.\n
	 * RshInitVoltmeter: for B7/78 voltmeter.\n
	 * RshInitPort: access to digital port.\n
	 * RshInitDAC: access to DACs.\n
	 * Using these structures one can set various settings such as
	 * buffer size, frequency, number of channels and so on. See structure
	 * description for more details.
	 * Example:
	 * \include IRshDevice_Init_RshInitDMA_EN.cpp
	 *
	 * \remarks
	 * You can find what init structure is supported by your device using
	 * Get() method with parameter ::RSH_GET_DEVICE_IS_CAPABLE and check
	 * capabilities ::RSH_CAPS_SOFT_INIT_MEMORY, ::RSH_CAPS_SOFT_INIT_DMA etc.
	 *
	 * \see
	 * RshInitDMA | RshInitMemory | RshInitDAC | RshInitVoltmeter | RshInitPort | RshInitGSPF
	 *
	 * \~russian
	 * \brief
	 * Установка параметров сбора и настройка устройства.
	 *
	 * \param[in,out] structure
	 * Структура инициализации, унаследованная от RshBaseType.\n
	 * Каждое устройство поддерживает как минимум одну структуру
	 * инициализации, такую как RshInitDMA или RshInitMemory.
	 *
	 * \param[in] mode
	 * ::RSH_INIT_MODE_CHECK - проверка полей в переданной структуре инициализаци,
	 * неподходящие значения будут исправлены на допустимые. После исправления
	 * параметров метод завершает работу, обращений к устройству не производится.\n
	 * ::RSH_INIT_MODE_INIT - действительня реализация, после исправления значений
	 * в структуре производится передача выбранных параметров к устройству, запись
	 * в регистры и тому подобное.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS или код ошибки.
	 *
	 * Данный метод используется для задания параметров сбора (генерации).
	 * Для передачи параметров используется одна из специальных структур инициализации:\n
	 * RshInitDMA: как правило, данная структура используется для низкочастотных (до 10Мгц) устройств
	 * без установленной флэш памяти для данных.\n
	 * RshInitMemory: для высокочастотных устройств с собственной памятью.\n
	 * RshInitGSPF: для цифровых генераторов сигнала.\n
	 * RshInitVoltmeter: для вольтметра В7/78\n
	 * RshInitPort: для доступа к цифровым портам.\n
	 * RshInitDAC: для доступа к ЦАПам.\n
	 * Используя данные структуры, можно задать различные параметры сбора,
	 * такие как частоту дискретизации, число каналов, размер буфера и др.
	 * Более подробно - в описании структур инициализации.\n
	 *
	 * Пример использования метода:
	 * \include IRshDevice_Init_RshInitDMA_RU.cpp
	 *
	 * \remarks
	 * Чтобы узнать, поддерживает или нет конкретное устройство ту или иную структуру
	 * инициализации, используйте метод Get() с параметром ::RSH_GET_DEVICE_IS_CAPABLE
	 * с соответствующими кодами для структур (::RSH_CAPS_SOFT_INIT_MEMORY, ::RSH_CAPS_SOFT_INIT_DMA и т.д.)
	 *
	 * \see
	 * RshInitDMA | RshInitMemory | RshInitDAC | RshInitVoltmeter | RshInitPort | RshInitGSPF
	 *
	 */
	virtual U32    __RSHCALLCONV  Init( IN OUT RshBaseType* structure, IN U32 mode = RSH_INIT_MODE_INIT ) = 0;

	/*!
	 *
	 * \~english
	 * \brief
	 * Start data acquisition or signal generation process.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS or error code.
	 *
	 * ADC devices start data acquisition
	 * and transfer when this method is called.
	 * After data transfer complete, data ready event
	 * will be signalled and data can be collected using
	 * GetData() method.\n
	 * Signal generators (GSPF family for instance) start
	 * buffer translation on this command. So, for them command order
	 * is reverced: one need first to transfer data buffer using
	 * GetData() method and only after that start generation with Start().
	 *
	 * \remarks
	 * Process of starting device is not immidiate,
	 * some time required for transfer command and process it.\n
	 * Moreover, if synchronization of some kind used,
	 * device only start actual data acquisition process
	 * when synchronization event will be triggered.
	 *
	 * \see
	 * RSH_GET_WAIT_BUFFER_READY_EVENT |  \link page_Synchronization Synchronization \endlink
	 *
	 * \~russian
	 * \brief
	 * Запуск процесса сбора данных или генерации сигнала.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS или код ошибки.
	 *
	 * Устройства сбора данных начинают процесс сбора
	 * и передачи данных, когда вызывается этот метод.
	 * После завершения передачи ставится флаг готовности данных,
	 * и данные можно получить с помощью метода GetData() \n
	 * Генераторы сигнала (серия ГСПФ) начинают проигрование
	 * буфера по этой команде. Таким образом, для генераторов
	 * порядок команд будет обратным: сначала нужно передать подготовленный
	 * буфер с данными с помощью метода GetData() и только затем
	 * запустить генарацию, вызвав Start().
	 *
	 * \remarks
	 * Процесс запуска происходит не мгновенно,
	 * требуется некоторое время на передачу команды
	 * и подготовку устройства.\n
	 * Более того, если используется синхронизация в каком-либо виде,
	 * непосредственно процесс сбора данных начнется только
	 * после получения события синхронизации.
	 *
	 * \see
	 * RSH_GET_WAIT_BUFFER_READY_EVENT |  \link page_Synchronization Синхронизация\endlink
	 *
	 */
	virtual U32    __RSHCALLCONV  Start() = 0;

	/*!
	 *
	 * \~english
	 * \brief
	 * Stop data acquisition or signal generation process.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS or error code.
	 *
	 * When this method is called, all data connected
	 * activity is terminated: continious data transfer
	 * is cancelled, ADC (or DAC) is stopped, USB pipes
	 * and DMA channels are reseted to default state and ready
	 * to next Start() call.
	 *
	 * \remarks
	 * If this method is called before any actual data transfer
	 * commited, Get() method with RSH_GET_WAIT_BUFFER_READY_EVENT
	 * parameter will return error code as result.
	 *
	 * \see
	 * RSH_GET_WAIT_BUFFER_READY_EVENT
	 *
	 * \~russian
	 * \brief
	 * Остановка процесса сбора данных или генерации сигнала.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS или код ошибки.
	 *
	 * При вызове этого метода вся активность, связанная со сбором
	 * или генерацией прекращается: останавливается АЦП (ЦАП), сбрасывается
	 * процесс передачи по USB, ПДП каналы возвращаются в исходное состояние.
	 * Устройство готово к следующему вызову метода Start().
	 *
	 * \remarks
	 * Если данный метод вызывается до того, как была успешно завершена
	 * передача блока данных (размер блока определяется настройками сбора
	 * в структуре инициализации), метод Get() с параметром RSH_GET_WAIT_BUFFER_READY_EVENT
	 * вернет код ошибки в качестве результата работы.
	 *
	 * \see
	 * RSH_GET_WAIT_BUFFER_READY_EVENT
	 *
	 */
	virtual U32    __RSHCALLCONV  Stop() = 0;

	/*!
	 *
	 * \~english
	 * \brief
	 * Get (or send) data buffer to device.
	 *
	 * \param [in,out] buffer
	 * Buffer for (with) data.
	 * One of the RshBufferType simple type classes.\n
	 * ::RSH_BUFFER_S8, ::RSH_BUFFER_S16, ::RSH_BUFFER_S32 are used
	 * in most cases for transfer converted by ADC data.
	 * Data is always shifted so MSB will be first (left) bit.\n
	 * ::RSH_BUFFER_DOUBLE can be used too, for transfer
	 * data converted to volts.\n
	 * User must allocate memory for data he wants to transfer before
	 * GetData() Call! One can do so with RshBufferType::Allocate method.
	 *
	 * \param [in] flags
	 * Additional flags for data conversation.
	 * Can be used, for example, to get acces to digital lines
	 * at the same time with data acquisition and write this
	 * information with data. Flag list: ::RSH_DATA_MODES.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS or error code.
	 *
	 * This method is used to transfer data from (or to)
	 * device abstraction library. After receiving ::RSH_GET_WAIT_BUFFER_READY_EVENT
	 * data is ready for transfer. One must allocate buffer. If allocated buffer size
	 * is less than total data available, buffer will be filled with data that fit
	 * into it, no error will be thrown. If the opposite true, its no problem also.
	 * One can check actual data transfered to buffer looking to RshBufferType::size value.
	 *
	 * \remarks
	 * When selecting buffer size in initialization structure its always per channel value.
	 * So, for example, one set buffer size in initialization structure to 2048 and number
	 * of channels to 2, one must expect total data available in GetData() method
	 * to be equal to 2*2048 = 4096 samples.
	 *
	 * \see
	 * RshBufferType | RSH_DATA_MODES
	 *
	 * \~russian
	 * \brief
	 * Получение (или отправка) буфера с данными.
	 *
	 * \param [in, out] buffer
	 * Буфер для данных (или буфер с данными при генерации сигнала).
	 * Одна из реализаций шаблона RshBufferType для простых типов данных.\n
	 * Чаще всего для передачи данных используются типы буфера
	 * ::RSH_BUFFER_S8, ::RSH_BUFFER_S16, ::RSH_BUFFER_S32.
	 * Данные всегда приводятся к старшему биту (исключение - специальные
	 * дополнительные флаги преобразования данных, см. \b flags).\n
	 * Кроме того, иногда удобно использовать ::RSH_BUFFER_DOUBLE,
	 * в котором данные будут сразу в вольтах, а не в МЗР.\n
	 * Пользователь должен сам выделить память под данные перед
	 * вызовом GetData(). Это можно сделать с помощью метода RshBufferType::Allocate.
	 *
	 * \param [in] flags
	 * Дополнительные флаги преобразования данных. Могут использоваться, например,
	 * для того, чтобы получить сигналы с цифровых линий одновременно с данными
	 * аналоговых каналов. Полный список флагов: ::RSH_DATA_MODES.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS или код ошибки.
	 *
	 * Этот метод используется для передачи данных из (или к) библиотеке абстракции устройства.
	 * После получения события ::RSH_GET_WAIT_BUFFER_READY_EVENT данные готовы (уже находятся в компьютере,
	 * в памяти библиотеки абстракции). Можно забрать их в свою программу, предварительно выделив память
	 * в объекте RshBufferType. Если размер выделенного буфера окажется меньше, чем размер доступных данных,
	 * ничего страшного не произойдет - буфер будет заполнен полностью. В обратной ситуации (когда размер
	 * выделенного буфера больше чем размер доступных данных) в буфер будет переложено столько данных,
	 * сколько есть в наличии. В поле RshBufferType::size после вызова GetData() содержится фактический
	 * размер переданных данных.\n
	 * Все размеры всегда в словах (отсчетах АЦП).
	 *
	 * \remarks
	 * В структурах инициализации размер буфера всегда привязан к каналу.
	 * Если, к примеру, при инициализации был задан размер 2048 отсчетов, и выбрано 2 канала,
	 * следует ожидать 2048*2 = 4096 отсчетов при вызове метода GetData().
	 *
	 * \see
	 * RshBufferType | RSH_DATA_MODES
	 *
	 */
	virtual U32    __RSHCALLCONV  GetData( IN OUT RshBaseType* buffer, IN U32 flags=RSH_DATA_MODE_NO_FLAGS) = 0;

	/*!
	 *
	 * \~english
	 * \brief
	 * Access to different information about device and abstraction library.
	 *
	 * \param [in] mode
	 * One of ::RSH_GET constants. Depending on constant,
	 * behavior of method can vary, see corresponding
	 * constant description.
	 *
	 * \param [in, out] adr
	 * Address of one RshBaseType dirived data object.
	 * Depends on \b mode parameter used. Can be NULL
	 * in some cases.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS or error code.
	 *
	 * This method used to perform wide range of
	 * activities due to its flexibility. With it one
	 * can get various static and non-static information
	 * about device and abstraction library, and also control
	 * some device activities.
	 *
	 * \remarks
	 * Always check method return value to be sure that
	 * you actually got infromation you need.\n
	 * In most cases, passed data object will be
	 * changed and filled with data you required,
	 * but sometimes data is passed to device (or library).
	 * In these cases, ::RSH_GET constant have \b _SET suffix,
	 * and data must be prepared \a before method call.
	 *
	 * \see
	 * RSH_GET
	 *
	 * \~russian
	 * \brief
	 * Получение различной информации об устройстве и библиотеке абстракции.
	 *
	 * \param [in] mode
	 * Одна из констант списка ::RSH_GET. В зависимости от используемой
	 * константы, действие метода будет меняться. Все необходимые сведения
	 * приведены в описании конкретной константы.
	 *
	 * \param [in, out] adr
	 * Адрес объекта, унаследованного от класса RshBaseType.
	 * Какой конкретно тип будет использоваться, зависит от
	 * используемого режима (параметр \b mode ). В некоторых
	 * случаях можно передавать NULL.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS или код ошибки.
	 *
	 * Данный метод используется для выполнения широкого спектра
	 * действий ввиду его гибкости. С помощью этого метода
	 * можно получать информацию (как статическую, так и динамическую)
	 * об устройстве и библиотеке абстракции, а также управлять некоторыми
	 * функциями библиотеки и устройства.
	 *
	 * \remarks
	 * Всегда проверяйте возвращаемое методом значение, чтобы быть
	 * уверенными в том, что полученная информация актуальна.\n
	 * В большинстве случаев, передаваемый в метод объект будет модифицирован,
	 * и после вызова метода будет содержать запрошенные данные.
	 * Но в некоторых случаях данные отправляются в устройство (или библиотеку
	 * абстракции). В этих случаях, константы из списка ::RSH_GET имеют
	 * суффикс \b _SET, и данные должны быть подготовлены \a до вызова метода.
	 *
	 *
	 * \see
	 * RSH_GET
	 *
	 */
	virtual U32    __RSHCALLCONV  Get( IN U32 mode, IN OUT RshBaseType* adr=NULL) = 0;
};
/*----------------------------------------------------------------------*/
/*!
 *
 * \~english
 * \brief
 * Identification key of a device
 *
 * This structure is used in IRshDevice::Connect() method.
 * It allows to connect to corresponding device via driver.\n
 *
 * \~russian
 * \brief
 * Идентификационный ключ устройства
 *
 * Данная структура используется в методе IRshDevice::Connect()
 * для получения доступа через драйвер к конкретному устройству.\n
 * Характеризующий устройство ключ (например, базовый адрес,
 * или заводской номер, или ip адрес с портом в виде строки)
 * заносится в структуру, которая затем передается в качестве параметра
 * в метод IRshDevice::Connect() .\n
 * Для заполнения структуры используется конструктор нужного типа.\n
 * Пример:\n
 *  \code
 * IRshDevice* device;
 * device->Connect(&RshDeviceKey(1)); //подключение по базовому адресу
 * \endcode
 *
 *
 */
struct RshDeviceKey : public RshBaseType
{
	U32    storedTypeId;
	U32    value_U32;
	S8*    value_S8P;
	U16*   value_U16P;
    RshDeviceKey();
	RshDeviceKey(U32 value);
	RshDeviceKey(S8* value);
	RshDeviceKey(U16* value);
};

/*!
 *
 * \~english
 * \brief
 * Analog channel parameters
 *
 * Using this structure one can
 * setup parameters of analog input channel.\n
 * Usually this structure is used as an element
 * in RshBufferType list of channels (::RSH_BUFFER_CHANNEL),
 * passed to IRshDevice::Init() within initialization
 * structure.
 *
 * \~russian
 * \brief
 * Параметры аналогового канала
 *
 * Используя данную структуру, можно задать параметры
 * аналогового входного канала.\n
 * Обычно, данная структура используется как элемент
 * списка каналов RshBufferType (::RSH_BUFFER_CHANNEL),
 * передаваемый с другими данными (в структуре инициализации)
 * в метод IRshDevice::Init().
 *
 */
struct RshChannel : public RshBaseType
{
	/*!
	 *
	 * \~english
	 * \brief
	 * Gain coefficient
	 *
	 * Gain coefficient to use for this channel
	 * in data acquisition process.
	 *
	 * \remarks
	 * One can get list of availiable gain coefficients,
	 * using IRshDevice::Get() method with
	 * ::RSH_GET_DEVICE_GAIN_LIST parameter.
	 *
	 * \~russian
	 * \brief
	 * Коэффициент усиления
	 *
	 * Коэффициент усиления, который будет использован
	 * для данного канала при сборе данных
	 *
	 * \remarks
	 * Получить список доступных коэффициентов усиления
	 * можно, вызвав метод IRshDevice::Get() с параметром
	 * ::RSH_GET_DEVICE_GAIN_LIST.
	 *
	 */
	U32 gain;

	/*!
	 *
	 * \~english
	 * \brief
	 * Additional flags
	 *
	 * 'OR' Combination of RshChannel::Control
	 * enum elements. If flag set is not
	 * supported by device, it will be ignored.
	 *
	 * \remarks
	 * By default, this parameter is equal to zero,
	 * which corresponds to following flag combination:\n
	 * <b>RshChannel::NotUsed | RshChannel::NoSynchro | RshChannel::Resist1MOhm | RshChannel::DC | RshChannel::ICPPowerOff</b>
	 *
	 * \~russian
	 * \brief
	 * Дополнительные флаги
	 *
	 * Комбинация по 'ИЛИ' элементов перечисления
	 * RshChannel::Control. Если установить флаг,
	 * который не поддерживается данным устройством,
	 * он будет проигнорирован (при этом, ошибки не возникнет).
	 *
	 * \remarks
	 * По умолчанию, данный параметр равен нулю, что
	 * соответствует следующей комбинации флагов:\n
	 * <b>RshChannel::NotUsed | RshChannel::NoSynchro | RshChannel::Resist1MOhm | RshChannel::DC | RshChannel::ICPPowerOff</b>
	 *
	 */
	U32 control;

	/*!
	 *
	 * \~english
	 * \brief
	 * Input voltage adjustment
	 *
	 * Input level adjustment to use
	 * in data acquisition for this channel.
	 * This parameter is set in volts.
	 *
	 * \see
	 * RSH_CAPS_DEVICE_INPUT_LEVEL_ADJUSTMENT
	 *
	 * \~russian
	 * \brief
	 * Подстройка уровня входного напряжения
	 *
	 * Подстройка (смещение) уровня входного напряжения
	 * для данного канала. Данный параметр задается в вольтах.
	 *
	 * \see
	 * RSH_CAPS_DEVICE_INPUT_LEVEL_ADJUSTMENT
	 *
	 */
	double adjustment;


	/*!
	 *
	 * \~english
	 * \brief
	 * Channel flag list
	 *
	 * Additional channel parameters that
	 * can be passed in RshChannel::control field.
	 *
	 * \~russian
	 * \brief
	 * Список опций для канала
	 *
	 * Дополнительные параметры канала, которые могут быть
	 * заданы в поле RshChannel::control.
	 *
	 */
	enum Control
	{
		/*!
		 *
		 * \~english
		 * \brief
		 * Channel will NOT be used in data acquisition
		 *
		 * Mutually exclusive with RshChannel::Used
		 *
		 * \~russian
		 * \brief
		 * Канал НЕ будет использоваться при сборе данных
		 *
		 * Взаимоисключающий с RshChannel::Used
		 *
		 */
		NotUsed = 0x0,

		/*!
		 *
		 * \~english
		 * \brief
		 * Channel will NOT be used as trigger source
		 *
		 * Mutually exclusive with RshChannel::Synchro
		 *
		 * \~russian
		 * \brief
		 * Канал НЕ будет использоваться в качестве источника синхронизации
		 *
		 * Взаимоисключающий с RshChannel::Synchro
		 *
		 */
		NoSynchro = 0x0,

		/*!
		 *
		 * \~english
		 * \brief
		 * Set 1MOhm resist for channel
		 *
		 * Mutually exclusive with RshChannel::Resist50Ohm
		 *
		 * \~russian
		 * \brief
		 * Установить входное сопротивление канала 1МОм
		 *
		 * Взаимоисключающий с RshChannel::Resist50Ohm
		 *
		 */
		Resist1MOhm	= 0x0,

		/*!
		 *
		 * \~english
		 * \brief
		 * Set input to DC mode
		 *
		 * Mutually exclusive with RshChannel::AC
		 *
		 * \~russian
		 * \brief
		 * Установить состояние входа "открытый"
		 *
		 * Взаимоисключающий с RshChannel::AC
		 *
		 */
		DC = 0x0,

		/*!
		 *
		 * \~english
		 * \brief
		 * Turn OFF ICP power supply for channel
		 *
		 * Mutually exclusive with RshChannel::ICPPowerOn
		 *
		 * \~russian
		 * \brief
		 * Отключение ICP питания канала
		 *
		 * Взаимоисключающий с RshChannel::ICPPowerOn
		 *
		 */
		ICPPowerOff	= 0x0,

		/*!
		 *
		 * \~english
		 * \brief
		 * Channel will be used in data acquisition
		 *
		 * Mutually exclusive with RshChannel::NotUsed
		 *
		 * \~russian
		 * \brief
		 * Канал будет использоваться при сборе данных
		 *
		 * Взаимоисключающий с RshChannel::NotUsed
		 *
		 */
		Used = 0x1,

		/*!
		 *
		 * \~english
		 * \brief
		 * Channel will be used as trigger source
		 *
		 * Mutually exclusive with RshChannel::NoSynchro
		 *
		 * \~russian
		 * \brief
		 * Канал будет использоваться в качестве источника синхронизации
		 *
		 * Взаимоисключающий с RshChannel::NoSynchro
		 *
		 */
		Synchro = 0x2,

		/*!
		 *
		 * \~english
		 * \brief
		 * Set input to AC mode
		 *
		 * Mutually exclusive with RshChannel::DC
		 *
		 * \~russian
		 * \brief
		 * Установить состояние входа "закрытый"
		 *
		 * Взаимоисключающий с RshChannel::DC
		 *
		 */
		AC = 0x4,

		/*!
		 *
		 * \~english
		 * \brief
		 * Set 50Ohm resist for channel
		 *
		 * Mutually exclusive with RshChannel::Resist1MOhm
		 *
		 * \~russian
		 * \brief
		 * Установить входное сопротивление канала 50Ом
		 *
		 * Взаимоисключающий с RshChannel::Resist1MOhm
		 *
		 */
		Resist50Ohm	= 0x8,

		/*!
		 *
		 * \~english
		 * \brief
		 * Turn On ICP power supply for channel
		 *
		 * Mutually exclusive with RshChannel::ICPPowerOff
		 *
		 * \~russian
		 * \brief
		 * Включение ICP питания канала
		 *
		 * Взаимоисключающий с RshChannel::ICPPowerOff
		 *
		 */
		ICPPowerOn = 0x10,

		/*!
		 *
		 * \~english
		 * \brief
		 * Set channel as first channel to analog-digital conversation
		 *
		 * Not supported by most devices.
		 *
		 * \~russian
		 * \brief
		 * Выбрать канал в качестве первого канала для оцифровки
		 *
		 * Большинство устройств не поддерживает данную опцию.
		 *
		 */
		FirstChannel = 0x10000,
	};

	RshChannel();

	RshChannel& operator=(RshChannel& obj);

	bool operator==(RshChannel& obj);

	bool operator!=(RshChannel& obj);

	bool operator< (RshChannel& obj);

	bool operator> (RshChannel& obj);

	friend std::ostream& operator<< (std::ostream &out,  RshChannel& obj);
};

/*!
 *
 * \~english
 * \brief
 * RshBufferType of RshChannel structures
 *
 * Default size is equal to ::RSH_MAX_CHANNEL_NUMBER
 *
 * \~russian
 * \brief
 * Буфер RshBufferType из структур RshChannel
 *
 * Размер списка по умолчанию равен ::RSH_MAX_CHANNEL_NUMBER
 *
 */
typedef RshBufferType< RshChannel, rshBufferTypeChannel > RSH_BUFFER_CHANNEL;


/*!
 *
 * \~english
 * \brief
 * External trigger channel parameters
 *
 * This structure is similar to RshChannel,
 * but used to setup external trigger channel.
 *
 * \~russian
 * \brief
 * Параметры канала внешней синхронизации
 *
 * Данная структура похожа на структуру RshChannel,
 * но используется для настройки параметров
 * канала внешней синхронизации.
 *
 */
struct RshSynchroChannel : public RshBaseType
{
	/*!
	 *
	 * \~english
	 * \brief
	 * Gain coefficient
	 *
	 * Gain coefficient to use for
	 * external trigger channel.
	 *
	 * \remarks
	 * One can get list of availiable gain coefficients,
	 * using IRshDevice::Get() method with
	 * ::RSH_GET_DEVICE_EXT_SYNC_GAINLIST  parameter.
	 *
	 * \~russian
	 * \brief
	 * Коэффициент усиления
	 *
	 * Коэффициент усиления, который будет использован
	 * для канала внешней синхронизации
	 *
	 * \remarks
	 * Получить список доступных коэффициентов усиления
	 * можно, вызвав метод IRshDevice::Get() с параметром
	 * ::RSH_GET_DEVICE_EXT_SYNC_GAINLIST.
	 *
	 */
	U32 gain;

	/*!
	 *
	 * \~english
	 * \brief
	 * Additional flags
	 *
	 * 'OR' Combination of RshSynchroChannel::Control
	 * enum elements. If flag set is not
	 * supported by device, it will be ignored.
	 *
	 * \remarks
	 * By default, this parameter is equal to zero,
	 * which corresponds to following flag combination:\n
	 * <b>RshSynchroChannel::FilterOff | RshSynchroChannel::Resist1MOhm | RshSynchroChannel::DC</b>
	 *
	 * \~russian
	 * \brief
	 * Дополнительные флаги
	 *
	 * Комбинация по 'ИЛИ' элементов перечисления
	 * RshSynchroChannel::Control. Если установить флаг,
	 * который не поддерживается данным устройством,
	 * он будет проигнорирован (при этом, ошибки не возникнет).
	 *
	 * \remarks
	 * По умолчанию, данный параметр равен нулю, что
	 * соответствует следующей комбинации флагов:\n
	 * <b>RshSynchroChannel::FilterOff | RshSynchroChannel::Resist1MOhm | RshSynchroChannel::DC</b>
	 *
	 */
	U32 control;

	/*!
	 *
	 * \~english
	 * \brief
	 * Channel flag list
	 *
	 * Additional channel parameters that
	 * can be passed in RshSynchroChannel::control field.
	 *
	 * \~russian
	 * \brief
	 * Список опций для канала
	 *
	 * Дополнительные параметры канала внешней синхронизации, которые могут быть
	 * заданы в поле RshSynchroChannel::control.
	 *
	 */
	enum Control
	{
		/*!
		 *
		 * \~english
		 * \brief
		 * Turn OFF filters for external trigger channel
		 *
		 * Mutually exclusive with RshSynchroChannel::FilterLow and RshSynchroChannel::FilterHigh
		 *
		 * \~russian
		 * \brief
		 * Выключить фильтр для канала внешней синхронизации
		 *
		 * Взаимоисключающий с RshSynchroChannel::FilterLow и RshSynchroChannel::FilterHigh
		 *
		 */
		FilterOff	= 0x0,

		/*!
		 *
		 * \~english
		 * \brief
		 * Set 1MOhm resist for external trigger channel
		 *
		 * Mutually exclusive with RshSynchroChannel::Resist50Ohm
		 *
		 * \~russian
		 * \brief
		 * Установить входное сопротивление канала внешней синхронизации 1МОм
		 *
		 * Взаимоисключающий с RshSynchroChannel::Resist50Ohm
		 *
		 */
		Resist1MOhm	= 0x0,

		/*!
		 *
		 * \~english
		 * \brief
		 * Set input to DC mode
		 *
		 * Mutually exclusive with RshSynchroChannel::AC
		 *
		 * \~russian
		 * \brief
		 * Установить состояние входа "открытый"
		 *
		 * Взаимоисключающий с RshSynchroChannel::AC
		 *
		 */
		DC = 0x0,

		/*!
		 *
		 * \~english
		 * \brief
		 * Turn on low pass filter for external trigger channel
		 *
		 * Mutually exclusive with RshSynchroChannel::FilterOff and RshSynchroChannel::FilterHigh
		 *
		 * \see
		 * RSH_CAPS_DEVICE_EXT_SYNC_FILTER_LOW
		 *
		 * \~russian
		 * \brief
		 * Включить фильтр низких частот для канала внешней синхронизации.
		 *
		 * Взаимоисключающий с RshSynchroChannel::FilterOff и RshSynchroChannel::FilterHigh
		 *
		 * \see
		 * RSH_CAPS_DEVICE_EXT_SYNC_FILTER_LOW
		 *
		 */
		FilterLow = 0x1,

		/*!
		 *
		 * \~english
		 * \brief
		 * Turn on high pass filter for external trigger channel
		 *
		 * Mutually exclusive with RshSynchroChannel::FilterOff and RshSynchroChannel::FilterLow
		 *
		 * \see
		 * RSH_CAPS_DEVICE_EXT_SYNC_FILTER_HIGH
		 *
		 * \~russian
		 * \brief
		 * Включить фильтр высоких частот для канала внешней синхронизации
		 *
		 * Взаимоисключающий с RshSynchroChannel::FilterOff и RshSynchroChannel::FilterLow
		 *
		 * \see
		 * RSH_CAPS_DEVICE_EXT_SYNC_FILTER_HIGH
		 *
		 */
		FilterHigh = 0x2,

		/*!
		 *
		 * \~english
		 * \brief
		 * Set input to AC mode
		 *
		 * Mutually exclusive with RshSynchroChannel::DC
		 *
		 * \~russian
		 * \brief
		 * Установить состояние входа "закрытый"
		 *
		 * Взаимоисключающий с RshSynchroChannel::DC
		 *
		 */
		AC = 0x4,

		/*!
		 *
		 * \~english
		 * \brief
		 * Set 50Ohm resist for channel
		 *
		 * Mutually exclusive with RshSynchroChannel::Resist1MOhm
		 *
		 * \~russian
		 * \brief
		 * Установить входное сопротивление канала 50Ом
		 *
		 * Взаимоисключающий с RshSynchroChannel::Resist1MOhm
		 *
		 */
		Resist50Ohm	= 0x8
	};

	RshSynchroChannel();

	RshSynchroChannel& operator=(RshSynchroChannel& obj);

	bool operator==(RshSynchroChannel& obj);

	bool operator!=(RshSynchroChannel& obj);
};


/*!
 *
 * \~english
 * \brief
 * Base initialization structure for ADC devices
 *
 * This structure contains parameters,
 * common for all ADC devices.\n
 * It is never used in real device libraries,
 * just as parent for RshInitDMA and
 * RshInitMemory structures.
 *
 * \see
 * RshInitDMA | RshInitMemory
 *
 * \~russian
 * \brief
 * Базовая структура инициализации для АЦП устройств
 *
 * В этой структуре содержатся параметры инициализации,
 * общие для всех АЦП устройств, такие как размер буфера,
 * частота дискретизации и др. \n
 * Данная структура сама по себе не используется ни
 * в одной из библиотек абстракции устройств - только
 * в качестве предка для структур RshInitDMA и RshInitMemory.
 *
 * \see
 * RshInitDMA | RshInitMemory
 *
 */
struct RshInitADC : public RshBaseType
{
	/*!
	 *
	 * \~english
	 * \brief
	 * Data acquisition start mode
	 *
	 * One of RshInitADC::StartType enum
	 * constants, that determine, how
	 * ADC will be started.
	 *
	 * \~russian
	 * \brief
	 * Режим запуска сбора данных
	 *
	 * Одна из констант перечисления
	 * RshInitADC::StartType, которая определяет,
	 * как будет запускаться сбор данных.
	 *
	 */
	U32 startType;

	/*!
	 *
	 * \~english
	 * \brief
	 * Buffer size in samples
	 *
	 * Size of buffer need to acquire, in samples
	 * per channel.
	 *
	 * \remarks
	 * When using more than one channel in data
	 * acquisition, be sure to set correct size
	 * for IRshDevice::GetData() method. It will be
	 * equal to RshInitADC::bufferSize*N,
	 * where N is used channels number.
	 *
	 * \see
	 * IRshDevice::GetData()
	 *
	 * \~russian
	 * \brief
	 * Размер буфера в отсчетах
	 *
	 * Размер буфера данных, в отсчетах на канал.
	 *
	 * \remarks
	 * Размер задается на канал, т.е. при использовании
	 * нескольких каналов фактический размер собранных
	 * данных будет пропорционален числу каналов.\n
	 * Это надо учитывать при вызове метода IRshDevice::GetData().
	 * Если число используемых каналов равно N,
	 * при получении данных в метод IRshDevice::GetData()
	 * нужно передать размер, равный RshInitADC::bufferSize*N.
	 *
	 * \see
	 * IRshDevice::GetData()
	 *
	 */
	U32 bufferSize;

	/*!
	 *
	 * \~english
	 * \brief
	 * Sampling rate in Hz
	 *
	 * Sampling rate in Hz per channel.
	 * As for RshInitADC::bufferSize, this
	 * setting is always bound to channel.
	 *
	 * \remarks
	 * For devices that use multiplexor, maximum
	 * frequency depends on number of channels used.\n
	 * Always check sampling and size values after
	 * IRshDevice::Init() call to be sure which
	 * values are actually set.
	 *
	 * \see
	 * RSH_CAPS_DEVICE_SYNCHRO_CHANNELS | RSH_INIT_MODE_CHECK
	 *
	 * \~russian
	 * \brief
	 * Частота дискретизации в Гц
	 *
	 * Частота дискретизации в Гц на канал.
	 * Также как и размер RshInitADC::bufferSize,
	 * эта настройка привязывается к каналу.
	 *
	 * \remarks
	 * Для устройств с мультиплексированными каналами,
	 * максимальная частота дискретизации зависит
	 * от числа используемых каналов.\n
	 * Всегда проверяйте поле частоты дискретизации и
	 * размера буфера в стркутуре после вызова
	 * метода IRshDevice::Init() чтобы узнать,
	 * какие параметры были фактически установлены
	 * при инициализации.
	 *
	 * \see
	 * RSH_CAPS_DEVICE_SYNCHRO_CHANNELS | RSH_INIT_MODE_CHECK
	 *
	 */
	double frequency;

	/*!
	 *
	 * \~english
	 * \brief
	 * Analog channels settings
	 *
	 * List of RshChannel structures.
	 * Number of channels is set to  ::RSH_MAX_CHANNEL_NUMBER.
	 * If device have less than that value (for most devices
	 * its true), channels past actual device number of channels
	 * are ignored.
	 *
	 * \remarks
	 * You can get device channel number using
	 * IRshDevice::Get() method with ::RSH_GET_DEVICE_NUMBER_CHANNELS
	 * parameter.
	 *
	 * \see
	 * RshChannel
	 *
	 * \~russian
	 * \brief
	 * Настройки аналоговых каналов
	 *
	 * Список структур RshChannel.
	 * Количество структур в списке равно ::RSH_MAX_CHANNEL_NUMBER.
	 * Если устройство имеет меньшее число каналов (это справедливо
	 * для большинства устройств), используется только часть
	 * списка, отвечающая фактическому количеству каналов.
	 *
	 * \remarks
	 * Узнать количество каналов устройства можно, вызвав
	 * метод IRshDevice::Get() с параметром ::RSH_GET_DEVICE_NUMBER_CHANNELS.
	 *
	 * \see
	 * RshChannel
	 *
	 */
	RSH_BUFFER_CHANNEL channels;

	/*!
	 *
	 * \~english
	 * \brief
	 * Start mode list
	 *
	 * This enum entities can be used
	 * to specify how to start ADC process.\n
	 * To do so, set one of them as value
	 * for RshInitADC::startType field.
	 *
	 * \see
	 * \link page_Synchronization Synchronization \endlink
	 *
	 * \~russian
	 * \brief
	 * Список режимов запуска
	 *
	 * Элементы этого перечисления используются
	 * для задания режима запуска АЦП.\n
	 * Чтобы выбрать режим, нужно установить
	 * одно из этих значений в поле RshInitADC::startType.
	 *
	 * \see
	 * \link page_Synchronization Синхронизация \endlink
	 *
	 */
	enum  StartType
	{
		/*!
		 *
		 * \~english
		 * \brief
		 * Program start
		 *
		 * Conversation will start as soon as
		 * command will be acquired by the device.\n
		 * That means, ADC will start as soon as
		 * IRshDevice::Start() method is called.
		 *
		 * \remarks
		 * "As soon" is not immediately, because
		 * there is always some delays for functions calls,
		 * data transfer and device responce.
		 *
		 * \~russian
		 * \brief
		 * Программный запуск
		 *
		 * Преобразование будет запущено сразу же
		 * после получения устройством команды на запуск.\n
		 * Это означает, что АЦП будет запущено сразу же после
		 * вызова метода IRshDevice::Start().
		 *
		 * \remarks
		 * "Сразу же" на самом деле происходит не мгновенно, так
		 * как всегда есть задержки - вызов функций, передача
		 * данных по шине, отклик устройства.
		 *
		 */
		Program = 0x1,

		/*!
		 *
		 * \~english
		 * \brief
		 * Timer start
		 *
		 * Start data acquisition process
		 * using internal timer.
		 *
		 * \deprecated
		 * This flag is not used in any library at the moment.
		 * In SDK1 some devices used Program mode, some Timer mode
		 * for the same thing. May be this flag will be removed in
		 * the future.
		 *
		 * \~russian
		 * \brief
		 * Запуск по таймеру
		 *
		 * Запуск преобразования по внутреннему таймеру.
		 *
		 * \deprecated
		 * На текущий момент данный режим не используется ни в одной
		 * библиотеке абстракции устройства. В SDK1 некоторые устройства
		 * использовали флаг Program, другие Timer для одного и того же
		 * режима - запуск преобразования по вызову команды IRshDevice::Start().\n
		 * Возможно, данный флаг будет удален в будущих релизах.
		 *
		 * \todo
		 * Найти применение данному режиму или исключить его совсем?
		 *
		 */
		Timer = 0x2,

		/*!
		 *
		 * \~english
		 * \brief
		 * Start data acquisition on external trigger
		 *
		 * Use external trigger input as source for
		 * data acquisition start command.
		 *
		 * \remarks
		 * There are two different mechanisms that can
		 * be activated by this flag: using external
		 * trigger input (see ::RSH_CAPS_DEVICE_SYNCHRO_EXTERNAL)
		 * or external start input (see ::RSH_CAPS_DEVICE_EXTERNAL_START).
		 *
		 * \~russian
		 * \brief
		 * Внешняя синхронизации (или внешний запуск)
		 *
		 * Использовать внешний источник
		 * для запуска преобразования.\n
		 * В качестве внешнего источнка может выступать либо
		 * вход внешней синхронизации, либо вход внешнего запуска.
		 *
		 * \remarks
		 * Существуют два различных механизма, которые включаются
		 * данным флагом: использование входа внешней синхронизации
		 * (как правило, данный режим есть у "Memory" устройств -
		 * см. ::RSH_CAPS_DEVICE_SYNCHRO_EXTERNAL) или использование
		 * внешнего старта (как правило, данный режим есть у "DMA"
		 * устройств - см. ::RSH_CAPS_DEVICE_EXTERNAL_START).
		 *
		 */
		External = 0x4,

		/*!
		 *
		 * \~english
		 * \brief
		 * Internal synchronization
		 *
		 * Use analog input channels as source
		 * for trigger to start ADC.
		 *
		 * \remarks
		 * One can check if device supports this mode
		 * using ::RSH_CAPS_DEVICE_SYNCHRO_INTERNAL.\n
		 * Usually, this feature is used by "Memory" devices.
		 *
		 * \see
		 * RshChannel::Synchro | RshInitMemory::threshold
		 *
		 * \~russian
		 * \brief
		 * Внутренняя синхронизация
		 *
		 * Использование одного из аналоговых входов
		 * в качестве источника сигнала для запуска АЦП.
		 *
		 * \remarks
		 * Используйте ::RSH_CAPS_DEVICE_SYNCHRO_INTERNAL
		 * чтобы проверить, поддерживает ли устройство
		 * данный режим или нет.\n
		 * Как правило, данная возможность поддерживается
		 * устройствами типа "Memory".
		 *
		 * \see
		 * RshChannel::Synchro | RshInitMemory::threshold
		 *
		 */
		Internal = 0x8,

		/*!
		 *
		 * \~english
		 * \brief
		 * Use external frequency source for sampling
		 *
		 * External frequency will be used for
		 * sampling ADC. See ::RSH_CAPS_DEVICE_EXTERNAL_FREQUENCY
		 * for more details.
		 *
		 * \~russian
		 * \brief
		 * Использование внешней частоты дискретизации
		 *
		 * АЦП будет сэмплироваться от внешнего источника частоты
		 * дискретизации. См. ::RSH_CAPS_DEVICE_EXTERNAL_FREQUENCY
		 *
		 * \todo
		 * Напрашивается перенос данного поля в control, возможно нужно
		 * объединить контролы из RshInitMemory и RshInitDMA и перенести
		 * все флаги в RshInitADC
		 */
		FrequencyExternal = 0x10,

		/*!
		 *
		 * \~english
		 * \brief
		 * Use master device as start trigger source
		 *
		 * This flag may be used in systems where
		 * two or more devieces are started synchronously
		 * from one source. One device usually is master
		 * device, and all other are slave devices.
		 *
		 * \see
		 * RSH_CAPS_DEVICE_SLAVE_MASTER_SWITCH
		 *
		 * \~russian
		 * \brief
		 * Запуск от ведущего устройства
		 *
		 * Актуально для систем, где несколько устройств
		 * запускаются синхронно. Одно из устройств
		 * выступает в качестве ведущего, все остальные -
		 * в качестве ведомых.
		 *
		 * \see
		 * RSH_CAPS_DEVICE_SLAVE_MASTER_SWITCH
		 *
		 */
		Master = 0x20
	};

	/*!
	 *
	 * \~english
	 * \brief
	 * Synchronization threshold
	 *
	 * Trigger threshold in volts.
	 * This parameter is actual, when device
	 * working in synchronization mode
	 * (RshInitDMA::startType is set to
	 * RshInitDMA::Internal or RshInitDMA::External).\n
	 * When sygnal will bypass threshold, synchronization event
	 * will be triggered, and device start acquire and transfer data.
	 *
	 * \remarks
	 * When synchronization is turned off, this
	 * field is ignored.
	 *
	 * \see
	 * \link page_Synchronization Synchronization \endlink
	 *
	 * \~russian
	 * \brief
	 * Уровень синхронизации
	 *
	 * Уровень синхронизации в вольтах.
	 * Данный параметр актуален, когда устройство
	 * работает в режиме синхронизации (т.е. тип запуска
	 * RshInitDMA::startType установлен либо RshInitDMA::Internal,
	 * либо RshInitMemory::External).\n
	 * Когда сигнал на выбранном для синхронизации входе достигает
	 * заданного порога (сверху или снизу, в зависимости от состояния
	 * поля RshInitDMA::controlSynchro), происходит событие
	 * синхронизации, и начинается процесс сбора и передача данных.
	 *
	 * \remarks
	 * Если синхронизация выключана, данное поле не используется
	 *
	 * \see
	 * \link page_Synchronization Синхронизация \endlink
	 *
	 */
	double threshold;

	/*!
	 *
	 * \~english
	 * \brief
	 * Additional synchonization options
	 *
	 * Combination of flags from
	 * RshInitDMA::ControlSynchro enum.\n
	 * By default this parameter is equal to zero,
	 * which corresponds to following flag combination:
	 * RshInitDMA::FrequencySwitchOff | RshInitDMA::SlopeFront
	 *
	 * \~russian
	 * \brief
	 * Дополнительные параметры синхронизации
	 *
	 * Комбинация флагов из перечисления
	 * RshInitDMA::ControlSynchro.\n
	 * По умолчанию, данный параметр равен нулю,
	 * что соответствует сочетанию флагов:
	 * RshInitDMA::FrequencySwitchOff | RshInitDMA::SlopeFront
	 *
	 */
    U32 controlSynchro;

	/*!
	 *
	 * \~english
	 * \brief
	 * List of additional synchronization flags
	 *
	 * Use flags combined by 'OR' statement
	 * to specify additional options in
	 * RshInitDMA::controlSynchro field.
	 *
	 * \~russian
	 * \brief
	 * Список дополнительных опций сбора данных
	 *
	 * Флаги из этого списка можно объединять
	 * по 'ИЛИ'. Установив нужную комбинацию
	 * флагов в поле RshInitDMA::controlSynchro,
	 * можно задействовать дополнительные опции.
	 *
	 */
	enum ControlSynchro
	{
		/*!
		 *
		 * \~english
		 * \brief
		 * Sampling rates for history and prehistory are equal
		 *
		 * Default option, work for all devices.
		 * SDK1 analog is ADC_CONTROL_ESW constant.
		 *
		 * \see
		 * RSH_CAPS_DEVICE_FREQUENCY_SWITCH_PREHISTORY
		 *
		 * \~russian
		 * \brief
		 * Предыстория и история собираются с одной частотой дискретизации
		 *
		 * Данный флаг ставится по умолчанию, и работает
		 * для всех устройств.\n
		 * Аналог в SDK1 - константа ADC_CONTROL_ESW.
		 *
		 * \see
		 * RSH_CAPS_DEVICE_FREQUENCY_SWITCH_PREHISTORY
		 *
		 */
		FrequencySwitchOff = 0x0,

		/*!
		 *
		 * \~english
		 * \brief
		 * Use signal front for synchronization
		 *
		 * This field is only actual when
		 * synchonization mode is active
		 * (see RshInitDMA::startType).\n
		 * Mutually exclusive with RshInitDMA::SlopeDecline.
		 *
		 * \~russian
		 * \brief
		 * Синхронизация по фронту
		 *
		 * Данный флаг актуален только при работе
		 * в режиме синхронизации (см.  RshInitDMA::startType).\n
		 * Взаимоисключающий с флагом RshInitDMA::SlopeDecline.
		 *
		 */
		SlopeFront = 0x0,

		/*!
		 *
		 * \~english
		 * \brief
		 * Use signal decline for synchronization
		 *
		 * This field is only actual when
		 * synchonization mode is active
		 * (see RshInitDMA::startType).\n
		 * Mutually exclusive with RshInitDMA::SlopeFront.
		 *
		 * \~russian
		 * \brief
		 * Синхронизация по спаду
		 *
		 * Данный флаг актуален только при работе
		 * в режиме синхронизации (см.  RshInitDMA::startType).\n
		 * Взаимоисключающий с флагом RshInitDMA::SlopeFront.
		 *
		 */
		SlopeDecline = 0x2,

		/*!
		 *
		 * \~english
		 * \brief
		 * Sampling rates for history and prehistory are different.
		 *
		 * This flag can be used only for devices that
		 * support this feature ( ::RSH_CAPS_DEVICE_FREQUENCY_SWITCH_PREHISTORY).
		 * When flag is active, sampling rate is changed when
		 * synchronization event occures. As a result, prehistory and
		 * main data will have different sampling rate.\n
		 * SDK1 analog is ADC_CONTROL_FSW constant.
		 *
		 * \~russian
		 * \brief
		 * Предыстория и история собираются с разными частотами
		 *
		 * Данный флаг может использоваться только для устройств,
		 * которые поддерживают данную возможность
		 * ( ::RSH_CAPS_DEVICE_FREQUENCY_SWITCH_PREHISTORY).
		 * Если флаг активен, частота дискретизации будет изменена
		 * в момент срабатывания синхронизации, и предыстория будет
		 * оцифрована с одной частотой, а основной сигнал - с другой.\n
		 * Аналог в SDK1 - константа ADC_CONTROL_FSW.
		 *
		 */
		FrequencySwitchOn = 0x4
	};

	RshInitADC();
	
	RshInitADC(RshInitADC const& obj);

	RshInitADC& operator=(RshInitADC& obj);


	bool operator==(RshInitADC& obj);

	bool operator!=(RshInitADC& obj);
};

/*!
 *
 * \~english
 * \brief
 * Initialization structure for DMA-type devices
 *
 * This structure usually is used for low-frequency
 * (10MHz or less) devices. One can use
 * ::RSH_CAPS_SOFT_INIT_DMA to find if
 * device abstraction library supports this structure
 * or not.\n
 * DMA-type device features:\n
 * low-frequency, can work in continious data acquisition moode,
 * do not have onboard memory (usually), do not have many
 * synchronization possibilities.
 *
 * \remarks
 * DMA name is used for historical reasons, some devices
 * do not have actual DMA feature (all USB devices, for examples).
 *
 * \~russian
 * \brief
 * Структура инициализации для устройств типа "DMA"
 *
 * Обычно, данная структура используется для относительно
 * низкочастотных устройств (10МГц и ниже). Узнать, поддерживается
 * ли эта структура библиотекой абстракции устройства можно,
 * используя капс ::RSH_CAPS_SOFT_INIT_DMA.\n
 * Характерные особенности устройства типа "DMA":\n
 * относительно низкая частота дискретизации, возможность
 * работать в непрерывном режиме, отсутствие собственной
 * памяти (хотя есть исключения), отсутствие (или сильно ограничены)
 * возможности синхронизации.
 *
 * \remarks
 * Название "DMA" у структуры осталось по историческим причинам (SDK1).
 * Некоторые устройства, которые можно инициализировать данной структурой,
 * на самом деле не могут передавать данные, используя ПДП (например, все
 * USB устройства).
 *
 */
struct RshInitDMA : public RshInitADC
{
	/*!
	 *
	 * \~english
	 * \brief
	 * Data acquisition mode.
	 *
	 * One of the elements of RshInitDMA::DmaMode
	 * enum. There are only two modes actually:
	 * "Start-stop" mode and continuous mode.\n
	 * One can use ::RSH_CAPS_SOFT_GATHERING_IS_AVAILABLE
	 * and ::RSH_CAPS_SOFT_PGATHERING_IS_AVAILABLE caps
	 * to check if device supports work in this two modes.
	 *
	 * \~russian
	 * \brief
	 * Режим сбора данных
	 *
	 * Один из элементов перечисления RshInitDMA::DmaMode.
	 * Существует два возможных сценария работы: работа в режиме
	 * "Старт-Стоп" или работа в непрерывном режиме.\n
	 * Используейте соответствующие капсы (::RSH_CAPS_SOFT_GATHERING_IS_AVAILABLE
	 * и ::RSH_CAPS_SOFT_PGATHERING_IS_AVAILABLE) чтобы проверить,
	 * поддерживает ли устройство работу в каждом из режимов.
	 *
	 */
	U32 dmaMode;

	/*!
	 *
	 * \~english
	 * \brief
	 * Additional options
	 *
	 * Combination of flags from
	 * RshInitDMA::Control enum.
	 * By default, RshInitDMA::StandardMode
	 * flag is used.
	 *
	 * \~russian
	 * \brief
	 * Дополнительные опции
	 *
	 * Комбинация флагов перечисления
	 * RshInitDMA::Control. По умолчанию,
	 * используется флаг RshInitDMA::StandardMode.
	 *
	 */
	U32 control;

	/*!
	 *
	 * \~english
	 * \brief
	 * Sampling rate inside frame
	 *
	 * Actual for \link page_FrameFreqMode Frame Frequency work mode \endlink.
	 *
	 * \remarks
	 * Use ::RSH_CAPS_DEVICE_FRAME_FREQUENCY_MODE caps
	 * to find if devie supports this mode or not.
	 *
	 * \~russian
	 * \brief
	 * Частота дискретизации внутри кадра
	 *
	 * Данный параметр используется при включенном
	 * \link page_FrameFreqMode режиме кадрового сбора \endlink.
	 *
	 * \remarks
	 * Используйте ::RSH_CAPS_DEVICE_FRAME_FREQUENCY_MODE для того,
	 * чтобы определить, поддерживает ли устройство данный
	 * режим работы или нет.
	 *
	 */
	double frequencyFrame;

	/*!
	 *
	 * \~english
	 * \brief
	 * List of additional flags
	 *
	 * Use flags combined by 'OR' statement
	 * to specify additional options in
	 * RshInitDMA::control field.
	 *
	 * \~russian
	 * \brief
	 * Список дополнительных опций сбора данных
	 *
	 * Флаги из этого списка можно объединять
	 * по 'ИЛИ'. Установив нужную комбинацию
	 * флагов в поле RshInitDMA::control, можно
	 * задействовать дополнительные опции.
	 *
	 */
	enum  Control
	{
		/*!
		 *
		 * \~english
		 * \brief
		 * No special options used
		 *
		 * This flag can be omited and used only for
		 * more readable code.\n
		 * This flag meaning is: no differential mode, no frame
		 * frequency mode.
		 *
		 * \~russian
		 * \brief
		 * Дополнительные опции не используются
		 *
		 * Нет необходимости специально указывать
		 * данный флаг (т.к. это значение по умолчанию).
		 * Используется для повышения читаемости кода.\n
		 * Данный флаг означает: однополюсный вход,
		 * кадровый режим не используется.
		 *
		 */
		StandardMode = 0x0,

		/*!
		 *
		 * \~english
		 * \brief
		 * Turn on differential mode
		 *
		 * One can check if device supports this mode
		 * or not by using ::RSH_CAPS_DEVICE_DIFFERENTIAL_INPUT_MODE.\n
		 * See device user manual for details about analog input pin connect
		 * in two work modes.
		 *
		 * \~russian
		 * \brief
		 * Включение дифференциального режима работы
		 *
		 * Проверить, поддерживает ли устройство данный режим
		 * работы или нет, можно с помощью ::RSH_CAPS_DEVICE_DIFFERENTIAL_INPUT_MODE.\n
		 *
		 * \remarks
		 * Схема подключения для однополюсного и дифференциального режимов
		 * обычно приводится в руководстве пользователя для данного устройства.
		 *
		 */
		DiffMode = 0x1,

		/*!
		 *
		 * \~english
		 * \brief
		 * Turn on frame sampling mode
		 *
		 * When this flag is active, RshInitDMA::frequencyFrame
		 * field value is used as sampling rate inside frame.
		 *
		 * \~russian
		 * \brief
		 * Включение кадрового режима сбора
		 *
		 * Когда данный флаг включен, значение поля
		 * RshInitDMA::frequencyFrame используется для
		 * установки частоты дискретизации внутри кадра.
		 *
		 */
		FrameMode = 0x2,

		/*!
		 *
		 * \~english
		 * \brief
		 * Switch multiplexor on start
		 *
		 * \deprecated
		 * Not used at this moment, can be
		 * excluded from future releases.
		 *
		 * \~russian
		 * \brief
		 * Переключение мультиплексора по старту
		 *
		 * \deprecated
		 * В текущий момент не используется, возможно будет удалено
		 * в следующих релизах.
		 *
		 */
		MulSwitchStart = 0x4,
	};

	/*!
	 *
	 * \~english
	 * \brief
	 * Data acquisition modes list
	 *
	 * Values from this list are
	 * mutually exclusive (that is, use
	 * only one mode). Set it to the
	 * RshInitDMA::dmaMode field.
	 *
	 * \~russian
	 * \brief
	 * Список режимов сбора данных
	 *
	 * Значения из этого списка - взаимоисключающие
	 * (можно единовременно задать только один из
	 * режимов работы). Режим задается в поле RshInitDMA::dmaMode
	 * структуры инициализации.
	 *
	 */
	enum  DmaMode
	{
		/*!
		 *
		 * \~english
		 * \brief
		 * Single buffer mode
		 *
		 * So called "Start-Stop" mode.\n
		 * Only one buffer of specified size is
		 * acquired, and then data transfer process is
		 * terminated.
		 *
		 * \see
		 * RSH_CAPS_SOFT_GATHERING_IS_AVAILABLE
		 *
		 * \~russian
		 * \brief
		 * Одиночный запуск
		 *
		 * Так называемый режим "Старт-Стоп".\n
		 * В этом режиме собирается один буфер заданного
		 * размера, после чего процесс сбора данных останавливается
		 * (прекращается переда по ПДП, если она была активна).
		 *
		 * \see
		 * RSH_CAPS_SOFT_GATHERING_IS_AVAILABLE
		 *
		 */
		Single= 0x0,

		/*!
		 *
		 * \~english
		 * \brief
		 * Continuous mode
		 *
		 * In this mode data acquisition and transfer
		 * will continue forever, until IRshDevice::Stop()
		 * method is called.
		 *
		 * \see
		 * RSH_CAPS_SOFT_PGATHERING_IS_AVAILABLE
		 *
		 * \~russian
		 * \brief
		 * Непрерывный режим
		 *
		 * В этом режиме процесс сбора и передачи данных
		 * будет продолжаться бесконечно (в теории), до вызова
		 * метода IRshDevice::Stop().
		 *
		 * \see
		 * RSH_CAPS_SOFT_PGATHERING_IS_AVAILABLE
		 *
		 */
		Persistent = 0x1,
	};

	RshInitDMA();

    RshInitDMA(RshInitADC const& obj);

    RshInitDMA(RshInitDMA const& obj);

	RshInitDMA& operator=(RshInitDMA& obj);


	bool operator==(RshInitDMA& obj);

	bool operator!=(RshInitDMA& obj);
};

/*!
 *
 * \~english
 * \brief
 * Initialization structure for Memory-type devices
 *
 * This structure usually is used for high-frequency
 * devices. One can use
 * ::RSH_CAPS_SOFT_INIT_MEMORY to find if
 * device abstraction library supports this structure
 * or not.\n
 * Memory-type device features:\n
 * high-frequency, have onboard memory, extended
 * synchronization possibilities.
 *
 * \remarks
 * Memory name is used for historical reasons - this
 * structure is successor to SDK1
 * sturctures ADCInitParametersMemory, ADCInitParametersMemory1
 *
 * \~russian
 * \brief
 * Структура инициализации для устройств типа "Memory"
 *
 * Обычно, данная структура используется для высокочастотных
 * устройств. Узнать, поддерживается
 * ли эта структура библиотекой абстракции устройства можно,
 * используя капс ::RSH_CAPS_SOFT_INIT_MEMORY.\n
 * Характерные особенности устройств типа "Memory":\n
 * высокая частота дискретизации, наличие собственной памяти,
 * расширенные возможности синхронизации.
 *
 * \remarks
 * Название "Memory" у структуры осталось
 * по историческим причинам - данная структура выступает
 * наследником таких структур из SDK1,
 * как ADCInitParametersMemory и ADCInitParametersMemory1
 *
 */
struct RshInitMemory : public RshInitADC
{
	/*!
	 *
	 * \~english
	 * \brief
	 * External synchronization channel settings
	 *
	 * Using this field one can specify
	 * parameters for external synchronization
	 * input.
	 *
	 * \remarks
	 * Use ::RSH_CAPS_DEVICE_SYNCHRO_EXTERNAL to
	 * check if device have programmable external
	 * synchronization input.
	 *
	 * \see
	 * RshSynchroChannel
	 *
	 * \~russian
	 * \brief
	 * Параметры входа внешней синхронизации
	 *
	 * Используя данное поле, можно задать
	 * необходимые параметры (коэффициент усиления,
	 * сопротвление входа и др.) для входа внешней
	 * синхронизации.
	 *
	 * \remarks
	 * Используйте капс ::RSH_CAPS_DEVICE_SYNCHRO_EXTERNAL
	 * чтобы проверить, есть ли возможность использовать
	 * программно настраиваемый вход внешней синхронизации.
	 *
	 * \see
	 * RshSynchroChannel
	 *
	 */
	RshSynchroChannel channelSynchro;

	/*!
	 *
	 * \~english
	 * \brief
	 * Additional options
	 *
	 * Combination of flags from
	 * RshInitMemory::Control enum.
	 *
	 * \~russian
	 * \brief
	 * Дополнительные опции
	 *
	 * Комбинация флагов перечисления
	 * RshInitMemory::Control.
	 *
	 */
	U32 control;

	/*!
	 *
	 * \~english
	 * \brief
	 * Prehistory size
	 *
	 * This field specify which part of
	 * data buffer will be used to save prehistory
	 * data. Valid values range from 0 (no prehistory)
	 * to 15 (15/16 of the buffer is prehistory data).\n
	 * See \link page_SynchronizationPrehistory this page \endlink
	 * for more details.
	 *
	 * \remarks
	 * Corresponding caps code is ::RSH_CAPS_DEVICE_PREHISTORY
	 *
	 * \~russian
	 * \brief
	 * Размер предыстории
	 *
	 * Данный параметр определяет, какая часть буфера данных
	 * будет использована для записи предыстории измерений.
	 * Диапазон допустимых значений - от 0 (предыстория не записывается)
	 * до 15 (15/16 буфера будет отдано под данные предыстории).\n
	 * Дополнительные подробности можно найти
	 * в \link page_SynchronizationPrehistory статье о синхронизации \endlink.
	 *
	 * \remarks
	 * Соответствующий капс-код: ::RSH_CAPS_DEVICE_PREHISTORY
	 *
	 */
	U32 preHistory;

	/*!
	 *
	 * \~english
	 * \brief
	 * Start delay
	 *
	 * Using this field one can specify
	 * start delay value for synchronization.
	 * See \link page_SynchronizationStartDelay this page \endlink for more details.
	 *
	 * \remarks
	 * Corresponding caps code: ::RSH_CAPS_DEVICE_START_DELAY
	 *
	 * \~russian
	 * \brief
	 * Задержка старта
	 *
	 * В этом поле можно задать значение
	 * задержки старта для синхронизации.
	 * Подробности можно найти на
	 * \link page_SynchronizationStartDelay странице с описанием синхронизации \endlink.
	 *
	 * \remarks
	 * Соответствующий капс-код: ::RSH_CAPS_DEVICE_START_DELAY
	 *
	 * \todo
	 * Провести ревизию устройств, у которых есть данная возможность,
	 * выбрать, в каком виде будет задаваться параметр.
	 *
	 */
	U32	startDelay;

	/*!
	 *
	 * \~english
	 * \brief
	 * Hysteresis
	 *
	 * This field is used to specify
	 * hysteresis value. Hysteresis must be
	 * in range from 0 to 20.
	 * See \link page_SynchronizationHysteresis this page \endlink for more details.
	 *
	 * \remarks
	 * Corresponding caps code: ::RSH_CAPS_DEVICE_HYSTERESIS
	 *
	 * \~russian
	 * \brief
	 * Гистерезис
	 *
	 * В данном поле можно задать уровень гистерезиса.
	 * Диапазон допустимых значений - от 0 до 20.
	 * Подробности можно найти на
	 * \link page_SynchronizationHysteresis странице с описанием синхронизации \endlink.
	 *
	 * \remarks
	 * Соответствующий капс-код: ::RSH_CAPS_DEVICE_HYSTERESIS
	 *
	 */
	U32	hysteresis;

	/*!
	 *
	 * \~english
	 * \brief
	 * Packet number
	 *
	 * This field is actual only for devices
	 * that support packet data acquisition mode
	 * (See ::RSH_CAPS_DEVICE_PACKET_MODE).\n
	 * Using this field, one can specify, how
	 * many packets of RshInitMemory::bufferSize
	 * will be acquired in one cycle.
	 *
	 * \remarks
	 * If device do not support packet acquisition,
	 * this field is ignored.
	 *
	 * \~russian
	 * \brief
	 * Количество пакетов
	 *
	 * Данная опция актуальна только для устройств,
	 * которые поддерживают пакетный режим сбора данных
	 * (см. ::RSH_CAPS_DEVICE_PACKET_MODE).\n
	 * Используя данное поле, можно задать, сколько
	 * пакетов данных размера RshInitMemory::bufferSize
	 * нужно собрать за один цикл сбора данных.
	 *
	 * \remarks
	 * Если устройство не может работать в пакетном режиме,
	 * данный параметр игнорируется.
	 *
	 */
	U32 packetNumber;	

	/*!
	 *
	 * \~english
	 * \brief
	 * List of additional flags
	 *
	 * Use this flags
	 * to specify additional options in
	 * RshInitMemory::control field.
	 *
	 * \~russian
	 * \brief
	 * Список дополнительных опций сбора данных
	 *
	 * Флаги из этого списка можно использовать
	 * в поле RshInitMemory::control, чтобы
	 * задействовать дополнительные опции сбора данных.
	 *
	 */
	enum Control
	{
		/*!
		 *
		 * \~english
		 * \brief
		 * Normal mode
		 *
		 * Double and quadro frequency modes are not used.
		 * This flag is default and can be used for all
		 * devices.\n
		 * Mutually exclusive with RshInitMemory::FreqDouble
		 * and RshInitMemory::FreqQuadro
		 *
		 * \see
		 * RSH_CAPS_DEVICE_DOUBLE_FREQUENCY_MODE | RSH_CAPS_DEVICE_QUADRO_FREQUENCY_MODE
		 *
		 * \~russian
		 * \brief
		 * Обычный режим
		 *
		 * Удвоение и учетверение частоты не используется.
		 * Данный режим используется по умолчанию и подходит
		 * для всех устройств.\n
		 * Взаимоисключающий с флагами RshInitMemory::FreqDouble и RshInitMemory::FreqQuadro
		 *
		 * \see
		 * RSH_CAPS_DEVICE_DOUBLE_FREQUENCY_MODE | RSH_CAPS_DEVICE_QUADRO_FREQUENCY_MODE
		 *
		 */
		FreqSingle = 0x0,

		/*!
		 *
		 * \~english
		 * \brief
		 * Auto start off
		 *
		 * Automatic start is turned off. This is default value.
		 * This flag is actual only for devices which
		 * support ::RSH_CAPS_DEVICE_AUTO_START_MODE mode.
		 *
		 *
		 * \see
		 * RSH_CAPS_DEVICE_AUTO_START_MODE | RSH_GET_DEVICE_AUTO_START_INTERVAL_SET
		 *
		 * \~russian
		 * \brief
		 * Автозапуск выключен
		 *
		 * Автоматический запуск сбора данных выключен. Данный
		 * флаг актуален только для устройств с поддержкой
		 * режима ::RSH_CAPS_DEVICE_AUTO_START_MODE.
		 *
		 * \see
		 * RSH_CAPS_DEVICE_AUTO_START_MODE | RSH_GET_DEVICE_AUTO_START_INTERVAL_SET
		 *
		 */
		AutoStartOff = 0x0,

		/*!
		 *
		 * \~english
		 * \brief
		 * Double sampling rate
		 *
		 * When this flag is active, device will work in double
		 * frequency mode.
		 * This flag can be set only for devices that support
		 * this feature (::RSH_CAPS_DEVICE_DOUBLE_FREQUENCY_MODE).
		 * Mutually exclusive with RshInitMemory::FreqSingle
		 * and RshInitMemory::FreqQuadro
		 *
		 * \see
		 * RSH_CAPS_DEVICE_DOUBLE_FREQUENCY_MODE
		 *
		 * \~russian
		 * \brief
		 * Удвоение частоты
		 *
		 * Если данный флаг активен, устройство будет работать
		 * в режиме удвоения частоты дискретизации.
		 * Этот флаг можно включить только для устройств, которые
		 * поддерживают данный режим работы (::RSH_CAPS_DEVICE_DOUBLE_FREQUENCY_MODE).
		 * Взаимоисключающий с флагами RshInitMemory::FreqSingle и RshInitMemory::FreqQuadro
		 *
		 * \see
		 * RSH_CAPS_DEVICE_DOUBLE_FREQUENCY_MODE
		 *
		 */
		FreqDouble	= 0x1,

		/*!
		 *
		 * \~english
		 * \brief
		 * Quadro sampling rate
		 *
		 * When this flag is active, device will work in quadro
		 * frequency mode.
		 * This flag can be set only for devices that support
		 * this feature (::RSH_CAPS_DEVICE_QUADRO_FREQUENCY_MODE).
		 * Mutually exclusive with RshInitMemory::FreqSingle
		 * and RshInitMemory::FreqDouble
		 *
		 * \see
		 * RSH_CAPS_DEVICE_QUADRO_FREQUENCY_MODE
		 *
		 * \~russian
		 * \brief
		 * Учетверение частоты
		 *
		 * Если данный флаг активен, устройство будет работать
		 * в режиме учетверения частоты дискретизации.
		 * Этот флаг можно включить только для устройств, которые
		 * поддерживают данный режим работы (::RSH_CAPS_DEVICE_QUADRO_FREQUENCY_MODE).
		 * Взаимоисключающий с флагами RshInitMemory::FreqSingle и RshInitMemory::FreqDouble
		 *
		 * \see
		 * RSH_CAPS_DEVICE_QUADRO_FREQUENCY_MODE
		 *
		 */
		FreqQuadro	= 0x2,

		/*!
		 *
		 * \~english
		 * \brief
		 * Auto start on
		 *
		 * Automatic start is turned on.
		 * This flag is actual only for devices which
		 * support ::RSH_CAPS_DEVICE_AUTO_START_MODE mode.\n
		 * When this mode is active, device hardware will start
		 * data acquisition process automatically, using current
		 * settings. One can call IRshDevice::Start() method
		 * only once, and the just obtain data until end (need
		 * to call IRshDevice::Stop() to terminate process).
		 *
		 *
		 * \see
		 * RSH_CAPS_DEVICE_AUTO_START_MODE | RSH_GET_DEVICE_AUTO_START_INTERVAL_SET
		 *
		 * \~russian
		 * \brief
		 * Автозапуск включен
		 *
		 * Автоматический запуск сбора данных выключен. Данный
		 * флаг актуален только для устройств с поддержкой
		 * режима ::RSH_CAPS_DEVICE_AUTO_START_MODE.\n
		 * Когда данный режим включен, устройство будет циклически
		 * запускать процесс сбора и передачи данных с заданным интервалом,
		 * используя текущие настройки сбора. Нужно вызвать метод
		 * IRshDevice::Start() только один раз для запуска процесса, и затем
		 * просто получать данные до тех пор, пока это необходимо
		 * (вызов метода IRshDevice::Stop() останавливает процесс).
		 *
		 * \see
		 * RSH_CAPS_DEVICE_AUTO_START_MODE | RSH_GET_DEVICE_AUTO_START_INTERVAL_SET
		 *
		 */
		AutoStartOn = 0x4,
	};

	RshInitMemory();

    RshInitMemory(RshInitADC const& obj);

	RshInitMemory(RshInitMemory const& obj);

	RshInitMemory& operator=(RshInitMemory& obj);

	bool operator==(RshInitMemory& obj);

	bool operator!=(RshInitMemory& obj);
};
/*!
 \brief


 *
 * \~english
 * \brief
 * Initialization structure for signal generators
 *
 * This structure is used for GSPF series devices.\n
 * Corresponding caps codes are ::RSH_CAPS_SOFT_GENERATION_IS_AVAILABLE
 * and ::RSH_CAPS_SOFT_INIT_GSPF.
 *
 * \see
 * RshInitDAC
 *
 * \~russian
 * \brief
 * Cтруктура инициализации для генераторов
 *
 * Данная структура используется для управления
 * генераторами сигнала серии ГСПФ.\n
 * Соответствующие капс-коды: ::RSH_CAPS_SOFT_GENERATION_IS_AVAILABLE
 * и ::RSH_CAPS_SOFT_INIT_GSPF.
 *
 * \see
 * RshInitDAC
 *
 */
struct  RshInitGSPF : public RshBaseType
{
	/*!
	 *
	 * \~english
	 * \brief
	 * Data generation start mode
	 *
	 * One of RshInitGSPF::StartType enum
	 * constants, that determine, how
	 * DAC will be started.
	 *
	 * \~russian
	 * \brief
	 * Режим запуска генерации сигнала
	 *
	 * Одна из констант перечисления
	 * RshInitGSPF::StartType, которая определяет,
	 * как будет запускаться генерация сигнала.
	 *
	 */
    U32 startType;

    /*!
	 *
	 * \~english
	 * \brief
	 * Generator sampling rate
	 *
	 * Sampling rate of the generator in Hz.\n
	 * Depending on RshInitGSPF::control value
	 * may be one of the predefined frequencies from
	 * frequency list (see ::RSH_GET_DEVICE_FREQUENCY_LIST),
	 * or frequency synthesizer created frequency.
	 *
	 * \~russian
	 * \brief
	 * Частота дискретизации
	 *
	 * Частота дискретизации, которая будет
	 * использована при генерации сигнала, в Гц.\n
	 * В зависимости от состояния поля RshInitGSPF::control будет
	 * использована частота из списка частот
	 * (см. ::RSH_GET_DEVICE_FREQUENCY_LIST), либо близкая к
	 *  заданной частота, созданная синтезатором частоты.
	 *
	 */
	double frequency;

    /*!
	 *
	 * \~english
	 * \brief
	 * Attenuator value
	 *
	 * One of the RshInitGSPF::Attenuator enum
	 * values. Predifined set of attenuation level
	 * constants can be used.\n
	 * By default, signal is not attenuated.
	 *
	 * \remarks
	 * Attenuation is applied to signal just
	 * before output, that means, that using attenuator
	 * one can improve DAC dynamic range for small amplitude
	 * signals generated.
	 *
	 * \~russian
	 * \brief
	 * Аттенюатор
	 *
	 * Один из элементов перечисления RshInitGSPF::Attenuator.\n
	 * С помощью этого поля можно задать дополнительное
	 * ослабление сигнала.\n
	 * По умолчанию, ослабление не используется.
	 *
	 * \remarks
	 * При использовании аттенюатора, ослабление сигнала происходит
	 * уже после его формирования ЦАПом, непосредственно перед
	 * выдачей сигнала на выход генератора. Таким образом, используя
	 * аттенюатор, можно более полно использовать динамический
	 * диапазон ЦАП при генерации слабых сигналов.
	 *
	 */
	U32 attenuator;

	/*!
	 *
	 * \~english
	 * \brief
	 * Additional flags
	 *
	 * 'OR' Combination of RshInitGSPF::Control
	 * enum elements. This field is used to specify
	 * additional signal generation options, such as
	 * using of frequency synthesizer, filter and so on.
	 *
	 * \remarks
	 * By default, this parameter is equal to zero,
	 * which corresponds to following flag combination:\n
	 * <b>RshInitGSPF::SynthesizerOff | RshInitGSPF::FilterOff | RshInitGSPF::PlayOnce | RshInitGSPF::SynchroFront</b>
	 *
	 * \~russian
	 * \brief
	 * Дополнительные флаги
	 *
	 * Комбинация по 'ИЛИ' элементов перечисления
	 * RshInitGSPF::Control. Данное поле используется
	 * для управления дополнительными параметрами генерации
	 * сигнала, такими как использование синтезатора частоты,
	 * фильтра и др.
	 *
	 * \remarks
	 * По умолчанию, данный параметр равен нулю, что
	 * соответствует следующей комбинации флагов:\n
	 * <b>RshInitGSPF::SynthesizerOff | RshInitGSPF::FilterOff | RshInitGSPF::PlayOnce | RshInitGSPF::SynchroFront</b>
	 *
	 */
	U32 control;

	/*!
	 *
	 * \~english
	 * \brief
	 * Start mode list
	 *
	 * This enum entities can be used
	 * to specify how to start generation process.\n
	 * To do so, set one of them as value
	 * for RshInitGSPF::startType field.
	 *
	 * \~russian
	 * \brief
	 * Список режимов запуска
	 *
	 * Элементы этого перечисления используются
	 * для задания режима запуска процесса генерации.\n
	 * Чтобы выбрать режим, нужно установить
	 * одно из этих значений в поле RshInitGSPF::startType.
	 *
	 */
	enum  StartType
	{
		/*!
		 *
		 * \~english
		 * \brief
		 * Program start
		 *
		 * Generation will start as soon as
		 * command will be acquired by the device.\n
		 * That means, DAC will start as soon as
		 * IRshDevice::Start() method is called.
		 *
		 * \remarks
		 * "As soon" is not immediately, because
		 * there is always some delays for functions calls,
		 * data transfer and device responce.
		 *
		 * \~russian
		 * \brief
		 * Программный запуск
		 *
		 * Генерация будет запущена сразу же
		 * после получения устройством команды на запуск.\n
		 * Это означает, что ЦАП будет запущен сразу же после
		 * вызова метода IRshDevice::Start().
		 *
		 * \remarks
		 * "Сразу же" на самом деле происходит не мгновенно, так
		 * как всегда есть задержки - вызов функций, передача
		 * данных по шине, отклик устройства.
		 *
		 */
		Program = 0x1,

		/*!
		 *
		 * \~english
		 * \brief
		 * Start signal generation on external trigger
		 *
		 * Use external trigger input as source for
		 * signal generation start command.
		 *
		 * \~russian
		 * \brief
		 * Внешний запуск
		 *
		 * Использовать внешний источник
		 * для запуска процесса генерации сигнала.\n
		 * Генерация будет запущена по приходу соответствующего
		 * строба запуска на вход внешнего запуска.\n
		 * Дополнительную информацию о необходимых параметрах
		 * запускающего сигнала можно найти в руководстве пользователя.
		 *
		 */
		External = 0x4,

		/*!
		 *
		 * \~english
		 * \brief
		 * Use external frequency source for sampling
		 *
		 * External frequency will be used for
		 * sampling DAC.\n
		 * External input is used as sampling source for
		 * DAC. See user manual for details about sampling
		 * signal parameters.
		 *
		 * \~russian
		 * \brief
		 * Использование внешней частоты дискретизации
		 *
		 * ЦАП будет тактироваться от внешнего источника частоты
		 * дискретизации.\n
		 * Дополнительную информацию о необходимых параметрах
		 * тактирующего сигнала можно найти
		 * в руководстве пользователя.
		 *
		 */
		FrequencyExternal = 0x10
	};

	/*!
	 *
	 * \~english
	 * \brief
	 * List of additional flags
	 *
	 * Use this flags
	 * to specify additional options in
	 * RshInitGSPF::control field.
	 *
	 * \~russian
	 * \brief
	 * Список дополнительных параметров генерации
	 *
	 * Флаги из этого списка можно использовать
	 * в поле RshInitGSPF::control, чтобы
	 * задействовать дополнительные режимы генерации сигнала.
	 *
	 */
	enum Control
	{
		/*!
		 *
		 * \~english
		 * \brief
		 * Turn OFF filter
		 *
		 * Mutually exclusive with RshInitGSPF::FilterOn
		 *
		 * \~russian
		 * \brief
		 * Выключить фильтр НЧ
		 *
		 * Выключение ФНЧ.
		 * Взаимоисключающий с RshInitGSPF::FilterOn
		 *
		 */
		FilterOff = 0x0,

		/*!
		 *
		 * \~english
		 * \brief
		 * Play buffer only once
		 *
		 * Signal in buffer will be played once, and then DAC will
		 * be stopped.\n
		 * Mutually exclusive with RshInitGSPF::PlayLoop.
		 *
		 * \~russian
		 * \brief
		 * Одиночное проигрование
		 *
		 * Записанный в буфер сигнал будет выведен на ЦАП,
		 * после чего проигрование остановится.\n
		 * Взаимоисключающий с RshInitGSPF::PlayLoop.
		 *
		 */
		PlayOnce = 0x0,

		/*!
		 *
		 * \~english
		 * \brief
		 * Start generation on trigger front
		 *
		 * Only valid when RshInitGSPF::External is used as
		 * RshInitGSPF::startType value.\n
		 * Generation will be started on trigger front.
		 * Mutually exclusive with RshInitGSPF::SynchroDecline.
		 *
		 * \~russian
		 * \brief
		 * Запуск по фронту
		 *
		 * Актуально только при использовании запуска
		 * с флагом RshInitGSPF::External.\n
		 * Генерация сигнала начнется по фронту запускающего импульса.
		 * Взаимоисключающий с RshInitGSPF::SynchroDecline.
		 *
		 */
		SynchroFront = 0x0,

		/*!
		 *
		 * \~english
		 * \brief
		 * Frequency synthesizer is not used
		 *
		 * DAC will be sampled using Quartz frequency and devider.\n
		 * Mutually exclusive with RshInitGSPF::SynthesizerOn.
		 *
		 * \~russian
		 * \brief
		 * Синтезатор частоты не исползуется
		 *
		 * ЦАП будет тактироваться от кварцевого генератора (с делителем)\n
		 * Взаимоисключающий с RshInitGSPF::SynthesizerOn.
		 *
		 */
		SynthesizerOff = 0x0,

		/*!
		 *
		 * \~english
		 * \brief
		 * Frequency synthesizer is used
		 *
		 * DAC will be sampled by frequency synthesizer.\n
		 * Mutually exclusive with RshInitGSPF::SynthesizerOff.
		 *
		 * \~russian
		 * \brief
		 * Использовать синтезатор частоты
		 *
		 * Синтезатор частоты будет использоваться в качестве
		 * источника тактовых импульсов для ЦАП.\n
		 * Взаимоисключающий с RshInitGSPF::SynthesizerOff.
		 *
		 */
		SynthesizerOn = 0x1,

		/*!
		 *
		 * \~english
		 * \brief
		 * Turn ON filter
		 *
		 * Turn on low pass filter with cut edge at 10.7MHz
		 * Mutually exclusive with RshInitGSPF::FilterOff
		 *
		 * \~russian
		 * \brief
		 * Включить фильтр НЧ
		 *
		 * Включение ФНЧ с частотой среза 10.7МГц.
		 * Взаимоисключающий с RshInitGSPF::FilterOff
		 *
		 */
		FilterOn = 0x2,

		/*!
		 *
		 * \~english
		 * \brief
		 * Play buffer continuously
		 *
		 * Signal in buffer will be played continuously.\n
		 * Mutually exclusive with RshInitGSPF::PlayOnce.
		 *
		 * \remarks
		 * When using this flag, signal will be played even
		 * when your program is no more running. Call
		 * IRshDevice::Stop() to stop playing.
		 *
		 * \~russian
		 * \brief
		 * Циклическое проигрование
		 *
		 * Записанный в буфер сигнал будет проигрываться
		 * циклически.\n
		 * Взаимоисключающий с RshInitGSPF::PlayOnce.
		 *
		 * \remarks
		 * При использовании данного флага, сигнал будет
		 * проигрываться даже после завершения работы
		 * программы. Чтобы остановать проигрование, нужно вызвать
		 * метод IRshDevice::Stop().
		 *
		 */
		PlayLoop = 0x4,

		/*!
		 *
		 * \~english
		 * \brief
		 * Start generation on trigger decline
		 *
		 * Only valid when RshInitGSPF::External is used as
		 * RshInitGSPF::startType value.\n
		 * Generation will be started on trigger decline.
		 * Mutually exclusive with RshInitGSPF::SynchroFront.
		 *
		 * \~russian
		 * \brief
		 * Запуск по спаду
		 *
		 * Актуально только при использовании запуска
		 * с флагом RshInitGSPF::External.\n
		 * Генерация сигнала начнется по спаду запускающего импульса.
		 * Взаимоисключающий с RshInitGSPF::SynchroFront.
		 *
		 */
		SynchroDecline = 0x8
	};

	/*!
	 *
	 * \~english
	 * \brief
	 * List of attenuator modes
	 *
	 * Elements from this list can be used
	 * as value for RshInitGSPF::attenuator field.
	 *
	 * \~russian
	 * \brief
	 * Список режимов работы аттенюатора
	 *
	 * Элементы этого перечисления могут быть
	 * использованы в качестве значений для
	 * поля RshInitGSPF::attenuator.
	 *
	 */
	enum Attenuator
	{
		/*!
		 *
		 * \~english
		 * \brief
		 * Attenuation is not used
		 *
		 * \~russian
		 * \brief
		 * Ослабление не используется
		 *
		 */
		AttenuationOff = 0x0,

		/*!
		 *
		 * \~english
		 * \brief
		 * 6dB attenuation
		 *
		 * \~russian
		 * \brief
		 * Ослабление 6дБ
		 *
		 */
		Attenuation6dB = 0x1,

		/*!
		 *
		 * \~english
		 * \brief
		 * 12dB attenuation
		 *
		 * \~russian
		 * \brief
		 * Ослабление 12дБ
		 *
		 */
		Attenuation12db = 0x2,

		/*!
		 *
		 * \~english
		 * \brief
		 * 18dB attenuation
		 *
		 * \~russian
		 * \brief
		 * Ослабление 18дБ
		 *
		 */
		Attenuation18dB = 0x3,

		/*!
		 *
		 * \~english
		 * \brief
		 * 24dB attenuation
		 *
		 * \~russian
		 * \brief
		 * Ослабление 24дБ
		 *
		 */
		Attenuation24dB = 0x4,

		/*!
		 *
		 * \~english
		 * \brief
		 * 30dB attenuation
		 *
		 * \~russian
		 * \brief
		 * Ослабление 30дБ
		 *
		 */
		Attenuation30dB = 0x5,

		/*!
		 *
		 * \~english
		 * \brief
		 * 36dB attenuation
		 *
		 * \~russian
		 * \brief
		 * Ослабление 36дБ
		 *
		 */
		Attenuation36dB = 0x6,

		/*!
		 *
		 * \~english
		 * \brief
		 * 42dB attenuation
		 *
		 * \~russian
		 * \brief
		 * Ослабление 42дБ
		 *
		 */
		Attenuation42dB = 0x7

	};

    RshInitGSPF();

	RshInitGSPF(RshInitGSPF const& obj);

	RshInitGSPF& operator=(RshInitGSPF& obj);

	bool operator==(RshInitGSPF& obj);

	bool operator!=(RshInitGSPF& obj);
};

/*!
 *
 * \~english
 * \brief
 * DAC control structure.
 *
 * This structure is used to get access to device's
 * DAC (or DACs). Some of ADC boards have additional
 * DAC output channels, and using this structure
 * one can set output voltage on these DACs.
 *
 * \remarks
 * Devices which whole purpose is signal generation
 * use more complicated structures, such as RshInitGSPF.
 *
 * \see
 * RSH_CAPS_SOFT_INIT_DAC | IRshDevice::Init()
 *
 * \~russian
 * \brief
 * Структура для доступа к ЦАП.
 *
 * Данная структура используется для управления
 * ЦАПом (или ЦАПами), которые имеются у некоторых АЦП
 * устройств. Используя эту структуру, можно установить
 * нужное напряжение на одном из ЦАПов.
 *
 * \remarks
 * Устройства, основным предназначением которых
 * является генерация сигнала,
 * используют другие, более сложные структуры для
 * инициализации, например RshInitGSPF.
 *
 * \see
 * RSH_CAPS_SOFT_INIT_DAC | IRshDevice::Init()
 */
struct RshInitDAC : public RshBaseType
{
	/*!
	 *
	 * \~english
	 * \brief
	 * ID (order number) of DAC.
	 *
	 * With this field one can specify,
	 * which DAC is used for voltage output.
	 *
	 * \remarks
	 * If only one DAC installed, this parameter is ignored.\n
	 * Device ID is zero based.
	 *
	 * \~russian
	 * \brief
	 * Порядковй номер ЦАПа.
	 *
	 * Используя данное поле, можно задать номер
	 * ЦАПа, который будет использоваться для выдачи напряжения
	 *
	 * \remarks
	 * Если устройство имеет только один ЦАП,
	 * данный параметр игнорируется.\n
	 * Нумерация начинается с нуля.
	 *
	 */
	U32 id;

	/*!
	 *
	 * \~english
	 * \brief
	 * Output voltage.
	 *
	 * Use IRshDevice::Get() method with
	 * RSH_GET_DEVICE_OUTPUT_RANGE_VOLTS parameter to
	 * find out output voltage range for device.
	 *
	 * \remarks
	 * Wrong values are corrected when IRshDevice::Init() is called.
	 *
	 * \see
	 * RSH_GET_DEVICE_OUTPUT_RANGE_VOLTS | IRshDevice::Get()
	 *
	 * \~russian
	 * \brief
	 * Выходное напряжение.
	 *
	 * Используйте метод IRshDevice::Get() с
	 * параметром RSH_GET_DEVICE_OUTPUT_RANGE_VOLTS для того,
	 * чтобы определить допустимый диапазон выходного напряжения на ЦАПе.
	 *
	 * \remarks
	 * Если будет установлено неправильное (выходящее за диапазон) значение
	 * напряжения, оно будет откорректировано при вызове метода IRshDevice::Init().
	 *
	 * \see
	 * RSH_GET_DEVICE_OUTPUT_RANGE_VOLTS | IRshDevice::Get()
	 */
	double voltage;

	/*!
	 *
	 * \~english
	 * \brief
	 * Default values: id=0, voltage=0.0
	 *
	 * \~russian
	 * \brief
	 * Значения по умолчанию: id=0, voltage=0.0
	 *
	 */
	RshInitDAC();

	RshInitDAC(RshInitDAC const& obj);

	RshInitDAC& operator=(RshInitDAC& obj);

	bool operator==(RshInitDAC& obj);

	bool operator!=(RshInitDAC& obj);
};

/*!
 *
 * \~english
 * \brief
 * Digital port operations
 *
 * This structure is used to pass data to
 * and from digital port. Corresponding
 * caps codes are ::RSH_CAPS_DEVICE_HAS_DIGITAL_PORT
 * and ::RSH_CAPS_SOFT_INIT_PORT.
 *
 * \~russian
 * \brief
 * Операции с цифровым портом
 *
 * Данная структура используется для записи
 * и чтения данных из цифрового порта устройства.
 * Соответствующие капс-коды: ::RSH_CAPS_DEVICE_HAS_DIGITAL_PORT
 * и ::RSH_CAPS_SOFT_INIT_PORT.
 *
 */
struct  RshInitPort : public RshBaseType
{
    /*!
	 *
	 * \~english
	 * \brief
	 * Port operation
	 *
	 * Operation defined by code from
	 * RshInitPort::OperationType enum
	 *
	 * \~russian
	 * \brief
	 * Выполняемая операция
	 *
	 * Действие (чтение, запись и т.д.) которое
	 * необходимое выполнить. Список
	 * доступных операций - в перечислении
	 * RshInitPort::OperationType.
	 *
	 */
	U32 operationType;

	/*!
	 *
	 * \~english
	 * \brief
	 * Operation list
	 *
	 * Use codes from this enum to specify
	 * operation, that must be performed.\n
	 * Pass operation code in RshInitPort::operationType
	 * field of the initialization structure.
	 *
	 * \~russian
	 * \brief
	 * Список операций
	 *
	 * Используйте коды операций из данного списка,
	 * чтобы задать действие, которое необходимо
	 * выполнить с цифровыми портом.\n
	 * Код операции нужно записать в поле
	 * RshInitPort::operationType  структуры инициализации.
	 *
	 */
	enum OperationType
	{
		/*!
		 *
		 * \~english
		 * \brief
		 * Read data from port
		 *
		 * Read digital port state. Data
		 * acquired will be placed in
		 * RshInitPort::portValue field.
		 * If port have less than 32 lines (usually
		 * it is true), data will be placed in low
		 * part of the variable.
		 *
		 * \~russian
		 * \brief
		 * Чтение данных из цифрового порта
		 *
		 * Чтение состояния цифрового порта. Полученные
		 * данные будут помещены в поле RshInitPort::portValue.
		 * Если разрядность порта меньше 32, используется
		 * младшая часть переменной.\n
		 * Например, если порт имеет 8 линий, данные будут
		 * находиться в младшем байте поля RshInitPort::portValue
		 * после успешного выполнения операции чтения (вызов метода
		 * IRshDevice::Init() ).
		 *
		 */
		Read = 0x0,

		/*!
		 *
		 * \~english
		 * \brief
		 * Write to digital port
		 *
		 * Value specified in RshInitPort::portValue
		 * will be written in digital port.
		 *
		 * \~russian
		 * \brief
		 * Запись в цифровой порт
		 *
		 * Значение, указанное в поле RshInitPort::portValue,
		 * будет выведено в цифровой порт.\n
		 * Если количество линий цифрового порта меньше, чем разрядность
		 * RshInitPort::portValue (32 бита), используется младшая часть.
		 *
		 */
		Write = 0x1,

		/*!
		 *
		 * \~english
		 * \brief
		 * Update port state using "AND"
		 *
		 * This operation consists of two parts:\n
		 * First, current value of digital port will
		 * be obtained. After that, obtained value will
		 * be combined using AND bit operation with value
		 * specified in RshInitPort::portValue field.
		 * Resulting value will be written to port.
		 *
		 * \~russian
		 * \brief
		 * Обновить состояние цифрового порта по "И"
		 *
		 * Данная операция состоит из двух частей:\n
		 * Сначала, будет прочитано текущее состояние цифрового порта.
		 * Далее, полученно значение будет объединено по "И"
		 * (логическая побитовая операция) со значением, переданным в поле
		 * RshInitPort::portValue структуры инициализации. Результат
		 * операции будет записан в цифровой порт.
		 *
		 */
		WriteAND = 0x2,

		/*!
		 *
		 * \~english
		 * \brief
		 * Update port state using "OR"
		 *
		 * This operation consists of two parts:\n
		 * First, current value of digital port will
		 * be obtained. After that, obtained value will
		 * be combined using OR bit operation with value
		 * specified in RshInitPort::portValue field.
		 * Resulting value will be written to port.
		 *
		 * \~russian
		 * \brief
		 * Обновить состояние цифрового порта по "ИЛИ"
		 *
		 * Данная операция состоит из двух частей:\n
		 * Сначала, будет прочитано текущее состояние цифрового порта.
		 * Далее, полученно значение будет объединено по "ИЛИ"
		 * (логическая побитовая операция) со значением, переданным в поле
		 * RshInitPort::portValue структуры инициализации. Результат
		 * операции будет записан в цифровой порт.
		 *
		 */
		WriteOR = 0x3
	};

    /*!
	 *
	 * \~english
	 * \brief
	 * Digital port adress
	 *
	 * This field specify adress of the digital port
	 * to use for operation.\n
	 * One can get device port list using
	 * IRshDevice::Get() method.
	 * See description of RshBoardPortInfo for details on
	 * how get list of availiable ports.
	 *
	 * \~russian
	 * \brief
	 * Адрес цифрового порта
	 *
	 * Данное поле определяет адрес порта,
	 * который будет использован для вывполения
	 * операции чтения/записи.\n
	 * Список доступных адресов можно получить,
	 * используя метод IRshDevice::Get().
	 * Смотрите описание структуры RshBoardPortInfo
	 * и примеры.
	 *
	 */
	U32 portAddress;

    /*!
	 *
	 * \~english
	 * \brief
	 * Port value
	 *
	 * Value to write to port or result of reading operation.
	 *
	 * \~russian
	 * \brief
	 * Состояние порта
	 *
	 * Значение, которое нужно записать в порт,
	 * либо результат операции чтения данных.\n
	 * Если разрядность цифрового порта меньше,
	 * чем размер переменной (32 бита), используeтся
	 * младшая часть.\n
	 * Для 8ми битных цифровых портов, например, данные
	 * будут находитсья в младшем байте.
	 *
	 */
	U32 portValue;

    RshInitPort();

	RshInitPort(RshInitPort const& obj);

	RshInitPort& operator=(RshInitPort& obj);

	bool operator==(RshInitPort& obj);

	bool operator!=(RshInitPort& obj);
};

/*!
	\brief
	USB packet

	This structure can be used to send packets
	directly to USB device.

	\todo Documentation and possible refactoring
*/
struct  RshInitPacket : public RshBaseType
{
    U32 operationType;  //!< Operation to perform

	enum OperationType
	{
		Read = 0,
		Write,
		WriteAndRead
	};

    RSH_BUFFER_U8 packet;   //!< Packet data

    RshInitPacket();

	void Set(U8 b0,U8 b1,U8 b2,U8 b3);

	U8& operator[] (const size_t index);

	RshInitPacket(RshInitPacket const& obj);

	RshInitPacket& operator=(RshInitPacket& obj);

	bool operator==(RshInitPacket& obj);

	bool operator!=(RshInitPacket& obj);

};

/*!
  \brief
  Cтруктура инициализации для вольтметров типа B7-78.

  \todo Documentation and possible refactoring

*/
struct  RshInitVoltmeter : public RshBaseType
{

    U32         startType;	//!< тип запуска
	U32	        bufferSize;	//!< размер буфера в отсчётах (значение пересчитывается при инициализации в зависимости от сопутствующих настроек)
	U32         filter;		//!< фильтр
	U32         control;	//!< настройки устройства

	enum StartType // возможные режимы запуска устройства
	{
		Program			= 0x0, //!< программный запуск
	};
	enum Control
	{
		VoltageDC = 0x0, //!< измерение отношения постоянных напряжений
		VoltageAC = 0x1, //!< измерение переменного напряжения
		CurrentDC = 0x2, //!< измерение постоянного тока
		CurrentAC = 0x4  //!< измерение переменного тока
	};
    RshInitVoltmeter();

	RshInitVoltmeter(RshInitVoltmeter const& obj);

	RshInitVoltmeter& operator=(RshInitVoltmeter& obj);

	bool operator==(RshInitVoltmeter& obj);

	bool operator!=(RshInitVoltmeter& obj);
};

/*!
 *
 * \~english
 * \brief
 * Direct timers control structure
 *
 * This structure can be used for
 * devices with 3 channel timer installed.\n
 * Using this structure one can setup each
 * timer work mode and value, programming
 * device to do data acquisition in some
 * sophisticated manner.
 *
 * \see
 * ::RSH_CAPS_DEVICE_TIMER_8254
 *
 * \~russian
 * \brief
 * Прямое управление таймерами
 *
 * Данная структура инициализации может быть
 * использована для устройств, в которых
 * установлен трехканальный счетчик-таймер.\n
 * С помощью данной структуры можно задать значение
 * для каждого таймера, а также режим его работы.
 * Таким образом, можно установить некий специфический
 * режим работы устройства, который невозможно реализовать
 * через стандартные структуры инициализации.
 *
 * \see
 * ::RSH_CAPS_DEVICE_TIMER_8254
 *
 */
struct  RshInitTimer : public RshBaseType
{
	/*!
	 *
	 * \~english
	 * \brief
	 * Work mode for timer 0.
	 *
	 * One value from RshInitTimer::TimerMode enum.
	 *
	 * \~russian
	 * \brief
	 * Режима работы счетчика-таймера 0.
	 *
	 * Одно из значений перечисления RshInitTimer::TimerMode.
	 *
	 */
    U32 timer0Mode;

	/*!
	 *
	 * \~english
	 * \brief
	 * Work mode for timer 1.
	 *
	 * One value from RshInitTimer::TimerMode enum.
	 *
	 * \~russian
	 * \brief
	 * Режима работы счетчика-таймера 1.
	 *
	 * Одно из значений перечисления RshInitTimer::TimerMode.
	 *
	 */
	U32 timer1Mode;

	/*!
	 *
	 * \~english
	 * \brief
	 * Work mode for timer 2.
	 *
	 * One value from RshInitTimer::TimerMode enum.
	 *
	 * \~russian
	 * \brief
	 * Режима работы счетчика-таймера 2.
	 *
	 * Одно из значений перечисления RshInitTimer::TimerMode.
	 *
	 */
	U32 timer2Mode;

	/*!
	 *
	 * \~english
	 * \brief
	 * Counter value for timer 0.
	 *
	 * \~russian
	 * \brief
	 * Значение счетчика для таймера 0.
	 *
	 */
	U32 timer0Count;

	/*!
	 *
	 * \~english
	 * \brief
	 * Counter value for timer 1.
	 *
	 * \~russian
	 * \brief
	 * Значение счетчика для таймера 1.
	 *
	 */
	U32 timer1Count;

	/*!
	 *
	 * \~english
	 * \brief
	 * Counter value for timer 2.
	 *
	 * \~russian
	 * \brief
	 * Значение счетчика для таймера 2.
	 *
	 */
	U32 timer2Count;

	/*!
	 *
	 * \~english
	 * \brief
	 * Timer work mode selection
	 *
	 * \~russian
	 * \brief
	 * Выбор режима работы счетчика-таймера.
	 *
	 */
	enum TimerMode
	{
		/*!
		 * \~english
		 * \brief
		 * Interrupt on terminal count.
		 * \~russian
		 * \brief
		 * Прерывание терминального счета.
		 */
		InterruptOnTerminalCount = 0,

		/*!
		 * \~english
		 * \brief
		 * Programmable one shot.
		 * \~russian
		 * \brief
		 * Ждущий мультивибратор.
		 */
		ProgrammableOneShot = 1,

		/*!
		 * \~english
		 * \brief
		 * Rate generator.
		 * \~russian
		 * \brief
		 * Генератор частоты.
		 */
		RateGenerator = 2,

		/*!
		 * \~english
		 * \brief
		 * Square wave generator.
		 * \~russian
		 * \brief
		 * Генератор меандра.
		 */
		SquareWaveGenerator = 3,

		/*!
		 * \~english
		 * \brief
		 * Software triggered strobe.
		 * \~russian
		 * \brief
		 * Счетчик событий.
		 */
		SoftwareTriggeredStrobe = 4,

		/*!
		 * \~english
		 * \brief
		 * Hardware tiggered strobe.
		 * \~russian
		 * \brief
		 * Счетчик событий с автозагрузкой.
		 */
		HardwareTriggeredStrobe = 5
	};

	RshInitTimer();

	RshInitTimer(RshInitTimer const& obj);

	RshInitTimer& operator=(RshInitTimer& obj);

	bool operator==(RshInitTimer& obj);

	bool operator!=(RshInitTimer& obj);
};

#pragma pack(pop)

#endif //__RSH_DEVICE_H__
