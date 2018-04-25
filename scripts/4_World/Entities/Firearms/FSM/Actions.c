/**@class	WeaponActionBase
 * @brief	represents action executed on transition just between OnExit from old state and OnEntry to new state
 **/
class WeaponActionBase
{
	/**@fn		Action
	 * @brief	executed when transition occurs
	 **/
	void Action (WeaponEventBase e) { }
};

// FSM Actions /////////////////////////////////////////////////////////////////////////////////////////////////////////
// fire
class WeaponActionDryFire extends WeaponActionBase
{
	protected Weapon_Base m_weapon;
	void WeaponActionDryFire (Weapon_Base w = NULL) { m_weapon = w; }

	override void Action (WeaponEventBase e)
	{
		Print("[wpnfsm] action=dry fire");
		int mi = m_weapon.GetCurrentMuzzle();
		m_weapon.DryFire(mi);
	}
};
class WeaponActionDryDryFire extends WeaponActionBase
{
	protected Weapon_Base m_weapon;
	void WeaponActionDryDryFire (Weapon_Base w = NULL) { m_weapon = w; }

	override void Action (WeaponEventBase e)
	{
		Print("[wpnfsm] action=dry dry fire");
		int mi = m_weapon.GetCurrentMuzzle();
		m_weapon.DryDryFire(mi);
	}
};

