/**@class	CZ61_Base
 * @brief	basic CZ61 Skorpion submachine gun
 **/
class CZ61 : RifleBoltLock_Base
{
	void CZ61()
	{
	}
	
	//some command is different for this weapon
	override int GetWeaponSpecificCommand(int weaponAction ,int subCommand)
	{
		if ( weaponAction == WeaponActions.CHAMBERING)
		{
			switch (subCommand)
			{
				case WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_UNIQUE_CLOSED:
					return WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_OPENED;
				
				default:
					return subCommand;
			}
		
		}
		return subCommand;
	}
};