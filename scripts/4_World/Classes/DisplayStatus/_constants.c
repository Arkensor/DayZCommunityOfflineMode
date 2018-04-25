enum eDisplayElements
{
	DELM_BADGE_STUFFED,
	DELM_BADGE_WET,
	DELM_BADGE_SICK,
	DELM_BADGE_POISONED,
	DELM_BADGE_FRACTURE,
	DELM_BADGE_BLEEDING,
	//------------------
	DELM_TDCY_HEALTH,
	DELM_TDCY_BLOOD,
	DELM_TDCY_TEMPERATURE,
	DELM_TDCY_ENERGY,
	DELM_TDCY_WATER,
	DELM_TDCY_BACTERIA,
	//------------------
	DELM_STANCE,
	COUNT
}


/*
// HUD badges, must be in range 1 - 99
const int	eDisplayElements.DELM_BADGE_STUFFED		= 1;
const int	eDisplayElements.DELM_BADGE_WET			= 2;
const int	eDisplayElements.DELM_BADGE_SICK			= 3;
const int	eDisplayElements.DELM_BADGE_POISONED		= 4;
const int	eDisplayElements.DELM_BADGE_FRACTURE		= 5;
const int	eDisplayElements.DELM_BADGE_BLEEDING		= 6;

// HUD tendencies, must be in range 100 - 199
const int	eDisplayElements.DELM_TDCY_HEALTH		= 100;
const int	eDisplayElements.DELM_TDCY_BLOOD			= 101;
const int	eDisplayElements.DELM_TDCY_TEMPERATURE	= 102;
const int	eDisplayElements.DELM_TDCY_ENERGY 		= 103;
const int	eDisplayElements.DELM_TDCY_WATER			= 104;
const int	eDisplayElements.DELM_TDCY_BACTERIA		= 105;

// HUD notifiers, must be in range 200 - 299
const int	eDisplayElements.DELM_NTFR_BLOOD 		= 200;
const int	eDisplayElements.DELM_NTFR_FULLNESS		= 201;
const int	eDisplayElements.DELM_NTFR_HUNGER		= 202;
const int	eDisplayElements.DELM_NTFR_FEVER			= 203;
const int	eDisplayElements.DELM_NTFR_FRACTURE		= 204;
const int	eDisplayElements.DELM_NTFR_HEALTHY		= 205;
const int	eDisplayElements.DELM_NTFR_SICK			= 206;
const int	eDisplayElements.DELM_NTFR_THIRST		= 207;
const int	eDisplayElements.DELM_NTFR_TEMPERATURE	= 208;
const int	eDisplayElements.DELM_NTFR_WETNESS		= 209;
// HUD other 300 - 399
const int	eDisplayElements.DELM_BAR_STAMINA		= 300;
const int	eDisplayElements.DELM_STANCE		      	= 301;
const int	eDisplayElements.DELM_PROGRESS_BAR		      	= 302;
const int	DELM_SINGLE_USE_ACTION_NAME = 303;
const int	DELM_CONTINUOUS_ACTION_NAME = 304;
const int DELM_SHOW_SINGLE_USE_ACTION = 305;
const int DELM_SHOW_CONTINUOUS_ACTION = 306;
*/

//----------------------------------------------

// levels - notifiers
const int	DELM_LVL_0	 			= 0;
const int	DELM_LVL_1	 			= 1;
const int	DELM_LVL_2	 			= 2;
const int	DELM_LVL_3	 			= 3;
const int	DELM_LVL_4	 			= 4;
const int	DELM_LVL_5	 			= 5;
const int	DELM_LVL_6	 			= 6;

// modes - badges
const int	DELM_LVL_BLINK 			= 1;
const int	DELM_LVL_NOBLINK 		= 2;
//----------------------------------------------


//internal stuff bellow
// types
const int	TYPE_BADGE	 		= 1;
const int	TYPE_TDCY	 		= 2;
const int	TYPE_NTFR	 		= 3;
const int	TYPE_BAR	 		= 4;
const int	TYPE_STANCE	 	= 5;
const int	TYPE_PROGRESS_BAR	 	= 6;
const int	TYPE_SINGLE_USE_ACTION_NAME = 7;
const int	TYPE_CONTINUOUS_ACTION_NAME = 8;
const int TYPE_SHOW_SINGLE_USE_ACTION = 9;
const int TYPE_SHOW_CONTINUOUS_ACTION = 10;

//------------------------------------------------
const int	NTF_HEALTHY 	= 0;
const int	NTF_BLEEDISH 	= 1;
const int	NTF_HUNGRY 		= 2;
const int	NTF_THIRSTY 	= 3;
const int	NTF_STUFFED	 	= 4;
const int	NTF_SICK	 	= 5;
const int	NTF_LEG		 	= 6;	// temp for old system (sqf)
const int	NTF_WETNESS 	= 8;
const int	NTF_WARMTH 		= 9;
const int	NTF_FRACTURE 	= 10;
const int	NTF_FEVERISH 	= 11;
const int	NTF_BLOOD 		= 12;
const int	NTF_LIVES 		= 13;
const int	NTF_STAMINA		= 14;
const int	NTF_COUNT		= 15;
const int	NTF_FRACTURED_LEGS	= 16;
const int	NTF_FRACTURED_ARMS	= 17;
const int	NTF_AGENT_INFECTION	= 18;

//constants related to ui
//key constants
const int	NTFKEY_THIRSTY 	= 1;
const int	NTFKEY_HUNGRY 	= 2;
const int	NTFKEY_WARMTH 	= 3;
const int	NTFKEY_WETNESS 	= 4;
const int	NTFKEY_FRACTURE = 5;
const int	NTFKEY_HEALTHY 	= 6;
const int	NTFKEY_FEVERISH = 7;
const int	NTFKEY_SICK	 	= 8;
const int	NTFKEY_STUFFED	= 9;
const int	NTFKEY_BLEEDISH = 10;
const int	NTFKEY_BLOOD	= 11;
const int	NTFKEY_LIVES	= 12;
const int	NTFKEY_POISONED	= 13;
const int	NTFKEY_BACTERIA	= 13;

//priority constants
const int	NTFP_THIRSTY 	= 1;
const int	NTFP_HUNGRY 	= 2;
const int	NTFP_WARMTH 	= 3;
const int	NTFP_WETNESS 	= 4;
const int	NTFP_FRACTURE 	= 5;
const int	NTFP_HEALTHY 	= 6;
const int	NTFP_FEVERISH 	= 7;
const int	NTFP_SICK	 	= 8;
const int	NTFP_STUFFED	= 9;
const int	NTFP_BLEEDISH 	= 10;

//tendency 
const int	TENDENCY_INC_LOW	= 1;
const int	TENDENCY_INC_MED	= 2;
const int	TENDENCY_INC_HIGH	= 3;
const int	TENDENCY_STABLE 	= 0;
const int	TENDENCY_DEC_LOW 	= -1;
const int	TENDENCY_DEC_MED 	= -2;
const int	TENDENCY_DEC_HIGH	= -3;

// misc
const int STAMINA_UI_RANGE		= 100;

