/*!
 * \copyright 2013 JSC "Rudnev-Shilyaev"
 *
 * \file RshDllClient.cpp
 * \date 29.11.2013
 *
 * \~english
 * \brief
 * Dll client class
 *
 * This class is used to load dlls and create interface instances.
 *
 * \~russian
 * \brief
 * Класс для работы с dll
 *
 * Содержит методы для загрузки библиотек и инстанцирования классов из этих библиотек.
 *
 */
#ifndef __RSHDLLCLIENT_CPP__
#define __RSHDLLCLIENT_CPP__

#include    "RshDllClient.h"
//------------------------------------------------------------------------------------------------
//RshDllInterfaceKey
RshDllInterfaceKey::RshDllInterfaceKey(const char* libraryName, void** Interface, void** Factory, void* Parameter)
	{
		this->_Name = libraryName;
		this->_Interface = Interface;
		this->_Factory = Factory;
		this->_Parameter = Parameter;
	}
RshDllInterfaceKey::~RshDllInterfaceKey()
{

}

#if defined(RSH_MSWINDOWS)
//------------------------------------------------------------------------------------------------
//RshDllClient

void* RshDllClient::LoadByPath(TCHAR const* szPath, char const* szExportName)
{
	if(szPath == 0 || szExportName == 0)
		return 0;

	HMODULE m_hLib;

	m_hLib = HMODULE(::LoadLibrary(szPath));

	if(m_hLib == 0)
		return 0;
	return (void*)::GetProcAddress(m_hLib, szExportName);
}
	
U32 RshDllClient::LoadRegistered(std::string szCompanyName, std::string szProductName,
						std::string szVersionName, std::string szModuleName,
						std::string szExportName)
	{

        WCHAR szPath[256];

		std::string registryPath("SOFTWARE");
		registryPath += "\\" + szCompanyName;
		registryPath += "\\" + szProductName;
		registryPath += "\\" + szVersionName;
		registryPath += "\\" + szModuleName;


		std::wstring wRegistryPath(registryPath.begin(),registryPath.end());

		HKEY hKeyModule;
		LONG r = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, wRegistryPath.c_str(), 0, KEY_READ, &hKeyModule);
		if(r != ERROR_SUCCESS)
			return RSH_API_REGISTRY_KEYCANTOPEN | r;

		unsigned long dwType, dwSize = sizeof(szPath)/sizeof(TCHAR);
        r = ::RegQueryValueEx(hKeyModule, L"path", 0, &dwType,
			(unsigned char*)szPath, &dwSize);

		if(r != ERROR_SUCCESS || dwType != REG_SZ)
		{
			::RegCloseKey(hKeyModule);
			return RSH_API_REGISTRY_PATHRECNOTFOUND | r;
		}


		::RegCloseKey(hKeyModule);

		HMODULE m_hLib;

		m_hLib = HMODULE(::LoadLibrary(szPath));

		if(m_hLib == 0)
			return RSH_API_DLL_WASNOTLOADED | GetLastError();

		// create a new inctance of IFDIC
		IFDIC* fdic = new IFDIC;
		fdic->factory = (IRshFactory*)::GetProcAddress(m_hLib, szExportName.c_str()); // get StaticFactory

		if( fdic->factory == 0 )
		{
			::FreeLibrary(m_hLib);
			return RSH_API_DLL_NOFACTORY | GetLastError();
		}

		fdic->hLib = m_hLib;
		fdic->libraryName = szModuleName;

		m_libraries.push_back(fdic);

		return RSH_API_SUCCESS;
	}
	

RshDllClient::RshDllClient()
{

}

RshDllClient::~RshDllClient()
{
	Free();
}	

