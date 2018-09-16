#include "stdafx.h"
#include "LFX2.h"
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
	LFX2ACTIONCOLOR lightActionColorFunction;
	LFX2ACTIONCOLOREX lightActionColorExFunction;
	LFX2SETTIMING setTiming;

	HINSTANCE hLibrary = LoadLibrary(_T(LFX_DLL_NAME));
	if (hLibrary)
	{
		initFunction = (LFX2INITIALIZE)GetProcAddress(hLibrary, LFX_DLL_INITIALIZE);
		releaseFunction = (LFX2RELEASE)GetProcAddress(hLibrary, LFX_DLL_RELEASE);
		resetFunction = (LFX2RESET)GetProcAddress(hLibrary, LFX_DLL_RESET);
		updateFunction = (LFX2UPDATE)GetProcAddress(hLibrary, LFX_DLL_UPDATE);
		lightFunction = (LFX2LIGHT)GetProcAddress(hLibrary, LFX_DLL_LIGHT);
		lightActionColorFunction = (LFX2ACTIONCOLOR)GetProcAddress(hLibrary, LFX_DLL_ACTIONCOLOR);
		lightActionColorExFunction = (LFX2ACTIONCOLOREX)GetProcAddress(hLibrary, LFX_DLL_ACTIONCOLOREX);
		setTiming = (LFX2SETTIMING)GetProcAddress(hLibrary, LFX_DLL_SETTIMING);

		LFX_RESULT result = initFunction();
		if (result == LFX_SUCCESS)
        {
			result = setTiming(100);
			result = resetFunction();

			result = lightFunction(LFX_ALL, LFX_BLUE | LFX_FULL_BRIGHTNESS);
			std::cout << "Setting all lights, color: " << hex << setw(6) << setfill('0') << LFX_BLUE << std::endl;
			result = updateFunction();

			Sleep(5000);
		
			result = lightActionColorFunction(LFX_ALL, LFX_ACTION_PULSE, LFX_RED | LFX_FULL_BRIGHTNESS);
			std::cout << "Pulsing all lights, color: " << hex << LFX_RED << std::endl;
			result = updateFunction();

			Sleep(5000);

			result = lightActionColorExFunction(LFX_ALL, LFX_ACTION_MORPH, LFX_RED | LFX_FULL_BRIGHTNESS, LFX_BLUE | LFX_FULL_BRIGHTNESS);
			std::cout << "Morphing all lights, from color: " << hex << LFX_RED << " to color: " << hex << setw(6) << setfill('0') << LFX_BLUE << std::endl;
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