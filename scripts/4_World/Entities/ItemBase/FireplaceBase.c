enum FireplaceFireState
{
	NO_FIRE			= 1,
	START_FIRE		= 2,
	SMALL_FIRE		= 3,	
	NORMAL_FIRE		= 4,
	END_FIRE		= 5,
	EXTINGUISH_FIRE	= 6,
	REIGNITED_FIRE	= 7,
	
	COUNT			= 8			//for net sync purposes
}

class FireplaceBase extends ItemBase
{
	//State params
	protected bool m_IsBurning 	= false;   
	protected bool m_HasAshes	= false;
	protected bool m_IsOven		= false;
	protected FireplaceFireState m_FireState	 = FireplaceFireState.NO_FIRE;
	protected FireplaceFireState m_LastFireState = FireplaceFireState.NO_FIRE;		//for synchronization purposes
	
	//Fireplace params
	protected float m_TemperatureLossMP;		//! determines how fast will the fireplace loose its temperature when cooling (lower is better)
	protected float m_FuelBurnRateMP;			//! determines how fast will the fuel item burn before spending (lower is better)
	
	//cooking
	protected ref Cooking m_CookingProcess;
	
	//
	const float PARAM_SMALL_FIRE_TEMPERATURE 		= 150;		//! maximum fireplace temperature of a small fire (degree Celsius)
	const float PARAM_NORMAL_FIRE_TEMPERATURE 		= 1000;		//! maximum fireplace temperature of a normal fire (degree Celsius)
	const float PARAM_MIN_FIRE_TEMPERATURE 			= 40;		//! minimum fireplace temperature under which the fireplace is inactive (degree Celsius)
	const float	PARAM_TEMPERATURE_INCREASE 			= 30;		//! how much will temperature increase when fireplace is burning (degree Celsius)
	const float	PARAM_TEMPERATURE_DECREASE 			= 10;		//! how much will temperature decrease when fireplace is cooling (degree Celsius)
	const float	PARAM_MAX_WET_TO_IGNITE 			= 0.2;		//! maximum wetness value when the fireplace can be ignited
	const float PARAM_MIN_TEMP_TO_REIGNITE 			= 100;		//! minimum fireplace temperature under which the fireplace can be reignited using air only (degree Celsius)
	const float	PARAM_IGNITE_RAIN_THRESHOLD 		= 0.05;		//! maximum rain value when the fireplace can be ignited
	const float	PARAM_BURN_WET_THRESHOLD 			= 0.40;		//! maximum wetness value when the fireplace is able to burn
	const float	PARAM_WET_INCREASE_COEF 			= 0.02;		//! value for calculating of  wetness that fireplace gain when raining
	const float	PARAM_WET_DECREASE_COEF 			= 0.01;		//! value for calculating of  wetness that fireplace gain when raining
	const float	PARAM_FIRE_CONSUM_RATE_AMOUNT		= 1.0;		//! base value of fire consumption rate (how many base energy will be spent on each update)
	const float	PARAM_BURN_DAMAGE_COEF				= 0.05;		//! value for calculating damage on items located in fireplace cargo
	const float	PARAM_CARGO_TEMP_INCREASE_COEF		= 10;		//! value for calculating temperature increase on each heat update interval (degree Celsius)
	const float	PARAM_CARGO_MIN_TEMP				= 40;		//! minimum temperature for items that can be heated in cargo (degree Celsius)
	const float PARAM_MAX_CARGO_TEMP_INCREASE		= 200;		//! maximum value of temperature of items in fireplace cargo when heating (degree Celsius)
	const float PARAM_HEAT_RADIUS 					= 2.5;		//! radius in which objects are heated by fire
	const float PARAM_HEAT_WET_DECREASE_COEF		= 0.02;		//! value for calculating wet loss when heating by fire
	//! 
	const int 	MIN_STONES_TO_BUILD_OVEN			= 8;		//! minimum amount of stones for oven
	const int 	MAX_TEMPERATURE_TO_DISMANTLE_OVEN	= 40;		//! maximum temperature for dismantling oven
	//
	const float TEMPERATURE_LOSS_MP_DEFAULT			= 1.0;
	const float FUEL_BURN_RATE_DEFAULT				= 1.0;
	const float TEMPERATURE_LOSS_MP_STONES			= 0.83;		//20% boost
	const float FUEL_BURN_RATE_STONES				= 0.83;
	const float TEMPERATURE_LOSS_MP_OVEN			= 0.67;		//50% boost
	const float FUEL_BURN_RATE_OVEN					= 0.67;
	
	//! cooking
	const float PARAM_COOKING_TEMP_THRESHOLD		= 100;		//! temperature threshold for starting coooking process (degree Celsius)
	const float PARAM_COOKING_EQUIP_MAX_TEMP		= 250;		//! maximum temperature of attached cooking equipment (degree Celsius)
	const float PARAM_COOKING_EQUIP_TEMP_INCREASE	= 10;		//! how much will temperature increase when attached on burning fireplace (degree Celsius)
	//! 
	const int 	TIMER_HEATING_UPDATE_INTERVAL 		= 2;		//! update interval duration of heating process (seconds)
	const int 	TIMER_COOLING_UPDATE_INTERVAL 		= 2;		//! update interval duration of cooling process (seconds)
	
	//Timers
	protected ref Timer m_HeatingTimer;
	protected ref Timer m_CoolingTimer;
	
	//Attachments
	protected ItemBase m_CookingEquipment;
	protected ref FireConsumable m_ItemToConsume;
	
	//Particles - default for FireplaceBase
	protected int PARTICLE_FIRE_START 	= ParticleList.CAMP_FIRE_START;
	protected int PARTICLE_SMALL_FIRE 	= ParticleList.CAMP_SMALL_FIRE;
	protected int PARTICLE_NORMAL_FIRE	= ParticleList.CAMP_NORMAL_FIRE;
	protected int PARTICLE_SMALL_SMOKE 	= ParticleList.CAMP_SMALL_SMOKE;
	protected int PARTICLE_NORMAL_SMOKE	= ParticleList.CAMP_NORMAL_SMOKE;
	protected int PARTICLE_FIRE_END 	= ParticleList.CAMP_FIRE_END;
	protected int PARTICLE_STEAM_END	= ParticleList.CAMP_STEAM_2END;
	//
	protected Particle m_ParticleFireStart;
	protected Particle m_ParticleSmallFire;
	protected Particle m_ParticleNormalFire;
	protected Particle m_ParticleSmallSmoke;
	protected Particle m_ParticleNormalSmoke;
	protected Particle m_ParticleFireEnd;
	protected Particle m_ParticleSteamEnd;
	
	//Sounds
	const string SOUND_BURNING_CAMP 				= "fireplaceCamp";
	const string SOUND_BURNING_LIGHT				= "fireplaceCampLight";
	const string SOUND_BURNING_NO_FUEL				= "fireplaceCampNoFuel";
	
	//
	protected SoundOnVehicle m_SoundBurningLoop;
	//
	
