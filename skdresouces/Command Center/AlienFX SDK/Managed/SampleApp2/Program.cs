using LightFX;
using System;
using System.Text;

namespace SampleApp2
{
    class Program
    {
        static void Main()
        {
            var lightFX = new LightFXController();

            var result = lightFX.LFX_Initialize();
            if (result == LFX_Result.LFX_Success)
            {
                uint numDevices;
                lightFX.LFX_GetNumDevices(out numDevices);
                Console.WriteLine(string.Format("Devices: {0}", numDevices));

            	for (uint devIndex = 0; devIndex < numDevices; devIndex++)
                {
                    LFX_DeviceType devType;
                	StringBuilder description;
                	result = lightFX.LFX_GetDeviceDescription(devIndex, out description, 255, out devType);
                    if (result != LFX_Result.LFX_Success)
                        continue;

                    Console.WriteLine(string.Format("Device {0}: \tDescription: {1} \tType: {2}", devIndex, description, devType));

                    uint numLights;
                    result = lightFX.LFX_GetNumLights(devIndex, out numLights);
                    if (result != LFX_Result.LFX_Success)
                        continue;

                	for (uint lightIndex = 0; lightIndex < numLights; lightIndex++)
                    {
                        result = lightFX.LFX_GetLightDescription(devIndex, lightIndex, out description, 255);
                        if (result != LFX_Result.LFX_Success)
                            continue;

                        LFX_Position location;
                        result = lightFX.LFX_GetLightLocation(devIndex, lightIndex, out location);
                        if (result != LFX_Result.LFX_Success)
                            continue;

                        Console.WriteLine(string.Format("\tLight: {0} \tDescription: {1} \tLocation: {2}", lightIndex, description, location));
                    }
                }

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
