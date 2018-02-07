/*!
 * \copyright JSC "Rudnev-Shilyaev"
 *
 * \file RshApi.cpp
 * \date 11.04.2014
 * \version 1.0 [SDK 2.00]
 *
 * \~english
 * \brief
 * Rsh Api internal class and template implementation
 *
 * \~russian
 * \brief
 * Статическая библиотека с реализацией методов классов и шаблонов
 *
 */

#ifndef __RSH_API_CPP__
#define __RSH_API_CPP__

#include "RshApi.h"
#include "IRshDevice.h"

//------------------------------------------------------------------------------------------------
//RshBaseType
RshBaseType::RshBaseType()
{
	_type = rshTypeUndefined;
	_typeSize = 0;

}

const char* RshBaseType::GetTypeName()
	{
		switch(_type)
		{
		//<BeginTypeStrings>
		case rshU8: return "RshU8";
		case rshS8: return "RshS8";
		case rshU16: return "RshU16";
		case rshS16: return "RshS16";
		case rshU32: return "RshU32";
		case rshS32: return "RshS32";
		case rshU64: return "RshU64";
		case rshS64: return "RshS64";
		case rshFloat: return "RshFloat";
		case rshDouble: return "RshDouble";
		case rshBool: return "RshBool";
		case rshVoidP: return "RshVoidP";
		case rshU8P: return "RshU8P";
		case rshS8P: return "RshS8P";
		case rshU16P: return "RshU16P";
		case rshS16P: return "RshS16P";
		case rshU32P: return "RshU32P";
		case rshS32P: return "RshS32P";
		case rshU64P: return "RshU64P";
		case rshS64P: return "RshS64P";
		case rshFloatP: return "RshFloatP";
		case rshDoubleP: return "RshDoubleP";
		case rshBoolP: return "RshBoolP";
		case rshU8PP: return "RshU8PP";
		case rshS8PP: return "RshS8PP";
		case rshU16PP: return "RshU16PP";
		case rshS16PP: return "RshS16PP";
		case rshU32PP: return "RshU32PP";
		case rshS32PP: return "RshS32PP";
		case rshU64PP: return "RshU64PP";
		case rshS64PP: return "RshS64PP";
		case rshFloatPP: return "RshFloatPP";
		case rshDoublePP: return "RshDoublePP";
		case rshBoolPP: return "RshBoolPP";
		case rshBufferTypeU8: return "RshBufferTypeU8";
		case rshBufferTypeS8: return "RshBufferTypeS8";
		case rshBufferTypeU16: return "RshBufferTypeU16";
		case rshBufferTypeS16: return "RshBufferTypeS16";
		case rshBufferTypeU32: return "RshBufferTypeU32";
		case rshBufferTypeS32: return "RshBufferTypeS32";
		case rshBufferTypeU64: return "RshBufferTypeU64";
		case rshBufferTypeS64: return "RshBufferTypeS64";
		case rshBufferTypeFloat: return "RshBufferTypeFloat";
		case rshBufferTypeDouble: return "RshBufferTypeDouble";
		case rshBufferTypeBool: return "RshBufferTypeBool";
		case rshBufferTypeChannel: return "RshBufferTypeChannel";
		case rshBufferTypePortInfo: return "RshBufferTypePortInfo";
		case rshBufferTypeSetting: return "RshBufferTypeSetting";
		case rshBufferTypeSettingChannel: return "RshBufferTypeSettingChannel";
		case rshDBufferTypeU8: return "RshDBufferTypeU8";
		case rshDBufferTypeS8: return "RshDBufferTypeS8";
		case rshDBufferTypeU16: return "RshDBufferTypeU16";
		case rshDBufferTypeS16: return "RshDBufferTypeS16";
		case rshDBufferTypeU32: return "RshDBufferTypeU32";
		case rshDBufferTypeS32: return "RshDBufferTypeS32";
		case rshDBufferTypeU64: return "RshDBufferTypeU64";
		case rshDBufferTypeS64: return "RshDBufferTypeS64";
		case rshDBufferTypeFloat: return "RshDBufferTypeFloat";
		case rshDBufferTypeDouble: return "RshDBufferTypeDouble";
		case rshDBufferTypeBool: return "RshDBufferTypeBool";
		case rshBufferTypeU8P: return "RshBufferTypeU8P";
		case rshBufferTypeDeviceBaseInfo: return "RshBufferTypeDeviceBaseInfo";
		case rshBufferTypeDeviceFullInfo: return "RshBufferTypeDeviceFullInfo";
		case rshBufferTypeChannelVoltage: return "RshBufferTypeChannelVoltage";
		case rshBufferTypeCalibrationItem: return "RshBufferTypeCalibrationItem";
		case rshBufferTypeCalibrationGroup: return "RshBufferTypeCalibrationGroup";
		case rshInitADC: return "RshInitADC";
		case rshInitGSPF: return "RshInitGSPF";
		case rshInitDMA: return "RshInitDMA";
		case rshInitMemory: return "RshInitMemory";
		case rshInitVoltmeter: return "RshInitVoltmeter";
		case rshInitPort: return "RshInitPort";
		case rshInitPacket: return "RshInitPacket";
		case rshInitDAC: return "RshInitDAC";
		case rshInitTimer: return "RshInitTimer";
		case rshRegister: return "RshRegister";
		case rshPort: return "RshPort";
		case rshGatheringParameters: return "RshGatheringParameters";
		case rshDeviceActiveList: return "RshDeviceActiveList";
		case rshOnboardBaseInfo: return "RshOnboardBaseInfo";
		case rshDeviceBaseInfo: return "RshDeviceBaseInfo";
		case rshDeviceFullInfo: return "RshDeviceFullInfo";
		case rshDeviceBuffer: return "RshDeviceBuffer";
		case rshPortInfo: return "RshPortInfo";
		case rshBoardPortInfo: return "RshBoardPortInfo";
		case rshIFactory: return "RshIFactory";
		case rshIRSHDevice: return "RshIRSHDevice";
		case rshChannel: return "RshChannel";
		case rshSynchroChannel: return "RshSynchroChannel";
		case rshCalibrationParameters: return "RshCalibrationParameters";
		case rshCalibrationControl: return "RshCalibrationControl";
		case rshIDebug: return "RshIDebug";
		case rshChannelVoltage: return "RshChannelVoltage";
		case rshDeviceKey: return "RshDeviceKey";
		case rshCalibrationChannelParameter: return "RshCalibrationChannelParameter";
		case rshCalibrationItemBase: return "RshCalibrationItemBase";
		case rshCalibrationItemEntity: return "RshCalibrationItemEntity";
		case rshCalibrationItemRegister: return "RshCalibrationItemRegister";
		case rshCalibrationItemButton: return "RshCalibrationItemButton";
		case rshCalibrationItemFilePath: return "RshCalibrationItemFilePath";
		case rshBoardInfoDMA: return "RshBoardInfoDMA";
		case rshBoardInfoMemory: return "RshBoardInfoMemory";
		case rshBoardInfoDAC: return "RshBoardInfoDAC";
		case rshBoardInfoDP: return "RshBoardInfoDP";
		case rshFRDACTune: return "RshFRDACTune";
		case rshDPADataFFTComplex: return "RshDPADataFFTComplex";
		case rshDPADataWindowFunction: return "RshDPADataWindowFunction";
		case rshDPADataFindGap: return "RshDPADataFindGap";
		case rshDPADataFindFront: return "RshDPADataFindFront";
		case rshDPADataGeneratorSignalBase: return "RshDPADataGeneratorSignalBase";
		//</BeginTypeStrings>
		}
	return "UnknownDataType";
	}

