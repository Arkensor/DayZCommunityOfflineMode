 /**
 * \defgroup Various timers
 * \desc constants for various timers
 * @{
 */ 
const int DISPLAY_STATUS_UPDATE_INTERVAL = 1000;
/** @}*/

/**
 * \defgroup UI
 * \desc constants User Interface
 * @{
 */
const int IDC_OK								= 1;
const int IDC_CANCEL						= 2;
const int IDC_RESTART						= 5;

const int IDC_MAIN_NEXT_CHARACTER	= 50;
const int IDC_MAIN_PREV_CHARACTER	= 51;

const int IDC_MAIN_OPTIONS      = 102;
const int IDC_MAIN_MULTIPLAYER  = 105;
const int IDC_MAIN_QUIT         = 106;
const int IDC_MAIN_CONTINUE			= 114;
const int IDC_MAIN_PLAY         = 142;
const int IDC_MAIN_CHARACTER		= 143;
const int IDC_MULTI_REFRESH			= 123;

const int IDC_BOOK_VIEWER_PREV  = 102;
const int IDC_BOOK_VIEWER_NEXT  = 103;

//! ingame menu
const int IDC_INT_RETRY					= 105;
const int IDC_INT_EXIT					= 107;
/** @}*/

/**
 * \defgroup MenuID
 * \desc constants for menu pages
 * @{
 */
const int MENU_ANY									= 1;
const int MENU_NONE									= 1 << 1;
const int MENU_UNKNOWN								= 1 << 2;
const int MENU_ASSIGNMENT							= 1 << 3;
const int MENU_CHARACTER							= 1 << 4;
const int MENU_CHAT									= 1 << 5;
const int MENU_EARLYACCESS							= 1 << 6;
const int MENU_INGAME								= 1 << 7;
const int MENU_INSPECT								= 1 << 8;
const int MENU_INVENTORY							= 1 << 9;
const int MENU_LOADING								= 1 << 10;
const int MENU_MAIN									= 1 << 11;
const int MENU_OPTIONS								= 1 << 12;
const int MENU_SCENE_EDITOR							= 1 << 13;
const int MENU_SCRIPTCONSOLE						= 1 << 14;
const int MENU_STARTUP								= 1 << 15;
const int MENU_CHAT_INPUT							= 1 << 16;
const int MENU_SCRIPTCONSOLE_DIALOG_PRESET_NAME		= 1 << 17;
const int MENU_SCRIPTCONSOLE_DIALOG_PRESET_RENAME 	= 1 << 18;
const int MENU_CONTROLS_PRESET						= 1 << 19;
const int MENU_NOTE									= 1 << 20;
const int MENU_MAP									= 1 << 21;
const int MENU_BOOK									= 1 << 22;
const int MENU_HELP_SCREEN      					= 1 << 23;
const int MENU_GESTURES		      					= 1 << 24;
const int MENU_LOGOUT 								= 1 << 25;
const int MENU_TITLE_SCREEN							= 1 << 26;
const int MENU_XBOX_CONTROLS						= 1 << 27;
const int MENU_RADIAL_QUICKBAR						= 1 << 28;
const int MENU_INGAME_XBOX							= 1 << 38;

const int GUI_WINDOW_MISSION_LOADER = 1;

const string CFG_VEHICLESPATH = "CfgVehicles";
const string CFG_WEAPONSPATH = "CfgWeapons";
const string CFG_MAGAZINESPATH = "CfgMagazines";
const string CFG_RECIPESPATH = "CfgRecipes";

/** @}*/

/**
 * \defgroup Local data saving to files
 * \desc constants for saving local datas
 * @{
 */
