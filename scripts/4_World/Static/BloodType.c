class BloodTypes
{
	static const int NUM_OF_BLOOD_TYPES = 8;
	
	
	
	static int GenerateBloodType()
	{
		int types[NUM_OF_BLOOD_TYPES] 		= {LIQUID_BLOOD_0_P, LIQUID_BLOOD_0_N, LIQUID_BLOOD_A_P, LIQUID_BLOOD_A_N, LIQUID_BLOOD_B_P, LIQUID_BLOOD_B_N, LIQUID_BLOOD_AB_P, LIQUID_BLOOD_AB_N};
		int probability[NUM_OF_BLOOD_TYPES] 	= {42,10,25,5,9,3,2,2};
		
		int probability_sum = 0;
		
		for(int i = 0; i < NUM_OF_BLOOD_TYPES; i++)
		{
			probability_sum += probability[i];
		}
		
		//TIMERDEPRECATED - randomized blodtype for new character
		int probability_randomized = Math.RandomInt(0,probability_sum);
		
		int tmp = 0;
		int index;
		
		for(i = 0; i < NUM_OF_BLOOD_TYPES; i++)
		{
			tmp += probability[i];
			
			if( tmp > probability_randomized )
			{
				index = i;
				break;
			}
		}
		
		return types[index];
	}

	static bool MatchBloodCompatibility(int bloodtypetarget, int bloodtype)
	{
		bool result = false;
		switch ( bloodtype )
		{
			case LIQUID_BLOOD_0_P: 
				if ( bloodtypetarget == LIQUID_BLOOD_0_P || bloodtypetarget == LIQUID_BLOOD_0_N ) result = true;
			break;
			
			case LIQUID_BLOOD_0_N:
				if ( bloodtypetarget == LIQUID_BLOOD_0_N ) result = true;
			break;
			
			case LIQUID_BLOOD_A_P:
				if ( bloodtypetarget == LIQUID_BLOOD_A_P || bloodtypetarget == LIQUID_BLOOD_A_N || bloodtypetarget == LIQUID_BLOOD_0_P || bloodtypetarget == LIQUID_BLOOD_0_N ) result = true;
			break;
			
			case LIQUID_BLOOD_A_N: 
				if ( bloodtypetarget == LIQUID_BLOOD_A_N || bloodtypetarget == LIQUID_BLOOD_0_N ) result = true;
			break;
			
			case LIQUID_BLOOD_B_P: 
				if ( bloodtypetarget == LIQUID_BLOOD_B_P || bloodtypetarget == LIQUID_BLOOD_B_N || bloodtypetarget == LIQUID_BLOOD_0_P || bloodtypetarget == LIQUID_BLOOD_0_N ) result = true;
			break;
			
			case LIQUID_BLOOD_B_N:
				if ( bloodtypetarget == LIQUID_BLOOD_B_N || bloodtypetarget == LIQUID_BLOOD_0_N ) result = true;
			break;
			
			case LIQUID_BLOOD_AB_P:
				if ( bloodtypetarget == LIQUID_BLOOD_AB_P || bloodtypetarget == LIQUID_BLOOD_0_N || bloodtypetarget == LIQUID_BLOOD_0_P || bloodtypetarget == LIQUID_BLOOD_A_N || bloodtypetarget == LIQUID_BLOOD_A_P || bloodtypetarget == LIQUID_BLOOD_B_N || bloodtypetarget == LIQUID_BLOOD_B_P || bloodtypetarget == LIQUID_BLOOD_AB_N ) result = true;
			break;
			
			case LIQUID_BLOOD_AB_N: 
				if ( bloodtypetarget == LIQUID_BLOOD_AB_N || bloodtypetarget == LIQUID_BLOOD_0_N || bloodtypetarget == LIQUID_BLOOD_A_N || bloodtypetarget == LIQUID_BLOOD_B_N ) result = true;
			break;
			
			default: 
				Debug.Log("Incorrect blood type");
			break;
		}
		if ( result )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	static string GetBloodTypeName(int bloodtype)
	{
		string bloodTypeName;
		switch ( bloodtype )
		{
			case LIQUID_BLOOD_0_P: 
				bloodTypeName = "0+";
			break;
			
			case LIQUID_BLOOD_0_N:
				bloodTypeName = "0-";
			break;
			
			case LIQUID_BLOOD_A_P:
				bloodTypeName = "A+";
			break;
			
			case LIQUID_BLOOD_A_N: 
				bloodTypeName = "A-";
			break;
			
			case LIQUID_BLOOD_B_P: 
				bloodTypeName = "B+";
			break;
			
			case LIQUID_BLOOD_B_N:
				bloodTypeName = "B-";
			break;
			
			case LIQUID_BLOOD_AB_P:
				bloodTypeName = "AB+";
			break;
			
			case LIQUID_BLOOD_AB_N:
				bloodTypeName = "AB-";
			break;
			
			default: 
				bloodTypeName = "bad value";
			break;
		}
		
		return bloodTypeName;
	}
	
};