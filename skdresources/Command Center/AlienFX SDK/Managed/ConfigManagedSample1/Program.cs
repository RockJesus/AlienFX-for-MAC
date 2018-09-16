using System;
using System.Text;
using LightFXConfigurator;
using LightFXConfigurator.Classes;

namespace ManagedSample1
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

			Console.WriteLine("********************** Reading Users Config filename ************************");

			StringBuilder configFile;
			configurationController.LFX_CONFIGURATOR_GetUserConfigurationFilePath(afxGame, out configFile, 255);
			Console.WriteLine(string.Format("Config file for {0}: {1}", afxGame, configFile));

			Console.WriteLine("********************* Reading Event from index ************************");

			uint[] colors;
			uint count;
			configurationController.LFX_CONFIGURATOR_GetConfigurationEvent(afxGame, 1, out colors, out count);

			Console.WriteLine(string.Format("App: {0} Event: 1 ", afxGame));
			for (int i = 0; i < count; i++)
				Console.WriteLine(string.Format("Color: {0:X8}", colors[i]));

			Console.WriteLine("**************************** Removing game ****************************");

			configurationController.LFX_CONFIGURATOR_UnregisterConfigurationFile(afxGame);
			Console.WriteLine(string.Format("Unregistering: {0}", afxGame));

			configurationController.LFX_CONFIGURATOR_Release();
		}
	}
}
