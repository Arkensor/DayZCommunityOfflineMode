enum EUniversal_Bandana_Redpattern_stages
{
	UNTIED,
	MASK,
	HEADHEAR,
	MAX
}

class Universal_Bandana_Redpattern extends ClothingBase
{	
	void Universal_Bandana_Redpattern()
	{
		//SetItemStage(EUniversal_Bandana_Redpattern_stages.UNTIED); //starts at stage 0
	}
	
	override bool CanPutAsAttachment( EntityAI parent )
	{
		if(!super.CanPutAsAttachment(parent)) {return false;}
		if ( parent.IsMan() )// TODO
		{
			return true;
		}
		return false;
	}
	
	override void OnWasAttached( EntityAI parent, string slot_name )
	{
		super.OnWasAttached(parent, slot_name);
		
		switch (slot_name)
		{
			case "mask":
				SetItemStage(EUniversal_Bandana_Redpattern_stages.MASK);
			break;
			case "headgear":
				SetItemStage(EUniversal_Bandana_Redpattern_stages.HEADHEAR);
			break;
		}
		Print("stage_attach = " + m_Item_Stage);
	}
	
	override void OnWasDetached( EntityAI parent, string slot_name )
	{
		super.OnWasDetached(parent, slot_name);
		
		SetItemStage(EUniversal_Bandana_Redpattern_stages.UNTIED);
		Print("stage_unattach = " + m_Item_Stage);
	}
}