	//Fuel
	//Kindling / Fuel item types
	protected static ref map<typename, ref FireConsumableType> 	m_FireConsumableTypes;
	protected ref map<ItemBase, ref FireConsumable> 			m_FireConsumables;
	
	//Player action / status messages
	const string MESSAGE_IGNITE_IGNIT_ITEM_DAMAGED 		= "You cannot start fire. Fireplace is ruined.";
	const string MESSAGE_IGNITE_NO_KINDLING 			= "There needs to be some kindling to start a fire.";
	const string MESSAGE_IGNITE_TOO_WET 				= "The fireplace is too wet to be ignited.";
	const string MESSAGE_IGNITE_RAIN	 				= "The fire went out because of the rain.";
	const string MESSAGE_IGNITE_WIND	 				= "The fire went out because of the wind.";
	const string MESSAGE_IGNITE_IN_WATER 				= "I cannot ignite the fireplace in the water.";
	const string MESSAGE_IGNITE_UNDER_ROOF 				= "I cannot ignite the fireplace here, it's not safe.";
	const string MESSAGE_REIGNITE_NO_KINDLING 			= "There needs to be some kindling to start a fire.";
	const string MESSAGE_REIGNITE_RAIN	 				= "Cannot reignite fireplace in rain.";
	const string MESSAGE_REIGNITE_TOO_WET 				= "The fireplace is too wet to be reignited.";
	const string MESSAGE_BURY_ASHES_FAILED_NOT_EMPTY	= "Cannot bury ashes. There are sill items in it.";
	const string MESSAGE_BURY_ASHES_FAILED_SURFACE		= "Cannot bury ashes on hard surface.";
	const string MESSAGE_BURY_ASHES_FAILED_BURNING		= "Cannot bury ashes, the fireplace is still burning.";
	const string MESSAGE_BURY_ASHES_FAILED_TOO_HOT		= "Cannot bury ashes, the fireplace is too hot.";
	const string MESSAGE_CANNOT_BUILD_OVEN				= "Cannot build oven.";
	const string MESSAGE_CANNOT_DISMANTLE_OVEN			= "Cannot dismantle oven.";
	
	//Visual animations
	const string ANIMATION_ASHES 			= "Ashes";
	const string ANIMATION_KINDLING			= "Kindling";
	const string ANIMATION_STICKS			= "Sticks";
	const string ANIMATION_WOOD 			= "Wood";
	const string ANIMATION_BURNT_WOOD 		= "BurntWood";
	const string ANIMATION_STONES 			= "Stones";
	const string ANIMATION_TRIPOD 			= "Tripod";
	const string ANIMATION_OVEN 			= "Oven";
	const string ANIMATION_INVENTORY	 	= "Inventory";
	const string ANIMATION_COOKWARE_HANDLE 	= "handleRotate";

	
	//Visual textures and materials
	const string MATERIAL_FIREPLACE_GLOW 		= "dz\\gear\\cooking\\data\\stoneground.rvmat";
	const string MATERIAL_FIREPLACE_NOGLOW 		= "dz\\gear\\cooking\\data\\stonegroundnoemit.rvmat";
	
	//Attachment types
	//Fuel
	typename ATTACHMENT_STICKS			= WoodenStick;
	typename ATTACHMENT_FIREWOOD		= Firewood;
	//Kindling
	typename ATTACHMENT_RAGS 			= Rag;
	typename ATTACHMENT_BANDAGE 		= BandageDressing;
	typename ATTACHMENT_BOOK 			= ItemBook;
	typename ATTACHMENT_BARK_OAK 		= Bark_Oak;
	typename ATTACHMENT_BARK_BIRCH 		= Bark_Birch;
	typename ATTACHMENT_PAPER 			= Paper;
	//Other
	typename ATTACHMENT_TRIPOD 			= Tripod;
	typename ATTACHMENT_STONES 			= Stone;
	typename ATTACHMENT_COOKING_POT 	= Pot;
	typename ATTACHMENT_FRYING_PAN 		= FryingPan;
	//
	const string OBJECT_CLUTTER_CUTTER 	= "ClutterCutterFireplace";
	
	//================================================================
	// INIT / STORE LOAD-SAVE
	//================================================================
	//Constructor
	protected void FireplaceBase()
	{
		//STATIC: define kindling types
		if ( !m_FireConsumableTypes )
		{
			m_FireConsumableTypes = new ref map<typename, ref FireConsumableType>;
			m_FireConsumableTypes.Insert( ATTACHMENT_RAGS, 			new FireConsumableType( ATTACHMENT_RAGS, 		7, 	true ) );
			m_FireConsumableTypes.Insert( ATTACHMENT_BANDAGE, 		new FireConsumableType( ATTACHMENT_BANDAGE, 	7, 	true ) );
			m_FireConsumableTypes.Insert( ATTACHMENT_BOOK, 			new FireConsumableType( ATTACHMENT_BOOK, 		18, true ) );
			m_FireConsumableTypes.Insert( ATTACHMENT_BARK_OAK, 		new FireConsumableType( ATTACHMENT_BARK_OAK, 	10, true ) );
			m_FireConsumableTypes.Insert( ATTACHMENT_BARK_BIRCH, 	new FireConsumableType( ATTACHMENT_BARK_BIRCH, 	7, 	true ) );
			m_FireConsumableTypes.Insert( ATTACHMENT_PAPER, 		new FireConsumableType( ATTACHMENT_PAPER, 		5, 	true ) );
			
			//define fuel types
			m_FireConsumableTypes.Insert( ATTACHMENT_STICKS, 		new FireConsumableType( ATTACHMENT_STICKS, 		20, false ) );
			m_FireConsumableTypes.Insert( ATTACHMENT_FIREWOOD, 		new FireConsumableType( ATTACHMENT_FIREWOOD, 	50, false ) );			
		}

		//define fuel / kindling items (fire consumables)
		m_FireConsumables = new ref map<ItemBase, ref FireConsumable>;
		
		//default burning parameters
		SetTemperatureLossMP( TEMPERATURE_LOSS_MP_DEFAULT );
		SetFuelBurnRateMP( FUEL_BURN_RATE_DEFAULT );
		
		//synchronized variables
		RegisterNetSyncVariableBool( "m_IsBurning" );
		RegisterNetSyncVariableBool( "m_HasAshes" );
		RegisterNetSyncVariableBool( "m_IsOven" );
		RegisterNetSyncVariableInt( "m_FireState", FireplaceFireState.NO_FIRE, FireplaceFireState.COUNT );
	}

	protected void ~FireplaceBase()
	{
		SetFireState( FireplaceFireState.NO_FIRE );
	}
	
	//on store save/load
	override void OnStoreSave( ParamsWriteContext ctx )
	{   
		super.OnStoreSave(ctx);

		//Save ashes state
		ctx.Write( m_HasAshes );
		
		//Save temperature loss MP
		ctx.Write( GetTemperatureLossMP() );

		//Save fuel burn rate MP
		ctx.Write( GetFuelBurnRateMP() );
		
		//Save burning state
		ctx.Write( IsBurning() );
	}

