/**@class		Weapon
 * @brief		script counterpart to engine's class Weapon
 **/
class Weapon extends InventoryItemSuper
{
	override bool IsWeapon () { return true; }

	/**@fn		GetMuzzleCount
	 * @return	number of muzzles
	 **/
	proto native int GetMuzzleCount ();
	/**@fn		GetMuzzleIndexFromMagazineSlot
	 * @brief 	conversion slotId -> muzzleIndex
	 * @param[in] magazineSlotId		slotId of the magazine slot
	 * @return	muzzleIndex on success, -1 otherwise
	 **/
	//proto native int GetMuzzleIndexFromMagazineSlot (int magazineSlotId);
	/**@fn		GetSlotFromMuzzleIndex
	 * @brief 	conversion muzzleIndex -> slotId
	 * @param[in] muzzleIndex		muzzle index
	 * @return	slotId on success, -1 otherwise
	 **/
	proto native int GetSlotFromMuzzleIndex (int muzzleIndex);
	/**@fn		GetCurrentMuzzle
	 * @return	returns index of current muzzle
	 **/
	proto native int GetCurrentMuzzle ();
	/**@fn		SetCurrentMuzzle
	 * @brief 	sets index of active muzzle
	 * @param[in] muzzleIndex
	 **/
	proto native void SetCurrentMuzzle (int muzzleIndex);

	/**@fn		CanFire
	 * @brief 	query whether the weapon can be fired or not
	 * @return	true if fire possible
	 **/
	proto native bool CanFire (int muzzleIndex);
	proto native bool Fire (int muzzleIndex, vector pos, vector dir, vector speed);
	proto native bool DryFire (int muzzleIndex);
	proto native bool DryDryFire (int muzzleIndex);
	/**@fn		EjectCasing
	 * @brief	ejects casing from chamber
	 **/
	proto native bool EjectCasing (int muzzleIndex);
	
	proto native int GetMuzzleModeCount (int muzzleIndex);
	proto native void SetMuzzleMode (int muzzleIndex, int modeIndex);
	proto native void SetNextMuzzleMode (int muzzleIndex);
	proto native int GetCurrentModeBurstSize (int muzzleIndex);
	proto native bool GetCurrentModeAutoFire (int muzzleIndex);
	/**@fn		IsChamberEmpty
	 * @brief	detects empty chamber
	 * @returns true if no cartridge in chamber
	 **/
	proto native bool IsChamberEmpty (int muzzleIndex);
	/**@fn		IsChamberFiredOut
	 * @brief	detects presence of fired out casing in chamber
	 * @returns true if casing in chamber
	 **/
	proto native bool IsChamberFiredOut (int muzzleIndex);
	/**@fn		IsChamberJammed
	 * @brief	detects presence of damaged cartridge in chamber
	 * @returns true if jammed
	 **/
	proto native bool IsChamberJammed (int muzzleIndex);
	/**@fn		IsCartridgeInChamber
	 * @brief	detects presence of fire-able cartridge in muzzle's chamber
	 *
	 * @returns true if fire-able
	 **/
	proto native bool IsCartridgeInChamber (int muzzleIndex);
	/**@fn		IsChamberFull
	 * @brief	maximum number of cartridges in chamber
	 * @returns true if full
	 **/
	proto native bool IsChamberFull (int muzzleIndex);

	/**@fn		GetChamberCartridgeCount
	 * @return	returns number of available cartridges in chamber
	 **/
	proto native int GetChamberCartridgeCount (int muzzleIndex);

	/**@fn		GetChamberMaxCartridgeCount
	 * @return	returns number of max cartridges in chamber
	 **/
	proto native int GetChamberMaxCartridgeCount (int muzzleIndex);


	/**@fn		GetChamberAmmoTypeCount
	 * @return	returns number of available ammo types for chambering
	 **/
	proto native int GetChamberAmmoTypeCount (int muzzleIndex);