U32 RshDllClient::GetRegisteredList(std::vector<std::string>* list, RshDllClient::LibraryType libType)
{
		if(list == NULL)
			return RSH_API_PARAMETER_ZEROADDRESS;
		list->clear();

		//format key path
		std::string registryPath("SOFTWARE");
		registryPath += "\\" + std::string(CompanyName);

		switch(libType)
		{
			case libraryDriver:
				default:
				registryPath += "\\" + std::string(ProductNameDRV);
				registryPath += "\\" + std::string(VersionNameDRV);
				break;
			case libraryBase:
				registryPath += "\\" + std::string(ProductName);
				registryPath += "\\" + std::string(VersionName);
				break;
			case libraryDPA:
				registryPath += "\\" + std::string(ProductName);
				registryPath += "\\" + std::string(VersionName);
				registryPath += "\\DPA" ;
				break;
		}
		std::wstring wRegistryPath(registryPath.begin(),registryPath.end());

		//open regKey
		HKEY hKeyModule;
		LONG r = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, wRegistryPath.c_str(), 0, KEY_READ, &hKeyModule);
		if(r != ERROR_SUCCESS)
			return RSH_API_REGISTRY_KEYCANTOPEN | r;

		//enumerate all subkeys
		r = ERROR_SUCCESS;
		U32 index = 0;
		WCHAR subKeyName[1024];
		DWORD subKeyNameLength = 1024;
		while (r == ERROR_SUCCESS)
		{
			subKeyNameLength = 1024;
			r = ::RegEnumKeyEx(hKeyModule, index, subKeyName, &subKeyNameLength, 0, 0, 0, 0);			
			if(r != ERROR_SUCCESS) 
				break;
			std::wstring wskName(subKeyName);
			list->push_back(std::string(wskName.begin(), wskName.end()));	
			index++;
		}
		::RegCloseKey(hKeyModule);
		return RSH_API_SUCCESS;
}


U32 RshDllClient::Free()
{
	std::vector<IFDIC*>::iterator itf = m_libraries.begin();

	for (; itf != m_libraries.end(); itf++)
		::FreeLibrary(((IFDIC*) * itf)->hLib);

	m_libraries.clear();

	return RSH_API_SUCCESS;
}

U32 RshDllClient::GetLibraryInterface(IN OUT RshDllInterfaceKey& key, IN std::string interfaceName)
{

		U32 st;
		bool deviceRecordFound = false;

		
		if(interfaceName=="")		
		{
			interfaceName = "I" + std::string(key._Name);
		}

		std::vector<IFDIC*>::iterator it = m_libraries.begin();
		for (; it != m_libraries.end(); it++) {
			if (((IFDIC*) * it)->libraryName == key._Name) {
				deviceRecordFound = true;
				break;
			}
		}

		if (deviceRecordFound)
		{
			if (key._Factory)
				*key._Factory = ((IFDIC*) * it)->factory;

			return ((IFDIC*) * it)->factory->CreateInstance(interfaceName.c_str(), key._Interface, key._Parameter );// create device class instance
		}
		else { // if we haven't found the entry in m_libraries then try to load library ang get an interface from it

			st = LoadRegistered(CompanyName, ProductName, VersionName, key._Name);
			if( st != RSH_API_SUCCESS ) return st;

			// so, LoadRegistered finished successfully => we can find all what we need in m_libraries, let's make GetLibraryInterface call again
			return GetLibraryInterface( key );
		}

		return RSH_API_SUCCESS;
}
	

U32 RshDllClient::GetDPALibraryInterface(IN OUT RshDllInterfaceKey& key)
{

		U32 st;
		bool deviceRecordFound = false;

		std::string iface("IDPA");


		std::vector<IFDIC*>::iterator it = m_libraries.begin();
		for (; it != m_libraries.end(); it++) {
			if (((IFDIC*) * it)->libraryName == "DPA\\"+std::string(key._Name)) {
				deviceRecordFound = true;
				break;
			}
		}

		if (deviceRecordFound)
		{
			if (key._Factory)
				*key._Factory = ((IFDIC*) * it)->factory;

			return ((IFDIC*) * it)->factory->CreateInstance(iface.c_str(), key._Interface, key._Parameter );// create device class instance
		}
		else { // if we haven't found the entry in m_libraries then try to load library ang get an interface from it

			st = LoadRegistered(CompanyName, ProductName, VersionName, "DPA\\"+std::string(key._Name));
			if( st != RSH_API_SUCCESS ) return st;

			// so, LoadRegistered finished successfully => we can find all what we need in m_libraries, let's make GetLibraryInterface call again
			return GetDPALibraryInterface( key );
		}

		return RSH_API_SUCCESS;
}
	

U32 RshDllClient::GetDeviceInterface(IN OUT RshDllInterfaceKey& key)
{
		U32 st;
		bool deviceRecordFound = false;

		std::vector<IFDIC*>::iterator it = m_libraries.begin();
		for (; it != m_libraries.end(); it++) {
			if (((IFDIC*) * it)->libraryName == key._Name) {
				deviceRecordFound = true;
				break;
			}
		}

		if (deviceRecordFound)
		{
			if (key._Factory)
				*key._Factory = ((IFDIC*) * it)->factory;

			return ((IFDIC*) * it)->factory->CreateInstance("IRshDevice", key._Interface, key._Parameter );// create device class instance

		} else { // if we haven't found the entry in m_libraries then try to load library ang get an interface from it

			st = LoadRegistered(CompanyNameDRV, ProductNameDRV, VersionNameDRV, key._Name);
			if( st != RSH_API_SUCCESS ) return st;

			// so, LoadRegistered finished successfully => we can find all what we need in m_libraries, let's make GetLibraryInterface call again
			return GetDeviceInterface( key );
		}

		return RSH_API_SUCCESS;
	}

