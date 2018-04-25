class NoiseSystem
{
	proto native void AddNoise(EntityAI source_entity, NoiseParams noise_params, float external_strenght_multiplier = 1.0);
}
	
class NoiseParams
{	
	void NoiseParams()
	{
		
	}
	
	proto native void Load(string noise_name);
	
	proto native void LoadFromPath(string noise_path);
}