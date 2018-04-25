//WIP
//Very ugly and unfinished script!

/*
Postup vyberu partiklu:
	1- Zisti Surface material
	2- Zisti Impact Type (enter, ricochet, exit, melee)
	3- Zisti Projectile type (bullet alebo item)
	4- Zisti Absorbed force
	
	
	
		Zistime co je surface. Napriklad Hit_Gravel.
		Vstupime do DB efektov pre Hit_Gravel.
		Zistime Impact Type.
		V DB pre Hit_Gravel pristupime do Impact Type. Ak nie je, tak STOP.
		V DB > Hit_Gravel > Impact Type > zistime ci je definovana vynimka pre aktualny projektil. Ak nie, tak pouzi default.
		DB > Hit_Gravel > Impact Type > PROJEKTIL > PlayEffect(id, absorbed_force, ...)
		

*/

enum ImpactTypes
{
	UNKNOWN;
	ENTER;
	EXIT;
	RICOCHET;
	MELEE;
}

class ImpactMaterials
{
	/*
	Hit_Gravel
	Hit_Concrete
	Hit_Dirt
	Hit_Foliage
	Hit_Wood
	Hit_Metal
	Hit_Glass
	*/
	
	/*ref static map<string, Effect> 	m_Materials;
	static int m_LastRegisteredMaterial = 0;
	
	static int CONCRETE = RegisterSurface("Hit_Concrete");
	
	static int RegisterSurface(string surface)
	{
		if (!m_Materials)
			m_Materials = new map<string, Effect>;
		
		m_Materials.Insert(surface, new Effect);
		 
		return ++m_LastRegisteredMaterial;
	}*/
	
	static void EvaluateImpactEffect(string surface, vector pos, int impact_type, vector surfNormal, vector exitPos, vector inSpeed, vector outSpeed, bool deflected, string ammoType)
	{
		// Get all effects for the surface type
		//Effect effect = m_Materials.Get(surface);
		
		// Evaluate impact type (ENTER, EXIT, RICOCHET, MELEE). This section works only for bullets. Melee evaluation is handled in engine.
		if (impact_type == ImpactTypes.UNKNOWN)
			impact_type = ImpactTypes.ENTER;
		
		if (deflected)
			impact_type = ImpactTypes.RICOCHET;
		else if (outSpeed)
			impact_type = ImpactTypes.EXIT;
		
		
		
		
		
		
		float inSpeedf = inSpeed.Length();
		float outSpeedf = outSpeed.Length();
		
		Particle p;
		vector surfNormalAngl1;
		
		if (deflected)
		{
			p = Particle.Play(ParticleList.IMPACT_TEST_RICOCHET, pos);
			surfNormalAngl1 = surfNormal.VectorToAngles();
			surfNormalAngl1 = surfNormalAngl1 + "0 270 0";
			p.SetOrientation(surfNormalAngl1);
			p.SetParameter(0, EmitorParam.VELOCITY, inSpeedf/90);
			
			p = Particle.Play(ParticleList.IMPACT_TEST_ENTER, pos);
			surfNormalAngl1 = inSpeed.VectorToAngles();
			surfNormalAngl1 = surfNormalAngl1 + "0 90 0";
			p.SetOrientation(surfNormalAngl1);
			p.SetParameter(0, EmitorParam.VELOCITY_RND, inSpeedf/30);
			
			if (outSpeedf > 0)
			{
				p = Particle.Play(ParticleList.IMPACT_TEST_EXIT, pos);
				surfNormalAngl1 = outSpeed.VectorToAngles();
				surfNormalAngl1 = surfNormalAngl1 + "0 -90 0";
				p.SetOrientation(surfNormalAngl1);
				p.SetParameter(-1, EmitorParam.VELOCITY_RND, outSpeedf/30);
			}
		}
		else
		{
			EffBulletImpactTest eff = new EffBulletImpactTest();
			//m_eff = new EffBulletImpactTest();
			
			surfNormalAngl1 = inSpeed.VectorToAngles();
			surfNormalAngl1 = surfNormalAngl1 + "0 90 0";
			
			SEffectManager.PlayInWorld( eff, pos, surfNormalAngl1 );
			p = eff.GetParticle();
			p.SetParameter(0, EmitorParam.VELOCITY_RND, inSpeedf/30);
			
			if (outSpeedf > 0)
			{
				p = Particle.Play(ParticleList.IMPACT_TEST_EXIT, exitPos);
				surfNormalAngl1 = outSpeed.VectorToAngles();
				surfNormalAngl1 = surfNormalAngl1 + "0 -90 0";
				p.SetOrientation(surfNormalAngl1);
				p.SetParameter(-1, EmitorParam.VELOCITY_RND, outSpeedf/30);
			}
		}
	}
}

class ImpactEffectBase
{
	void ImpactEffectBase()
	{
		
	}
	
	

	//============================
	//        IMPACT TYPES        
	//============================
	
	void OnImpactUniversal()
	{
		
	}
	
	void OnImpactEnter()
	{
		OnImpactUniversal();
	}
	
	void OnImpactExit()
	{
		OnImpactUniversal();
	}
	
	void OnImpactRicochet()
	{
		OnImpactUniversal();
	}
	
	void OnImpactMelee()
	{
		OnImpactUniversal();
	}
}

class ImpactEffectConcrete : ImpactEffectBase
{
	
}