#elif defined(RSH_LINUX)

U32 RshDllClient::LoadDriver(std::string libraryName, std::string fullLibraryPath, IFDIC** f) // loads SO object
{
		void* library = NULL;
		void* (*CreateFactory)(const char*, const char*);


		library = dlopen(fullLibraryPath.c_str(), RTLD_NOW | RTLD_LOCAL);

		if (!library) {
			perror(dlerror());
			return RSH_API_DLL_WASNOTLOADED;
		}

		CreateFactory = (void* (*)(const char*, const char*))dlsym(library, "CreateFactory"); // load CreateFactory function

		if (!CreateFactory) {
			return RSH_API_DLL_NOFACTORY;
		}


		IFDIC* fdic = new IFDIC;
                fdic->hLib = library;
		fdic->libraryName = libraryName;
		fdic->factory = (IRshFactory*) (*CreateFactory)(path_RSHBoards.c_str(), path_RSHLibraries.c_str());

		*f = fdic;

		m_libraries.push_back(fdic);

		return RSH_API_SUCCESS;
	}

U32 RshDllClient::GetInterface(std::string libraryName, std::string libraryInterfaceName, std::string libraryPath,
		void** interface, void** factory, void* parameter) {
			U32 st;
			bool deviceRecordFound = false;

			std::vector<IFDIC*>::iterator it = m_libraries.begin();
			for (; it != m_libraries.end(); it++) {
				if (((IFDIC*) * it)->libraryName == libraryName) {
					deviceRecordFound = true;
					break;
				}
			}
                        std::string lin = libraryInterfaceName;
			if (deviceRecordFound) {

                        	if (factory)
                                    *factory = ((IFDIC*) * it)->factory;


                                return ((IFDIC*) * it)->factory->CreateInstance(lin.c_str(), interface, parameter );// create device class instance
		        } else { // if we haven't found the entry in m_libraries then try to load library ang get an interface from it

			        std::string fullPath;
                                fullPath = libraryPath;
                                fullPath += "lib";
                                fullPath += libraryName;
                                fullPath += ".so";

                                IFDIC* f;
				st = LoadDriver(libraryName, fullPath, &f);
                                if( st != RSH_API_SUCCESS ) return st;

                                if (factory)
                                    *factory = f->factory;

                                return f->factory->CreateInstance(lin.c_str(), interface, parameter );// create device class instance
		        }


			}
RshDllClient::RshDllClient(const char* path_Boards, const char* path_Libs)
{
		if (path_Boards)
			path_RSHBoards = std::string(path_Boards);
		else
			path_RSHBoards =  std::string(RSH_DLL_BOARDS_DIRECTORY);

		if (path_Libs)
			path_RSHLibraries = std::string(path_Libs);
		else
			path_RSHLibraries = std::string(RSH_DLL_LIBRARIES_DIRECTORY);
}

RshDllClient::RshDllClient()
{
	path_RSHLibraries =  std::string(RSH_DLL_LIBRARIES_DIRECTORY);
	path_RSHBoards =  std::string(RSH_DLL_BOARDS_DIRECTORY);
}

U32 RshDllClient::GetLibraryInterface(IN OUT RshDllInterfaceKey& key)
{
	std::string libraryInterfaceName = "I";
	libraryInterfaceName += key._Name;

	return GetInterface( key._Name, libraryInterfaceName, path_RSHLibraries,
				key._Interface, key._Factory, key._Parameter);
}

	// получить интерфейс для работы с платой (все платы поддерживают интерфейс IADCDevice)

U32 RshDllClient::GetDeviceInterface(IN OUT RshDllInterfaceKey& key)
{
	return GetInterface(key._Name, "IRshDevice", path_RSHBoards,
			key._Interface, key._Factory, key._Parameter);
}

U32 RshDllClient::Free() // free allocated memory for drivers and objects
{
		std::vector<IFDIC*>::iterator itf = m_libraries.begin();

		for (; itf != m_libraries.end(); itf++)
		{
                    ((IFDIC*) * itf)->factory->Free();
                    dlclose(((IFDIC*) * itf)->hLib);
                }

		m_libraries.clear();

		return RSH_API_SUCCESS;
}
#endif // LINUX

#endif //__RSHDLLCLIENT_CPP__
