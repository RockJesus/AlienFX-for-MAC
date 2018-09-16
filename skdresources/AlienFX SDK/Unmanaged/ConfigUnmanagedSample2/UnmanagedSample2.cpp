/******************************************************************
// UnmanagedSample2.cpp : This sample has several sections. 
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
		LFXCONFIGURATORGETCONFIGURATIONEVENTFROMPOSITION getConfigEventFromPositionFunction = (LFXCONFIGURATORGETCONFIGURATIONEVENTFROMPOSITION)GetProcAddress(hLibrary, LFX_CONFIGURATOR_DLL_GETCONFIGURATIONEVENTFROMPOSITION);

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
			* Iterating through Event's configuration - SECTION 2
			* Theses steps are needed if you want to iterate reading event's
			* configurations, from first event until end of the list
			****************************************************************/
			unsigned int* colors = NULL;
			unsigned int colorCount = 0;
			unsigned int bufferSize = 255;
			char* eventName = new char[bufferSize];
			int eventID = 0;
			
			result = getConfigEventFromPositionFunction(appName, LFX_FIRSTEVENT, eventID, eventName, bufferSize, colors, colorCount);
			if (result != LFX_SUCCESS)
				cout << "There was an error getting the events for the application: " << appName << std::endl;
			else
				{
					while (result == LFX_SUCCESS)
					{
						cout << "App: "<< appName << "\tEvent ID: " << eventID << "\tEvent Name: " << eventName << std::endl;
						for(int i = 0; i < colorCount; i++)
							std::cout << "Color: " << hex << setw(8) << setfill('0') << colors[i] << std::endl;
					
						delete [] colors;
						colors = NULL;

						result = getConfigEventFromPositionFunction(appName, LFX_NEXTEVENT, eventID, eventName, bufferSize, colors, colorCount);
					}

					delete [] colors;
					colors = NULL;
				}

			delete [] eventName;
			/***************************************************************
			* END - Iterating through Event's configuration - SECTION 2
			****************************************************************/

			/***************************************************************
			* Unregistering game - SECTION 3
			* These steps are needed only if you want to unregister the 
			* game. It is recommended to be used at uninstallation time
			****************************************************************/
			result = unregisterAppFXFunction(appName);
			if (result != LFX_SUCCESS)
				std::cout << "There was an error unregistering the application: " << appName << std::endl;
			/***************************************************************
			* END - Unregistering game - SECTION 3
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