const int		CFG_ARRAY_ITEMS_MAX		= 64;
const string	CFG_FILE_FIXED_PROFILE	= "Scripts/profile_fixed.cfg";
const string	CFG_FILE_USER_PROFILE	= "$profile:profile.cfg";
const string	CFG_FILE_DEBUG_PROFILE	= "$profile:debugProfile.cfg";
const string	CFG_FILE_EMOTES_PROFILE	= "$profile:emotesProfile.cfg";
const string	CFG_FILE_SEARCH_HISTORY	= "$profile:search_history.history";
const string	CFG_FILE_ENS_HISTORY	= "$profile:script_enscript.history";
const string	CFG_FILE_SCRIPT_LOG		= "$profile:script.log";
const string	CFG_FILE_SCRIPT_LOG_EXT	= "$profile:scriptExt.log";
const string	CFG_FILE_ADDITIONAL_INFO= "$profile:serverInfo.cfg";
/** @}*/

/**
 * \defgroup RPC commands
 * \desc Constants for sending RPC commands on server
 * @{
 */
/*
const int RPC_SYNC_ITEM_VAR                     = 1;
const int RPC_SYNC_STAT                         = 2;
const int RPC_WRITE_NOTE                        = 3;
const int RPC_SYNC_DISPLAY_STATUS               = 4;
const int RPC_RECIPE_SEND    					= 7;
const int RPC_ON_SET_CAPTIVE					= 8;
const int RPC_SYNC_SCENE_OBJECT					= 9;
const int RPC_READ_A_BOOK               		= 10;	
const int RPC_USER_ACTION_PROMPT_SYNCH			= 11;
const int RPC_USER_ACTION_MESSAGE 				= 12;
const int RPC_ITEM_DIAG							= 13;
const int RPC_ITEM_DIAG_CLOSE					= 14;
const int RPC_SET_OBJECT_POSITION 				= 15;
const int RPC_USER_ACTION_MESSAGES 				= 16;
const int RPC_ITEM_SPLIT						= 17;
const int RPC_ITEM_COMBINE						= 18;
const int RPC_PLAYER_STATES_ON					= 19;
const int RPC_PLAYER_STATES_OFF					= 20;
const int RPC_PLACING_DEBUG						= 21;
const int RPC_PLACING_START						= 22;
const int RPC_PLACING_STOP						= 23;
const int RPC_STAMINA							= 25;
const int RPC_DAMAGE_VALUE_SYNC					= 26;
const int RPC_USER_ACTION_PROMPT_PROGRESS		= 27;
const int RPC_PLACING_UPDATE					= 28;
const int RPC_USER_ACTION_PROMPT_CLEAN			= 29;
const int RPC_DISABLE_MODIFIERS					= 30;
const int RPC_KILL_PLAYER						= 31;
const int RPC_ENABLE_INVINCIBILITY				= 32;
const int RPC_ITEM_DEBUG_ACTIONS				= 33;
const int RPC_LOG_PLAYER_STATS					= 34;
const int RPC_EM_IS_PLUGGED						= 35;
const int RPC_EM_IS_UNPLUGGED					= 36;
*/
/** @}*/


/**
 * \defgroup Developer RPC commands
 * \desc Constants for sending developer RPC commands on server
 * @{
 */
/*
const int DEV_RPC_SPAWN_GROUND_ITEM             = 101;
const int DEV_RPC_CLEAR_INV                     = 102;
const int DEV_RPC_TELEPORT                      = 103;
const int DEV_RPC_SET_PLAYER_DIRECTION          = 104;
const int DEV_RPC_SEND_SERVER_LOG               = 105;
const int DEV_RPC_TOGGLE_GOD_MODE               = 106;
const int DEV_RPC_STATS_DATA					= 107;
const int DEV_RPC_MODS_DATA						= 108;
const int DEV_RPC_AGENTS_DATA					= 109;
const int DEV_RPC_CREATE_SCENE_OBJECT 			= 110;
const int DEV_RPC_SCENE_LOAD 					= 111;
const int DEV_RPC_SPAWN_INV_ITEM                = 112;
const int DEV_RPC_SPAWN_ATTACHMENT_ITEM         = 113;
const int DEV_RPC_SPAWN_ITEM_ON_CURSOR          = 114;
*/
/** @}*/