	override void OnStoreLoad( ParamsReadContext ctx )
	{
		super.OnStoreLoad(ctx);

		//Load ashes state
		bool has_ashes = false;
		ctx.Read( has_ashes );
		SetAshesState( has_ashes );	//set loaded value
		
		//Load temperature loss MP
		float temp_loss_mp = 1.0;
		ctx.Read( temp_loss_mp );
		SetTemperatureLossMP( temp_loss_mp );
		
		//Load fuel burn rate MP
		float fuel_burn_rate_mp = 1.0;
		ctx.Read( fuel_burn_rate_mp );
		SetFuelBurnRateMP( fuel_burn_rate_mp );

		//Load burning state, if true start fire
		bool burning_state = false;
		ctx.Read( burning_state );
		if ( burning_state )
		{
			if ( GetGame() && GetGame().IsServer() ) 
			{
				StartFire();
			}
		}
		
		//refresh visual after load
		RefreshFireplaceVisuals();
	}
	
	//================================================================
	// SYNCHRONIZATION
	//================================================================	
	void Synchronize()
	{
		if ( GetGame() && GetGame().IsMultiplayer() && GetGame().IsServer() )
		{
			SetSynchDirty();
		}
		else
		{
			//Refresh local visuals
			RefreshFireplaceVisuals();
			RefreshFireParticlesAndSounds();			
		}
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		//Refresh client visuals
		RefreshFireplaceVisuals();
		RefreshFireParticlesAndSounds();
	}
	
	//================================================================
	// BASE
	//================================================================
	
	//States
	//check fireplace types
	override bool IsFireplace()
	{
		return true;
	}
	
	bool IsBaseFireplace()
	{
		return false;
	}
	
	bool IsBarrelWithHoles()
	{
		return false;
	}
	
	bool IsFireplaceIndoor()
	{
		return false;
	}
	
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
	
	//================================================================
	// VISUAL
	//================================================================
	//Fireplace visual
	void RefreshFireplaceVisuals()
	{
		//set default burn parameters
		if ( IsBarrelWithHoles() || IsFireplaceIndoor() )
		{
			SetFuelBurnRateMP( FUEL_BURN_RATE_OVEN );
			SetTemperatureLossMP( TEMPERATURE_LOSS_MP_OVEN );
		}
		else
		{
			SetFuelBurnRateMP( FUEL_BURN_RATE_DEFAULT );
			SetTemperatureLossMP( TEMPERATURE_LOSS_MP_DEFAULT );			
		}
		
		//ANIMATION_STICKS
		if ( IsItemTypeAttached( ATTACHMENT_STICKS ) )
		{
			SetAnimationPhase( ANIMATION_STICKS, 0 );
		}
		else
		{
			SetAnimationPhase( ANIMATION_STICKS, 1 );
		}
		
		//ANIMATION_WOOD / ANIMATION_BURNT_WOOD
		if ( IsItemTypeAttached( ATTACHMENT_FIREWOOD ) )
		{
			if ( IsBurning() ) 
			{
				SetAnimationPhase( ANIMATION_BURNT_WOOD, 0 );
				SetAnimationPhase( ANIMATION_WOOD, 1 );
			}
			else
			{
				SetAnimationPhase( ANIMATION_WOOD, 0 );
				SetAnimationPhase( ANIMATION_BURNT_WOOD, 1 );
			}
		}
		else
		{
			SetAnimationPhase( ANIMATION_WOOD, 1 );
			SetAnimationPhase( ANIMATION_BURNT_WOOD, 1 );		
		}
		
		//Kindling states
		if ( HasAshes() )
		{
			SetAnimationPhase( ANIMATION_KINDLING, 1 );
			SetAnimationPhase( ANIMATION_ASHES, 0 );
		}
		else
		{
			SetAnimationPhase( ANIMATION_ASHES, 1 );
			
			int  kindling_items_count = GetKindlingCount();
			
			if ( kindling_items_count == 0 )
			{
				SetAnimationPhase( ANIMATION_KINDLING, 1 );
			}
			else
			{
				SetAnimationPhase( ANIMATION_KINDLING, 0 );
			}
		}
		
		//Oven state
		if ( IsOven() )
		{
			SetAnimationPhase( ANIMATION_OVEN, 0 );
			
			//set burn parameters
			SetFuelBurnRateMP( FUEL_BURN_RATE_OVEN );
			SetTemperatureLossMP( TEMPERATURE_LOSS_MP_OVEN );
		}
		else
		{
			SetAnimationPhase( ANIMATION_OVEN, 1 );
		}
		
		//ANIMATION_STONES
		if ( IsItemTypeAttached ( ATTACHMENT_STONES ) )
		{
			ItemBase attached_item = ItemBase.Cast( GetAttachmentByType( ATTACHMENT_STONES ) );
			float item_quantity = attached_item.GetQuantity();
		
			//show stones
			if ( item_quantity >= 8 && !IsOven() )
			{
				SetAnimationPhase( ANIMATION_STONES, 0 );
				
				//set burn parameters
				SetFuelBurnRateMP( FUEL_BURN_RATE_STONES );
				SetTemperatureLossMP( TEMPERATURE_LOSS_MP_STONES );				
			}
			else
			{
				SetAnimationPhase( ANIMATION_STONES, 1 );
			}
		}
		else
		{
			SetAnimationPhase( ANIMATION_STONES, 1 );
		}
		
		//ANIMATION_TRIPOD
		if ( IsItemTypeAttached( ATTACHMENT_TRIPOD ) )
		{
			SetAnimationPhase( ANIMATION_TRIPOD, 0 );
		}
		else
		{
			SetAnimationPhase( ANIMATION_TRIPOD, 1 );
		}
	}