//------------------------------------------------------------------------------------------------
//RshType template
template <class T, RshDataTypes dataCode>
	RshType<T, dataCode>::RshType() {
		this->data = 0;
		this->_type = dataCode;
		this->_typeSize = sizeof (RshType<T, dataCode>);
	}

template <class T, RshDataTypes dataCode>
	RshType<T, dataCode>::RshType(T value) {
		this->data = value;
		this->_type = dataCode;
		this->_typeSize = sizeof (RshType<T, dataCode>);
	}

template <class T, RshDataTypes dataCode>
	RshType<T, dataCode>::RshType(RshType<T, dataCode> const& obj) {
		this->data = obj.data;
		this->_type = dataCode;
		this->_typeSize = sizeof (RshType<T, dataCode>);
	}

template <class T, RshDataTypes dataCode>
	RshType<T, dataCode>& RshType<T, dataCode>::operator=(RshType<T, dataCode> & obj) {
		data = obj.data;
		return *this;
	}

template <class T, RshDataTypes dataCode>
	RshType<T, dataCode>& RshType<T, dataCode>::operator=(T value) {
		data = value;
		return *this;
	}
//------------------------------------------------------------------------------------------------
//RshBufferType template
template <class T, RshDataTypes dataCode>
	RshBufferType<T, dataCode>::RshBufferType(size_t bufferSize) {
		this->size = 0;
		this->ptr = 0;
		this->_type = dataCode;
		this->_typeSize = sizeof (RshBufferType<T, dataCode>);

		try {
			this->ptr = new T[bufferSize];
			this->psize = bufferSize;
			//            this->it = this->ptr;
		} catch (...) {
			this->psize = 0;
			this->ptr = 0;
			//           this->it = 0;
		}
	}

template <class T, RshDataTypes dataCode>
	RshBufferType<T, dataCode>::RshBufferType(const RshBufferType<T, dataCode>& obj) {
		this->_type = dataCode;
		this->_typeSize = sizeof (RshBufferType);

		try {
			this->ptr = new T[obj.psize];
			this->psize = obj.psize;
			//this->it = this->ptr;
			Copy(obj);
		} catch (...) {
			this->psize = 0;
			this->ptr = 0;
			//            this->it = 0;
		}
	}

template <class T, RshDataTypes dataCode>
	RshBufferType<T, dataCode>::~RshBufferType() {
		if (ptr)
		{
			delete [] ptr;
			ptr = 0;
		}
	}

template <class T, RshDataTypes dataCode>
	U32 RshBufferType<T, dataCode>::Allocate(size_t bufferSize) {

		if( bufferSize == 0 ) return RSH_API_BUFFER_ALLOCATIONZEROSIZE;

		if (ptr)
		{
			if( this->psize == bufferSize ) return RSH_API_SUCCESS; //  if requested the same size  then nothing happens
			else
				delete ptr;
		}

		this->size = 0;
		this->ptr = 0;

		try {
			this->ptr = new T[bufferSize];
			this->psize = bufferSize;
			memset(ptr, 0, bufferSize * sizeof (T));
			//            this->it = this->ptr;
		} catch (...) {
			this->psize = 0;
			this->ptr = 0;
			//           this->it = 0;

			return RSH_API_MEMORY_ALLOCATIONERROR;
		}

		return RSH_API_SUCCESS;
	}

template <class T, RshDataTypes dataCode>
	void RshBufferType<T, dataCode>::Copy(T* iptr, size_t psize, size_t size) {
		size_t copySize;
		if( size == 0 )
			copySize = this->psize > psize ? psize : this->psize;
		else
			copySize = this->psize > size ? size : this->psize;


		for (size_t i = 0; i < copySize; i++)
			this->ptr[i] = iptr[i];

		this->size = copySize;
	}

template <class T, RshDataTypes dataCode>
	size_t RshBufferType<T, dataCode>::CopyTo(T* iptr, size_t iptrSize) {
		size_t copySize;
		if( size == 0 ) return 0;

		copySize = this->psize > iptrSize ? iptrSize : this->psize;

		for (size_t i = 0; i < copySize; i++)
			iptr[i] = this->ptr[i];

		return copySize;
	}

template <class T, RshDataTypes dataCode>
	void RshBufferType<T, dataCode>::Copy(RshBufferType<T, dataCode> const& srcBuffer) {
		size_t copySize;
		if( srcBuffer.size == 0 )
		{
			copySize = this->psize > srcBuffer.psize ? srcBuffer.psize : this->psize;
			this->size = 0;
		}
		else
		{
			copySize = this->psize > srcBuffer.size ? srcBuffer.size : this->psize;
			this->size = srcBuffer.size;
		}

		for (size_t i = 0; i < copySize; i++)
			this->ptr[i] = srcBuffer.ptr[i];
	}

template <class T, RshDataTypes dataCode>
	U32 RshBufferType<T, dataCode>::SortAscending()
	{
		if( ptr && (this->psize != 0))
		{
			if( this->size == 0 )
				QuickSortAscending(0, this->psize - 1);
			else
				QuickSortAscending(0, this->size - 1);
		}
		else
			return RSH_API_BUFFER_NOTINITIALIZED;

		return RSH_API_SUCCESS;
	}

template <class T, RshDataTypes dataCode>
	U32 RshBufferType<T, dataCode>::SortDescending()
	{
		if( ptr && (this->psize != 0))
		{
			if( this->size == 0 )
				QuickSortDescending(0, this->psize - 1);
			else
				QuickSortDescending(0, this->size - 1);
		}
		else
			return RSH_API_BUFFER_NOTINITIALIZED;

		return RSH_API_SUCCESS;
	}

template <class T, RshDataTypes dataCode>
	void RshBufferType<T, dataCode>::WriteDataToConsole(unsigned size)
	{
		if( ptr && (this->psize != 0))
		{
			U32 printSize = 0;
			if(size>0)
				printSize = size <= this->psize ? size: this->psize;
			else
				printSize = (this->size == 0) ? this->psize : this->size;

			std::cout << std::endl;
			for(U32 i=0 ; i<printSize; ++i)
				std::cout << this->ptr[i] << " ";
		}
		else
			std::cout << "\nBuffer is not initialized!";
	}

template <class T, RshDataTypes dataCode>
	U32 RshBufferType<T, dataCode>::WriteDataToFile(const wchar_t* fileName)
	{
		return WriteToFile(fileName, ByteSize());
	}

template <class T, RshDataTypes dataCode>
	U32 RshBufferType<T, dataCode>::WriteBufferToFile(const wchar_t* fileName)
	{
		return WriteToFile(fileName, psize * TypeSize());
	}