	/**@fn		GetChamberAmmoTypeName
	 * @param[in]
	 * @return	returns name of i-th ammo type for chambering
	 **/
	proto native string GetChamberAmmoTypeName (int muzzleIndex);
	/**@fn		LoadChamber
	 * @brief	loads cartridge(damage, type) to chamber
	 * @param[in] muzzleIndex
	 * @param[in] ammoDamage \p  damage of the ammo
	 * @param[in] ammoTypeName \p	 type name of the ejected ammo
	 * @return	true if loaded
	 **/
	proto native bool LoadChamber (int muzzleIndex, float ammoDamage, string ammoTypeName);
	/**@fn		EjectCartridge
	 * @brief	unload bullet from chamber
	 *
	 * @NOTE: 	EjectCartridge = GetCartridgeInfo + PopCartridge
	 *
	 * @param[in] muzzleIndex
	 * @param[out] ammoDamage \p  damage of the ammo
	 * @param[out] ammoTypeName \p	 type name of the ejected ammo
	 * @return	true if bullet removed from chamber
	 **/
	proto bool EjectCartridge (int muzzleIndex, out float ammoDamage, out string ammoTypeName);
	/**@fn		GetCartridgeInfo
	 * @brief	get info about the cartridge in the chamber of a muzzle
	 * @param[in] muzzleIndex
	 * @param[out] ammoDamage \p  damage of the ammo
	 * @param[out] ammoTypeName \p	 type name of the ejected ammo
	 * @return	true if info retrieved
	 **/
	proto bool GetCartridgeInfo (int muzzleIndex, out float ammoDamage, out string ammoTypeName);
	/**@fn		PopCartridge
	 * @brief	removes top-most cartridge
	 **/
	proto native bool PopCartridge (int muzzleIndex);
	/**@fn		PushCartridge
	 * @brief	push cartridge in chamber into internal magazine (if any)
	 **/
	proto native bool PushCartridge (int muzzleIndex);
	
	/**@fn		CanAttachMagazine
	 * @brief 	query if a magazine can be attached to weapon
	 * @param[in] muzzleIndex
	 * @param[in] mag		magazine to attach
	 * @return	true if magazine can be attached
	 **/		
	proto native bool CanAttachMagazine (int muzzleIndex, Magazine mag);

	/**@fn		TestAttachMagazine
	 * @brief 	query if a magazine can be attached to weapon
	 * @param[in] muzzleIndex
	 * @param[in] mag		magazine to attach
	 * @param[in] do_occupancy_test	  if false, skips test for presence of another magazine in weapon
	 * @param[in] do_script_check	  if false, skips script conditions
	 * @return	true if magazine can be attached
	 **/		
	proto native bool TestAttachMagazine (int muzzleIndex, Magazine mag, bool do_occupancy_test, bool do_script_check);

	/**@fn		AttachMagazine
	 * @brief 	perform attachment of a magazine to weapon
	 * @param[in] muzzleIndex
	 * @param[in] mag		magazine to attach
	 * @return	true if magazine was attached
	 **/		
	proto native bool AttachMagazine (int muzzleIndex, Magazine mag);

	/**@fn		GetMagazine
	 * @param[in] muzzleIndex
	 * @return	attached magazine
	 **/		
	proto native Magazine GetMagazine (int muzzleIndex);

	/**@fn		GetReloadTime
	 * @brief	defines time from one shot to another
	 * @param[in] muzzleIndex
	 * @return	reload time in seconds
	 **/		
	proto native float GetReloadTime (int muzzleIndex);

	/**@fn		CanChamberFromMag
	 * @brief 	query if a bullet from magazine can be loaded into chamber
	 * @param[in] muzzleIndex
	 * @param[in] mag		magazine to load from
	 * @return	true if cartidge from mag can be put into chamber
	 **/
	proto native bool CanChamberFromMag (int muzzleIndex, Magazine mag);

	/**@fn		GetCartridgeMagazineTypeName
	 * @brief query the type of magazine the cartridge can be unloaded to
	 * @param[in] muzzleIndex
	 * @return entity type name
	 **/
	proto native string GetCartridgeMagazineTypeName (int muzzleIndex);

	/**@fn		EnterOptics
	 * @brief	switches to optics mode if possible
	 * @return true if success, false otherwise
	 **/
	proto native bool EnterOptics ();
	/**@fn		IsInOptics
	 * @brief	is weapon in optics mode or not
	 * @return true if in optics mode, false otherwise
	 **/
	proto native bool IsInOptics ();
	/**@fn		ExitOptics
	 * @brief	switches out of optics mode (if possible)
	 * @return true if success, false otherwise
	 **/
	proto native bool ExitOptics ();

