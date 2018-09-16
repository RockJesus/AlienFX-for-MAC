/*

  ** AlienFX Light Control Program **
  -- Controls the accessory lights for AlienWare M11X R3 --

  Requires libusb-1.0 headers to build, and the libusb-1.0.so library to execute.

*/

#include <stdio.h>
#include <cstdlib>
#include <string>
#include <string.h>
#include <iostream>
#include <libusb-1.0/libusb.h>

using namespace std;

//Include the zone presets & useful consts
#include "presets.h"
#include "consts.h"

libusb_context* context;
libusb_device_handle* alienFx;
int alienFXid, START_BYTE, /*magicnum,*/ preset=-1, speed, usb_sleep=DEFAULT_USB_SLEEP, power_block, command_count;
char command[1024]; unsigned char rawcmd[10];
bool user_is_root, debug, debug2, help, reset, reboot, saving, darkness, wait, rawmode;

void detach(libusb_device_handle* device){
  int r = libusb_kernel_driver_active(device, 0);
  if (r == 1)
    r = libusb_detach_kernel_driver(device, 0);
}

void attach(libusb_device_handle* device){
  libusb_attach_kernel_driver(device, 0);
}

int ReadDevice(unsigned char* pData, int pDataLength){
 if (debug2) { printf("Debug: USB Read"); for (int i=0;i<pDataLength;i++) { printf("[%i]",pData[i]); } printf("\n"); }
 int v = libusb_control_transfer(alienFx, READ_REQUEST_TYPE, READ_REQUEST, READ_VALUE, READ_INDEX, pData,pDataLength,0);
 return v;
}

int WriteDevice(unsigned char* pData, int pDataLength){
 if (debug2) { printf("Debug: USB Write"); for (int i=0;i<pDataLength;i++) { printf("[%i]",pData[i]); } printf("\n"); }
 int v = libusb_control_transfer(alienFx, SEND_REQUEST_TYPE, SEND_REQUEST, SEND_VALUE, SEND_INDEX, pData,pDataLength,0);
 usleep(usb_sleep);//sdk recommends 250msec
 memset(pData,0,pDataLength);
 ReadDevice(pData,pDataLength);
 return v;
}

int AlienfxInit(){
  libusb_init(&context);
  if (debug2) libusb_set_debug (context, 3);
  if (alienFXid == 0) alienFXid = ALIENFX_PID_ALLPOWERFULL_M11XR3;
  if (debug2) printf("Debug: USB ProductID set to: %llx. Connecting...", *(unsigned int *)&alienFXid);
  alienFx = libusb_open_device_with_vid_pid(context, ALIENFX_VID, alienFXid);
  START_BYTE = START_BYTE_M11XR3;
  if(alienFx == NULL){
    if (debug2) printf("failed.\n");
    alienFXid = ALIENFX_PID_AREA51;
    if (debug2) printf("Debug: USB ProductID set to: %llx. Connecting...", *(unsigned int *)&alienFXid);
    alienFx = libusb_open_device_with_vid_pid(context, ALIENFX_VID, alienFXid);
    START_BYTE = START_BYTE_AREA51;
    if(alienFx == NULL){
      if (debug2) printf("failed.\n");
      alienFXid = ALIENFX_PID_ALLPOWERFULL_M11X;
      if (debug2) printf("Debug: USB ProductID set to: %llx. Connecting...", *(unsigned int *)&alienFXid);
      alienFx = libusb_open_device_with_vid_pid(context, ALIENFX_VID, alienFXid);
      START_BYTE = START_BYTE_M11X;
      if(alienFx == NULL){
        if (debug2) printf("failed.\n");
        alienFXid = ALIENFX_PID_ALLPOWERFULL;
        if (debug2) printf("Debug: USB ProductID set to: %llx. Connecting...", *(unsigned int *)&alienFXid);
        alienFx = libusb_open_device_with_vid_pid(context, ALIENFX_VID, alienFXid);
        START_BYTE = START_BYTE_ALLP;
        if(alienFx == NULL){
          if (debug2) printf("failed.\n");
          alienFXid = ALIENFX_PID_AURORA_NONALX;
          if (debug2) printf("Debug: USB ProductID set to: %llx. Connecting...", *(unsigned int *)&alienFXid);
          alienFx = libusb_open_device_with_vid_pid(context, ALIENFX_VID, alienFXid);
          START_BYTE = START_BYTE_NONALX;
          if(alienFx == NULL){
            if (debug2) printf("failed.\n");
            alienFXid = ALIENFX_PID_UNKNOWN;
            if (debug2) printf("Debug: USB ProductID set to: %llx. Connecting...", *(unsigned int *)&alienFXid);
            alienFx = libusb_open_device_with_vid_pid(context, ALIENFX_VID, alienFXid);
            START_BYTE = START_BYTE_M11XR3;
            if(alienFx == NULL) {
              if (debug2) printf("failed.\nDebug: Exhausted list of known product ID's.\n");
              return NOT_FOUND;
            } else { if (debug2) printf("success!\n"); };
          } else { if (debug2) printf("success!\n"); };
        } else { if (debug2) printf("success!\n"); };
      }else { if (debug2) printf("success!\n"); };
    }else { if (debug2) printf("success!\n"); };
  }else { if (debug2) printf("success!\n"); };
  detach(alienFx);
  int res = libusb_claim_interface(alienFx,0);
  if(res < 0) return NOT_FOUND;
  return alienFXid;
}

