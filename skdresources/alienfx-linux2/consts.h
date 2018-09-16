/*
  Useful constants and variable maxims when communicating with AlienFX lightchip.
*/

const int SEND_REQUEST_TYPE = 0x21;
const int SEND_REQUEST = 0x09;
const int SEND_VALUE = 0x202;
const int SEND_INDEX = 0x00;
const int READ_REQUEST_TYPE = 0xa1;
const int READ_REQUEST = 0x01;
const int READ_VALUE = 0x101; //for reading existing light state?
const int READ_INDEX = 0x0;
const int SEND_DATA_SIZE = 9;
//const int READ_DATA_SIZE (alienFXid == ALLPOWERFULL_ALIENFX ? 8 : 9) //is this needed?
const int READ_DATA_SIZE = 9;

const int STATE_BUSY = 0x11;
const int STATE_READY = 0x10;
const int STATE_UNKNOWN_COMMAND = 0x12;
const int SUPPORTED_COMMANDS = 15;
const int COMMAND_END_STORAGE = 0x00;// = End Storage block (See storage)
const int COMMAND_SET_COLOUR_MORPH = 0x01;// = Set morph color (See set commands)
const int COMMAND_SET_COLOUR_BLINK = 0x02;// = Set blink color (See set commands)
const int COMMAND_SET_COLOUR_FIXED = 0x03;// = Set color (See set commands)
const int COMMAND_LOOP_BLOCK_END = 0x04;// = Loop Block end (See loops)
const int COMMAND_TRANSMIT_EXECUTE = 0x05;// = End transmition and execute
const int COMMAND_GET_STATUS = 0x06;// = Get device status (see get device status)
const int COMMAND_RESET = 0x07;// = Reset (See reset)
const int COMMAND_SAVE_NEXT = 0x08;// = Save next instruction in storage block (see storage)
const int COMMAND_SAVE = 0x09;// = Save storage data (See storage)
const int COMMAND_UNKNOWN_1 = 0x0A;
const int COMMAND_UNKNOWN_2 = 0x0B;
const int COMMAND_REBOOT_CHIP = 0X0C; //Reboot Chip
const int COMMAND_UNKNOWN_3 = 0x0D;
const int COMMAND_SET_SPEED = 0x0E;// = Set display speed (see set speed)
const int COMMAND_BATTERY_STATE = 0x0F;// = Set battery state (See set commands)
const int RESET_TOUCH_CONTROLS = 0x01;
const int RESET_SLEEP_LIGHTS_ON = 0x02;
const int RESET_ALL_LIGHTS_OFF = 0x03;
const int RESET_ALL_LIGHTS_ON = 0x04;
const int DATA_LENGTH = 9;
const int FILL_BYTE = 0x00;
const int BLOCK_LOAD_ON_BOOT = 0x01; //Blocks used with BATTERY-STATE
const int BLOCK_STANDBY = 0x02;
const int BLOCK_AC_POWER = 0x05;
const int BLOCK_CHARGING = 0x06;
const int BLOCK_BATT_CRITICAL = 0x07;
const int BLOCK_BAT_POWER = 0x08;
const int MAX_SPEED = 6000;
const int MIN_SPEED = 10;
const int STEP_SPEED = 100;
const int ALIENFX_VID = 0x187c;
const int ALIENFX_PID_AURORA_NONALX = 0x513;
const int ALIENFX_PID_ALLPOWERFULL_M11XR3 = 0x522;
const int ALIENFX_PID_ALLPOWERFULL_M11X = 0x514;
const int ALIENFX_PID_ALLPOWERFULL = 0x512;
const int ALIENFX_PID_AREA51 = 0x511;
const int ALIENFX_PID_UNKNOWN = 0x520;
const int NOT_FOUND = 0;
const int START_BYTE_M11XR3 = 0x02;
const int START_BYTE_M11X = 0x02;
const int START_BYTE_NONALX = 0x02;
const int START_BYTE_ALLP = 0x02;
const int START_BYTE_AREA51 = 0x00;

const int DEFAULT_USB_SLEEP = 137500; //usec!

const int MAX_COMMAND = 1024;
