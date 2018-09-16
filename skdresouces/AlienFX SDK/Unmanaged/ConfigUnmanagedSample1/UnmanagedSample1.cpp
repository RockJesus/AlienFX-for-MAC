/******************************************************************
// UnmanagedSample1.cpp : This sample has several sections. 
// Even when those sections are in the same sample for 
// explanation and execution purposes, each one of these 
// sections should be used independently in real live applications
******************************************************************/
#include "stdafx.h"
#include "LFXConfigurator.h"
#include <iostream>
#include <iomanip>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	HINSTANCE hLibrary = LoadLibrary(_T(LFX_CONFIGURATOR_DLL_NAME));
	if (hLibrary)
	{
		LFXCONFIGURATORINITIALIZE initFunction = (LFXCONFIGURATORINITIALIZE)GetProcAddress(hLibrary, LFX_CONFIGURATOR_DLL_INITIALIZE);
		LFXCONFIGURATORRELEASE releaseFunction = (LFXCONFIGURATORRELEASE)GetProcAddress(hLibrary, LFX_CONFIGURATOR_DLL_RELEASE);
		LFXCONFIGURATORREGISTERCONFIGURATIONFILE registerAppFXFunction = (LFXCONFIGURATORREGISTERCONFIGURATIONFILE)GetProcAddress(hLibrary, LFX_CONFIGURATOR_DLL_REGISTERCONFIGURATIONFILE);
		LFXCONFIGURATORUNREGISTERCONFIGURATIONFILE unregisterAppFXFunction = (LFXCONFIGURATORUNREGISTERCONFIGURATIONFILE)GetProcAddress(hLibrary, LFX_CONFIGURATOR_DLL_UNREGISTERCONFIGURATIONFILE);
		LFXCONFIGURATORGETUSERCONFIGURATIONFILEPATH getConfigFileFunction = (LFXCONFIGURATORGETUSERCONFIGURATIONFILEPATH)GetProcAddress(hLibrary, LFX_CONFIGURATOR_DLL_GETUSERCONFIGURATIONFILEPATH);
		LFXCONFIGURATORGETCONFIGURATIONEVENT getConfigEventFunction = (LFXCONFIGURATORGETCONFIGURATIONEVENT)GetProcAddress(hLibrary, LFX_CONFIGURATOR_DLL_GETCONFIGURATIONEVENT);

		// Initialization function needs to be called always
		LFX_RESULT result = initFunction();
		if (result == LFX_SUCCESS)
        {
			/***************************************************************
			* Registering game - SECTION 1
			* These steps are needed only if you want to register the game
			* It is recommended to be used at installation time
			****************************************************************/
			char appName[] = "AIF";
			char appConfigFilePath[] = "AIF-TheGame.afx";
			LFX_APPTYPE appType = LFX_GAME;
			result = registerAppFXFunction(appName, appConfigFilePath, appType);
			if (result != LFX_SUCCESS)
				std::cout << "There was an error registering the application: " << appName << std::endl;
			/***************************************************************
			* END - Registering game - SECTION 1
			****************************************************************/

			/***************************************************************
			* Retrieving User's configuration file - SECTION 2
			* Theses steps are needed only if you want to know what is the 
			* user's config file in order to read it by yourself later
			****************************************************************/
			unsigned int bufferSize = 255;
			char* configFile = new char[bufferSize];
			result = getConfigFileFunction(appName, configFile, bufferSize);
			if (result != LFX_SUCCESS)
				std::cout << "There was an error getting the user's config file for: " << appName << std::endl;
			else
				std::cout << "AppFX: " << appName << "\t User's config file: " << configFile << std::endl;
			delete []configFile;
			/***************************************************************
			* END - Retrieving User's configuration file - SECTION 2
			****************************************************************/

			/***************************************************************
			* Retrieving Event configuration by ID - SECTION 3
			* Theses steps are needed if you want to read events by its 
			* event id (event id = 1 in this case)
			****************************************************************/
			unsigned int* colors = NULL;
			unsigned int colorCount = 0;
			
			result = getConfigEventFunction(appName, 1, colors, colorCount);
			if (result != LFX_SUCCESS)
				cout << "There was an error getting the events for the application: " << appName << std::endl;
			else
				{
					for(int i = 0; i < colorCount; i++)
						std::cout << "Color: " << hex << setw(8) << setfill('0') << colors[i] << std::endl;

					delete [] colors;
					colors = NULL;
				}
			/***************************************************************
			* END - Retrieving Event configuration by ID - SECTION 3
			****************************************************************/

			/***************************************************************
			* Unregistering game - SECTION 4
			* These steps are needed only if you want to unregister the 
			* game. It is recommended to be used at uninstallation time
			****************************************************************/
			result = unregisterAppFXFunction(appName);
			if (result != LFX_SUCCESS)
				std::cout << "There was an error unregistering the application: " << appName << std::endl;
			/***************************************************************
			* END - Unregistering game - SECTION 4
			****************************************************************/

			// Release function needs to be called always
			result = releaseFunction();
			if (result != LFX_SUCCESS)
				std::cout << "There was an error releasing the LightFXConfigurator.dll" << std::endl;
		}
		else
			std::cout << "There was an error initializing the LightFXConfigurator.dll" << std::endl;

		FreeLibrary(hLibrary);
	}
	else
		std::cout << "Failed to load LightFXConfigurator.dll" << std::endl;
	return 0;
}
