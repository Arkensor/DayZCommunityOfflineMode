class TripwireTrap : TrapBase
{
	int 		m_State;
	
	static const int 	FOLDED = 3;
	static const int 	DEPLOYED = 2;
	static const int 	TRIGGERED = 1;
	
	void TripwireTrap()
	{
		m_State = FOLDED;
		m_DamagePlayers = 0; 			//How much damage player gets when caught
		m_InitWaitTime = 3; 			//After this time after deployment, the trap is activated
		m_NeedActivation = false;
		m_AnimationPhaseGrounded = "inventory";
		m_AnimationPhaseSet = "placing";
		m_AnimationPhaseTriggered = "TripWire_triggered";
		m_InfoActivationTime = "#STR_TripwireTrap0" + m_InitWaitTime.ToString() + "#STR_TripwireTrap1";  // nefunguje dynamicke vyrazy mimo funkcii
		
		
		m_SurfaceForSetup = new array<string>; // declared in TrapBase
		m_SurfaceForSetup.Insert("cp_dirt");
		m_SurfaceForSetup.Insert("cp_broadleaf_dense1");
		m_SurfaceForSetup.Insert("cp_broadleaf_dense2");
		m_SurfaceForSetup.Insert("cp_broadleaf_sparse1");
		m_SurfaceForSetup.Insert("cp_broadleaf_sparse2");
		m_SurfaceForSetup.Insert("cp_conifer_common1");
		m_SurfaceForSetup.Insert("cp_conifer_common2");
		m_SurfaceForSetup.Insert("cp_conifer_moss1");
		m_SurfaceForSetup.Insert("cp_conifer_moss2");
		m_SurfaceForSetup.Insert("cp_grass");
		m_SurfaceForSetup.Insert("cp_grass_tall");
		m_SurfaceForSetup.Insert("cp_gravel");
		m_SurfaceForSetup.Insert("cp_rock");
	}

	override void CreateTrigger()
	{
		m_TrapTrigger = TrapTrigger.Cast( GetGame().CreateObject( "TrapTrigger", this.GetPosition(), false ) );
		vector mins = "-0.75 0.3 -0.01";
		vector maxs = "0.75 0.32 0.01";
		m_TrapTrigger.SetOrientation( this.GetOrientation() );
		m_TrapTrigger.SetExtents(mins, maxs);	
		m_TrapTrigger.SetParentObject( this );
	}
	
	override void OnSteppedOn(EntityAI victim)
	{
		if ( GetInventory().AttachmentCount() > 0)
		{
			ItemBase attachment = ItemBase.Cast( GetInventory().GetAttachmentFromIndex(0) );
			
			if (attachment)
			{
				attachment.OnActivatedByTripWire();
			}
		}
		
		SetState(TRIGGERED);
		UpdateProxySelections();
	}
	
	override void EEItemLocationChanged ( EntityAI old_owner, EntityAI new_owner ) 
	{
		super.EEItemLocationChanged( old_owner, new_owner );
		
		FoldTripWire();
		UpdateProxySelections();
	}
	
	void SetState(int state_ID)
	{
		m_State = state_ID;
	}
	
	int GetState()
	{
		return m_State;
	}
	
	override void SetupTrapPlayer( PlayerBase player, bool set_position = true )
	{
		super.SetupTrapPlayer( player, set_position );
		SetState(DEPLOYED);
	}
	
	override void StartDeactivate( PlayerBase player )
	{
		super.StartDeactivate(player);
		SetState(FOLDED);
	}
	
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);
		UpdateProxySelections();
	}
	
	override void EEItemDetached (EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		UpdateProxySelections();
	}
	
	void FoldTripWire()
	{
		if ( m_AnimationPhaseGrounded != "" )
		{
			SetAnimationPhase( m_AnimationPhaseSet, 1 );
			
			if ( m_AnimationPhaseTriggered != m_AnimationPhaseGrounded ) 
			{
				SetAnimationPhase( m_AnimationPhaseTriggered, 1 );
			}
			
			SetAnimationPhase( m_AnimationPhaseGrounded, 0 );
		}
	}
	
	void UpdateProxySelections()
	{
		if ( GetInventory().AttachmentCount() > 0)
		{
			ItemBase attachment = ItemBase.Cast( GetInventory().GetAttachmentFromIndex(0) );
			
			if (attachment)
			{
				string att_model = attachment.GetModelName();
				
				if (att_model != "")
				{
					// Hide all proxies
					for (int i = 1; i <= 3; i++)
					{
						HideSelection("s" + i + "_flashbang");
						HideSelection("s" + i + "_frag_RGD5");
						HideSelection("s" + i + "_grenade");
						HideSelection("s" + i + "_Road_flare");
						HideSelection("s" + i + "_smokegrenade");
						HideSelection("s" + i + "_smokegrenade_rdg2");
						HideSelection("s" + i + "_SodaCan_Used");
					}
					
					// Now show the one we need to see
					string proxy_to_show = "s" + GetState() + "_" + att_model; 
					Print(proxy_to_show);
					ShowSelection(proxy_to_show);
				}
			}
		}
	}
}

class TripwireTrapDeployed : TripwireTrap
{
	
}
