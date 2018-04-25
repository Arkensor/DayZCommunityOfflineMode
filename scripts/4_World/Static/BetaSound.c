class BetaSound
{
	static SoundOnVehicle SaySound(Object source, string  sound_name, float distance, bool looped)  
	{
		bool is_female = source.ConfigGetBool("woman");
		
		
		string path_class = "CfgActionSounds " + sound_name;
		string path_sound = path_class + " sounds";
		//is the action sound class defined ? if not, try to play it as a regular sound
		if( GetDayZGame().ConfigIsExisting(path_sound) )
		{
			string path_sound_female = path_class + "_female" + " sounds";
			
			if(is_female && GetDayZGame().ConfigIsExisting(path_sound_female))
			{
				path_sound = path_sound_female;
			}
			
			GetGame().ConfigGetTextArray(path_sound,CashedObjectsArrays.ARRAY_STRING);
			int rnd_index = Math.RandomInt(0,CashedObjectsArrays.ARRAY_STRING.Count());
			string sound_class = CashedObjectsArrays.ARRAY_STRING.Get(rnd_index);
			SoundOnVehicle sound_object = GetGame().CreateSoundOnObject(source, sound_class, distance, looped);
			return sound_object;
		}
		else
		{
			return GetGame().CreateSoundOnObject(source, sound_name, distance, looped);
		}
	}

};