template <class T, RshDataTypes dataCode>
	U32 RshBufferType<T, dataCode>::ReadFromFile(const wchar_t* fileName)
	{
#if defined(RSH_MSWINDOWS)
		HANDLE hFile = NULL;
		//DWORD wrtn;

		hFile = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

		if (hFile != NULL ) {
			DWORD fileSize, bytesRead;
			size = 0;

			fileSize = GetFileSize(hFile,NULL);

			U32 res = this->Allocate(fileSize / sizeof(T));
			if(res != RSH_API_SUCCESS)
				return res;

			BOOL st = ReadFile(hFile,ptr,fileSize, &bytesRead,NULL);

			CloseHandle(hFile);
			if( st == TRUE )
			{
				size = bytesRead / sizeof(T);
				return RSH_API_SUCCESS;
			}
			else
				return RSH_API_FILE_CANTREAD | GetLastError();
		} else
			return RSH_API_FILE_CANTOPEN | GetLastError();
#elif defined(RSH_LINUX)
		//		 \todo Linux version of this method
#endif

		return RSH_API_SUCCESS;
	}

template <class T, RshDataTypes dataCode>
	void RshBufferType<T, dataCode>::QuickSortAscending(S64 left, S64 right)
	{
		S64 i = left, j = right;
		T tmp;
		T pivot = this->ptr[(left + right) / 2];

		/* partition */
		while (i <= j) {
			while (this->ptr[i] < pivot)
				i++;
			while (this->ptr[j] > pivot)
				j--;
			if (i <= j) {
				tmp = this->ptr[i];
				this->ptr[i] = this->ptr[j];
				this->ptr[j] = tmp;
				i++;
				j--;
			}
		};

		/* recursion */
		if (left < j) QuickSortAscending(left, j);
		if (i < right) QuickSortAscending(i, right);
	}

template <class T, RshDataTypes dataCode>
	void RshBufferType<T, dataCode>::QuickSortDescending(S64 left, S64 right)
	{
		S64 i = left, j = right;
		T tmp;
		T pivot = this->ptr[(left + right) / 2];

		/* partition */
		while (i <= j) {
			while (this->ptr[i] > pivot)
				i++;
			while (this->ptr[j] < pivot)
				j--;
			if (i <= j) {
				tmp = this->ptr[i];
				this->ptr[i] = this->ptr[j];
				this->ptr[j] = tmp;
				i++;
				j--;
			}
		};

		/* recursion */
		if (left < j) QuickSortDescending(left, j);
		if (i < right) QuickSortDescending(i, right);
	}

template <class T, RshDataTypes dataCode>
	U32 RshBufferType<T, dataCode>::WriteToFile(const wchar_t* fileName, U32 dataSize)
	{
		U32 byteSize = dataSize;
		if (byteSize == 0) return RSH_API_BUFFER_ISEMPTY;
#if defined(RSH_MSWINDOWS)
		HANDLE hFile;
		DWORD wrtn;

		hFile = CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

		if (hFile && (WriteFile(hFile, ptr, byteSize, &wrtn, NULL) == TRUE)) {
			CloseHandle(hFile);
			return RSH_API_SUCCESS;
		} else
			return RSH_API_FILE_CANTCREATE | GetLastError();
#elif defined(RSH_LINUX)

		std::wstring tmp = std::wstring(fileName);
		std::string fnChar = std::string(tmp.begin(),tmp.end());
		FILE* hFile; // Файловый дескриптор.
		hFile = fopen(fnChar.c_str(), "w");
		if (hFile != NULL) {
			fwrite(ptr, byteSize, 1, hFile);
			fclose(hFile);
			return RSH_API_SUCCESS;
		} else
			return RSH_API_FILE_CANTCREATE;

#endif
	}

template <class T, RshDataTypes dataCode>
	size_t RshBufferType<T, dataCode>::ByteSize()
	{
		return size * sizeof (T);
	}

template <class T, RshDataTypes dataCode>
	size_t RshBufferType<T, dataCode>::BytePSize()
	{
		return psize * sizeof (T);
	}

template <class T, RshDataTypes dataCode>
	size_t RshBufferType<T, dataCode>::TypeSize()
	{
		return sizeof (T);
	}

template <class T, RshDataTypes dataCode>
	size_t RshBufferType<T, dataCode>::Size()
	{
		return this->size;
	}

template <class T, RshDataTypes dataCode>
	size_t RshBufferType<T, dataCode>::PSize()
	{
		return this->psize;
	}

template <class T, RshDataTypes dataCode>
	T& RshBufferType<T, dataCode>::operator[](size_t i) {
		return ptr[i];
	}

template <class T, RshDataTypes dataCode>
	void RshBufferType<T, dataCode>::Clear()
	{
		this->size = 0;
	}

template <class T, RshDataTypes dataCode>
	void RshBufferType<T, dataCode>::SetSize(size_t size)
	{
		if( size < 0 ) { this->size = 0; return; }
		if( size > psize ) { this->size = this->psize; return; }

		this->size = size;
	}

template <class T, RshDataTypes dataCode>
	RshBufferType<T, dataCode>&	RshBufferType<T, dataCode>::operator=(RshBufferType<T, dataCode>& obj)
	{
            Allocate(obj.psize);
			Copy(obj.ptr, obj.psize);
			return *this;
	}

template <class T, RshDataTypes dataCode>
	bool RshBufferType<T, dataCode>::operator==(RshBufferType<T, dataCode>& obj)
	{
			if (
				(this->psize == obj.psize)
				) {
					for (size_t i = 0; i < this->psize; i++) {
						if (this->ptr[i] != obj.ptr[i]) return false;
					}

					return true;
			} else
				return false;
	}

template <class T, RshDataTypes dataCode>
	bool RshBufferType<T, dataCode>::operator!=(RshBufferType<T, dataCode>& obj) {
			if (
				(this->psize == obj.psize)
				) {
					for (size_t i = 0; i < this->psize; i++) {
						if (this->ptr[i] != obj.ptr[i]) return true;
					}

					return false;
			} else
				return true;
	}

template <class T, RshDataTypes dataCode>
U32 RshBufferType<T, dataCode>::PushBack(T obj)
	{
		if( psize == size ) return RSH_API_BUFFER_INSUFFICIENTSIZE;

		ptr[size++] = obj;

		return RSH_API_SUCCESS;
	}
//------------------------------------------------------------------------------------------------
//RshDBufferType template
template <class T, RshDataTypes dataCode>
	RshDBufferType<T, dataCode>::RshDBufferType(size_t bufferSize) {
		this->size = 0;
		this->ptr = 0;
		this->_type = dataCode;
		this->_typeSize = sizeof (RshDBufferType);

		try {
			this->ptr = new RshBufferType<T, dataCode>[bufferSize];
			this->psize = bufferSize;
			//this->it = this->ptr;
		} catch (...) {
			this->psize = 0;
			this->ptr = 0;
//			this->it = 0;
		}
	}

template <class T, RshDataTypes dataCode>
	void RshDBufferType<T, dataCode>::Allocate(size_t bufferSize) {
		if (ptr)
			delete [] ptr;

		this->size = 0;
		this->ptr = 0;

		try {
			this->ptr = new RshBufferType<T, dataCode>[bufferSize];
			this->psize = bufferSize;
			//this->it = this->ptr;
		} catch (...) {
			this->psize = 0;
			this->ptr = 0;
//			this->it = 0;
		}
	}

template <class T, RshDataTypes dataCode>
	void RshDBufferType<T, dataCode>::Copy(RshBufferType<T, dataCode>* iptr, size_t size) {
		size_t copySize = psize > size ? size : psize;
		for (size_t i = 0; i < copySize; i++)
			this->ptr[i] = iptr[i];

		this->size = copySize;
	}