void AlienfxDeinit(){
  if (debug2) printf("Debug: USB disconnect...");
  libusb_release_interface(alienFx, 0);
  attach(alienFx);
  libusb_close(alienFx);
  libusb_exit(context);
  if (debug2) printf("done.\n");
}

char hextoint(char c) {
	if (c >= '0' && c <= '9') return c-'0';
	if (c >= 'A' && c <= 'F') return c-'A'+10;
	if (c >= 'a' && c <= 'f') return c-'a'+10;
	return -1;
}

void print_help() {
	printf("Alienware AlienFX Light Manipulation Program v1.0\n");
	printf("Usage:    alienfx [options]\n");
	printf("   or     alienfx -h\n");
	printf("Options:\n");
	printf("  -c ZnnM[RGB[RGB]]..   Lightchip Command\n");
	printf("  -d <num>  Set morph/flash delay time. (1=Fastest to 10=Slowest)\n");
	printf("  -h        This helpful message\n");
	printf("  -P <hex>  Specify the hex ProductID of lightchip\n");
	printf("  -r        Reset chip; software reset of lightchip\n");
	printf("  -R        Reboot chip; hardware reset of lightchip\n");
	printf("  -s <num>  Save to PowerBlock (for load-on-boot, etc)\n");
	printf("  -t <num>  Increase USB timing delay by increment (1 to 100)\n");
	printf("  -u        Show USB debug if needed\n");
	printf("  -v        Show verbose debug output\n");
	printf("  -w        Check for lightchip ready (excess,slow)\n");
	printf("  -X <num>  Raw command mode. Expects nine 0-255 values\n");
	printf("  -z <num>  Apply a preset. Use '0' to get a list. Implies ignorance of -c.\n");
	printf("Each Command is made up of Z (zone), M (mode) and RGB (colour) values.\n");
	printf("Zone is either 00=All or 01-27.\n");
	printf("Mode is either B=Blink, M=Morph or F=Fixed.\n");
	printf("RGB is three single-digits of hex. Specify two colours, even if 2nd unused.\n");
	printf("Zones are either 1 or 0. 1 is on, 0 is off. There are sixteen zones.\n");
	printf("PowerBlock is either 0=Default, 1=Bootup, 5=AC, 6=Charge, 7=BattLow or 8=Batt.\n");
	printf("Defaults: Power=ALL, Zones=ALL, Mode=FIXED, RGB=000(black)\n");
	printf("Permanently saving the settings into lightchip not available on some models.\n");
	printf("If called with no arguments the lights will silently turn off.\n");
	printf("For example commands see the preset list.\n");
	printf("See 'man alienfx' for more information.\n");
}

