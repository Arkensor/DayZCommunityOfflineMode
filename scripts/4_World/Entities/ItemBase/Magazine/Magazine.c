class Magazine : InventoryItemSuper
{
	void Magazine () { }

	//! Gets magazine ammo count
	proto native int GetAmmoCount();
	//! Sets magazine ammo count
	proto native void ServerSetAmmoCount (int ammoCount);
	proto native void LocalSetAmmoCount (int ammoCount);

	/**@fn		AcquireCartridge
	 * @brief	acquires cartridge(damage, type) to magazine
	 * @param[out] ammoDamage \p  damage of the ammo
	 * @param[out] cartTypeName \p	 type name of the ejected ammo
	 * @return	true if acquired
	 **/
	proto bool LocalAcquireCartridge (out float dmg, out string cartTypeName);
	proto bool ServerAcquireCartridge (out float dmg, out string cartTypeName);
	/**@fn		StoreCartridge
	 * @brief	stores cartridge(damage, type) to magazine
	 * @param[in] ammoDamage \p  damage of the cartridge
	 * @param[in] cartTypeName \p	 type name of the stored cartridge
	 * @return	true if stored
	 **/
	proto native bool LocalStoreCartridge (float ammoDamage, string cartTypeName);
	proto native bool ServerStoreCartridge (float ammoDamage, string cartTypeName);

	/**@fn		GetCartridgeAtIndex
	 * @brief	queries cartridge(damage, type) info at specified index
	 * @param[in] cartIndex \p  index of the cartridge.
	 * @param[out] ammoDamage \p  damage of the cartridge
	 * @param[out] cartTypeName \p	 type name of the cartridge
	 * @return	true if index valid
	 **/
	proto bool GetCartridgeAtIndex (int cartIndex, out float dmg, out string cartTypeName);

	/**@fn		SetCartridgeAtIndex
	 * @brief	modifies cartridge(damage, type) info at specified index
	 * @param[in] cartIndex \p  index of the cartridge.
	 * @param[in] ammoDamage \p  damage of the cartridge
	 * @param[in] cartTypeName \p	 type name of the cartridge
	 * @return	true if index valid
	 **/
	proto bool SetCartridgeAtIndex (int cartIndex, out float dmg, out string cartTypeName);

	override void SetUserActions()
	{
		g_Game.ConfigGetIntArray("cfgMagazines " +GetType() + " ContinuousActions", m_ContinuousActions);
		g_Game.ConfigGetIntArray("cfgMagazines " +GetType() + " SingleUseActions", m_SingleUseActions);	
		g_Game.ConfigGetIntArray("cfgMagazines " +GetType() + " InteractActions", m_InteractActions);	
	}	
	
	bool CanAddCartridges (int count)
	{
		int spc_avail = GetAmmoMax() - GetAmmoCount();
		return count <= spc_avail;
	}

	//! Adds magazine ammo, MP safe
	void ServerAddAmmoCount(int ammoCount)
	{
		ServerSetAmmoCount(GetAmmoCount() + ammoCount);
	}
	void LocalAddAmmoCount(int ammoCount)
	{
		LocalSetAmmoCount(GetAmmoCount() + ammoCount);
	}
	//! returns max rounds for this mag (returns "count" config value)
	int GetAmmoMax()
	{
		return ConfigGetInt("count");
	}
	//! set max rounds for this mag
	void ServerSetAmmoMax()
	{
		ServerSetAmmoCount( GetAmmoMax() );
	}
	void LocalSetAmmoMax()
	{
		LocalSetAmmoCount( GetAmmoMax() );
	}
	//! Returns if this entity is Magazine
	override bool IsMagazine()
	{
		return true;
	}
	
	
	override bool CanBeSplit()
	{
		if( ConfigGetBool("canBeSplit") )
		{
			if( this.GetAmmoCount() > 1 )
			{
				return true;
			}
			else
			{
				return false;			
			}
		}
		else return false;
	}
	/*
	override void SplitItem()
	{
		if ( !CanBeSplit() )
		{
			return;
		}
		
		float quantity = this.GetAmmoCount();
		float split_quantity_new = Math.Floor( quantity / 2 );
		
		this.AddAmmoCount(-split_quantity_new);
		
		PlayerBase player = this.GetHierarchyRootPlayer();
		Magazine new_item;
		if( player )
		{
			new_item = player.CopyInventoryItem( this );
		}
		else
		{
			new_item = GetGame().CreateObject(this.GetType(), this.GetPosition() );
		}
			
		new_item.SetAmmoCount( split_quantity_new );
	}
	*/

	override void SplitItem(PlayerBase player)
	{
		if ( !CanBeSplit() )
		{
			return;
		}

		
		Magazine new_pile = Magazine.Cast( player.CopyInventoryItem( this ) );
		
		if(!new_pile)
		{
			//new_pile = GetGame().CreateObject(this.GetType(), this.GetPosition() + "0.2 0 0" );
			new_pile = Magazine.Cast( SpawnEntityOnGroundPos( this.GetType(), this.GetPosition() ) );
		}

		new_pile.ServerSetAmmoCount(0);
		int quantity = this.GetAmmoCount();
		
		for(int i = 0; i < Math.Floor( quantity / 2 ); i++)
		{
			float damage;
			string cartrige_name;
			ServerAcquireCartridge(damage, cartrige_name);
			new_pile.ServerStoreCartridge(damage, cartrige_name);
		}
	}

	override bool IsFullQuantity()
	{
		if( GetAmmoCount() == GetAmmoMax() )
		{
			return true;			
		}
		else
		{
			return false;			
		}
	}

	//TODO add functionality for multiple ammo types (does not exist yet)
	override int GetItemWeight()
	{
		float item_wetness = GetWet();
		int ConfWeight = ConfigGetInt("weight");
		int AmmoWeight = 0;
		int AmmoTypeWeight = 0;
		array<string> ammo_array = new array<string>;
		
		ConfigGetTextArray ("ammoItems", ammo_array);
		if (ammo_array.Count() > 0)
		{
			string AmmoType = ammo_array.Get(0);
			
			AmmoTypeWeight = GetGame().ConfigGetInt("cfgMagazines " + AmmoType + " weight");
			AmmoWeight = GetAmmoCount() * AmmoTypeWeight;
		}
		return Math.Round((item_wetness + 1) * ConfWeight + AmmoWeight);
	}

	/*
	override void CombineItems( ItemBase other_item )
	{
		if( !CanBeCombined(other_item) ) return;
		if( other_item.GetType() != this.GetType() ) return;
		
		Magazine other_magazine = other_item;
		float other_item_quantity = other_magazine.GetAmmoCount();
		float this_free_space = this.GetAmmoMax() - this.GetAmmoCount();
		float quantity_used = 0;
		
		if( other_item_quantity > this_free_space )
		{
			quantity_used = this_free_space;
		}
		else
		{
			quantity_used = other_item_quantity;
		}
		if( quantity_used!= 0 )
		{
			this.AddAmmoCount(quantity_used);
			other_magazine.AddAmmoCount(-quantity_used);
			if(other_magazine.GetAmmoCount() <= 0) other_magazine.Delete();
		}
	}
	*/
	
	override void CombineItems( ItemBase other_item, bool use_stack_max = false )
	{
		if( !CanBeCombined(other_item) ) return;
		if( other_item.GetType() != this.GetType() ) return;
		
		Magazine other_magazine;
		if ( Class.CastTo(other_magazine, other_item) )
		{
			//int other_item_quantity = other_magazine.GetAmmoCount();
			int this_free_space = this.GetAmmoMax() - this.GetAmmoCount();
			
			for(int i = 0; i < this_free_space && other_magazine.GetAmmoCount() > 0 ; i++)
			{
				float damage;
				string cartrige_name;
				other_magazine.ServerAcquireCartridge(damage, cartrige_name);
				this.ServerStoreCartridge(damage, cartrige_name);
			}
		}
	}
};
