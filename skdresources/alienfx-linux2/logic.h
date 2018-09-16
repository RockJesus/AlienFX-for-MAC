/*

  Contains the AlienFX lightchip programming reference logic.

	/=================================================
	|			     INDEX=1
	|	/=========================================
	|	|		     RESET_ALL_LIGHTS_ON
	|	|		     SET_SPEED
	|	|	/=================================
	| 	|	|	  /=======================
AlienFX	| FOR	| FOR   | FOR EACH|  SAVE_NEXT
Light-->| EACH->| EACH->| COLOUR->|  SET_COLOUR
Profile	| POWER	| ZONE  | SEQUENCE|  INDEX++
	| MDOE	|	|	  \=======================
	|	|	|	     SAVE_NEXT
	|	|	|	     LOOP_BLOCK_END
	|	|	\=================================
	|	|		     SAVE
	|	\=========================================
	|			     TRANSMIT_EXECUTE
	\=================================================

*/
//Re-written as code...

//Preset begin=====================================================================
	int index = 1;
	for (int p=0;p<number_of_powermodes;p++) { //Powermode begins==============
		afx_cmd(COMMAND_RESET, RESET_ALL_LIGHTS_ON,0,0,0,0,0,wait);
		afx_cmd(COMMAND_SET_SPEED,b1,b2,0,0,0,0,wait);
		for (int z=0;z<number_of_zones;z++) { //Zone begins================
			for (int c=0;c<number_of_colours;c++) { //Colour begins====
				afx_cmd(COMMAND_SAVE_NEXT, POWER_MODE_BLOCK,0,0,0,0,0,wait);
				afx_set(COMMAND_SET_COLOUR_MORPH,R1,G1,B1,R2,G2,B2,zone);
				index++
			} //Colour ends============================================
			afx_cmd(COMMAND_SAVE_NEXT, POWER_MODE_BLOCK,0,0,0,0,0,wait);
			afx_cmd(COMMAND_LOOP_BLOCK_END,0,0,0,0,0,0,wait);
		} //Zone ends======================================================
		afx_cmd(COMMAND_SAVE, POWER_MODE_BLOCK,0,0,0,0,0,wait);
	} //Powermode ends=========================================================
	afx_cmd(COMMAND_TRANSMIT_EXECUTE,0,0,0,0,0,0,wait);
//Preset end=======================================================================

//Restructuring notes...

int power[] = {
	char mode;
	int zones[] = {
		int colours[] = {
			char action;
			int r,g,b;
		}
	}
}

//Or, read linear fashion?
//
//Example data

/*
Power1:
	Zone01: Morph FFF to EEE, Morph EEE to FFF
	Zone12: Morph BBB to AAA, Morph AAA to BBB
	Zone13: Fixed AAA
	Zone14: Blink FFF, Blink 999
	Zone25: Fixed 333
	Zone26: Fixed 333
Power2:
	Zone01: Morph 555 to 444, Morph 444 to 555
	Zone12: Morph 333 to 222, Morph 222 to 333
	Zone13: Fixed 111
	Zone14: Blink AAA, Blink 333
	Zone25: Fixed 000
	Zone26: Fixed 000
Power3:
	All Zones: Fixed 000
*/ //146
P1Z01MFFFEEEMEEEFFFZ12MBBBAAAMAAABBBZ13FAAAZ14BFFFB999Z25F333Z26F333P2Z01M555444M444555Z12M333222M222333Z13F111Z14BAAAB333Z25F000Z26F000P3Z00F000

//8
//all powers&zones, fixed black (lights out)
P0Z00F000
