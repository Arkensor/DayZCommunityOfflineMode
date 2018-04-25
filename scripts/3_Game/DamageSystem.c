class TotalDamageResult: Managed
{
	proto native float GetDamage(string zoneName, string healthType);
	proto native float GetHighestDamage(string healthType);
};

//-----------------------------------------------------------------------------
class DamageSystem
{
	static proto native void CloseCombatDamage(EntityAI source, Object targetObject, int targetComponentIndex, string ammoTypeName, vector worldPos);

	static proto native void ExplosionDamage(EntityAI source, Object directHitObject, string ammoTypeName, vector worldPos);
}