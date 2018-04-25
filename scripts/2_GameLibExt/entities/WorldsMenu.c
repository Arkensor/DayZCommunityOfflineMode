#ifdef GAME_TEMPLATE

[EditorAttribute("box", "GameLib/Scripted", "Worlds menu", "-0.25 -0.25 -0.25", "0.25 0.25 0.25", "255 0 0 255")]
class WorldsMenuClass
{

}

WorldsMenuClass WorldsMenuSource;

class WorldsMenu: GenericEntity
{
	int m_DbgListSelection = 0;
	int m_WorldsCount = 0;
	string DEFAULT_WORLD = "worlds/default.ent";
	ref array<string> m_DbgOptions = {};
	ref ImageWidget m_MouseWidget;
	
	void WorldsMenu(IEntitySource src, IEntity parent)
	{
		SetFlags(EntityFlags.ACTIVE, false);
		SetEventMask(EntityEvent.POSTFRAME);
		
		Class.CastTo(m_MouseWidget, GetGame().GetWorkspace().CreateWidgets("gui/layouts/mouse.layout"));
		m_MouseWidget.SetSort(1024);
		SetCursorWidget(m_MouseWidget);
		
		LoadWorlds();
	}
	
	void ~WorldsMenu()
	{
		delete m_MouseWidget;
	}

	array<string> GetWorldList()
	{
		return m_DbgOptions;
	}

	override void EOnPostFrame(IEntity other, int extra) //EntityEvent.POSTFRAME
	{
		InputManager im = GetGame().GetInputManager();
		im.ActivateContext("MenuContext");
		bool menuSelect = im.GetActionTriggered("MenuSelect");
		bool menuBack = im.GetActionTriggered("MenuBack");
		
		DbgUI.Begin("Load world", 400, 100);

		DbgUI.Text("Select world to load from worlds directory");

		if (m_DbgOptions.Count() > 0)
		{
			DbgUI.List("Worlds", m_DbgListSelection, m_DbgOptions);
			if (DbgUI.Button("Start") || menuSelect)
			{
				string worldToLoad = m_DbgOptions.Get(m_DbgListSelection);
				GetGame().SetWorldFile(worldToLoad, true);
			}
		}
		if (DbgUI.Button("Exit") || menuBack)
		{
			GetGame().RequestClose();
		}		
		DbgUI.End();
	}

	void LoadWorlds()
	{
		string fileName;
		FileAttr fileAttr;
		FindFileHandle worlds = FindFile("worlds/*.ent", fileName, fileAttr, 0);
		
		if (!worlds)
			return;

		InsertWorldToList(fileName);

		while(FindNextFile(worlds, fileName, fileAttr))
		{
			InsertWorldToList(fileName);
		}

		CloseFindFile(worlds);

		m_WorldsCount = m_DbgOptions.Count();
	}

	void InsertWorldToList(string fileName)
	{
		string path = String("worlds/" + fileName);
		if (path != DEFAULT_WORLD)
			m_DbgOptions.Insert(String(path));
	}
}

#endif