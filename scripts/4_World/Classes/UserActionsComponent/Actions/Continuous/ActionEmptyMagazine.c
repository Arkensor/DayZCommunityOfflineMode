class ActionEmptyMagazineCB : ActionContinuousBaseCB
{
	Magazine m_magazinePile;
	
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousRepeat(UATimeSpent.MAG_EMPTY);
		m_magazinePile = NULL;
	}
};

class ActionEmptyMagazine: ActionContinuousBase
{
	private const float MAX_GROUND_PILE_DISTANCE = 1.5;
	
	void ActionEmptyMagazine()
	{
		m_MessageStartFail = "It's empty.";
		m_MessageStart = "I have started emptying magazine.";
		m_MessageSuccess = "I have finished emptying the magazine.";
		m_MessageFail = "It broke.";
		m_MessageCancel = "I stopped emptying the magazine.";
		
		m_CallbackClass = ActionEmptyMagazineCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EMPTYMAG;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_EMPTYMAG;

	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	
	override bool HasProneException()
	{
		return true;
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Magazine itm;
		if ( Class.CastTo(itm, item) && itm.GetAmmoCount() > 0 ) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	override int GetType()
	{
		return AT_EMPTY_MAGAZINE;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "empty magazine";
	}
	
	override void OnRepeatServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		ActionEmptyMagazineCB callback;
		Class.CastTo(callback, m_Callback);
		
		Magazine mag;
		Class.CastTo(mag, item);
		
		//float dmg;
		//string bulletType;
		
		/*int ammoCount = mag.GetAmmoCount();
		if(GetAmmoCount() > 0)
		{
			mag.Get
			mag.SetAmmoCount( ammoCount - 1 );
			array< Magazine > piles;
		
			if(!callback.m_magazinePile )
			{
				piles = MiscGameplayFunctions.CreateMagazinePilesFromBullet(bulletType,player.GetPosition(),1,dmg);
				if( piles.Count() > 0 )
					callback.m_magazinePile = piles.Get(0);
			}
			else
			{
				if( callback.m_magazinePile.GetAmmoMax() > callback.m_magazinePile.GetAmmoCount() && (vector.Distance(player.GetPosition(),callback.m_magazinePile.GetPosition()) < MAX_GROUND_PILE_DISTANCE ) )
				{
					callback.m_magazinePile.StoreCartridge(dmg,bulletType);
				}
				else
				{
					piles = MiscGameplayFunctions.CreateMagazinePilesFromBullet(bulletType,player.GetPosition(),1,dmg);
					if( piles.Count() > 0 )
						callback.m_magazinePile = piles.Get(0);
				}	
			}
			
			
			
		}	*/
		
		
		
		//----Uncomment after AcquireCartridge will work

		float dmg;
		string ammoType;
			
		if( !mag.ServerAcquireCartridge(dmg,ammoType) )
		{
			return;
		}	
		
		string magazineTypeName;
	 	GetGame().ConfigGetText( "CfgAmmo " + ammoType + " spawnPileType" , magazineTypeName);
		if( !DayZPlayerUtils.HandleStoreCartridge(player, NULL, -1, dmg, ammoType, magazineTypeName, false) )
		{
			array< Magazine > piles;
		
			if(!callback.m_magazinePile )
			{
				piles = MiscGameplayFunctions.CreateMagazinePilesFromBullet(ammoType,player.GetPosition(),1,dmg);
				if( piles.Count() > 0 )
					callback.m_magazinePile = piles.Get(0);
			}
			else
			{
				if( callback.m_magazinePile.GetAmmoMax() > callback.m_magazinePile.GetAmmoCount() && (vector.Distance(player.GetPosition(),callback.m_magazinePile.GetPosition()) < MAX_GROUND_PILE_DISTANCE ) )
				{
					callback.m_magazinePile.ServerStoreCartridge(dmg,ammoType);
				}
				else
				{
					piles = MiscGameplayFunctions.CreateMagazinePilesFromBullet(ammoType,player.GetPosition(),1,dmg);
					if( piles.Count() > 0 )
						callback.m_magazinePile = piles.Get(0);
				}	
			}
			//callback.m_magazinePile.SetSynchDirty();
		}
		//mag.SetSynchDirty();
	}

	/*override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{

	}*/
};