int is_valid_command_string(char *cmd) {
  //i know this could be better, but it works and i can't be bothered.
  int i=0; command_count=0;
  bool next_zone, next_action;
  next_zone=true;
  while (next_zone) {
    if (cmd[i]=='Z'|cmd[i]=='z') { i++;
      if (hextoint(cmd[i])>=0) i++; else return i+1;
      if (hextoint(cmd[i])>=0) i++; else return i+1;
      if ((hextoint(cmd[i-2])*10)+hextoint(cmd[i-1])>27) return i-1;
      next_action=true;
      while (next_action) {
        if (cmd[i]=='B'|cmd[i]=='b'|cmd[i]=='F'|cmd[i]=='f') { i++;
          if (hextoint(cmd[i])>=0) i++; else return i+1;
          if (hextoint(cmd[i])>=0) i++; else return i+1;
          if (hextoint(cmd[i])>=0) i++; else return i+1;
          command_count++;
        } else //if B or F
        if (cmd[i]=='M'|cmd[i]=='m') { i++;
          if (hextoint(cmd[i])>=0) i++; else return i+1;
          if (hextoint(cmd[i])>=0) i++; else return i+1;
          if (hextoint(cmd[i])>=0) i++; else return i+1;
          if (hextoint(cmd[i])>=0) i++; else return i+1;
          if (hextoint(cmd[i])>=0) i++; else return i+1;
          if (hextoint(cmd[i])>=0) i++; else return i+1;
          command_count++;
        } else //if M
          if (cmd[i]=='Z'|cmd[i]=='z') next_action=false;
        else
          if (strlen(cmd)==i) return 0; else return i+1;
      } //while actions
    } else { if (i==0) return 1; else next_zone=false; } //if Z
  } //while zones
  return 0; //we made it passed the checks
}

bool parse_arguments(int argc, char *argv[], char **envp) {
	//Trap username from environment
	char *var,tag[5],uservar[9],arg[2],setzones[16];
	int i;
        while(*envp != NULL) {
		var = *(envp++);
		if (strcmp(var,"USER=root")|strcmp(var,"USERNAME=root")) user_is_root=true;
	}
	bool colour_option;
        while(*argv != NULL) {
		var = *(argv++);
		//if (debug2) printf("Debug: Var: \"%s\"\n", var);
		if (!strcmp(var,"-c")) { strcpy(command,*(argv++)); } //command!
		if (!strcmp(var,"-d")) { speed = atoi(*(argv++)); }  //flash/morph speed
		if (!strcmp(var,"-h")|!strcmp(var,"--help")) { help = true;   }  //help output
		if (!strcmp(var,"-p")) { preset = atoi(*(argv++)); } //presets!
		if (!strcmp(var,"-P")) { sscanf(*(argv++), "%llx", (unsigned int *)&alienFXid); }  //USB ProductID, ugly!
		if (!strcmp(var,"-r")) { darkness = true; }//Plunge into darkness before set colours
		if (!strcmp(var,"-R")) { reboot = true; }  //COMMAND_REBOOT_CHIP
		if (!strcmp(var,"-s")) { saving = true; power_block = atoi(*(argv++)); }  //saving (save to CMOS not available)
		if (!strcmp(var,"-t")) { usb_sleep = DEFAULT_USB_SLEEP * atoi(*(argv++)); } //USB delay
		if (!strcmp(var,"-u")) { debug2 = true; }  //libusb debugging
		if (!strcmp(var,"-v")) { debug = true;  }  //alienfx debugging
		if (!strcmp(var,"-w")) { wait = true;   }  //usb in waiting
		if (!strcmp(var,"-X")) { //Raw command mode, useful for light notifications
			for (i=0;i<10;i++) { rawcmd[i] = atoi(*(argv++)); };
			rawmode = true;
		};
	}
	if (preset==0) {
		printf("All presets are applied by default to all zones.  Arguments supplied regarding\n");
		printf("power mode, light zones, effects or colours are ignored when selecting presets.\n");
		printf("Available lightchip presets:\n");
		for (i=0;i<20;i++) printf(" alienfx -p %.2i == alienfx -c %s == %s\n", i+1, preset_data[i], preset_name[i]);
		exit(0);
	}
	if ((preset>0)&(preset<=MAX_PRESET)) {
		if (debug) printf("Debug: Chosen preset %.2i = %s\n", preset, preset_name[preset-1]);
		memset(command,0,sizeof(command));
		if (debug) printf("Debug: Preset Colour Command [%s]\n", preset_data[preset-1]);
		strcpy(command, preset_data[preset-1]);
	}
	if (strlen(command)>0) {
		i = is_valid_command_string(command);
		if (i>0) {
			printf("Error at character %i in specified command.\n", i);
			exit(2);
		}
	}
	return true;
}

