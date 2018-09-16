#include "stdafx.h"
#include "LFX2.h"


int _tmain(int argc, _TCHAR* argv[])
{
	LFX2INITIALIZE initFunction;
	LFX2RELEASE releaseFunction;
	LFX2GETNUMDEVICES getNumDevicesFunction;
	LFX2GETDEVDESC getDeviceDescriptionFunction;
	LFX2GETNUMLIGHTS getNumLightsFunction;
	LFX2GETLIGHTDESC getLightDescriptionFunction;


	 HINSTANCE hLibrary = LoadLibrary(_T(LFX_DLL_NAME));
	 if (hLibrary)
	 {
		initFunction = (LFX2INITIALIZE)GetProcAddress(hLibrary, LFX_DLL_INITIALIZE);
		releaseFunction = (LFX2RELEASE)GetProcAddress(hLibrary, LFX_DLL_RELEASE);
		getNumDevicesFunction = (LFX2GETNUMDEVICES)GetProcAddress(hLibrary, LFX_DLL_GETNUMDEVICES);
		getDeviceDescriptionFunction = (LFX2GETDEVDESC)GetProcAddress(hLibrary, LFX_DLL_GETDEVDESC);
		getNumLightsFunction = (LFX2GETNUMLIGHTS)GetProcAddress(hLibrary, LFX_DLL_GETNUMLIGHTS);
		getLightDescriptionFunction = (LFX2GETLIGHTDESC)GetProcAddress(hLibrary, LFX_DLL_GETLIGHTDESC);

		LFX_RESULT result = initFunction();
		if (result == LFX_SUCCESS)
        {
			unsigned int numDevices = 0;
			result = getNumDevicesFunction(&numDevices);
			std::cout << "Devices: " << numDevices << std::endl;

			for(unsigned int devIndex = 0; devIndex < numDevices; devIndex++)
			{
				unsigned int numLights = 0;
				unsigned char devType = 0;
				unsigned char* devTypePtr = &devType;

				unsigned int descSize = 255;
				char* description = new char[descSize];
				result = getDeviceDescriptionFunction(devIndex, description, descSize, &devType);
				std::cout << "Description: " << description << std::endl;
				delete []description;

				description = new char[descSize];	
				result = getNumLightsFunction(devIndex, &numLights);
				for (unsigned int lightIndex = 0; lightIndex < numLights; lightIndex++)
				{
					result = getLightDescriptionFunction(devIndex, lightIndex, description, descSize);
					if (result != LFX_SUCCESS)
						continue;

					std::cout << "\tLight: " << lightIndex << "\tDescription: " << description << std::endl;
				}
				
				delete []description;
			}

			std::cout << "Done.\r\n\r\nPress any key to finish ..." << std::endl;
			_gettch();
			result = releaseFunction();
		}
		else
		{
			switch (result)
            {
				case LFX_ERROR_NODEVS:
					std::cout << "There is not AlienFX device available." << std::endl;
					break;
                default:
					std::cout << "There was an error initializing the AlienFX device." << std::endl;
					break;
                }
		}

		FreeLibrary(hLibrary);
	 }
	 else
		 std::cout << "Failed to load LightFX.dll." << std::endl;

	 return 0;
}