/**
 * \defgroup Sync update commands
 * \desc Constants for synchronization update commands
 * @{
 */
	/*
const int DEV_STATS_UPDATE						= 1;
const int DEV_MODS_UPDATE						= 2;
const int DEV_AGENTS_UPDATE						= 3;
	*/
/** @}*/

/**
 * \defgroup Message commands
 * \desc Messaging System - the numbers must be 0 or higher, and the highest number should not be bigger than NUM_OF_CHANNELS-1
 * @{
 */
const int NUM_OF_CHANNELS 		= 100;
const int MSG_TEST_CHANNEL		= 0;
const int MSG_IMMUNE_REACTION	= 1;
const int MSG_STATS_SYNCED		= 2;
const int MSG_NOTIFIERS_TICK	= 3;
/** @}*/
	
/**
 * \defgroup Modifier pools minimum tick times in ms
 * \desc Modifier pools minimum tick times
 * @{
 */
const int MIN_TICK_MDFR_POOL_MAIN = 10000;//in ms
const int MIN_TICK_MDFR_POOL_PLAYER = 3000;
const int MIN_TICK_MDFR_POOL_ENVIRONMENT = 3000;
const int MIN_TICK_MDFR_POOL_DISEASE = 30000;
const int MIN_TICK_NOTIFIERS = 1000;
/** @}*/

/**
 * \defgroup Emote ids
 * \desc These ids are stored in emotesProfile.cfg and defines which emote will be played after pressing emote shorcut keys
 * @{
 */
/*
const int ID_EMOTE_LYINGDOWN	= 1;
const int ID_EMOTE_DABBING 		= 2;
const int ID_EMOTE_THUMB		= 3;
const int ID_EMOTE_TAUNT 		= 4;
const int ID_EMOTE_TIMEOUT 		= 5;
const int ID_EMOTE_HEART 		= 6;
const int ID_EMOTE_FACEPALM 	= 7;
const int ID_EMOTE_CLAP 		= 8;
const int ID_EMOTE_SILENT 		= 9;
const int ID_EMOTE_GREETING 	= 10;
const int ID_EMOTE_SALUTE 		= 11;
const int ID_EMOTE_SOS 			= 12;
const int ID_EMOTE_DANCE 		= 13;
const int ID_EMOTE_RPS 			= 14;
/** @}
*/
	const int ID_EMOTE_GREETING			= 1;
	const int ID_EMOTE_SOS				= 2; //FB
	const int ID_EMOTE_HEART 			= 3;
	const int ID_EMOTE_TAUNT	 		= 4;
	const int ID_EMOTE_LYINGDOWN		= 5;
	const int ID_EMOTE_TAUNTKISS		= 6;
	const int ID_EMOTE_FACEPALM			= 7;
	const int ID_EMOTE_TAUNTELBOW		= 8;
	const int ID_EMOTE_THUMB			= 9;
	const int ID_EMOTE_THROAT		 	= 10;
	const int ID_EMOTE_SUICIDE			= 11; //FB
	const int ID_EMOTE_DANCE 		= 12;

const int ID_EMOTE_DABBING 		= 32;
const int ID_EMOTE_SITA 		= 33; //FB
const int ID_EMOTE_TIMEOUT 		= 35;
const int ID_EMOTE_CLAP 		= 39;
const int ID_EMOTE_POINT 		= 40;
const int ID_EMOTE_SILENT 		= 43;
const int ID_EMOTE_SALUTE 		= 44;
const int ID_EMOTE_RPS 			= 45;
/**
 * \defgroup Configurations for Environment class
 * \desc Configurations for Environment class
 * @{
 */
