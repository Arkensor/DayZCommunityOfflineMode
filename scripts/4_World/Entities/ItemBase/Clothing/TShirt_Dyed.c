class TShirt_Dyed : Clothing
{
	void TShirt_Dyed()
	{
		/*
		string color = GetItemVariableString("varColor"); //SYNCFAIL
		if ( color != "" )
		{
			SetObjectTexture (0,color);
			SetObjectTexture (1,color);
			SetObjectTexture (2,color);
		}
		*/
	}
	
	override bool IsClothing()
	{
		return true;
	}
}
