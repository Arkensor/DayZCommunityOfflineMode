class DayZInfectedAttackType
{
	float m_Distance; // [m]
	int m_Pitch; // 1 = up, 0 = center, -1 = down
	int m_Type; // int argument for attack command
	float m_Subtype; // float argument for attack command
	string m_AmmoType; // ammotype from config
	float m_Cooldown; // [s]
	float m_Probability; // [0..1]
};

enum DayZInfectedAttackGroupType
{
	CHASE,
	FIGHT
}

class DayZInfectedType extends DayZCreatureAIType
{
	//--------------------------------------------------------
	// Public
	//--------------------------------------------------------

	void RegisterAttacks()
	{
		//! chase group
		m_ChaseAttacksGroup = new array<ref DayZInfectedAttackType>;
		
		RegisterAttack(DayZInfectedAttackGroupType.CHASE, 1.0,  0, 4, 1, "MeleeZombieMale", 1.0, 1.0); // Attack short center
		RegisterAttack(DayZInfectedAttackGroupType.CHASE, 1.0,  0, 1, 1, "MeleeZombieMale", 1.0, 0.3); // Attack short combo center
		RegisterAttack(DayZInfectedAttackGroupType.CHASE, 3.0, 0, 0, 1, "MeleeZombieMale_Heavy", 3.0, 0.2); // Attack long
		
		//! fight group
		m_FightAttacksGroup = new array<ref DayZInfectedAttackType>;
		
		RegisterAttack(DayZInfectedAttackGroupType.FIGHT, 1.0,  1, 4, 0, "MeleeZombieMale", 1.0, 1.0); // Attack short up
		RegisterAttack(DayZInfectedAttackGroupType.FIGHT, 1.0,  0, 4, 1, "MeleeZombieMale", 1.0, 1.0); // Attack short center
		RegisterAttack(DayZInfectedAttackGroupType.FIGHT, 1.0, -1, 4, 2, "MeleeZombieMale", 1.0, 1.0); // Attack short down
		
		RegisterAttack(DayZInfectedAttackGroupType.FIGHT, 1.0,  1, 1, 0, "MeleeZombieMale", 1.0, 0.3); // Attack short combo up
		RegisterAttack(DayZInfectedAttackGroupType.FIGHT, 1.0,  0, 1, 1, "MeleeZombieMale", 1.0, 0.3); // Attack short combo center
		RegisterAttack(DayZInfectedAttackGroupType.FIGHT, 1.0, -1, 1, 2, "MeleeZombieMale", 1.0, 0.3); // Attack short combo down
		
		RegisterAttack(DayZInfectedAttackGroupType.FIGHT, 2.0,  0, 3, 1, "MeleeZombieMale", 2.0, 1.0); // Attack medium center
		RegisterAttack(DayZInfectedAttackGroupType.FIGHT, 2.0, -1, 3, 2, "MeleeZombieMale", 2.0, 1.0); // Attack medium down
		
		RegisterAttack(DayZInfectedAttackGroupType.FIGHT, 3.0, 0, 2, 1, "MeleeZombieMale_Heavy", 3.0, 0.2); // Attack long
		RegisterAttack(DayZInfectedAttackGroupType.FIGHT, 3.0, 0, 0, 1, "MeleeZombieMale_Heavy", 3.0, 0.2); // Attack long combo
	}
	
	DayZInfectedAttackType ChooseAttack(DayZInfectedAttackGroupType pAttackGroupType, float pDistance, int pPitch)
	{
		ref array<ref DayZInfectedAttackType> attackGroup = GetAttackGroup(pAttackGroupType);
		float rnd = Math.RandomFloat01();
		float highestUtility = -1;
		DayZInfectedAttackType mostSuitableAttack = NULL;
		
		for( int i = 0; i < attackGroup.Count(); ++i )
		{
			DayZInfectedAttackType attackType = attackGroup.Get(i);
			float utility = ComputeAttackUtility(attackType, pDistance, pPitch, rnd);
			if( utility <= 0 )
				continue;
			
			if( utility > highestUtility )
			{
				mostSuitableAttack = attackType;
				highestUtility = utility;
			}
		}
		
		return mostSuitableAttack;
	}
	
	//--------------------------------------------------------
	// Protected
	//--------------------------------------------------------
	
	protected void RegisterAttack(DayZInfectedAttackGroupType pAttackGroupType, float pDistance, int pPitch, int pType, float pSubtype, string pAmmoType, float pCooldown, float pProbability)
	{
		DayZInfectedAttackType newType = new DayZInfectedAttackType();
		
		newType.m_Distance = pDistance;
		newType.m_Pitch = pPitch;
		newType.m_Type = pType;
		newType.m_Subtype = pSubtype;
		newType.m_AmmoType = pAmmoType;
		newType.m_Cooldown = pCooldown;
		newType.m_Probability = pProbability;
		
		GetAttackGroup(pAttackGroupType).Insert(newType);
	}
	
	protected float ComputeAttackUtility(DayZInfectedAttackType pAttackType, float pTargetDistance, int pPitch, float pProbability)
	{
		// pitch
		if( pAttackType.m_Pitch != pPitch )
			return 0;

		// distance
		float distDiff = pAttackType.m_Distance - pTargetDistance;
		if( distDiff < 0 )
			return 0;
		float distDiffFrac = distDiff / 10;		
		float utilityDistance = (1 - distDiffFrac) * 100; // distance is most important
		
		// probability
		float probDiff = pAttackType.m_Probability - pProbability;
		if( probDiff < 0 )
			return 0;
		float utilityProbability = (1 - probDiff) * 10; // distance is most important
		
		// total
		return utilityDistance + utilityProbability;
	}

	ref array<ref DayZInfectedAttackType> GetAttackGroup(DayZInfectedAttackGroupType pType)
	{
		switch( pType )
		{
		case DayZInfectedAttackGroupType.CHASE:
			return m_ChaseAttacksGroup;
			
		case DayZInfectedAttackGroupType.FIGHT:
			return m_FightAttacksGroup;
		}
		
		return NULL;
	}
	
	//--------------------------------------------------------
	// Members
	//--------------------------------------------------------
	
	//! selected & sorted targets by utility function
	private ref array<ref DayZInfectedAttackType> m_ChaseAttacksGroup;
	private ref array<ref DayZInfectedAttackType> m_FightAttacksGroup;
}