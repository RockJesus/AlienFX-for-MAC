#include "stdafx.h"
#include "LFX2.h"
#include <ctime>
#include <iostream>
#include <iomanip>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	LFX2INITIALIZE initFunction;
	LFX2RELEASE releaseFunction;
	LFX2RESET resetFunction;
	LFX2UPDATE updateFunction;
	LFX2LIGHT lightFunction;
	LFX2GETVERSION versionFunction;

	 HINSTANCE hLibrary = LoadLibrary(_T(LFX_DLL_NAME));
	 if (hLibrary)
	 {
		initFunction = (LFX2INITIALIZE)GetProcAddress(hLibrary, LFX_DLL_INITIALIZE);
		releaseFunction = (LFX2RELEASE)GetProcAddress(hLibrary, LFX_DLL_RELEASE);
		resetFunction = (LFX2RESET)GetProcAddress(hLibrary, LFX_DLL_RESET);
		updateFunction = (LFX2UPDATE)GetProcAddress(hLibrary, LFX_DLL_UPDATE);
		lightFunction = (LFX2LIGHT)GetProcAddress(hLibrary, LFX_DLL_LIGHT);
		versionFunction = (LFX2GETVERSION)GetProcAddress(hLibrary, LFX_DLL_GETVERSION);

		LFX_RESULT result = initFunction();
		if (result == LFX_SUCCESS)
        {
			if (versionFunction == NULL)
				std::cout << "SDK version not found ..." << std::endl;
			else
			{
				unsigned int versionSize = 255;
				char* version = new char[versionSize];
				result = versionFunction(version, versionSize);
				std::cout << "SDK version: " << version << std::endl;
				delete []version;

				Sleep(3000);
			}

			resetFunction();
			for(int i = 0; i <= 255; i++)
			{
				int color = ((0xFF - i) << 16) | i;
				lightFunction(LFX_ALL, color | LFX_FULL_BRIGHTNESS);
				updateFunction();
				system("cls");
				std::cout << "Color: " << hex << setw(6) << setfill('0') << color <<std::endl;
				Sleep(100);
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