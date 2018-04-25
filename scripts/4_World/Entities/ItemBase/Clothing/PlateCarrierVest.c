class PlateCarrierComplete2 : Clothing
{
	int selectionIndex;
	
	void PlateCarrierComplete2()
	{
		//HideSelection("vestholster");
		//HideSelection("vestpouch");
		selectionIndex = GetHiddenSelectionIndex("vestholster");
		SetObjectTexture(selectionIndex,"");
		selectionIndex = GetHiddenSelectionIndex("vestpouch");
		SetObjectTexture(selectionIndex,"");
	}
	
	override bool IsClothing()
	{
		return true;
	}
	
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		TStringArray textureArray = new TStringArray;
		string texture;
		GetGame().ConfigGetTextArray("cfgVehicles PlateCarrierComplete2 hiddenSelectionsTextures", textureArray);
		
		switch (slot_name)
		{
			case("vestholster"):
				//ShowSelection("vestholster");
				selectionIndex = GetHiddenSelectionIndex("vestholster");
				texture = textureArray.Get(selectionIndex);
				SetObjectTexture(selectionIndex,texture);
			break;
			
			case("vestpouch"):
				//ShowSelection("vestpouch");
				selectionIndex = GetHiddenSelectionIndex("vestpouch");
				texture = textureArray.Get(selectionIndex);
				SetObjectTexture(selectionIndex,texture);
			break;
		}
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		switch (slot_name)
		{
			case("vestholster"):
				//HideSelection("vestholster");
				selectionIndex = GetHiddenSelectionIndex("vestholster");
				SetObjectTexture(selectionIndex,"");
			break;
			
			case("vestpouch"):
				//HideSelection("vestpouch");
				selectionIndex = GetHiddenSelectionIndex("vestpouch");
				SetObjectTexture(selectionIndex,"");
			break;
		}
	}
}