const float ENVIRO_TICK_RATE = 2; //in secs. how often should enviro effet process
const float	ENVIRO_TICKS_TO_WETNESS_CALCULATION = 30; // each X (ticks) is processed wetness on items on player
const float ENVIRO_WET_PENALTY_EFFECT = 5;//how much wetness of clothes affects players heatcomfort
const float ENVIRO_WET_INCREMENT = 0.01; // amount of wetness added to items wet value each tick if is raining
const float ENVIRO_DRY_INCREMENT = 0.0005; // amount of wetness subtracted from items wet value each tick if is not raining due to player heat
const float ENVIRO_SUN_INCREMENT = 0.002; // amount of wetness subtracted from items wet value each tick if is not raining due to sun
const float ENVIRO_CLOUD_DRY_EFFECT = 0.1; // how many % of ENVIRO_SUN_INCREMENT is reduced by cloudy sky
const float ENVIRO_CLOUDS_TEMP_EFFECT = 0.1; // how many % of environment temperature can be lowered by clouds
const float ENVIRO_FOG_TEMP_EFFECT = 0.2; // how many % of environment temperature can be lowered by fog
const float ENVIRO_WET_PENALTY = 0.5; //at which state of item wetness (0-1) will heat isolation start having negative effect on heat comfort of item
const float ENVIRO_WET_PASSTHROUGH_KOEF = 0.1; //how many times slower is wetting/drying items in backpacks
const float ENVIRO_ITEM_HEAT_TRANSFER_KOEF = 0.01; //converts temperature of items to entities heatcomfort gain
const float ENVIRO_WATER_TEMPERATURE_KOEF = 0.5; //how many time is water colder than air
const float ENVIRO_DEFAULT_ENTITY_HEAT = 3; //heat entity generates if not moving
const float ENVIRO_TEMPERATURE_HEIGHT_REDUCTION = 0.0065; //amount of ?C reduced for each 100 meteres of height above water level
const float ENVIRO_WIND_EFFECT = 1; //amount of % wind affect drying/wetting
const float ENVIRO_HIGH_NOON = 12; //when is sun highest on sky
const float	ENVIRO_HEAT_COMFORT_MAX = 210;
/** @}*/

/**
 * \defgroup Configurations for ItemsGenerator class
 * \desc Configurations for ItemsGenerator class
 * @{
 */
const float		ITEMSGEN_TICK = 10; //in seconds
const float		ITEMSGEN_MIN_DISTANCE_TO_REPOSITION = 80; //in meters
const float		ITEMSGEN_SPAWN_DISTANCE = 60; //in meters
const float		ITEMSGEN_MAX_SPREAD = 15; //in meters
const int		ITEMSGEN_ITEMS_AMOUNT = 5;
/** @}*/

/**
 * \defgroup Configurations for StaminaHandler class
 * \desc Configurations for StaminaHandler class
 * @{
 */
	// unit = currently percent (stamina max is 100)
const int 	STAMINA_DRAIN_STANDING_SPRINT_PER_SEC = 5; //in units (how much sprint depletes stamina)
const int 	STAMINA_DRAIN_CROUCHED_SPRINT_PER_SEC = 1; //in units (how much sprint in crouch depletes stamina)
const int 	STAMINA_DRAIN_PRONE_SPRINT_PER_SEC = 3; //in units (how much sprint in prone depletes stamina)
const float	STAMINA_DRAIN_HOLD_BREATH = 0.2; //in units (how much holding breath depletes stamina)
const int	STAMINA_DRAIN_MELEE_LIGHT = 5; //in units (how much light melee punch depletes stamina)
const float	STAMINA_DRAIN_MELEE_HEAVY = 20; //in units (how much heavy melee punch depletes stamina)
const int	STAMINA_DRAIN_MELEE_EVADE = 8; // in units (how much evade depletes stamina)
	
const int 	STAMINA_GAIN_JOG_PER_SEC = 2; //in units (how much of stamina units is gained while jogging)
const int 	STAMINA_GAIN_WALK_PER_SEC = 4; //in units (how much of stamina unitsis gained while walking)
const int 	STAMINA_GAIN_IDLE_PER_SEC = 1; //in units (how much of stamina unitsis gained while iddling)
const float STAMINA_GAIN_BONUS_CAP = 3.0; //in units (tells how much extra units can be added at best to stamina regain)

