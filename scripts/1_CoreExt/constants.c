/**
 * \defgroup Constants
 * \desc static script constants
 * @{
 */


/**
 * \defgroup InputDevice
 * \desc constants for input device - inputInterfaceDef.h
 * @{
 */
const int INPUT_MODULE_TYPE_MASK	= 0x00700000;
const int INPUT_KEY_MASK			= 0x000000ff;
const int INPUT_ACTION_TYPE_MASK	= 0x00000f00;
const int INPUT_AXIS				= 0x00010000;
const int INPUT_POV					= 0x00020000;
const int INPUT_COMBO_MASK			= 0xff000000;
const int INPUT_COMBO_AXIS			= 0x00800000;
const int INPUT_COMBO_AXIS_OFFSET	= 0x00000080;
const int INPUT_COMBO_KEY_OFFSET	= 0x01000000;

const int INPUT_DEVICE_KEYBOARD   	= 0x00000000;
const int INPUT_DEVICE_MOUSE      	= 0x00100000; // mouse button
const int INPUT_DEVICE_STICK		= 0x00200000;
const int INPUT_DEVICE_XINPUT     	= 0x00300000; // XInput device
const int INPUT_DEVICE_TRACKIR		= 0x00400000;
const int INPUT_DEVICE_GAMEPAD  	= 0x00500000;
const int INPUT_DEVICE_CHEAT		= 0x00600000;

const int INPUT_ACTION_TYPE_NONE			= 0x00000000;
const int INPUT_ACTION_TYPE_STATE			= 0x00000100;
const int INPUT_ACTION_TYPE_DOWN_EVENT		= 0x00000200;
const int INPUT_ACTION_TYPE_UP_EVENT		= 0x00000300;
const int INPUT_ACTION_TYPE_SHORTCLICK_EVENT= 0x00000400;
const int INPUT_ACTION_TYPE_HOLD_EVENT		= 0x00000500;
		
const int INPUT_ACTION_TYPE_COMBO			= 0x00002000;
const int INPUT_ACTION_TYPE_SPECIALCOMBO	= 0x00004000;
const int INPUT_ACTION_TYPE_DOUBLETAP		= 0x00008000;

const int INPUT_DEVICE_MOUSE_AXIS 		= (INPUT_DEVICE_MOUSE | INPUT_AXIS);
const int INPUT_DEVICE_STICK_AXIS		= (INPUT_DEVICE_STICK | INPUT_AXIS);
const int INPUT_DEVICE_STICK_POV		= (INPUT_DEVICE_STICK | INPUT_POV);
const int INPUT_DEVICE_GAMEPAD_AXIS		= (INPUT_DEVICE_GAMEPAD | INPUT_AXIS);

/** @}*/

/**
 * \defgroup String constants
 * \desc String constants
 * @{
 */
const string STRING_EMPTY = "";
/** @}*/


/**

 * \defgroup Colors
 * @{
 */
const int COLOR_RED = 0xFFF22613;
const int COLOR_GREEN = 0xFF2ECC71;
const int COLOR_BLUE = 0xFF4B77BE;
const int COLOR_YELLOW = 0xFFF7CA18;

const int COLOR_RED_A = 0x1fF22613;
const int COLOR_GREEN_A = 0x1f2ECC71;
const int COLOR_BLUE_A = 0x1f4B77BE;
const int COLOR_YELLOW_A = 0x1fF7CA18;

/** @}*/
	
/**

 * \defgroup Materials
 * @{
 */

/****************************************************************************
 *	MATERIALS LIST
 *	
 *	Note: If you add new materials here, don't forget to add physics
 *	parameters to them in physics/materials.xml
 ***************************************************************************/
const int MATERIAL_DEFAULT			= 0;
const int MATERIAL_METAL			= 1;	//full steel
const int MATERIAL_IRON				= 2;	//iron
const int MATERIAL_GLASS			= 3;	//glass pane
const int MATERIAL_PLASTIC			= 4;	//plastic object
const int MATERIAL_LIQUID			= 5;	//liquids, water
const int MATERIAL_SLIME			= 6;	//slime, oil etc
const int MATERIAL_BETON			= 7;	//concrete
const int MATERIAL_RUBBER			= 8;	//rubber, linoeum
const int MATERIAL_FLESH			= 9;	//flesh, humanoids
const int MATERIAL_GRASS			= 10;	//grass
const int MATERIAL_WOOD				= 11;	//wood
const int MATERIAL_SNOW				= 12;	//snow
const int MATERIAL_SAND				= 13;	//soft sand
const int MATERIAL_DIRT				= 14;	//super-soft dirt
const int MATERIAL_GRAVEL			= 15;	//gravel
const int MATERIAL_STONE			= 16;	//rocks, cliffs

/** @}*/