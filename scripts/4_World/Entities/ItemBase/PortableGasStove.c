class PortableGasStove extends ItemBase
{
	private const string FLAME_BUTANE_ON 			= "dz\\gear\\cooking\\data\\flame_butane_ca.paa";
	private const string FLAME_BUTANE_OFF 			= "";
	typename ATTACHMENT_COOKING_POT 				= Pot;
	
	//cooking
	private const float PARAM_COOKING_TEMP_THRESHOLD		= 100;		//temperature threshold for starting coooking process (degree Celsius)
	private const float PARAM_COOKING_EQUIP_TEMP_INCREASE	= 10;		//how much will temperature increase when attached on burning fireplace (degree Celsius)
	private const float PARAM_COOKING_EQUIP_MAX_TEMP		= 250;		//maximum temperature of attached cooking equipment (degree Celsius)
	private const float PARAM_COOKING_TIME_INC_COEF			= 0.5;		//cooking time increase coeficient, can be used when balancing how fast a food can be cooked
	
	//
	ref Cooking m_CookingProcess;
	ItemBase m_CookingEquipment;
	
	//sound
	protected SoundOnVehicle m_SoundGasFlameLoop;
	const string SOUND_BURNING_GAS 	= "gasFlame";

	//cooking equipment
	ItemBase GetCookingEquipment()
	{
		return m_CookingEquipment;
	}
	
	void SetCookingEquipment( ItemBase equipment )
	{
		m_CookingEquipment = equipment;
	}
	
	void ClearCookingEquipment()
	{
		SetCookingEquipment( NULL );
	}

	//Destroy
	void DestroyFireplace()
	{
		//delete object
		GetGame().ObjectDelete( this );
	}	
	
	//--- ATTACHMENTS
	override void EEItemAttached ( EntityAI item, string slot_name ) 
	{
		super.EEItemAttached( item, slot_name );
		
		//cookware
		if ( item.Type() == ATTACHMENT_COOKING_POT )
		{
			ItemBase item_base = ItemBase.Cast( item );
			SetCookingEquipment( item_base );
		}
	}
	
	override void EEItemDetached ( EntityAI item, string slot_name ) 
	{
		super.EEItemDetached( item, slot_name );
		
		//cookware
		if ( item.Type() == ATTACHMENT_COOKING_POT )
		{
			ClearCookingEquipment();
			
			//stop steam particle
			StopCookingEquipmentSteam();			
		}	
	}
	
	//--- POWER EVENTS
	override void OnWorkStart()
	{
		//refresh visual
		SetObjectTexture( 0, FLAME_BUTANE_ON );
		
		//play sound
		SoundBurningStart();
	}

	override void OnWorkStop()
	{
		//refresh visual
		SetObjectTexture( 0, FLAME_BUTANE_OFF );
		
		//stop steam particle
		StopCookingEquipmentSteam();
		
		//stop sound
		SoundBurningStop();
	}
	
	//on update 
	override void OnWork( float consumed_energy )
	{
		//manage cooking equipment
		if ( GetCookingEquipment() )
		{
			if ( GetGame() && GetGame().IsServer() )
			{
				float cook_equip_temp = GetCookingEquipment().GetTemperature();
				
				//start cooking
				if ( cook_equip_temp >= PARAM_COOKING_TEMP_THRESHOLD )
				{
					CookWithEquipment();
				}				
				
				//set temperature to cooking equipment
				cook_equip_temp = cook_equip_temp + PARAM_COOKING_EQUIP_TEMP_INCREASE;
				cook_equip_temp = Math.Clamp ( cook_equip_temp, 0, PARAM_COOKING_EQUIP_MAX_TEMP );
				GetCookingEquipment().SetTemperature( cook_equip_temp );
			}
		}
	}
	
	void CookWithEquipment()
	{
		if ( m_CookingProcess == NULL )
		{
			m_CookingProcess = new Cooking();
		}
		
		m_CookingProcess.CookWithEquipment ( GetCookingEquipment(), PARAM_COOKING_TIME_INC_COEF );
	}

	//================================================================
	// PARTICLES
	//================================================================	
	//cooking equipment steam
	protected void StopCookingEquipmentSteam()
	{
		if ( GetCookingEquipment() )
		{
			Bottle_Base cooking_pot = Bottle_Base.Cast( GetCookingEquipment() );
			cooking_pot.SteamStop();
		}
	}	
	
	//================================================================
	// SOUNDS
	//================================================================
	//Gas burning
	protected void SoundBurningStart()
	{
		if ( !m_SoundGasFlameLoop && GetGame() && ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
		{
			m_SoundGasFlameLoop = PlaySoundLoop( SOUND_BURNING_GAS, 50 );
		}
	}
	
	protected void SoundBurningStop()
	{
		if ( m_SoundGasFlameLoop && GetGame() && ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
		{
			GetGame().ObjectDelete( m_SoundGasFlameLoop );
			m_SoundGasFlameLoop = NULL;			
		}
	}
	
	//================================================================
	// CONDITIONS
	//================================================================
	//this into/outo parent.Cargo
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}
		if ( GetCompEM().IsSwitchedOn() )
		{
			return false;
		}
		
		return true;
	}

	override bool CanRemoveFromCargo( EntityAI parent )
	{
		return true;
	}
	
	//hands
	override bool CanPutIntoHands( EntityAI parent )
	{
		if( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		if ( GetCompEM().IsSwitchedOn() )
		{
			return false;
		}
		
		return true;
	}	
}