const float STAMINA_KG_TO_STAMINAPERCENT_PENALTY = 2.5; //in units (by how many  units is max stamina bar reduced for each 1 kg of load weight)
const float STAMINA_MIN_CAP = 5; //in units (overload won't reduce max stamina bar under this value)
const float STAMINA_SPRINT_THRESHOLD = 25; //in units (how many units of stamina you need regained in order to be able to start sprinting)
const float STAMINA_HOLD_BREATH_THRESHOLD = 10; // in units
const float STAMINA_MELEE_HEAVY_THRESHOLD = STAMINA_DRAIN_MELEE_HEAVY; // in units (how many units we need to make a heavy hit in melee)
const float STAMINA_MELEE_EVADE_THRESHOLD = 8; // in units
const float STAMINA_REGEN_COOLDOWN_DEPLETION = 0.45; // in secs (how much time we will spend in cooldown before the stamina will starts with regeneration)
const float STAMINA_REGEN_COOLDOWN_EXHAUSTION = 0.5;
const float STAMINA_KG_TO_GRAMS = 1000; //for kg to g conversion
const float STAMINA_SYNC_RATE = 1; //in secs
const float STAMINA_MAX = 100;
/** @}*/


/**
 * \defgroup Selectable action types ids
 * \desc These ids helsp diferentiate types of selectable actions 
 * @{
 */
const int SAT_INTERACT_ACTION	= 1;
const int SAT_CRAFTING	 		= 2;
const int SAT_DEBUG_ACTION		= 3;
/** @}*/

/**
 * \defgroup User action internal state machine states
 * \desc User action internal state machine states
 * @{
 */
const int 		UA_NONE = 0;
const int 		UA_FAILED = 1;
const int 		UA_PROCESSING = 2;
const int 		UA_REPEAT = 3;
const int 		UA_FINISHED = 4;
const int		UA_CANCEL = 5;
const int		UA_INTERRUPT = 6;
const int       UA_START = 7;
const int       UA_STARTT = 8;
const int       UA_CANCELT = 9;
const int       UA_FINISHEDT = 10;
const int    	UA_ANIM_EVENT = 11;
const int 		UA_INITIALIZE = 12;
const int		UA_CHECK_CON = 13;
const int		UA_AM_PENDING = 14;
const int		UA_AM_ACCEPTED = 15;
const int		UA_AM_REJECTED = 16;
	
const int		UA_ERROR = 24;
	
const int		UA_SETEND_2 = 32;
/** @}*/


/**
 * \defgroup Barel related functions constants
 * \desc Barel related functions constants
 * @{
 */
const int BAREL_LIME_PER_PELT = 100; //grams per pelt 
const int BAREL_BLEACH_PER_CLOTH = 50; //ml per item
const float BAREL_LIME_PER_PLANT = 50; //grams per gram
/** @}*/



/**

 * \defgroup Agent transmission system
 * \desc Agent transmission system
 * @{
 */
const int AGT_INV_IN	 		= 1;
const int AGT_INV_OUT	 		= 2;
const int AGT_UACTION_CONSUME	= 3;
const int AGT_TRANSFER_COPY	= 4;
const int AGT_UACTION_TOUCH		= 5;
const int AGT_WATER_POND		= 6;
const int AGT_AIRBOURNE			= 7;
	
const int DEF_BIOLOGICAL		= 1;
//agent list
const int AGT_CHOLERA 			= 1;
const int AGT_INFLUENZA 		= 2;
const int AGT_SALMONELLA		= 4;
const int AGT_BRAIN 			= 8;


/** @}*/
			
const int QUANTITY_HIDDEN = 0;
const int QUANTITY_COUNT = 1;
const int QUANTITY_PROGRESS = 2;


			
/**
 * \defgroup LiquidTypes
 * \desc Constants for liquid types
 * @{
 */