bool usb_is_ready() {
	bool ready; char crashcheck;
	unsigned char thedata[DATA_LENGTH],check[DATA_LENGTH];
	while (!ready) {
		memset(thedata, FILL_BYTE, DATA_LENGTH);
		thedata[0] = START_BYTE;
		thedata[1] = COMMAND_GET_STATUS;
		memcpy(check,thedata,DATA_LENGTH);
		WriteDevice(thedata, DATA_LENGTH);
		ready = memcmp(check,thedata,DATA_LENGTH);
//		ready = (thedata[0] == STATE_READY);
		if (debug) {
			if (ready) printf("Debug: Lightchip now ready\n");
			if (thedata[0] == STATE_BUSY) printf("Debug: Lightchip is busy\n");
			if (thedata[0] == STATE_UNKNOWN_COMMAND) printf("Debug: Lightchip claims 'unknown command'\n");
		}
		if (crashcheck == 4) printf("Warning: The lightchip is not responding!\n");
		if (crashcheck == 12) { printf("Error: Lightchip failed to respond.\n"); AlienfxDeinit(); exit(1); }
		if (!ready) usleep(usb_sleep);
		crashcheck++;
	}
	return ready;
}

void afx_cmd(unsigned char cmd, unsigned char block, unsigned char data1, unsigned char data2, unsigned char data3,
		unsigned char data4, unsigned char data5, unsigned char data6) {
	unsigned char thedata[DATA_LENGTH];
	memset(thedata, FILL_BYTE, DATA_LENGTH);
	thedata[0] = START_BYTE;
	thedata[1] = cmd;
	thedata[2] = block;
	thedata[3] = data1;
	thedata[4] = data2;
	thedata[5] = data3;
	thedata[6] = data4;
	thedata[7] = data5;
	thedata[8] = data6;
	WriteDevice(thedata, DATA_LENGTH);
}

int afx_get() {
  unsigned char thedata[DATA_LENGTH];
  return ReadDevice(thedata, DATA_LENGTH);
}

void afx_set(unsigned char cmd, unsigned char idx, unsigned char zone, unsigned char r1, unsigned char g1,
		unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2, bool chk) {
	r1*=16;	g1*=16;	b1*=16;	r2*=16;	g2*=16;	b2*=16;
	int RG = r1 & 0xF0; RG = RG | ((g1 >> 4) & 0x0F);
	int BR = b1 & 0xF0; BR = BR | ((r2 >> 4) & 0x0F);
	int GB = g2 & 0xF0; GB = GB | ((b2 >> 4) & 0x0F);
	char z1, z2, z3, z4; bool zones[32];
//FIXME: There must be a better way to bitwise the zone?
	memset(zones,1,32);
	if (zone>0) { memset(zones,0,32); zones[zone] = true; }
	//z1 = ((idx>>4)&0xFF); //phantom zoning, applies to older chips?
	z1 = (zones[0]<<7)+(zones[1]<<6)+(zones[2]<<5)+idx;
	z2 = (zones[3]<<7)+(zones[4]<<6)+(zones[5]<<5)+(zones[6]<<4)+(zones[7]<<3)+(zones[8]<<2)+(zones[9]<<1)+zones[10];
	z3 = (zones[11]<<7)+(zones[12]<<6)+(zones[13]<<5)+(zones[14]<<4)+(zones[15]<<3)+(zones[16]<<2)+(zones[17]<<1)+zones[18];
	z4 = (zones[19]<<7)+(zones[20]<<6)+(zones[21]<<5)+(zones[22]<<4)+(zones[23]<<3)+(zones[24]<<2)+(zones[25]<<1)+zones[26];
//	z1 = (zones[0]<<7)+(zones[1]<<6)+(zones[2]<<5)+(zones[3]<<4)+(zones[4]<<3)+(zones[5]<<2)+(zones[6]<<1)+zones[7];
//	z2 = (zones[8]<<7)+(zones[9]<<6)+(zones[10]<<5)+(zones[11]<<4)+(zones[12]<<3)+(zones[13]<<2)+(zones[14]<<1)+zones[15];
//	z3 = (zones[16]<<7)+(zones[17]<<6)+(zones[18]<<5)+(zones[19]<<4)+(zones[20]<<3)+(zones[21]<<2)+(zones[22]<<1)+zones[23];
//	z4 = (zones[24]<<7)+(zones[25]<<6)+(zones[26]<<5)+(zones[27]<<4)+(zones[28]<<3)+(zones[29]<<2)+(zones[30]<<1)+zones[31];
	if (debug) { //ugly but needed output
		printf("Zones: 01 02 03 04 05 06 07 08 09 10 11 12 13 14\n");
		printf("Zones:  %i  %i  %i  %i  %i  %i  %i  %i  %i  %i  %i  %i  %i  %i\n",
		 zones[0], zones[1], zones[2], zones[3], zones[4], zones[5], zones[6], zones[7],
		 zones[8], zones[9], zones[10], zones[11], zones[12], zones[13]);
		printf("Zones: -----------------------------------------\n");
		printf("Zones: 15 16 17 18 19 20 21 22 23 24 25 26 27\n");
		printf("Zones:  %i  %i  %i  %i  %i  %i  %i  %i  %i  %i  %i  %i  %i\n",
		 zones[14], zones[15], zones[16], zones[17], zones[18], zones[19], zones[20],
		 zones[21], zones[22], zones[23], zones[24], zones[25], zones[26]);
	}
	unsigned char thedata[DATA_LENGTH];
	memset(thedata, FILL_BYTE, DATA_LENGTH);
	thedata[0] = START_BYTE;//Always 0x02?
	thedata[1] = cmd;//Command
	thedata[2] = z1; //3-bits ZONE and 5-bits INDEX?
	thedata[3] = z2; //8-bits ZONE
	thedata[4] = z3; //8-bits ZONE
	thedata[5] = z4; //8-bits ZONE
	thedata[6] = RG; //4-bits 1stRED, 4-bits 1stGREEN
	thedata[7] = BR; //4-bits 1stBLUE, 4-bits 2ndRED
	thedata[8] = GB; //4-bits 2ndGREEN, 4-bits 2ndBLUE
	if (chk) {
		if (usb_is_ready()) WriteDevice(thedata, DATA_LENGTH);
	} else WriteDevice(thedata, DATA_LENGTH);
}

