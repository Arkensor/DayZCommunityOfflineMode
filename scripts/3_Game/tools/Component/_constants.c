
// ELECTRICITY SYSTEM CONSTANTS

// Plug types
const int PLUG_UNDEFINED 				= 0;
const int PLUG_BATTERY_SLOT 			= 1;
const int PLUG_COMMON_APPLIANCE 		= 2;
const int PLUG_CROCODILE_PINS 			= 3;
const int PLUG_CROCODILE_PINS_CHARGER 	= 4;
const int PLUG_PAS_DEVICE	 			= 5;
const int PLUG_BARBED_WIRE				= 6;
const int PLUG_GAS_CAN_SLOT				= 7;
const int PLUG_VEHICLE_BATTERY_PINS		= 8;

// Main timer for all devices
const int CONSUME_ENERGY_INTERVAL 		= 1;

// Automatic attachment actions
const int NO_ATTACHMENT_ACTION 			= 0;
const int PLUG_THIS_INTO_ATTACHMENT 	= 1;
const int PLUG_ATTACHMENTS_INTO_THIS 	= 2;