	protected void RefreshFireParticlesAndSounds()
	{
		FireplaceFireState fire_state = GetFireState();
		
		if ( m_LastFireState != fire_state )
		{
			if ( fire_state == FireplaceFireState.START_FIRE )
			{
				//particles
				ParticleFireStartStart();
	
				//sounds
				SoundBurningStop();
				SoundBurningSmallStart();			
			}
			else if ( fire_state == FireplaceFireState.SMALL_FIRE )
			{
				//particles
				ParticleFireStartStop();
				
				ParticleSmallFireStart();
				if ( !IsOven() ) ParticleSmallSmokeStart();
				
				ParticleNormalFireStop();
				ParticleNormalSmokeStop();
				
				//sounds
				SoundBurningStop();
				SoundBurningSmallStart();
			}
			else if ( fire_state == FireplaceFireState.NORMAL_FIRE )
			{
				//particles
				ParticleFireStartStop();
				
				ParticleSmallFireStop();
				ParticleSmallSmokeStop();
				
				ParticleNormalFireStart();
				if ( !IsOven() ) ParticleNormalSmokeStart();	
	
				//sounds
				SoundBurningStop();
				SoundBurningNormalStart();
			}
			else if ( fire_state == FireplaceFireState.END_FIRE )
			{
				//particles
				ParticleFireStartStop();
				
				ParticleSmallFireStop();
				ParticleSmallSmokeStop();
	
				ParticleNormalFireStop();
				ParticleNormalSmokeStop();
	
				ParticleFireEndStart();
				
				//sounds
				SoundBurningStop();
				SoundBurningNoFuelStart();
			}
			else if ( fire_state == FireplaceFireState.EXTINGUISH_FIRE )
			{
				//particles
				ParticleFireStartStop();
				
				ParticleSmallFireStop();
				ParticleSmallSmokeStop();
	
				ParticleNormalFireStop();
				ParticleNormalSmokeStop();	
	
				ParticleFireEndStop();
				
				ParticleSteamEndStart();
				
				//sounds
				SoundBurningStop();
				SoundBurningNoFuelStart();			
			}
			else if ( fire_state == FireplaceFireState.REIGNITED_FIRE )
			{
				//particles
				ParticleFireStartStart();
				
				ParticleSmallFireStop();
				ParticleSmallSmokeStop();
	
				ParticleNormalFireStop();
				ParticleNormalSmokeStop();	
	
				ParticleFireEndStop();
				
				ParticleSteamEndStop();
				
				//sounds
				SoundBurningStop();
			}
			else if ( fire_state == FireplaceFireState.NO_FIRE )
			{
				//particles
				ParticleFireStartStop();
				
				ParticleSmallFireStop();
				ParticleSmallSmokeStop();
	
				ParticleNormalFireStop();
				ParticleNormalSmokeStop();	
	
				ParticleFireEndStop();
				
				ParticleSteamEndStop();
				
				//sounds
				SoundBurningStop();
			}
			
			m_LastFireState = fire_state;
		}		
	}
	
	//Fireplace fire intensity
	protected void RefreshFireState()
	{
		float temperature = GetTemperature();
		FireplaceFireState fire_state = GetFireState();
		
		//if it's burning
		if ( IsBurning() )
		{
			//Small fire
			if ( temperature < PARAM_SMALL_FIRE_TEMPERATURE )
			{
				if ( GetFireState() != FireplaceFireState.START_FIRE )
				{
					SetFireState( FireplaceFireState.START_FIRE );
				}
			}
			//Small fire
			else if ( temperature >= PARAM_SMALL_FIRE_TEMPERATURE && temperature < PARAM_NORMAL_FIRE_TEMPERATURE )
			{
				if ( GetFireState() != FireplaceFireState.SMALL_FIRE )
				{
					SetFireState( FireplaceFireState.SMALL_FIRE );
				}
			}
			//Normal fire
			else if ( temperature > PARAM_SMALL_FIRE_TEMPERATURE )
			{
				if ( GetFireState() != FireplaceFireState.NORMAL_FIRE )
				{
					SetFireState( FireplaceFireState.NORMAL_FIRE );
				}
			}			
		}
	}