	/**@fn		GetZoom
	 * @brief	returns current percentage of zoom
	 * @return	current percentage of zoom
	 **/
	proto native float GetZoom ();
	/**@fn		SetZoom
	 * @brief sets current zoom (in %)
	 * @param[in] percent	\p	zoom to set in range [0.0f .. 100.0f]
	 * @return	true if zoom set
	 **/
	proto native bool SetZoom (float percent);
	/**@fn		ZoomIn
	 * @brief	moves current zoom by 1% up
	 * @return	true if zoom set
	 **/
	proto native bool ZoomIn ();
	/**@fn		ZoomOut
	 * @brief	moves current zoom by 1% down
	 * @return	true if zoom set
	 **/
	proto native bool ZoomOut ();

	/**@fn		GetStepZoom
	 * @brief	returns position of currently used value in zoom config array
	 * @return	position of currently used value in zoom config array
	 **/
	proto native int GetStepZoom ();
	/**@fn		SetStepZoom
	 * @brief sets zoom to fov value defined at given position in zoom fov config array
	 * @param[in] index of configured step, range [0..cfg_max]
	 * @return	true if zoom set
	 **/
	proto native bool SetStepZoom (int step);
	/**@fn		StepZoomIn
	 * @brief sets zoom to next defined (respective to current) value in zoom fov config array
	 * @return	true if zoom set
	 **/
	proto native bool StepZoomIn ();
	/**@fn		StepZoomOut
	 * @brief	sets zoom to previous (respective to current) defined value in zoom fov config array
	 * @return	true if zoom set
	 **/
	proto native bool StepZoomOut ();

	/**@fn		GetStepZeroing
	 * @brief	returns position of currently used value in zeroing config array
	 * @return	 position of currently used value in zeroing config array
	 **/
	proto native int GetStepZeroing ();
	/**@fn		SetZeroing
	 * @brief sets zeroing to value defined at given position in zeroing config array
	 * @param[in] index of zeroing to set [0..cfg_max]
	 * @return	true if zeroing set
	 **/
	proto native bool SetStepZeroing (int step);
	/**
	 * @fn		StepZeroingUp
	 * @brief sets zeroing to next defined (respective to current) value in zeroing config array
	 * @return true if zeroing set
	 **/
	proto native bool StepZeroingUp ();
	/**
	 * @fn		StepZeroingDown
	 * @brief	sets zeroing to previous (respective to current) defined value in zeroing config array
	 * @return true if zeroing set
	 **/
	proto native bool StepZeroingDown ();

	/**
	 * @fn		FlashlightOn
	 * @brief	attempt to turn on flashlight
	 * @return true if set on, false otherwise (no flashlight, no power, etc)
	 **/
	proto native bool FlashlightOn ();

	/**
	 * @fn		FlashlightOff
	 * @brief	attempt to turn off flashlight
	 * @return true if on -> off, false otherwise (no flashlight, no power, etc, already off)
	 **/
	proto native bool FlashlightOff ();

	/**
	 * @fn		IsFlashlightOn
	 * @brief	query state of flashlight
	 * @return true if set on
	 **/
	proto native bool IsFlashlightOn ();

	/**@fn		GetAttachedLauncher
	 * @return	returns attached launcher or null
	 **/
	proto native UnderSlugLauncher GetAttachedLauncher ();
	/**@fn		GetAttachedOptics
	 * @return	returns attached optics or null
	 **/
	proto native ItemOptics GetAttachedOptics ();
	/**@fn		GetAttachedSuppressor
	 * @return	returns attached suppressor or null
	 **/
	proto native ItemSuppressor GetAttachedSuppressor ();
	
	proto native void GetCameraPoint (int muzzleIndex, out vector pos, out vector dir);

	proto native void SelectionBulletShow ();
	proto native void SelectionBulletHide ();
	proto native void SelectionMagazineShow ();
	proto native void SelectionMagazineHide ();
};