void afx_reset() {
	if (debug) printf("Debug: Software reset\n");
	afx_cmd(COMMAND_RESET,RESET_ALL_LIGHTS_OFF,0,0,0,0,0,0);
	afx_cmd(COMMAND_TRANSMIT_EXECUTE,0,0,0,0,0,0,0);
}

void afx_reboot() {
	if (debug) printf("Debug: Reboot of lightchip requested\n");
	afx_cmd(COMMAND_REBOOT_CHIP,0,0,0,0,0,0,0);
	AlienfxDeinit();
	sleep(3);
	if (!AlienfxInit()) {
		if (debug) printf("Debug: Exiting\n");
		exit(1);
	}
}

void afx_spd(int speed) {
//	speed = speed * 100;
	if(speed > MAX_SPEED) speed = MAX_SPEED;
	if(speed < MIN_SPEED) speed = MIN_SPEED;
	int b1 = (speed >> 8) & 0xFF;
	int b2 = speed & 0xFF;
	afx_cmd(COMMAND_SET_SPEED,b1,b2,0,0,0,0,0);
}

bool afx_raw(unsigned char cmd[DATA_LENGTH]) {
	unsigned char chk[DATA_LENGTH];
	memcpy(chk,cmd,DATA_LENGTH);
	WriteDevice(cmd,DATA_LENGTH);
//	usleep(DEFAULT_USB_SLEEP);
	if (memcmp(chk,cmd,DATA_LENGTH)) printf("AFX: OK\n"); else printf("AFX: ERR\n");
}

