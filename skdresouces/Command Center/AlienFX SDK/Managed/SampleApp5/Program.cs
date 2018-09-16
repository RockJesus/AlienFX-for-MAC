using LightFX;
using System;
using System.Threading;

namespace SampleApp5
{
	class Program
	{
		static void Main()
		{
			var lightFX = new LightFXController();

			var result = lightFX.LFX_Initialize();
			if (result == LFX_Result.LFX_Success)
			{
                lightFX.LFX_SetTiming(100);
				lightFX.LFX_Reset();

				var primaryColor = new LFX_ColorStruct(255, 0, 255, 0);
				lightFX.LFX_Light(LFX_Position.LFX_All, primaryColor);
				Console.WriteLine(string.Format("Setting all lights color: {0}", primaryColor));
				lightFX.LFX_Update();

				Thread.Sleep(5000);

				var secondaryColor = new LFX_ColorStruct(255, 255, 0, 0);
				lightFX.LFX_ActionColor(LFX_Position.LFX_All, LFX_ActionEnum.Pulse, secondaryColor);
				Console.WriteLine(string.Format("Pulsing all lights in color: {0}", secondaryColor));
				lightFX.LFX_Update();

				Thread.Sleep(5000);

				lightFX.LFX_ActionColorEx(LFX_Position.LFX_All, LFX_ActionEnum.Morph, primaryColor, secondaryColor);
				Console.WriteLine(string.Format("Morphing all lights from color {0} to {1}.", primaryColor, secondaryColor));
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