template <class T, RshDataTypes dataCode>
	void RshDBufferType<T, dataCode>::Copy(RshDBufferType<T, dataCode> const& obj) {
		Copy(obj.ptr, obj.psize);
	}

template <class T, RshDataTypes dataCode>
	RshDBufferType<T, dataCode>::~RshDBufferType() {
		if (ptr)
			delete [] ptr;
	}

template <class T, RshDataTypes dataCode>
	size_t RshDBufferType<T, dataCode>::ByteSize() {
		return size * sizeof (RshBufferType<T, dataCode>);
	}

template <class T, RshDataTypes dataCode>
	RshBufferType<T, dataCode>& RshDBufferType<T, dataCode>::operator[](size_t i) {
		return ptr[i];
	}

template <class T, RshDataTypes dataCode>
	RshDBufferType<T, dataCode>& RshDBufferType<T, dataCode>::operator=(RshDBufferType<T, dataCode>& obj) {
			Copy(obj.ptr, obj.psize);
			return *this;
	}

template <class T, RshDataTypes dataCode>
	bool RshDBufferType<T, dataCode>::operator==(RshDBufferType<T, dataCode>& obj) {
			if (
				(this->psize == obj.psize)
				) {
					for (size_t i = 0; i < this->psize; i++) {
						if (this->ptr[i] != obj.ptr[i]) return false;
					}

					return true;
			} else
				return false;
	}

template <class T, RshDataTypes dataCode>
	bool RshDBufferType<T, dataCode>::operator!=(RshDBufferType<T, dataCode>& obj) {
			if (
				(this->psize == obj.psize)
				) {
					for (size_t i = 0; i < this->psize; i++) {
						if (this->ptr[i] != obj.ptr[i]) return true;
					}

					return false;
			} else
				return true;
	}

//------------------------------------------------------------------------------------------------
//RshDeviceBuffer
RshDeviceBuffer::RshDeviceBuffer() {
		this->_type = rshDeviceBuffer;
		this->_typeSize = sizeof (RshDeviceBuffer);
		sampleQuantity = 0;
		sampleSize = 0;
		buffer = 0;
	}

//------------------------------------------------------------------------------------------------
//RshRegister
RshRegister::RshRegister() {
		this->_type = rshRegister;
		this->_typeSize = sizeof (RshRegister);
		size = 1;
		offset = 0;
		value = 0;
	}

//------------------------------------------------------------------------------------------------
//RshGatheringParameters
RshGatheringParameters::RshGatheringParameters() {
		this->_type = rshGatheringParameters;
		this->_typeSize = sizeof (RshGatheringParameters);
		userBufferSize = 0;
		measureSize = 0;
		transferDirection = 0;
		userBuffer = 0;
		evenPart = 0;
	}

//------------------------------------------------------------------------------------------------
//RshFRDACTune
RshFRDACTune::RshFRDACTune()
	{
		this->_type = rshFRDACTune;
		this->_typeSize = sizeof(RshFRDACTune);
		flashBlocks = 0;
	}

//------------------------------------------------------------------------------------------------
//RshChannel class
RshChannel::RshChannel()
	{
		this->_typeSize = sizeof(RshChannel);
		this->_type = rshChannel;
		gain = 1;
		control = 0;
		adjustment = 0.0;
	}

bool RshChannel::operator==(RshChannel& obj)
	{
		return
			RSH_CMPDOUBLE(this->adjustment,obj.adjustment)	&&
			(this->control == obj.control)			&&
			(this->gain == obj.gain);
	}

bool RshChannel::operator!=(RshChannel& obj)
	{
		return
			!RSH_CMPDOUBLE(this->adjustment,obj.adjustment)	||
			(this->control != obj.control)			||
			(this->gain != obj.gain);
	}

bool RshChannel::operator< (RshChannel& obj)
	{
		return this->gain < obj.gain;
	}

bool RshChannel::operator> (RshChannel& obj)
	{
		return this->gain > obj.gain;
	}

std::ostream& operator<< (std::ostream &out, RshChannel& obj)
	{
	    // write obj to stream
		out << "[gain="<<obj.gain<<", adjustment="<<obj.adjustment<<", control="<<obj.control<<"]";
		return out;
	}

RshChannel& RshChannel::operator=(RshChannel& obj)
	{
		this->adjustment = obj.adjustment;
		this->control = obj.control;
		this->gain = obj.gain;
		return *this;
	}

//------------------------------------------------------------------------------------------------
//RshSynchroChannel
RshSynchroChannel::RshSynchroChannel()
	{
		this->_typeSize = sizeof(RshSynchroChannel);
		this->_type = rshSynchroChannel;
		gain = 1;
		control = 0;
	}

bool RshSynchroChannel::operator==(RshSynchroChannel& obj)
	{
		return
			(this->control == obj.control)		&&
			(this->gain == obj.gain);
	}

bool RshSynchroChannel::operator!=(RshSynchroChannel& obj)
	{
		return
			(this->control != obj.control)		||
			(this->gain != obj.gain);
	}

RshSynchroChannel& RshSynchroChannel::operator=(RshSynchroChannel& obj)
	{
		this->control = obj.control;
		this->gain = obj.gain;
		return *this;
	}
//------------------------------------------------------------------------------------------------
//RshInitADC
RshInitADC::RshInitADC()
	{
		this->_typeSize = sizeof(RshInitADC);
		this->_type = rshInitADC;
		startType = RshInitADC::Program;
		frequency = 0;
		bufferSize = 0;
	}
RshInitADC::RshInitADC(RshInitADC const& obj)
	{
		this->_typeSize = sizeof(RshInitADC);
		this->_type = rshInitADC;	
		this->startType  = obj.startType;
		this->bufferSize =  obj.bufferSize;
	    this->frequency =  obj.frequency;
		this->controlSynchro = obj.controlSynchro;
		this->threshold = obj.threshold;		

		this->channels.Copy(obj.channels);
	}

RshInitADC& RshInitADC::operator=(RshInitADC& obj)
	{
		this->bufferSize = obj.bufferSize;
		this->channels = obj.channels;		
		this->frequency = obj.frequency;	
		this->startType = obj.startType;
		this->controlSynchro = obj.controlSynchro;
		this->threshold = obj.threshold;
		
		return *this;
	}

bool RshInitADC::operator==(RshInitADC& obj)
	{
		return
			(this->bufferSize == obj.bufferSize)			&&
			(this->channels == obj.channels)				&&
			(this->controlSynchro == obj.controlSynchro)	&&		
			RSH_CMPDOUBLE(this->frequency, obj.frequency)	&&
			RSH_CMPDOUBLE(this->threshold, obj.threshold)	&&
			(this->startType == obj.startType);

	}

bool RshInitADC::operator!=(RshInitADC& obj)
	{
		return
			(this->bufferSize != obj.bufferSize)			||
			(this->channels != obj.channels)				||
			(this->controlSynchro != obj.controlSynchro)	||		
			!RSH_CMPDOUBLE(this->frequency, obj.frequency)	||
			!RSH_CMPDOUBLE(this->threshold, obj.threshold)	||
			(this->startType != obj.startType);
	}

//------------------------------------------------------------------------------------------------
//RshInitDMA
RshInitDMA::RshInitDMA()
	{
		this->_typeSize = sizeof(RshInitDMA);
		this->_type = rshInitDMA;
		dmaMode = RshInitDMA::Single;
		control = RshInitDMA::StandardMode;
		frequencyFrame = 0.0;
	}