bool afx_process(char *cmd) {
//Command begins====================================================================
  int index = 1, ref=0, zone, z1,z2,z3,r1,g1,b1,r2,g2,b2;
  if (debug) printf("Debug: Begin lightchip command sequence\n");
  while (!(cmd[ref]==0)) { //Powermode begins=======================================
    if (debug&saving) printf("Debug:   PowerBlock: %i\n",power_block);
    if (debug&power_block==0&saving) printf("Debug:   Warning: saving to EEPROM may not be available\n");
    if (debug) printf("Debug:   Reset All Lights On\n");
    afx_cmd(COMMAND_RESET,RESET_ALL_LIGHTS_ON,0,0,0,0,0,0);
    if (speed > 0) {
      if (debug) printf("Debug:   Setting speed to %i of 10\n", speed);
      afx_spd(speed);
    }
    while (!(cmd[ref+1]==0)) { //Zone begins======================================
      if (cmd[ref]=='Z'|cmd[ref]=='z') { ref++;
        zone = (10*hextoint(cmd[ref]))+hextoint(cmd[ref+1]); ref+=2;
        if (debug) {
          if (zone==0) printf("Debug:      LightZone(All)\n");
          else printf("Debug:      LightZone(%i)\n",zone);
        }
        while (!(cmd[ref+1]==0)) { //Colour begins================================
          if (cmd[ref]=='M'|cmd[ref]=='m') { ref++;
            r1 = hextoint(cmd[ref++]); g1 = hextoint(cmd[ref++]); b1 = hextoint(cmd[ref++]);
            r2 = hextoint(cmd[ref++]); g2 = hextoint(cmd[ref++]); b2 = hextoint(cmd[ref++]);
            if (saving&debug) printf("Debug:         Save Powermode(%i)\n",power_block);
            if (saving) afx_cmd(COMMAND_SAVE_NEXT,power_block,0,0,0,0,0,0);
            if (debug) printf("Debug:         Set Colour[%i]: Morph %i %i %i into %i %i %i\n",index,r1,g1,b1,r2,g2,b2);
            afx_set(COMMAND_SET_COLOUR_MORPH,command_count,zone,r1,g1,b1,r2,g2,b2,0);
            index++;
          }
          if (cmd[ref]=='B'|cmd[ref]=='b') { ref++;
            r1 = hextoint(cmd[ref++]); g1 = hextoint(cmd[ref++]); b1 = hextoint(cmd[ref++]);
            if (saving&debug) printf("Debug:         Save Powermode(%i)\n",power_block);
            if (saving) afx_cmd(COMMAND_SAVE_NEXT,power_block,0,0,0,0,0,0);
            if (debug) printf("Debug:         Set Colour[%i]: Blink %i %i %i\n",index,r1,g1,b1);
            afx_set(COMMAND_SET_COLOUR_BLINK,command_count,zone,r1,g1,b1,255,255,255,0);
            index++;
          }
          if (cmd[ref]=='F'|cmd[ref]=='f') { ref++;
            r1 = hextoint(cmd[ref++]); g1 = hextoint(cmd[ref++]); b1 = hextoint(cmd[ref++]);
            if (saving&debug) printf("Debug:         Save Powermode(%i)\n",power_block);
            if (saving) afx_cmd(COMMAND_SAVE_NEXT,power_block,0,0,0,0,0,0);
            if (debug) printf("Debug:         Set Colour[%i]: Fixed %i %i %i\n",index,r1,g1,b1);
            afx_set(COMMAND_SET_COLOUR_FIXED,command_count,zone,r1,g1,b1,0,0,0,0);
            index++;
          }
          if (cmd[ref]=='Z'|cmd[ref]=='z'|cmd[ref]=='P'|cmd[ref]=='p') break; //else ref++;
        } //Colour ends===========================================================
        if (saving&debug) printf("Debug:      Save Powermode(%i)\n",power_block);
        if (saving) afx_cmd(COMMAND_SAVE_NEXT,power_block,0,0,0,0,0,0);
        if (debug) printf("Debug:      End Loop Block\n");
        afx_cmd(COMMAND_LOOP_BLOCK_END, 0, 0, 0, 0, 0, 0,wait);
      } else break;
    } //Zone ends=================================================================
//    if (saving&debug) printf("Debug:   Save Powermode(%i)\n",power_block);
//    if (saving) afx_cmd(COMMAND_SAVE_NEXT,power_block,0,0,0,0,0,0);
  } //Powermode ends================================================================
  if (debug) printf("Debug: Execute Instruction\n");
  afx_cmd(COMMAND_TRANSMIT_EXECUTE,0,0,0,0,0,0,wait);
//Command end=======================================================================
  if (debug) printf("Debug: End lightchip command sequence\n");
}

int main(int argc, char *argv[], char **envp) {
	memset(command,0,MAX_COMMAND);
	if (debug) printf("Debug: Hit main code block. Parsing arguments...\n");
	parse_arguments(argc, argv, envp);
	if (help) { print_help(); return 0; }
	if (debug) {
		if (user_is_root) printf("Debug: Detection suggests user may be root\n");
		else printf("Debug: Detection suggests user may NOT be root\n");
	}
	if (command[0]==0) printf("Warning: no command or preset supplied\n");
	if (AlienfxInit()) {
		if (debug) printf("Debug: Found an AlienFX chip\n");
		if (reboot) afx_reboot();
		if (darkness) afx_reset();
		if (rawmode) {
			afx_raw(rawcmd);
		} else {
			if (command[0]>0) { /*if (usb_is_ready())*/ afx_process(command); }
			else if (!darkness) { if (usb_is_ready()) afx_reset(); }
		}
		AlienfxDeinit();
	} else {
		if (debug) printf("Debug: USB driver could not locate AlienFX chip.  Consult manpage?\n");
		if (debug2) printf("Debug: Use the -P setting to specify the product ID of your AlienFX chip.\n");
		if (debug2) printf("Debug: You may find the product ID by running the 'lsusb' command.\n");
		printf("Error: AlienFX not detected.\n");
		return 1;
	}
	if (debug) printf("Debug: Exiting\n");
	return 0;
} //EOF
