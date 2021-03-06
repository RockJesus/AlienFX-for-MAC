using LightFX;
using System;
using System.Text;

namespace SampleApp1
{
    class Program
    {
        static void Main()
        {
            var lightFX = new LightFXController();

            var result = lightFX.LFX_Initialize();
            if (result == LFX_Result.LFX_Success)
            {
                lightFX.LFX_Reset();

				uint numDevs;
            	lightFX.LFX_GetNumDevices(out numDevs);

				for (uint devIndex = 0; devIndex < numDevs; devIndex++)
				{
					uint numLights;
					lightFX.LFX_GetNumLights(devIndex, out numLights);

					var green = new LFX_ColorStruct(255, 0, 255, 0);
					var red = new LFX_ColorStruct(255, 255, 0, 0);
					for (uint lightIndex = 0; lightIndex < numLights; lightIndex++)
						lightFX.LFX_SetLightColor(devIndex, lightIndex, lightIndex%2 == 0 ? red : green);
				}


				for (uint devIndex = 0; devIndex < numDevs; devIndex++)
				{
					StringBuilder devDescription;
					LFX_DeviceType type;

					result = lightFX.LFX_GetDeviceDescription(devIndex, out devDescription, 255, out type);
					if (result != LFX_Result.LFX_Success)
						continue;

					Console.WriteLine(string.Format("Device: {0} \tDescription: {1} \tType: {2}", devIndex, devDescription, type));

					uint numLights;
					lightFX.LFX_GetNumLights(devIndex, out numLights);
					for (uint lightIndex = 0; lightIndex < numLights; lightIndex++)
					{
						StringBuilder description;
						result = lightFX.LFX_GetLightDescription(devIndex, lightIndex, out description, 255);
						if (result != LFX_Result.LFX_Success)
							continue;

						LFX_ColorStruct color;
						result = lightFX.LFX_GetLightColor(devIndex, lightIndex, out color);
						if (result != LFX_Result.LFX_Success)
							continue;

						Console.WriteLine(string.Format("\tLight: {0} \tDescription: {1} \tColor: {2}", lightIndex, description, color));
					}
				}

            	lightFX.LFX_Update();
				Console.WriteLine("Done.\r\rPress ENTER key to finish ...");
				Console.ReadLine();            
				lightFX.LFX_Release();
            }
            else
            {
                switch (result)
                {
                    case LFX_Result.LFX_Error_NoDevs:
                        Console.WriteLine("There is not AlienFX device available.");
                        break;
                    default:
                        Console.WriteLine("There was an error initializing the AlienFX device.");
                        break;
                }
            }
        }
    }
}