RshInitDMA::RshInitDMA(RshInitADC const& obj)
    {
        this->_typeSize = sizeof(RshInitDMA);
        this->_type = rshInitDMA;
        this->dmaMode = 0;
        this->control = 0;
        this->frequencyFrame = 0;
        this->startType  = obj.startType;
        this->bufferSize =  obj.bufferSize;
        this->frequency =  obj.frequency;
		this->threshold = obj.threshold;
		this->controlSynchro = obj.controlSynchro;

        this->channels.Copy(obj.channels);
    }

RshInitDMA::RshInitDMA(RshInitDMA const& obj)
	{
		this->_typeSize = sizeof(RshInitDMA);
		this->_type = rshInitDMA;
		this->dmaMode = obj.dmaMode;
		this->control = obj.control;
		this->frequencyFrame = obj.frequencyFrame;
		this->startType  = obj.startType;
		this->bufferSize =  obj.bufferSize;
	    this->frequency =  obj.frequency;
		this->threshold = obj.threshold;
		this->controlSynchro = obj.controlSynchro;

		this->channels.Copy(obj.channels);
	}

RshInitDMA& RshInitDMA::operator=(RshInitDMA& obj)
	{
		this->bufferSize = obj.bufferSize;
		this->channels = obj.channels;
		this->control = obj.control;
		this->dmaMode = obj.dmaMode;
		this->frequency = obj.frequency;
		this->frequencyFrame = obj.frequencyFrame;
		this->startType = obj.startType;
		this->threshold = obj.threshold;
		this->controlSynchro = obj.controlSynchro;

		return *this;
	}

bool RshInitDMA::operator==(RshInitDMA& obj)
	{
		return
			(this->bufferSize == obj.bufferSize)			&&
			(this->channels == obj.channels)				&&
			(this->control == obj.control)					&&
			(this->dmaMode == obj.dmaMode)					&&
			(this->controlSynchro == obj.controlSynchro)	&&
			RSH_CMPDOUBLE(this->frequency, obj.frequency)	&&
			RSH_CMPDOUBLE(this->frequencyFrame, obj.frequencyFrame)	&&
			RSH_CMPDOUBLE(this->threshold, obj.threshold)	&&
			(this->startType == obj.startType);

	}

bool RshInitDMA::operator!=(RshInitDMA& obj)
	{
		return
			(this->bufferSize != obj.bufferSize)			||
			(this->channels != obj.channels)				||
			(this->control != obj.control)					||
			(this->dmaMode != obj.dmaMode)					||
			(this->controlSynchro != obj.controlSynchro)	||
			!RSH_CMPDOUBLE(this->frequency, obj.frequency)	||
			!RSH_CMPDOUBLE(this->frequencyFrame, obj.frequencyFrame)	||
			!RSH_CMPDOUBLE(this->threshold, obj.threshold)	||
			(this->startType != obj.startType);
	}

//------------------------------------------------------------------------------------------------
//RshInitMemory
RshInitMemory::RshInitMemory()
	{
		this->_typeSize = sizeof(RshInitMemory);
		this->_type = rshInitMemory;
		control = 0;
		preHistory = 0;
		startDelay = 0;
		hysteresis = 0;
		packetNumber = 1;
		threshold = 0.0;
		controlSynchro = 0;
	}

RshInitMemory::RshInitMemory(RshInitADC const& obj)
    {
        this->_typeSize = sizeof(RshInitMemory);
        this->_type = rshInitMemory;
        this->preHistory = 0;
        this->control = 0;
        this->startDelay = 0;
        this->startType  = obj.startType;
        this->hysteresis  = 0;
        this->packetNumber  = 0;
        this->threshold  = 0;
        this->controlSynchro  = 0;
        this->bufferSize =  obj.bufferSize;
        this->frequency =  obj.frequency;

        this->channels.Copy(obj.channels);
    }

RshInitMemory::RshInitMemory(RshInitMemory const& obj)
	{
		
		this->_typeSize = sizeof(RshInitMemory);
		this->_type = rshInitMemory;
		this->preHistory = obj.preHistory;
		this->control = obj.control;
		this->startDelay = obj.startDelay;
		this->startType  = obj.startType;
		this->hysteresis  = obj.hysteresis;
		this->packetNumber  = obj.packetNumber;
		this->threshold  = obj.threshold;
		this->controlSynchro  = obj.controlSynchro;
		this->bufferSize =  obj.bufferSize;
	    this->frequency =  obj.frequency;

		this->channels.Copy(obj.channels);
	}

RshInitMemory& RshInitMemory::operator=(RshInitMemory& obj)
	{
		this->preHistory = obj.preHistory;
		this->control = obj.control;
		this->bufferSize = obj.bufferSize;
		this->channels = obj.channels;
		this->channelSynchro = obj.channelSynchro;
		this->controlSynchro = obj.controlSynchro;
		this->frequency = obj.frequency;
		this->hysteresis = obj.hysteresis;
		this->packetNumber = obj.packetNumber;
		this->startDelay = obj.startDelay;
		this->startType = obj.startType;
		this->threshold = obj.threshold;
		return *this;
	}

bool RshInitMemory::operator==(RshInitMemory& obj)
	{
		return
			(this->preHistory == obj.preHistory)		&&
			(this->bufferSize == obj.bufferSize)			&&
			(this->channels == obj.channels)				&&
			(this->channelSynchro == obj.channelSynchro)	&&
			(this->controlSynchro == obj.controlSynchro)	&&
			RSH_CMPDOUBLE(this->frequency, obj.frequency)	&&
			(this->hysteresis == obj.hysteresis)			&&
			(this->packetNumber == obj.packetNumber)		&&
			(this->startDelay == obj.startDelay)			&&
			(this->startType == obj.startType)				&&
			RSH_CMPDOUBLE(this->threshold, obj.threshold);
	}

bool RshInitMemory::operator!=(RshInitMemory& obj)
	{
		return
			(this->preHistory != obj.preHistory)		||
			(this->bufferSize != obj.bufferSize)			||
			(this->channels != obj.channels)				||
			(this->channelSynchro != obj.channelSynchro)	||
			(this->controlSynchro != obj.controlSynchro)	||
			!RSH_CMPDOUBLE(this->frequency, obj.frequency)	||
			(this->hysteresis != obj.hysteresis)			||
			(this->packetNumber != obj.packetNumber)		||
			(this->startDelay != obj.startDelay)			||
			(this->startType != obj.startType)				||
			!RSH_CMPDOUBLE(this->threshold, obj.threshold);
	}
//------------------------------------------------------------------------------------------------
//RshInitGSPF
RshInitGSPF::RshInitGSPF()
    {
        this->_typeSize = sizeof(RshInitGSPF);
        this->_type = rshInitGSPF;
		startType = RshInitGSPF::Program;
        frequency = 0.0;
		control = RshInitGSPF::SynthesizerOff|RshInitGSPF::FilterOff|
			RshInitGSPF::PlayOnce|RshInitGSPF::SynchroFront;
		attenuator = RshInitGSPF::AttenuationOff;
    }

RshInitGSPF::RshInitGSPF(RshInitGSPF const& obj)
	{
		this->_typeSize = sizeof(RshInitGSPF);
		this->_type = rshInitGSPF;
		this->control = obj.control;
		this->startType  = obj.startType;
		this->attenuator  = obj.attenuator;
	    this->frequency =  obj.frequency;

	}

