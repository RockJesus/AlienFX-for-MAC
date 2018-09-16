#include "stdafx.h"
#include "LFX2.h"


int _tmain(int argc, _TCHAR* argv[])
{
	LFX2INITIALIZE initFunction;
	LFX2RELEASE releaseFunction;
	LFX2RESET resetFunction;
	LFX2UPDATE updateFunction;
	LFX2GETNUMDEVICES getNumDevicesFunction;
	LFX2GETDEVDESC getDeviceDescriptionFunction;
	LFX2GETNUMLIGHTS getNumLightsFunction;
	LFX2SETLIGHTCOL setLightColorFunction;
	LFX2GETLIGHTCOL getLightColorFunction;
	LFX2GETLIGHTDESC getLightDescriptionFunction;

	 HINSTANCE hLibrary = LoadLibrary(_T(LFX_DLL_NAME));
	 if (hLibrary)
	 {
		initFunction = (LFX2INITIALIZE)GetProcAddress(hLibrary, LFX_DLL_INITIALIZE);
		releaseFunction = (LFX2RELEASE)GetProcAddress(hLibrary, LFX_DLL_RELEASE);
		resetFunction = (LFX2RESET)GetProcAddress(hLibrary, LFX_DLL_RESET);
		updateFunction = (LFX2UPDATE)GetProcAddress(hLibrary, LFX_DLL_UPDATE);
		getNumDevicesFunction = (LFX2GETNUMDEVICES)GetProcAddress(hLibrary, LFX_DLL_GETNUMDEVICES);
		getDeviceDescriptionFunction = (LFX2GETDEVDESC)GetProcAddress(hLibrary, LFX_DLL_GETDEVDESC);
		getNumLightsFunction = (LFX2GETNUMLIGHTS)GetProcAddress(hLibrary, LFX_DLL_GETNUMLIGHTS);
		setLightColorFunction = (LFX2SETLIGHTCOL)GetProcAddress(hLibrary, LFX_DLL_SETLIGHTCOL);
		getLightColorFunction = (LFX2GETLIGHTCOL)GetProcAddress(hLibrary, LFX_DLL_GETLIGHTCOL);
		getLightDescriptionFunction = (LFX2GETLIGHTDESC)GetProcAddress(hLibrary, LFX_DLL_GETLIGHTDESC);

		LFX_RESULT result = initFunction();
		if (result == LFX_SUCCESS)
        {
			result = resetFunction();

			unsigned int numDevs = 0;
			result = getNumDevicesFunction(&numDevs);

			for(unsigned int devIndex = 0; devIndex < numDevs; devIndex++)
			{
				unsigned int numLights = 0;
				result = getNumLightsFunction(devIndex, &numLights);

				LFX_COLOR red, blue;

				red.red = 255;
				red.green = 0;
				red.blue = 0;
				red.brightness = 255;

				blue.red = 0;
				blue.green = 0;
				blue.blue = 255;
				blue.brightness = 255;

				for(unsigned int lightIndex = 0; lightIndex < numLights; lightIndex++)
					setLightColorFunction(devIndex, lightIndex, (lightIndex % 2 == 0) ? &red : &blue);
			}


			for(unsigned int devIndex = 0; devIndex < numDevs; devIndex++)
			{
				unsigned char devType = 0;
				unsigned char* devTypePtr = &devType;

				unsigned int descSize = 255;
				char* description = new char[descSize];
				result = getDeviceDescriptionFunction(devIndex, description, descSize, &devType);
				std::cout << "Description: " << description << std::endl;
				delete []description;

				description = new char[descSize];	
				LFX_COLOR color;

				unsigned int numLights = 0;
				result = getNumLightsFunction(devIndex, &numLights);

				for (unsigned int lightIndex = 0; lightIndex < numLights; lightIndex++)
				{
					result = getLightDescriptionFunction(devIndex, lightIndex, description, descSize);
					if (result != LFX_SUCCESS)
						continue;

					result = getLightColorFunction(devIndex, lightIndex, &color);
					if (result != LFX_SUCCESS)
						continue;

					std::cout << "\tLight: " << lightIndex << "\tDescription: " << description << "\tColor: " << 
						(short)color.brightness << ", " << (short)color.red << ", " << 
						(short)color.green << ", " << (short)color.blue << std::endl;
				}
				
				delete []description;
			}

			result = updateFunction();
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

