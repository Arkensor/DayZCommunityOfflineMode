class Watchtower extends BaseBuildingBase
{	
	void Watchtower()
	{
		// Electric watchtower functionality is WIP!
		
		m_DmgTrgLocalPos				= { "0 0.50 2.6" , "-2.6 0.50 0" , "2.6 0.50 0" }; // {"<right> <up> <forward>", "<right> <up> <forward>", "<right> <up> <forward>"} model coordinates
		m_DmgTrgLocalDir 				= {0, -90, 90};
		
		//init construction base parts
		m_BaseParts = new array<string>;
		m_BaseParts.Insert( "level_1_base" );
	}
	
	void ~Watchtower()
	{
	}
	
	// --- ATTACHMENTS
	override void EEItemAttached ( EntityAI item, string slot_name )
	{
		super.EEItemAttached ( item, slot_name );
	}
	
	override void EEItemDetached ( EntityAI item, string slot_name )
	{
		super.EEItemDetached ( item, slot_name );
	}
}