RshInitGSPF& RshInitGSPF::operator=(RshInitGSPF& obj)
	{
		this->control = obj.control;
		this->frequency = obj.frequency;
		this->startType = obj.startType;
		this->attenuator  = obj.attenuator;
		return *this;
	}

bool RshInitGSPF::operator==(RshInitGSPF& obj)
	{
		return
			RSH_CMPDOUBLE(this->frequency, obj.frequency)	&&
			(this->startType == obj.startType)				&&
			(this->control == obj.control)					&&
			(this->attenuator  == obj.attenuator);
	}

bool RshInitGSPF::operator!=(RshInitGSPF& obj)
	{
		return
			!RSH_CMPDOUBLE(this->frequency, obj.frequency)	||
			(this->startType != obj.startType)				||
			(this->control != obj.control)					||
			(this->attenuator  != obj.attenuator);
	}

//------------------------------------------------------------------------------------------------
//RshInitDAC
RshInitDAC::RshInitDAC()
    {
        this->_typeSize = sizeof(RshInitDAC);
        this->_type = rshInitDAC;
		id = 0;
		voltage = 0.0;
    }

RshInitDAC::RshInitDAC(RshInitDAC const& obj)
	{
		this->_typeSize = sizeof(RshInitDAC);
		this->_type = rshInitDAC;
		this->id = obj.id;
	    this->voltage =  obj.voltage;
	}

RshInitDAC& RshInitDAC::operator=(RshInitDAC& obj)
	{
		this->id = obj.id;
		this->voltage = obj.voltage;
		return *this;
	}

bool RshInitDAC::operator==(RshInitDAC& obj)
	{
		return
			RSH_CMPDOUBLE(this->voltage, obj.voltage)	&&
			(this->id == obj.id);
	}

bool RshInitDAC::operator!=(RshInitDAC& obj)
	{
		return
			!RSH_CMPDOUBLE(this->voltage, obj.voltage)	||
			(this->id != obj.id);
	}

//------------------------------------------------------------------------------------------------
//RshInitPort
RshInitPort::RshInitPort()
    {
		this->_type = rshInitPort;
		this->_typeSize = sizeof(RshInitPort);
		operationType = Read;
		portAddress = 0;
		portValue = 0;
    }

RshInitPort::RshInitPort(RshInitPort const& obj)
	{
		this->_typeSize = sizeof(RshInitPort);
		this->_type = rshInitPort;
		this->operationType = obj.operationType;
	    this->portAddress =  obj.portAddress;
		this->portValue =  obj.portValue;
	}

RshInitPort& RshInitPort::operator=(RshInitPort& obj)
	{
		this->operationType = obj.operationType;
	    this->portAddress =  obj.portAddress;
		this->portValue =  obj.portValue;
		return *this;
	}

bool RshInitPort::operator==(RshInitPort& obj)
	{
		return
			(this->operationType == obj.operationType)	&&
			(this->portAddress == obj.portAddress)		&&
			(this->portValue == obj.portValue);
	}

bool RshInitPort::operator!=(RshInitPort& obj)
	{
		return
			(this->operationType != obj.operationType)	||
			(this->portAddress != obj.portAddress)		||
			(this->portValue != obj.portValue);
	}

//------------------------------------------------------------------------------------------------
//RshInitPacket class
RshInitPacket::RshInitPacket()
    {
		this->_type = rshInitPacket;
		this->_typeSize = sizeof(RshInitPacket);
		operationType = WriteAndRead;
    }

void RshInitPacket::Set(U8 b0,U8 b1,U8 b2,U8 b3)
    {
		packet[0] = b0;
		packet[1] = b1;
		packet[2] = b2;
		packet[3] = b3;
		packet.SetSize(4);
    }


U8& RshInitPacket::operator[] (const size_t index)
	{
		return packet[index];
	}

RshInitPacket::RshInitPacket(RshInitPacket const& obj)
{
	this->_typeSize = sizeof(RshInitPacket);
	this->_type = rshInitPacket;
	this->operationType = obj.operationType;
	this->packet.Copy(obj.packet);
}

RshInitPacket& RshInitPacket::operator=(RshInitPacket& obj)
	{
		this->operationType = obj.operationType;
		this->packet = obj.packet;
		return *this;
	}

bool RshInitPacket::operator==(RshInitPacket& obj)
	{
		return
			(this->operationType == obj.operationType)	&&
			(this->packet == obj.packet);
	}

bool RshInitPacket::operator!=(RshInitPacket& obj)
	{
		return
			(this->operationType != obj.operationType)	||
			(this->packet != obj.packet);
	}

//------------------------------------------------------------------------------------------------
//RshInitVoltmeter
RshInitVoltmeter::RshInitVoltmeter()
    {
		this->_typeSize = sizeof(RshInitVoltmeter);
		this->_type = rshInitVoltmeter;
		startType = 0;
		bufferSize = 0;
		filter = 0;
		control = 0;
    }

RshInitVoltmeter::RshInitVoltmeter(RshInitVoltmeter const& obj)
{
	this->_typeSize = sizeof(RshInitVoltmeter);
	this->_type = rshInitVoltmeter;
	this->control = obj.control;
	this->bufferSize = obj.bufferSize;
	this->filter = obj.filter;
	this->startType = obj.startType;
}

RshInitVoltmeter& RshInitVoltmeter::operator=(RshInitVoltmeter& obj)
{
	this->control = obj.control;
	this->bufferSize = obj.bufferSize;
	this->filter = obj.filter;
	this->startType = obj.startType;
	return *this;
}

bool RshInitVoltmeter::operator==(RshInitVoltmeter& obj)
	{
		return
			(this->startType == obj.startType)		&&
			(this->bufferSize == obj.bufferSize)	&&
			(this->filter == obj.filter)			&&
			(this->control == obj.control);
	}

bool RshInitVoltmeter::operator!=(RshInitVoltmeter& obj)
	{
		return
			(this->startType != obj.startType)		||
			(this->bufferSize != obj.bufferSize)	||
			(this->filter != obj.filter)			||
			(this->control != obj.control);
	}
//------------------------------------------------------------------------------------------------
//RshInitTimer
RshInitTimer::RshInitTimer()
{
	this->_type = rshInitTimer;
	this->_typeSize = sizeof(RshInitTimer);
	timer0Mode = RshInitTimer::InterruptOnTerminalCount;
	timer1Mode = RshInitTimer::InterruptOnTerminalCount;
	timer2Mode = RshInitTimer::InterruptOnTerminalCount;
	timer0Count = 0;
	timer1Count = 0;
	timer2Count = 0;
}

RshInitTimer::RshInitTimer(RshInitTimer const& obj)
{
	this->_typeSize = sizeof(RshInitTimer);
	this->_type = rshInitTimer;
	this->timer0Mode = obj.timer0Mode;
	this->timer1Mode = obj.timer1Mode;
	this->timer2Mode = obj.timer2Mode;
	this->timer0Count = obj.timer0Count;
	this->timer1Count = obj.timer1Count;
	this->timer2Count = obj.timer2Count;

}

RshInitTimer& RshInitTimer::operator=(RshInitTimer& obj)
{
	this->timer0Mode = obj.timer0Mode;
	this->timer1Mode = obj.timer1Mode;
	this->timer2Mode = obj.timer2Mode;
	this->timer0Count = obj.timer0Count;
	this->timer1Count = obj.timer1Count;
	this->timer2Count = obj.timer2Count;
	return *this;
}

