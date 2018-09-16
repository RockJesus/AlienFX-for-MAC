using System;
using System.Text;
using LightFX;
using System.Threading;

namespace SampleApp4
{
	class Program
	{
		static void Main(string[] args)
		{
			const int sleepTimeFrame = 80;

			var lightFX = new LightFXController();
			var result = lightFX.LFX_Initialize();
			if (result == LFX_Result.LFX_Success)
			{
				var version = new StringBuilder();
				lightFX.LFX_GetVersion(out version, 255);
				Console.WriteLine("Version number: " + version);
				Thread.Sleep(3000);

				useLFXLights(sleepTimeFrame, lightFX);

				Console.WriteLine();
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

		private static void useLFXLights(int sleepTimeFrame, LightFXController lightFX)
		{
			var green = new LFX_ColorStruct(255, 0, 255, 0);
			var red = new LFX_ColorStruct(255, 255, 0, 0);

			lightFX.LFX_Reset();
			for (int i = 0; i <= 100; i++)
			{
				lightFX.LFX_Light(LFX_Position.LFX_All, i % 2 == 0 ? red : green);

				Console.Clear();
				Console.Write("Iteration: " + i);
				lightFX.LFX_Update();

				Thread.Sleep(sleepTimeFrame);
			}
		}
	}
}