// BEWARE ALL INDIVIDUAL LIQUID TYPES ARE ALSO REPRESENTED CONFIG-SIDE AND MUST MATCH(all changes must be made on both sides)
// NOTE ANY NUMBER HERE MUST BE A 0 OR ANY POWER OF TWO, THERE IS A MAXIMUM OF 32 INDIVIDUAL LIQUID TYPES POSSIBLE
const int LIQUID_BLOOD_0_P	= 1;
const int LIQUID_BLOOD_0_N	= 2;
const int LIQUID_BLOOD_A_P	= 4;
const int LIQUID_BLOOD_A_N	= 8;
const int LIQUID_BLOOD_B_P	= 16;
const int LIQUID_BLOOD_B_N	= 32;
const int LIQUID_BLOOD_AB_P	= 64;
const int LIQUID_BLOOD_AB_N = 128;
const int LIQUID_SALINE 	= 256;

const int LIQUID_WATER = 512;
const int LIQUID_RIVERWATER = 1024;
const int LIQUID_VODKA = 2048;
const int LIQUID_BEER = 4096;
const int LIQUID_GASOLINE = 8192;
const int LIQUID_DIESEL = 16384;
const int LIQUID_DISINFECTANT = 32768;

// these are groups which do not have to correspond with configs
const int GROUP_LIQUID_BLOOD = 255;
const int GROUP_LIQUID_ALL = -1;//-1 = all bits to 1
/** @}*/
	
/**
 * \defgroup ChatMessagesVisibility
 * \desc Constants for toggle chat messages type visibility
 * @{
 */
const string SYSTEM_CHAT_MSG 	= "system_chat_msg";
const string GLOBAL_CHAT_MSG 	= "global_chat_msg";
const string DIRECT_CHAT_MSG	= "direct_chat_msg";
const string VEHICLE_CHAT_MSG 	= "vehicle_chat_msg";
const string RADIO_CHAT_MSG 	= "radio_chat_msg";	
/** @}*/

/**
 * \defgroup HUDVisibility
 * \desc Constants for toggling of HUD visibility
 * @{
 */
const string SHOW_QUICKBAR 		= "show_quickbar";
const string SHOW_HUD 			= "show_hud";
//const string SHOW_HUD_AUTOHIDE 	= "hud_autohide";
const string SHOW_CROSSHAIR 	= "show_crosshair";
/** @}*/

/**
 * \defgroup Controls locks
 * \desc Constants for toggling of alternate 
 * @{
 */
const string LOCK_SPRINT 	= "lock_sprint";
const string LOCK_FREELOOK 	= "lock_freelook";
const string LOCK_ZOOM 		= "lock_zoom";
/** @}*/

/**
 * \defgroup Item Variables
 * \desc Constants for Item Variables
 * @{
 */
const int VARIABLE_QUANTITY 	= 1;
const int VARIABLE_ENERGY 		= 2;
const int VARIABLE_TEMPERATURE 	= 4;
const int VARIABLE_WET 			= 8;
const int VARIABLE_LIQUIDTYPE	= 16;
const int VARIABLE_ISLIT		= 32;
const int VARIABLE_COLOR		= 64;
/** @}*/

const float CRAFTING_TIME_UNIT_SIZE = 4.0;// time unit size for crafting, this value is multiplied by the length given by a recipe

const float PROJECTED_CURSOR_DISTANCE = 5;//how long is the raycast from the weapon for projected cursor

/**
 * \defgroup FOV settings
 * \desc Constats for setting of DayZPlayerCamera FOV
 */
const float DZPLAYER_CAMERA_FOV_EYEZOOM		= 0.3926;	// 45deg (H) / 22.5deg (V)
const float DZPLAYER_CAMERA_FOV_IRONSIGHTS	= 0.5236;	// 60deg (H) / 30deg (V)