bool RshInitTimer::operator==(RshInitTimer& obj)
{
	return
		(this->timer0Mode == obj.timer0Mode) &&
		(this->timer1Mode == obj.timer1Mode) &&
		(this->timer2Mode == obj.timer2Mode) &&
		(this->timer0Count == obj.timer0Count) &&
		(this->timer1Count == obj.timer1Count) &&
		(this->timer2Count == obj.timer2Count);
}

bool RshInitTimer::operator!=(RshInitTimer& obj)
{
	return
		(this->timer0Mode != obj.timer0Mode) ||
		(this->timer1Mode != obj.timer1Mode) ||
		(this->timer2Mode != obj.timer2Mode) ||
		(this->timer0Count != obj.timer0Count) ||
		(this->timer1Count != obj.timer1Count) ||
		(this->timer2Count != obj.timer2Count);
}
//------------------------------------------------------------------------------------------------
//RshPortInfo class
RshPortInfo::RshPortInfo() {
		this->_type = rshPortInfo;
		this->_typeSize = sizeof(RshPortInfo);

		address = 0;
		bitSize = 0;
		name = (wchar_t*)L"";
	}

RshPortInfo::RshPortInfo(U32 address, U8 bitsize, wchar_t* name) {
		this->_type = rshPortInfo;
		this->_typeSize = sizeof(RshPortInfo);

		this->address = address;
		this->bitSize = bitsize;
		this->name = name;
	}

bool RshPortInfo::operator==(RshPortInfo& obj)
	{
		return
			(this->address == obj.address) &&
			(this->bitSize == obj.bitSize) &&
			(this->name == obj.name);
	}

bool RshPortInfo::operator!=(RshPortInfo& obj)
	{
		return
			(this->address != obj.address)||
			(this->bitSize != obj.bitSize)||
			(this->name != obj.name);
	}

bool RshPortInfo::operator< (RshPortInfo& obj)
	{
		return this->address < obj.address;
	}

bool RshPortInfo::operator> (RshPortInfo& obj)
	{
		return this->address > obj.address;
	}

std::ostream& operator<< (std::ostream &out,  RshPortInfo& obj)
{
	 // write obj to stream
	out << "[Name="<<obj.name<<", address="<<obj.address<<", bitSize="<<obj.bitSize<<"]";
		return out;
}

//------------------------------------------------------------------------------------------------
//RshDeviceBaseInfo class
RshDeviceBaseInfo::RshDeviceBaseInfo(U16 vid, U16 pid, U16 rev, U16 chip, U16 slot, U16 base, U32 serialNumber) {
		this->vid = vid;
		this->pid = pid;
		this->rev = rev;
		this->chip = chip;
		this->slot = slot;
		this->base = base;
		this->serialNumber = serialNumber;

		this->_type = rshDeviceBaseInfo;
		this->_typeSize = sizeof (RshDeviceBaseInfo);
	}

RshDeviceBaseInfo::RshDeviceBaseInfo(RshDeviceBaseInfo const& pi) {
		this->vid = pi.vid;
		this->pid = pi.pid;
		this->rev = pi.rev;
		this->chip = pi.chip;
		this->slot = pi.slot;
		this->base = pi.base;
		this->_type = rshDeviceBaseInfo;
		this->_typeSize = sizeof (RshDeviceBaseInfo);
	}

RshDeviceBaseInfo::RshDeviceBaseInfo() {
		chip = vid = rev = pid = slot = base = 0;
		serialNumber = 0;
		this->_type = rshDeviceBaseInfo;
		this->_typeSize = sizeof (RshDeviceBaseInfo);
	}

bool RshDeviceBaseInfo::operator==(RshDeviceBaseInfo& obj)
	{
		return
			(this->vid == obj.vid) &&
			(this->pid == obj.pid) &&
			(this->rev == obj.rev) &&
			(this->chip == obj.chip) &&
			(this->slot == obj.slot) &&
			(this->base == obj.base);
	}

bool RshDeviceBaseInfo::operator!=(RshDeviceBaseInfo& obj)
	{
		return
			(this->vid != obj.vid) ||
			(this->pid != obj.pid) ||
			(this->rev != obj.rev) ||
			(this->chip != obj.chip) ||
			(this->slot != obj.slot) ||
			(this->base != obj.base);
	}


bool RshDeviceBaseInfo::operator<(const RshDeviceBaseInfo& obj) {
		return this->base < obj.base;
	}

bool RshDeviceBaseInfo::operator>(const RshDeviceBaseInfo& obj) {
		return this->base > obj.base;
	}

std::ostream& operator<< (std::ostream &out,  RshDeviceBaseInfo& obj)
{
	 // write obj to stream
	out << "[Base="<<obj.base<<", chip="<<obj.chip<<", slot="<<obj.slot<<
		", vid="<<obj.vid<<", pid="<<obj.pid<<
		", rev="<<obj.rev<<", serialNumber="<<obj.serialNumber<<"]";
		return out;
}
//------------------------------------------------------------------------------------------------
//RshDeviceFullInfo
RshDeviceFullInfo::RshDeviceFullInfo() {
		itype = RSH_ITYPE_UNDEFINED;
		libraryName = libraryInterfaceName = (S8*)"";
		deviceOriginalName_EN = deviceOriginalName_RU = (U16*)L"";

		this->_type = rshDeviceFullInfo;
		this->_typeSize = sizeof (RshDeviceFullInfo);
	}

RshDeviceFullInfo::RshDeviceFullInfo(RshDeviceFullInfo const& obj)
	{
		this->itype = obj.itype;
		this->libraryName = obj.libraryName; //!< in English to find a record in registry
		this->libraryInterfaceName = obj.libraryInterfaceName; //!< corresponding library interface name
		this->deviceOriginalName_RU = obj.deviceOriginalName_RU; //!< in Russian for user
		this->deviceOriginalName_EN = obj.deviceOriginalName_EN; //!< in English for user

		this->vid = obj.vid;
		this->pid = obj.pid;
		this->rev = obj.rev;
		this->chip = obj.chip;
		this->slot = obj.slot;
		this->base = obj.base;
		this->serialNumber = obj.serialNumber;

		this->_type = rshDeviceFullInfo;
		this->_typeSize = sizeof (RshDeviceFullInfo);
	}
//------------------------------------------------------------------------------------------------
//RshDeviceActiveList
RshDeviceActiveList::RshDeviceActiveList() {
		this->_type = rshDeviceActiveList;
		this->_typeSize = sizeof (RshDeviceActiveList);
	}

//------------------------------------------------------------------------------------------------
//RshBoardPortInfo
RshBoardPortInfo::RshBoardPortInfo() {
		this->_type = rshBoardPortInfo;
		this->_typeSize = sizeof (RshBoardPortInfo);
	}

//------------------------------------------------------------------------------------------------
//RshDeviceKey
RshDeviceKey::RshDeviceKey()
    {
        this->storedTypeId = rshU32;
        this->_type = rshDeviceKey;
        this->_typeSize = sizeof (RshDeviceKey);

        this->value_U32  = 1;
        this->value_S8P  = 0;
        this->value_U16P = 0;
    }

