class COMCharacterSave extends UIScriptedMenu
{
	protected ref PersistencyModule 		m_oPersistencyModule;

    void COMCharacterSave( ref PersistencyModule oPersistencyModule )
	{
		m_oPersistencyModule = oPersistencyModule;
	}

    void ~COMCharacterSave()
    {
		g_Game.SetKeyboardHandle(NULL);
    }
	
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\Persistency\\gui\\layouts\\COMCharacterSave.layout" );

        return layoutRoot;
    }

}