/*

  Includes the presets used by the "-p" command option.

*/

const int MAX_PRESET = 20;    //if you add presets, increment this number!

const int preset_size[] = { 6,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1 }; //how many magics per preset?

const char preset_data[][46] = {
	{ "Z00MF00FF0MFF00F0M0F00FFM0FF00FM00FF0FMF0FF00" }, //01 Morph: Smooth Rainbow
	{ "Z00MFFF444M444FFF" },	//02 Morph: Grey/White
	{ "Z00MFF00F0M0F0FF0" },	//03 Morph: Yellow/Green
	{ "Z00M00FF00MF0000F" },	//04 Morph: Blue/Red
	{ "Z00MFFF000M000FFF" },	//05 Morph: Black/White
	{ "Z00MF0770FM70FF07" },	//06 Morph: Pink/Purple
	{ "Z00M00F70FM70F00F" },	//07 Morph: Blue/Purple
	{ "Z00MF60FF0MFF0F60" },	//08 Morph: Orange/Yellow
	{ "Z00MF07FF0MFF0F07" },	//09 Morph: Pink/Yellow
	{ "Z00FF48" },	//10 Fixed: Soft Pink
	{ "Z00FF00" },	//11 Fixed: Red
	{ "Z00FFF0" },	//12 Fixed: Yellow
	{ "Z00F0F0" },	//13 Fixed: Green
	{ "Z00F0FF" },	//14 Fixed: Cyan
	{ "Z00F00F" },	//15 Fixed: Blue
	{ "Z00FF60" },	//16 Fixed: Orange
	{ "Z00F111" },	//17 Fixed: Dark Night
	{ "Z00BF00" },	//18 Blink: Red
	{ "Z00B0F0" },	//19 Blink: Green
	{ "Z00B00F" }	//20 Blink: Blue
};

const char preset_name[][22] = {
	{ "Morph: Smooth Rainbow" },
	{ "Morph: Grey/White" },
	{ "Morph: Yellow/Green" },
	{ "Morph: Blue/Red" },
	{ "Morph: Black/White" },
	{ "Morph: Pink/Purple" },
	{ "Morph: Blue/Purple" },
	{ "Morph: Orange/Yellow" },
	{ "Morph: Pink/Yellow" },
	{ "Fixed: Soft Pink" },
	{ "Fixed: Red" },
	{ "Fixed: Yellow" },
	{ "Fixed: Green" },
	{ "Fixed: Cyan" },
	{ "Fixed: Blue" },
	{ "Fixed: Orange" },
	{ "Fixed: Dark Night" },
	{ "Blink: Red" },
	{ "Blink: Green" },
	{ "Blink: Blue" }
};