	//================================================================
	// PARTICLES & SOUNDS
	//================================================================
	//returns true if particle started, false if not
	protected bool PlayParticle( out Particle particle, int particle_type, vector local_pos )
	{
		if ( !particle && GetGame() && ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
		{
			particle = Particle.Create( particle_type, this, local_pos );
			particle.Play();
			
			return true;
		}
		
		return false;
	}
	
	//returns true if particle stopped, false if not
	protected bool StopParticle( out Particle particle )
	{
		if ( particle && GetGame() && ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
		{
			particle.Stop();
			particle = NULL;
			
			return true;
		}
		
		return false;
	}
	
	//small fire
	protected void ParticleFireStartStart()
	{
		PlayParticle( m_ParticleFireStart, PARTICLE_FIRE_START, GetFireEffectPosition() );
	}
	
	protected void ParticleFireStartStop()
	{
		StopParticle( m_ParticleFireStart );
	}
	
	//small fire
	protected void ParticleSmallFireStart()
	{
		PlayParticle( m_ParticleSmallFire, PARTICLE_SMALL_FIRE, GetFireEffectPosition() );
	}
	
	protected void ParticleSmallFireStop()
	{
		StopParticle( m_ParticleSmallFire );
	}	
	
	//normal fire
	protected void ParticleNormalFireStart()
	{
		PlayParticle( m_ParticleNormalFire, PARTICLE_NORMAL_FIRE, GetFireEffectPosition() );
	}
	
	protected void ParticleNormalFireStop()
	{
		StopParticle( m_ParticleNormalFire );
	}
	
	//small smoke
	protected void ParticleSmallSmokeStart()
	{
		PlayParticle( m_ParticleSmallSmoke, PARTICLE_SMALL_SMOKE, GetSmokeEffectPosition() );
	}
	
	protected void ParticleSmallSmokeStop()
	{
		StopParticle( m_ParticleSmallSmoke );
	}
	
	//normal smoke
	protected void ParticleNormalSmokeStart()
	{
		PlayParticle( m_ParticleNormalSmoke, PARTICLE_NORMAL_SMOKE, GetSmokeEffectPosition() );
	}
	
	protected void ParticleNormalSmokeStop()
	{
		StopParticle( m_ParticleNormalSmoke );
	}
	
	//fire end
	protected void ParticleFireEndStart()
	{
		PlayParticle( m_ParticleFireEnd, PARTICLE_FIRE_END, GetFireEffectPosition() );
	}
	
	protected void ParticleFireEndStop()
	{
		StopParticle( m_ParticleFireEnd );
	}	

	//steam end
	protected void ParticleSteamEndStart()
	{
		PlayParticle( m_ParticleSteamEnd, PARTICLE_STEAM_END, GetFireEffectPosition() );
	}
	
	protected void ParticleSteamEndStop()
	{
		StopParticle( m_ParticleSteamEnd );
	}
	
	//cooking equipment steam
	protected void ParticleCookingEquipmentSteamStop()
	{
		if ( GetCookingEquipment() )
		{
			Bottle_Base cooking_pot = Bottle_Base.Cast( GetCookingEquipment() );
			cooking_pot.SteamStop();
		}
	}
	
	//Particle Positions
	//Get local fire and smoke effect position
	protected vector GetFireEffectPosition()
	{
		return Vector( 0, 0.05, 0 );
	}
	
	protected vector GetSmokeEffectPosition()
	{
		return Vector( 0, 0.05, 0 );
	}

	//================================================================
	// SOUNDS
	//================================================================
	//returns true if sound started, false if not
	protected bool PlaySound( out SoundOnVehicle sound, string sound_name, float range )
	{
		if ( !sound && GetGame() && ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
		{
			sound = PlaySoundLoop( sound_name, range );
			
			return true;
		}
		
		return false;
	}
	
	//returns true if sound stopped, false if not
	protected bool StopSound( out SoundOnVehicle sound )
	{
		if ( sound && GetGame() && ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
		{
			GetGame().ObjectDelete( sound );
			sound = NULL;
			
			return true;
		}
		
		return false;
	}	
	
	//Burning
	//Start
	protected void SoundBurningSmallStart()
	{
		PlaySound( m_SoundBurningLoop, SOUND_BURNING_LIGHT, 50 );
	}
	
	protected void SoundBurningNormalStart()
	{
		PlaySound( m_SoundBurningLoop, SOUND_BURNING_CAMP, 50 );
	}

	protected void SoundBurningNoFuelStart()
	{
		PlaySound( m_SoundBurningLoop, SOUND_BURNING_NO_FUEL, 50 );
	}	
	//Stop
	protected void SoundBurningStop()
	{
		StopSound( m_SoundBurningLoop );
	}
	
	//================================================================
	// FUEL / KINDLING
	//================================================================
	//Add to fire consumables
	protected void AddToFireConsumables ( ItemBase item )
	{
		float energy = GetFireConsumableTypeEnergy ( item );
		m_FireConsumables.Insert( item, new FireConsumable ( item, energy ) );
	}

	protected float GetFireConsumableTypeEnergy ( ItemBase item )
	{
		//Kindling
		ref FireConsumableType fire_consumable_type = m_FireConsumableTypes.Get( item.Type() );
		if ( fire_consumable_type )
		{
			return fire_consumable_type.GetEnergy();
		}
		
		return 0;
	}

	//Remove from fire consumables
	protected void RemoveFromFireConsumables ( FireConsumable fire_consumable )
	{
		if ( fire_consumable )
		{
			m_FireConsumables.Remove( fire_consumable.GetItem() );
			delete fire_consumable;
		}
	}
	
	protected FireConsumable GetFireConsumableByItem( ItemBase item )
	{
		return m_FireConsumables.Get( item );
	}

	//Set fuel / kindling to consume
	//Sets the item with the lowest energy value as item that will be consumed next
	protected void SetItemToConsume()
	{
		int fire_consumables_count = m_FireConsumables.Count();
		
		if ( fire_consumables_count == 0 )
		{
			m_ItemToConsume = NULL;	
			return;
		}
		
		float energy = 0;
		
		for ( int i = 0; i < fire_consumables_count; ++i )
		{
			ItemBase key = m_FireConsumables.GetKey( i );
			ref FireConsumable fire_consumable = m_FireConsumables.Get( key );
			energy = fire_consumable.GetEnergy();
			
			if ( i == 0 )
			{
				m_ItemToConsume = fire_consumable;
			}
			else
			{
				if ( fire_consumable.GetEnergy() < m_ItemToConsume.GetEnergy() )
				{
					m_ItemToConsume = fire_consumable;
				}	
			}
		}
	}

	protected FireConsumable GetItemToConsume()
	{
		return m_ItemToConsume;
	}

	//Spend item that is used as consumable for fire (fuel, kindling)
	//if 'amount == 0', the whole quantity will be consumed (quantity -= 1 )
	protected void SpendFireConsumable ( float amount )
	{
		//spend item
		ref FireConsumable fire_consumable = GetItemToConsume();
		
		if ( !fire_consumable )
		{
			//Set new item to consume
			SetItemToConsume();
		}
		else
		{
			ItemBase item = fire_consumable.GetItem();
			
			//set remaining energy
			fire_consumable.SetRemainingEnergy( fire_consumable.GetRemainingEnergy() - amount );
			
			//Debug
			/*
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			string message = "item type = " + item.GetType() + " m_RemainingEnergy = " + fire_consumable.GetRemainingEnergy().ToString() + " quantity = " + item.GetQuantity().ToString() + " amount = " + amount.ToString();
			player.MessageAction ( message );
			*/
			
			if ( fire_consumable.GetRemainingEnergy() <= 0 || amount == 0 )
			{
				//set ashes
				if ( !HasAshes() )
				{
					SetAshesState( true );
				}	
				
				if ( item.IsInherited( ItemBook ) || item.IsInherited( Paper ) )
				{
					RemoveFromFireConsumables( fire_consumable );
					GetGame().ObjectDelete( item );
				}
				else
				{
					//if there is nothing left, delete and remove old consumable, set new fire consumable
					if ( item.GetQuantity() <= 1 )
					{
						//remove item from fuel items
						RemoveFromFireConsumables( fire_consumable );
					}
					else
					{
						fire_consumable.SetRemainingEnergy( fire_consumable.GetEnergy() );
					}
					
					item.AddQuantity( -1 );					
				}
			}
			
			//refresh visual
			RefreshFireplaceVisuals();			
		}
	}

	//GetKindlingCount
	//Returns count of all kindling type items (define in 'm_KindlingTypes') attached to fireplace
	protected int GetKindlingCount()
	{
		int attachments_count = GetInventory().AttachmentCount();
		int fire_consumables_count = m_FireConsumables.Count();
		int kindling_count = 0;
		
		for ( int i = 0; i < attachments_count; i++ )
		{
			ItemBase item = ItemBase.Cast( GetInventory().GetAttachmentFromIndex( i ) );
			
			for ( int j = 0; j < fire_consumables_count; j++ )
			{
				ref FireConsumableType fire_consumable_type = m_FireConsumableTypes.Get( item.Type() );

				if ( fire_consumable_type && fire_consumable_type.IsKindling() )
				{
					kindling_count++;
					break;
				}
			}
		}
		
		return kindling_count;
	}

	//GetFuelCount
	//Returns count of all fuel type items (define in 'm_FuelTypes') attached to fireplace
	protected int GetFuelCount()
	{
		int attachments_count = GetInventory().AttachmentCount();
		int fire_consumables_count = m_FireConsumables.Count();
		int fuel_count = 0;
		
		for ( int i = 0; i < attachments_count; i++ )
		{
			ItemBase item = ItemBase.Cast( GetInventory().GetAttachmentFromIndex( i ) );
			
			for ( int j = 0; j < fire_consumables_count; j++ )
			{
				ref FireConsumableType fire_consumable_type = m_FireConsumableTypes.Get( item.Type() );

				if ( fire_consumable_type && !fire_consumable_type.IsKindling() )
				{
					fuel_count++;
					break;
				}
			}
		}
		
		return fuel_count;
	}

	//returns if item attached to fireplace is kindling
	protected bool IsKindling ( ItemBase item )
	{
		ref FireConsumableType fire_consumable_type = m_FireConsumableTypes.Get( item.Type() );
		if ( fire_consumable_type )
		{
			return fire_consumable_type.IsKindling();
		}
		
		return false;
	}

	//returns if item attached to fireplace is fuel
	protected bool IsFuel ( ItemBase item )
	{
		ref FireConsumableType fire_consumable_type = m_FireConsumableTypes.Get( item.Type() );
		if ( fire_consumable_type )
		{
			return !fire_consumable_type.IsKindling();
		}
		
		return false;
	}

	//Has attachments of given type
	bool IsItemTypeAttached( typename item_type )
	{
		if ( GetAttachmentByType( item_type ) )
		{
			return true;
		}
		
		return false;
	}

	//Has attachments of required quantity
	bool IsItemAttachedQuantity( typename item_type, float quantity )
	{
		ItemBase item = ItemBase.Cast( GetAttachmentByType( item_type ) );
		
		if ( item.GetQuantity() >= quantity )
		{
			return true;
		}
		
		return false;
	}

	//Has last attached item
	bool HasLastAttachment()
	{
		if ( GetInventory().AttachmentCount() == 1 )
		{
			return true;
		}
		
		return false;
	}

	//Has last fuel/kindling attached
	bool HasLastFuelKindlingAttached()
	{
		int fuel_kindling_count = GetFuelCount() + GetKindlingCount();

		if ( fuel_kindling_count == 1 )
		{
			return true;
		}
		
		return false;
	}

	
	//================================================================
	// FIRE STATE
	//================================================================
	//Is fireplace burning?
	bool IsBurning()
	{
		return m_IsBurning;
	}
	
	//Set burning state
	protected void SetBurningState( bool is_burning )
	{
 		if ( m_IsBurning != is_burning )
		{
			m_IsBurning = is_burning;
			
			//synchronize
			Synchronize();
		}
	}

	//Are ashes present in fireplace?
	bool HasAshes()
	{
		return m_HasAshes;
	}

	//Set ashes state
	protected void SetAshesState( bool has_ashes )
	{
		if ( m_HasAshes != has_ashes )
		{
			m_HasAshes = has_ashes;
			
			//synchronize
			Synchronize();
		}
	}

	//Is in oven state
	bool IsOven()
	{
		return m_IsOven;
	}		
	
	void SetOvenState( bool is_oven )
	{
		if ( m_IsOven != is_oven )
		{
			m_IsOven = is_oven;
			
			//synchronize
			Synchronize();
		}
	}
	
	//Temperature loss MP
	protected float GetTemperatureLossMP()
	{
		return m_TemperatureLossMP;
	}	
	
	protected void SetTemperatureLossMP( float value )
	{
		m_TemperatureLossMP = value;
	}
	
	//Fuel burn rate MP
	protected float GetFuelBurnRateMP()
	{
		return m_FuelBurnRateMP;
	}	
	
	protected void SetFuelBurnRateMP( float value )
	{
		m_FuelBurnRateMP = value;
	}
	
	//Get fire state 
	protected int GetFireState()
	{
		return m_FireState;
	}

	//Sets fire state
	protected void SetFireState( FireplaceFireState state )
	{
		if ( m_FireState != state )
		{
			m_FireState = state;
			
			//synchronize
			Synchronize();
		}
	}

	//================================================================
	// FIRE PROCESS
	//================================================================
	
	//Start the fire process
	// 1. start heating
	// 2. heating
	// 3. stop heating
	void StartFire()
	{
		//Print("Starting fire...");
		
		if ( !IsBurning() )	
		{
			//set fuel to consume
			SetItemToConsume();
			
			//set fire state
			SetBurningState( true );
			
			//turn light on
			SetPilotLight( true );
			
			//start heating
			StartHeating();
		}
	}

	//Start heating
	protected void StartHeating()
	{
		//Print("Starting heat...");
		
		//visual
		SetObjectMaterial ( 0, MATERIAL_FIREPLACE_GLOW );
		
		m_HeatingTimer = new Timer ( CALL_CATEGORY_GAMEPLAY );
		m_HeatingTimer.Run ( TIMER_HEATING_UPDATE_INTERVAL, this, "Heating", NULL, true );	
	}

	//Do heating
	protected void Heating()
	{
		//Debug
		/*
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		player.MessageAction ( "Heating..." );
		*/
		
		float wetness = GetWet();
		float temperature = GetTemperature();
		float temperature_modifier = 0;
		
		//check burning conditions
		if ( wetness > PARAM_BURN_WET_THRESHOLD )		//wetness condition
		{
			ExtinguishFire();
			return; 
		}
		else
		{
			if ( m_FireConsumables.Count() == 0 )		//fire consumable condition
			{
				StopFire();
				return; 				
			}
		}

		//DEBUG
		/*
		string s_message = "-> fire amount = " + PARAM_FIRE_CONSUM_RATE_AMOUNT.ToString() + " burn rate = " + GetFuelBurnRateMP().ToString() + " temp loss = " + GetTemperatureLossMP().ToString();
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		player.MessageAction (s_message);
		Print ( s_message );
		*/
		
		//spend actual fire consumable
		float amount = PARAM_FIRE_CONSUM_RATE_AMOUNT * GetFuelBurnRateMP();
		SpendFireConsumable ( amount );
		
		//set wetness if raining and alter temperature modifier (which will lower temperature increase because of rain)
		float rain = GetGame().GetWeather().GetRain().GetActual();
		if ( rain >= PARAM_BURN_WET_THRESHOLD && IsRainingAbove() )
		{
			//set wet to firplace
			wetness = wetness + ( PARAM_WET_INCREASE_COEF * rain );
			wetness = Math.Clamp ( wetness, 0, 1 );  //wetness <0-1>
			SetWet( wetness );
			
			//set temperature modifier
			temperature_modifier = PARAM_TEMPERATURE_INCREASE * rain;
		}
		else
		{
			wetness = wetness - PARAM_WET_DECREASE_COEF;
			wetness = Math.Clamp ( wetness, 0, 1 );  //wetness <0-1>
			SetWet( wetness );	
		}
		
		//calculate and set temperature
		temperature = temperature + PARAM_TEMPERATURE_INCREASE - temperature_modifier;		//calculate new temperature 
		if ( GetFuelCount() == 0 && temperature <= ( PARAM_SMALL_FIRE_TEMPERATURE + PARAM_TEMPERATURE_INCREASE ) )	//no fuel present, temperature should be low but there can be high temperature from previous fuel burning
		{
			temperature = Math.Clamp ( temperature, 0, PARAM_SMALL_FIRE_TEMPERATURE );		//small fire
		}
		else
		{
			temperature = Math.Clamp ( temperature, 0, PARAM_NORMAL_FIRE_TEMPERATURE );		//normal fire
		}
		SetTemperature( temperature );
		
		//check fire state
		RefreshFireState();
		
		//damage cargo items
		BurnItemsInCargo();
		
		//transfer heat to near players
		TransferHeatToNearPlayers();
		
		//manage cooking equipment
		if ( m_CookingEquipment )
		{
			float cook_equip_temp = m_CookingEquipment.GetTemperature();
			
			if ( cook_equip_temp >= PARAM_COOKING_TEMP_THRESHOLD )
			{
				//start cooking
				CookWithEquipment();
			}
			
			//DEBUG
			/*
			PlayerBase player = ( PlayerBase ) GetGame().GetPlayer();
			string s_message = "cooking equipment temp = " + ftoa(cook_equip_temp) + " | PARAM_COOKING_TEMP_THRESHOLD = " + ftoa(PARAM_COOKING_TEMP_THRESHOLD);
			if (player != NULL) {player.MessageAction (s_message);}
			Print ( s_message );
			*/
			//set temperature to cooking equipment
			cook_equip_temp = cook_equip_temp + PARAM_COOKING_EQUIP_TEMP_INCREASE;
			if ( temperature <= PARAM_SMALL_FIRE_TEMPERATURE )
			{
				cook_equip_temp = Math.Clamp ( cook_equip_temp, PARAM_CARGO_MIN_TEMP, PARAM_SMALL_FIRE_TEMPERATURE );
			}
			else
			{
				cook_equip_temp = Math.Clamp ( cook_equip_temp, PARAM_CARGO_MIN_TEMP, PARAM_COOKING_EQUIP_MAX_TEMP );
			}
			m_CookingEquipment.SetTemperature( cook_equip_temp );
		}
	}
	
	//Stop the fire process
	// 1. start cooling
	// 2. cooling
	// 3. stop cooling
	void StopFire()
	{
		//Debug
		//PlayerBase player = ( PlayerBase ) GetGame().GetPlayer();
		//if (player != NULL) {player.MessageAction ( "Stop fire..." );}
		//Print("Stop fire...");
		
		//Stop heating
		StopHeating();
		
		//spend item
		SpendFireConsumable ( 0 );
		
		//turn fire off
		SetBurningState ( false );
		
		//start cooling
		StartCooling();
		
		//Refresh fire visual
		SetFireState( FireplaceFireState.END_FIRE );
		
		//stop steam particle
		ParticleCookingEquipmentSteamStop();
	}
	
	//Extinguish fire process
	void ExtinguishFire()
	{
		//Stop heating
		StopHeating();
		
		//spend item
		SpendFireConsumable ( 0 );
		
		//turn fire off
		SetBurningState ( false );
		
		//start cooling
		StartCooling();
		
		//start steam
		SetFireState( FireplaceFireState.EXTINGUISH_FIRE );
	}
	
	//Stop heating
	protected void StopHeating()
	{
		//Debug
		//PlayerBase player = ( PlayerBase ) GetGame().GetPlayer();
		//if (player != NULL) {player.MessageAction ( "Stop heating..." );}
		//Print("Stop heating...");
		
		//Stop heating
		m_HeatingTimer.Stop();
	}	

	//Start cooling
	protected void StartCooling()
	{
		//Debug
		//PlayerBase player = ( PlayerBase ) GetGame().GetPlayer();
		//if (player != NULL) {player.MessageAction ( "Start cooling..." );}
		//Print("Start cooling...");
		
		//Start cooling
		m_CoolingTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		m_CoolingTimer.Run( TIMER_COOLING_UPDATE_INTERVAL, this, "Cooling", NULL, true );		
	}

	//Start cooling       
	protected void Cooling()
	{
		//Debug
		/*
		PlayerBase player = ( PlayerBase ) GetGame().GetPlayer();
		player.MessageAction ( "Cooling..." );
		Print("Cooling...");
		*/
		
		float wetness = GetWet();
		float temperature = GetTemperature();
		float temperature_modifier = 0;
		
		if ( this && !IsBurning() && temperature >= PARAM_MIN_FIRE_TEMPERATURE )
		{
			//check wetness
			//set wetness if raining and alter temperature modifier (which will lower temperature increase because of rain)
			float rain = GetGame().GetWeather().GetRain().GetActual();
			if ( IsRainingAbove() )
			{
				//set wet to firplace
				wetness = wetness + ( PARAM_WET_INCREASE_COEF * rain );
				wetness = Math.Clamp( wetness, 0, 1 );  //wetness <0-1>
				SetWet( wetness );
				
				//set temperature modifier
				temperature_modifier = temperature_modifier + ( PARAM_TEMPERATURE_DECREASE * rain );
			}
			else		//subtrackt wetness
			{
				wetness = wetness - PARAM_WET_DECREASE_COEF;
				wetness = Math.Clamp( wetness, 0, 1 );  //wetness <0-1>
				SetWet( wetness );			
			}
			
			//calculate already obtained wetness (e.g. extinguished by water)
			temperature_modifier = temperature_modifier + ( PARAM_TEMPERATURE_DECREASE * wetness );
			
			//calculate and set temperature (decrease)
			temperature = temperature - PARAM_TEMPERATURE_DECREASE - ( temperature_modifier * GetTemperatureLossMP() );		//calculate new temperature 
			temperature = Math.Clamp ( temperature, 0, PARAM_NORMAL_FIRE_TEMPERATURE );		//cannot be bigger than normal fire temperature and lower than zero
			SetTemperature( temperature );		
			
			//damage cargo items
			BurnItemsInCargo();
			
			//transfer heat to near players
			TransferHeatToNearPlayers();			
			
			//cook with equipment if temperature of equipment is still high
			if ( m_CookingEquipment )
			{
				float cook_equip_temp = m_CookingEquipment.GetTemperature();
				
				if ( cook_equip_temp >= PARAM_COOKING_TEMP_THRESHOLD )
				{
					//cook
					CookWithEquipment();
				}
			}
			
			//Debug
			//PlayerBase player = ( PlayerBase ) GetGame().GetPlayer();
			//if (player != NULL) {player.MessageAction ( temperature.ToString() );}
		
			//turn off glow effect
			if ( temperature <= PARAM_MIN_TEMP_TO_REIGNITE && IsPilotLight() )
			{
				//visual
				SetObjectMaterial( 0, MATERIAL_FIREPLACE_NOGLOW );
				
				//turn light off
				SetPilotLight( false );
			}
		}
		else
		{
			//stop cooling
			StopCooling();
		}
	}

	protected void StopCooling()
	{
		//Debug
		//PlayerBase player = ( PlayerBase ) GetGame().GetPlayer();
		//if (player != NULL) {player.MessageAction ( "Stop cooling..." );}
		//Print("Stop cooling...");
		
		//Stop cooling
		m_CoolingTimer.Stop();
		
		//stop all fire visuals
		SetFireState( FireplaceFireState.NO_FIRE );
	}
	
	//================================================================
	// COOKING
	//================================================================
	protected void CookWithEquipment()
	{
		if ( m_CookingProcess == NULL )
		{
			m_CookingProcess = new Cooking();
		}
		
		m_CookingProcess.CookWithEquipment ( m_CookingEquipment );
	}
	
	Cooking GetCookingProcess()
	{
		if ( m_CookingProcess == NULL )
		{
			m_CookingProcess = new Cooking();
		}
		
		return m_CookingProcess;
	}
	
	//================================================================
	// FIRE VICINITY
	//================================================================
	//apply damage to all items, raise temperature to only specific items [in cargo]
	protected void BurnItemsInCargo()
	{
		Cargo cargo = GetInventory().GetCargo();
		
		for ( int i = 0; i < cargo.GetItemCount(); i++ )
		{
			ItemBase item = ItemBase.Cast( cargo.GetItem ( i ) );
			//set damage
			item.DecreaseHealth ( "", "", PARAM_BURN_DAMAGE_COEF * 100 );
			
			//set temperature if maximum temperature for this item is high enough
			if ( item.GetTemperatureMax() >= PARAM_CARGO_MIN_TEMP )
			{
				float temperature = item.GetTemperature() + PARAM_CARGO_TEMP_INCREASE_COEF;
				temperature = Math.Clamp ( temperature, PARAM_CARGO_MIN_TEMP, PARAM_MAX_CARGO_TEMP_INCREASE );
				item.SetTemperature( temperature );
			}
			
			//set burning state for foods
			if ( item.CanBeCooked() )
			{
				Edible_Base edible_item = Edible_Base.Cast( item );
				edible_item.ChangeFoodStage( FoodStageType.BURNED );
			}
		}
	}

	//transfer heat to all nearby players
	protected void TransferHeatToNearPlayers()
	{
		ref array<Object> nearest_objects = new array<Object>;
		ref array<Cargo> proxy_cargos = new array<Cargo>;
		GetGame().GetObjectsAtPosition ( GetPosition(), PARAM_HEAT_RADIUS, nearest_objects, proxy_cargos ); 
		
		for ( int i = 0; i < nearest_objects.Count(); i++ )
		{
			if ( nearest_objects.Get ( i ).IsInherited( PlayerBase ) )
			{
				PlayerBase player = PlayerBase.Cast( nearest_objects.Get( i ) );
				
				float wetness = player.GetStatWet().Get();
				float distance = vector.Distance ( player.GetPosition(), GetPosition() );
				
				//wet
				if ( wetness > 0 )
				{
					float heat = PARAM_HEAT_WET_DECREASE_COEF * ( PARAM_HEAT_RADIUS - distance );
					wetness = wetness - heat;
					wetness = Math.Clamp ( wetness, 0, 1 );
					player.GetStatWet().Set(wetness);
				}
			}
		}
	}

	//================================================================
	// SUPPORT
	//================================================================
	//Check if object is under a roof (height check)
	bool LineHit( float height )
	{
		vector from = GetPosition();
		vector to = GetPosition();
		from[1] = from[1] + 2;		//add fireplace height into calculation
		to[1] = to[1] + height;
		vector contactPos;
		vector contactDir;
		int contactComponent;
		
		return DayZPhysics.RaycastRV( from, to, contactPos, contactDir, contactComponent, NULL, NULL, this );
	}

	//Is player facing fireplace
	bool IsFacingFireplace( PlayerBase player )
	{
		vector fireplace_pos = GetPosition();
		vector player_pos = player.GetPosition();
		vector dir_vec = vector.Direction( fireplace_pos, player_pos );	//calculate direction vector
		dir_vec.Normalize();										//normalize direction vector
		
		//check zero vector
		if ( dir_vec.Length() == 0 )
		{
			return false;
		}
		
		float dot = vector.Dot ( player.GetOrientation(), dir_vec );
		if ( Math.Acos ( dot ) < 0.436332313 ) 		//0.436332313 => 25 degrees 
		{
			return true;
		}
		
		return false;
	}
	
	//================================================================
	// ACTIONS
	//================================================================
	// --- Actions / Action conditions
	//Check if there is some kindling attached
	bool HasAnyKindling()
	{
		if ( GetKindlingCount() > 0 )
		{
			return true;
		}
		
		return false;
	}

	//Check if the weather is too windy
	bool IsWindy()
	{
		//check wind
		float wind_speed = GetGame().GetWeather().GetWindSpeed();
		
		//calculate probability
		//fail
		float probability_fail = ( ( wind_speed / 10 ) * 0.5 );
		probability_fail = Math.Clamp ( probability_fail, 0, 1 ); //clamp 0-1
		//success
		float probability_success = Math.RandomFloat01();
		probability_success = Math.Clamp ( probability_success, 0.3, 1 ); //clamp 0.5-1
		
		if ( probability_fail > probability_success )
		{
			return true;
		}
		
		return false;
	}

	//Check if the fireplace is too wet to be ignited
	bool IsWet()
	{
		if ( GetWet() >= PARAM_MAX_WET_TO_IGNITE )
		{
			return true;
		}
		
		return false;
	}

	//Check if there is any roof above fireplace
	bool IsRoofAbove()
	{
		return LineHit ( 10 );
	}

	//Check if it's raining and there is only sky above fireplace
	bool IsRainingAbove()
	{
		//check rain
		if ( GetGame() && GetGame().GetWeather().GetRain().GetActual() >= PARAM_IGNITE_RAIN_THRESHOLD && !LineHit ( 100 ) )
		{
			return true;
		}
		
		return false;
	}

	//Check there is water surface bellow fireplace
	bool IsWaterSurface()
	{
		//check surface
		string surface_type;
		vector fireplace_pos = GetPosition();
		
		GetGame().SurfaceGetType ( fireplace_pos[0], fireplace_pos[2], surface_type ); 
		if ( surface_type == "FreshWater" || surface_type == "sea" || GetGame().SurfaceIsSea( fireplace_pos[0], fireplace_pos[2] ) )
		{
			return true;
		}
		
		return false;
	}

	//Checks if has not additional items in it
	override bool IsEmpty()
	{
		//check cargo and attachments
		Cargo cargo = GetInventory().GetCargo();
		if ( cargo.GetItemCount() == 0 && GetInventory().AttachmentCount() == 0 )
		{
			return true;			
		}
		
		return false;
	}
	
	//Checks if it has no items in cargo
	bool IsCargoEmpty()
	{
		//check cargo
		Cargo cargo = GetInventory().GetCargo();
		if ( cargo.GetItemCount() == 0 )
		{
			return true;			
		}
		
		return false;
	}
	
	//Check if object is in animation phase
	bool IsInAnimPhase( string anim_phase )
	{
		if ( GetAnimationPhase ( anim_phase ) == 0 )
		{
			return true;
		}
		
		return false;
	}
	
	//Action condition for building oven
	bool CanBuildOven()
	{
		ItemBase attached_item = ItemBase.Cast( GetAttachmentByType( ATTACHMENT_STONES ) );
		if ( attached_item )
		{
			float item_quantity = attached_item.GetQuantity();
			
			if ( !IsOven() && !IsBurning() && item_quantity >= MIN_STONES_TO_BUILD_OVEN && !IsItemTypeAttached( ATTACHMENT_TRIPOD ) )
			{
				return true;
			}
		}
	
		return false;
	}
	
	//Action condition for dismantling oven
	bool CanDismantleOven()
	{
		if ( IsOven() && !IsBurning() && !GetCookingEquipment() && GetTemperature() <= MAX_TEMPERATURE_TO_DISMANTLE_OVEN )
		{
			return true;
		}
		
		return false;
	}
	
	bool CanCookOnStick()
	{
		if ( GetTemperature() >= PARAM_COOKING_TEMP_THRESHOLD )
		{
			return true;
		}
		
		return false;
	}
}
