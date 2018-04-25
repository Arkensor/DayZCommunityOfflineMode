class ModifierConstants 
{
	const float STOMACH_ENERGY_TRANSFERED_PER_SEC 		= 10;	//amount of kcal transfered to energy per second
	const float STOMACH_WATER_TRANSFERED_PER_SEC 		= 20;	//amount of ml transfered to water per second
	const float STOMACH_SOLID_EMPTIED_PER_SEC 			= 5;	//amount of g/ml emptied from stomach per second
	//const float STOMACH_EMPTIED_LIQUID_PER_SEC	 		= ModifierConstants.WATER_TRANSFERED_PER_SEC;
	const float LOW_WATER_TRESHOLD 						= 250;	//threshold from which water affects health
	const float WATER_DECREMENT_PER_SEC 				= ModifierConstants.STOMACH_WATER_TRANSFERED_PER_SEC / 4; //used in poisoning for now
	const float LOW_ENERGY_TRESHOLD 					= 200;	//threshold from which energy affects health
	//const float ENERGY_DECREMENT_PER_SEC 				= 0.035; //not used?
	//const float DAMAGE_PER_SEC 							= 1; //not used? (was how much HP is lowered while low on energy/water)
	//const float DAMAGE_BLOOD_PER_SEC 					= 10; //not used?
	//--------------------------------------------------------
	const float METABOLIC_SPEED_ENERGY_BASAL	= 0.04;		//energy loss per second while idle
	
	const float METABOLIC_SPEED_ENERGY_WALK		= 0.1; 		//energy loss per second
	const float METABOLIC_SPEED_ENERGY_JOG		= 0.32; 	//energy loss per second
	const float METABOLIC_SPEED_ENERGY_SPRINT	= 0.54; 	//energy loss per second
	
	const float METABOLIC_SPEED_WATER_BASAL		= 0.070; 	//water loss per second while idle
	
	const float METABOLIC_SPEED_WATER_WALK		= 0.22; 	//water loss per second
	const float METABOLIC_SPEED_WATER_JOG		= 0.66; 	//water loss per second
	const float METABOLIC_SPEED_WATER_SPRINT	= 1.10; 	//water loss per second
	//--------------------------------------------------------	
	const float	THRESHOLD_ENERGY_WARNING		= 400;		//yellow tendency icon
	const float	THRESHOLD_ENERGY_CRITICAL		= 200;		//red tendency icon
	const float	THRESHOLD_ENERGY_EMPTY			= 0;		//red blinking tendency icon
	
	const float	THRESHOLD_WATER_WARNING			= 500;		//yellow tendency icon
	const float	THRESHOLD_WATER_CRITICAL		= 250;		//red tendency icon
	const float	THRESHOLD_WATER_EMPTY			= 0;		//red blinking tendency icon
	
	const float	THRESHOLD_BLOOD_WARNING			= 4000;		//yellow tendency icon
	const float	THRESHOLD_BLOOD_CRITICAL		= 3000;		//red tendency icon
	const float	THRESHOLD_BLOOD_EMPTY			= 2750;		//red blinking tendency icon
	
	const float	THRESHOLD_HEALTH_WARNING		= 60;		//yellow tendency icon
	const float	THRESHOLD_HEALTH_CRITICAL		= 40;		//red tendency icon
	const float	THRESHOLD_HEALTH_EMPTY			= 20;		//red blinking tendency icon	
	//--------------------------------------------------------
	const int BLOOD_THRESHOLD_FATAL 				= 2500;	//fatal blood level
	
	const float BLOOD_REGEN_SPEED					= 1; 	//base amount of blood regenerated per second (BLOOD_REGEN_RATE_PER_SEC ?)
	const float DAMAGE_ZONE_BLOOD_REGEN_MODIFIER 	= 1;
	
	const int BLOOD_REGEN_THRESHOLD_ENERGY_LOW		= 200;
	const int BLOOD_REGEN_THRESHOLD_ENERGY_MID		= 400;
	//const int BLOOD_REGEN_THRESHOLD_ENERGY_HIGH		= 2000;
	
	const float BLOOD_REGEN_MODIFIER_ENERGY_LOW		= 0; 	//multiplier for blood regen rate 
	const float BLOOD_REGEN_MODIFIER_ENERGY_MID		= 0.5;
	const float BLOOD_REGEN_MODIFIER_ENERGY_HIGH	= 1;
	
	const int BLOOD_REGEN_THRESHOLD_WATER_LOW		= 250;
	const int BLOOD_REGEN_THRESHOLD_WATER_MID		= 500;
	//const int BLOOD_REGEN_THRESHOLD_WATER_HIGH		= 2000;
	
	const float BLOOD_REGEN_MODIFIER_WATER_LOW		= 0; 	//multiplier for blood regen rate (BLOOD_REGEN_MULTIPLIER_WATER_LOW ?)
	const float BLOOD_REGEN_MODIFIER_WATER_MID		= 0.5;
	const float BLOOD_REGEN_MODIFIER_WATER_HIGH		= 1;
	
	const float SALINE_BLOOD_REGEN_PER_SEC			= 2;	//boost for blood regen per second, independent on BLOOD_REGEN_SPEED
	const float SALINE_LIQUID_AMOUNT				= 250;
	
	const float HEMOLYTIC_BLOOD_DRAIN_PER_SEC		= 2;	//hemolytic reaction blood drain per second
	const float HEMOLYTIC_BLOODLOSS_AMOUNT			= 250;
	//--------------------------------------------------------
	const float LOW_ENERGY_DAMAGE_PER_SEC			= 0.175;	//health loss per second while low on energy
	const float LOW_WATER_DAMAGE_PER_SEC			= 0.225;	//health loss per second while low on water
	
	const float HEALTH_REGEN_MIN					= 0.025;	//health regen rate at BLOOD_THRESHOLD_FATAL blood level
	const float HEALTH_REGEN_MAX					= 0.15;		//health regen rate at MAXIMUM blood level
}