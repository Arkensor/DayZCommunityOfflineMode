class ActionConstructCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_CONSTRUCT);
	}
};

class ActionConstruct: ActionContinuousBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = 0.03;
	float m_DamageAmount;
	string m_SlotName;
	
	void ActionConstruct()
	{
		m_CallbackClass = ActionConstructCB;
		m_MessageStartFail = "I cannot build a construction part.";
		m_MessageStart = "I have build a construction part.";
		m_MessageSuccess = "I have build a construction part.";
		m_MessageFail = "I have failed to build a construction part.";
		m_DamageAmount = 2;
		//m_Animation = "startFire";
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_CONSTRUCT;
	}
		
	override string GetText()
	{
		return "construct";		//add name of the construction part if possible
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object targetObject = target.GetObject();
		if ( targetObject.CanUseConstruction() )
		{
			string slot_name = targetObject.GetActionComponentName(target.GetComponentIndex());
			
			BaseBuildingBase base_building = BaseBuildingBase.Cast( targetObject );
			Construction construction = base_building.GetConstruction();
			
			if ( construction.CanConstructPart( slot_name ) )
			{
				//debug
				//player.MessageAction( slot_name );
				
				return true;
			}
		}
		
		return false;
	}
	
	override void OnStartServer( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		
		//Get actual construcion slot
		string slot_name = targetObject.GetActionComponentName(target.GetComponentIndex());
		SetSlotName( slot_name );
	}
	
	override void OnStartClient( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		
		//Get actual construcion slot
		string slot_name = targetObject.GetActionComponentName(target.GetComponentIndex());
		SetSlotName( slot_name );
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		BaseBuildingBase base_building = BaseBuildingBase.Cast( target.GetObject() );
		Construction construction = base_building.GetConstruction();
		string part_name = construction.GetConstructionPartName( GetSlotName() );
		
		if ( construction.HasMaterials( GetSlotName(), part_name ) )
		{
			//construct
			construction.ConstructPart( GetSlotName(), part_name );
			
			//add damage to tool
			item.DecreaseHealth ( "", "", m_DamageAmount, true );
		}
		else
		{
			SendMessageToClient( player, base_building.MESSAGE_NOT_ENOUGH_MATERIALS );
		}

		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
	
	protected void SetSlotName( string slot_name )
	{
		m_SlotName = slot_name;
	}
	protected string GetSlotName()
	{
		return m_SlotName;
	}
}