using System;
using System.Text;
using LightFXConfigurator;
using LightFXConfigurator.Classes;

namespace ManagedSample2
{
	class Program
	{
		static void Main()
		{
			ILightFXConfigurationController configurationController = new LightFXConfigurationController();
			configurationController.LFX_CONFIGURATOR_Initialize();

			const string afxFile = "AIF-TheGame.afx";
			const string afxGame = "AIF";

			Console.WriteLine("**************************** Registering game ****************************");

			configurationController.LFX_CONFIGURATOR_RegisterConfigurationFile(afxGame, afxFile, LFX_ApplicationType.Game);
			Console.WriteLine(string.Format("Game: {0}, Path: {1}", afxGame, afxFile));

			Console.WriteLine("********************* Reading Events ************************");

			uint[] colors;
			uint count;
			int eventID;
			StringBuilder eventName;

			LFX_Result result = configurationController.LFX_CONFIGURATOR_GetConfigurationEventAt(afxGame, LFX_EventPosition.FirstEvent, out eventID, out eventName, 255, out colors, out count);
			while (result == LFX_Result.LFX_Success)
			{
				Console.WriteLine(string.Format("App: {0} Event: {1} Event Name: {2}", afxGame, eventID, eventName));
				for (int i = 0; i < count; i++)
					Console.WriteLine(string.Format("Color: {0:X8}", colors[i]));

				result = configurationController.LFX_CONFIGURATOR_GetConfigurationEventAt(afxGame, LFX_EventPosition.NextEvent, out eventID, out eventName, 255, out colors, out count);
			}

			Console.WriteLine("**************************** Removing game ****************************");

			configurationController.LFX_CONFIGURATOR_UnregisterConfigurationFile(afxGame);
			Console.WriteLine(string.Format("Unregistering: {0}", afxGame));

			configurationController.LFX_CONFIGURATOR_Release();
		}
	}
}