RshDeviceKey::RshDeviceKey(U32 value)
	{
		this->storedTypeId = rshU32;
		this->_type = rshDeviceKey;
		this->_typeSize = sizeof (RshDeviceKey);

		this->value_U32 = value;
		this->value_S8P  = 0;
		this->value_U16P = 0;
	}
RshDeviceKey::RshDeviceKey(S8* value)
	{
		this->storedTypeId = rshS8P;
		this->_type = rshDeviceKey;
		this->_typeSize = sizeof (RshDeviceKey);

		this->value_U32 = 0;
		this->value_S8P  = value;
		this->value_U16P = 0;
	}
RshDeviceKey::RshDeviceKey(U16* value)
	{
		this->storedTypeId = rshU16P;
		this->_type = rshDeviceKey;
		this->_typeSize = sizeof (RshDeviceKey);

		this->value_U32 = 0;
		this->value_S8P  = 0;
		this->value_U16P = value;
	}


//------------------------------------------------------------------------------------------------
//Template instantiation

//RshType
template struct RshType< U8    , rshU8  >;
template struct RshType< S8    , rshS8  >;
template struct RshType< U16   , rshU16 >;
template struct RshType< S16   , rshS16  >;
template struct RshType< U32   , rshU32  >;
template struct RshType< S32   , rshS32  >;
template struct RshType< U64   , rshU64  >;
template struct RshType< S64   , rshS64  >;
template struct RshType< float , rshFloat   >;
template struct RshType< double, rshDouble  >;
template struct RshType< bool,   rshBool >;

template struct RshType< void*   , rshVoidP >;
template struct RshType< U8*     , rshU8P  >;
template struct RshType< S8*     , rshS8P  >;
template struct RshType< U16*    , rshU16P >;
template struct RshType< S16*    , rshS16P  >;
template struct RshType< U32*    , rshU32P  >;
template struct RshType< S32*    , rshS32P  >;
template struct RshType< U64*    , rshU64P  >;
template struct RshType< S64*    , rshS64P  >;
template struct RshType< float*  , rshFloatP  >;
template struct RshType< double* , rshDoubleP >;
template struct RshType< bool*   , rshBoolP >;

template struct RshType< U8**     , rshU8PP  >;
template struct RshType< S8**     , rshS8PP  >;
template struct RshType< U16**    , rshU16PP >;
template struct RshType< S16**    , rshS16PP  >;
template struct RshType< U32**    , rshU32PP  >;
template struct RshType< S32**    , rshS32PP  >;
template struct RshType< U64**    , rshU64PP  >;
template struct RshType< S64**    , rshS64PP  >;
template struct RshType< float**  , rshFloatPP  >;
template struct RshType< double** , rshDoublePP >;
template struct RshType< bool**   , rshBoolPP >;

//RshBufferType
template class RshBufferType< U8    , rshBufferTypeU8 >;
template class RshBufferType< S8    , rshBufferTypeS8>;
template class RshBufferType< U16   , rshBufferTypeU16>;
template class RshBufferType< S16   , rshBufferTypeS16>;
template class RshBufferType< U32   , rshBufferTypeU32>;
template class RshBufferType< S32   , rshBufferTypeS32>;
template class RshBufferType< U64   , rshBufferTypeU64>;
template class RshBufferType< S64   , rshBufferTypeS64>;
template class RshBufferType< float , rshBufferTypeFloat>;
template class RshBufferType< double, rshBufferTypeDouble>;
template class RshBufferType< bool  , rshBufferTypeBool>;
template class RshBufferType< U8*   , rshBufferTypeU8P>;
template class RshBufferType< RshChannel, rshBufferTypeChannel >;
template class RshBufferType< RshPortInfo, rshBufferTypePortInfo >;
template class RshBufferType< RshDeviceBaseInfo, rshBufferTypeDeviceBaseInfo >;
template class RshBufferType< RshDeviceFullInfo, rshBufferTypeDeviceFullInfo >;


//RshDBufferType
template class  RshDBufferType< U8    , rshDBufferTypeU8>;
template class  RshDBufferType< S8    , rshDBufferTypeS8>;
template class  RshDBufferType< U16   , rshDBufferTypeU16>;
template class  RshDBufferType< S16   , rshDBufferTypeS16>;
template class  RshDBufferType< U32   , rshDBufferTypeU32>;
template class  RshDBufferType< S32   , rshDBufferTypeS32>;
template class  RshDBufferType< U64   , rshDBufferTypeU64>;
template class  RshDBufferType< S64   , rshDBufferTypeS64>;
template class  RshDBufferType< float , rshDBufferTypeFloat>;
template class  RshDBufferType< double, rshDBufferTypeDouble>;
template class  RshDBufferType< bool  , rshDBufferTypeBool>;


//------------------------------------------------------------------------------------------------
//Garbage (need to be sorted/refactored)
/*!
	*
	* \~english
	* \brief
	* Fill buffer with random numbers.
	*
	* \param[in] seed
	* Seed for std::srand() function.
	* If no seed specified (or zero seed passed)
	* random seed generated by ::RshMix() function
	* will be used.
	*
	* Buffer will be filled with random numbers,
	* generated by std::rand() function.
	* RshBufferType::Size() field will be equal
	* to RshBufferType::PSize() after this method call.
	*
	* \warning
	* At this moment, default type converter
	* (<static_cast>) is used to convert generated
	* int values to buffer type.
	*
	* \see
	* ::RshMix()
	*
	* \~russian
	* \brief
	* Заполнение буфера случайными числами.
	*
	* \param[in] seed
	* Зерно для функции  std::srand().
	* По умолчанию (seed = 0) будет использовано
	* значение, полученное с помощью функции ::RshMix().
	*
	* Буфер будет заполнен случайными числами, сгенеренными
	* функцией std::rand(). После выполнения этого метода,
	* поле RshBufferType::Size() станет равным RshBufferType::PSize().
	*
	* \warning
	* В текущей реализации используется преобразование типов
	* по умолчанию (<static_cast>) для приведения
	* сгенеренных данных от типа int к типу буфера.\n
	* Поэтому, использование данного метода для буферов с
	* нечисловыми типами данных может привести к непредвиденным
	* последствиям.
	*
	* \todo
	* Возможно, требуется переработать (непонятно поведение для типов данных, отличных от стандартных)
	*
	*/
	/*
	void FillWithRandomNumbers(unsigned int seed=0)
	{
		if (seed ==0)
			std::srand((unsigned int) RshMix());
		else
			std::srand(seed);

		for (size_t i = 0; i < psize; i++)
			this->ptr[i] = std::rand();

		this->size = psize;
	}*/
	//\\ \todo fix it somehow, its wrong use concrete type in template
	/*
	U32 CopyString(const char* str)
	{
		if( sizeof(T) == sizeof(const char) )
		{
			Copy((U8*)str, strlen(str));
			return RSH_API_SUCCESS;
		}
		else
			return RSH_API_BUFFER_WRONGDATATYPE;
	}
	*/

//include all other cpp files
#if defined(__RSH_DLL_CLIENT_H__)
    #include "RshDllClient.cpp"
#endif

#include "RshError.cpp"
#include "RshEventManager.cpp"
#include "RshFunctions.cpp"

#if defined(__IRSH_CALIBRATOR_H__)
    #include "IRshCalibrator.cpp"
#endif
#if defined(__IRSH_DEBUG_H__)
    #include "IRshDebug.cpp"
#endif


#endif //__RSH_API_CPP__
