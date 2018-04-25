typedef int[] WBModuleDef;
typedef int[] ScriptEditor;
typedef int[] ResourceBrowser;
typedef int[] WorldEditor;

class Workbench
{
	static proto native WBModuleDef GetModule(string type);
	static proto native bool OpenModule(string type);
	static proto native bool CloseModule(string type);
	static proto native void Dialog(string caption, string text);
	static proto int ScriptDialog(string caption, string text, Class data);
	static proto bool SearchResources(string filter, func callback);
	static proto native int RunCmd(string command, bool wait = false);
	static proto void GetCwd(out string currentDir);
	static proto bool GetAbsolutePath(string relativePath, out string absPath);
};

class WBModuleDef
{
	proto native external bool SetOpenedResource(string filename);
	proto native external int GetNumContainers();
	proto native external BaseContainer GetContainer(int index = 0);
	proto external bool GetCmdLine(string name, out string value);
	proto native external bool Save();
	proto native external bool Close();
};

class ScriptEditor: WBModuleDef
{
	proto external bool GetCurrentFile(out string filename);
	proto native external int GetCurrentLine();
};

class ResourceBrowser: WBModuleDef
{
	proto external bool GetCurrentFile(out string filename);
};

class WorldEditor: WBModuleDef
{
	proto native external WorldEditorAPI GetAPI();
};

class WorldEditorAPI
{
	proto native bool BeginTerrainAction(string historyPointName = "", string historyPointIcon = "");
	proto native void EndTerrainAction(string historyPointName = "");
	proto native bool BeginEntityAction(string historyPointName = "", string historyPointIcon = "");	//begin of logical edit action
	proto native bool EndEntityAction(string historyPointName = "");	//end of edit action
	proto native bool IsDoingEditAction();	//true, if code stay betwen BeginEntityAction() and EndEntityAction()
	proto native bool UndoOrRedoIsRestoring();		//true, if editor is restoring undo or redo state
	proto native bool IsModifyingData();
	proto native IEntity SourceToEntity(IEntitySource entSrc);
	proto native IEntitySource EntityToSource(IEntity ent);
	proto native IEntitySource FindEntityByName(string name);
	
	proto native external void SetEntitySelection(IEntity ent);
	proto native external void AddToEntitySelection(IEntity ent);
	proto native void ClearEntitySelection();
	proto native void RemoveFromEntitySelection(IEntity ent);
	proto native void SetPropertySelection(string id);
	
	proto native external bool ModifyEntityKey(IEntity ent, string key, string value);
	proto native external bool ModifyEntityTemplateKey(IEntitySource tmpl, string key, string value);
		
	proto native external IEntity CreateEntity(string className, string name, int layerId, vector coords, vector angles);
	proto native external IEntity CreateClonedEntity(IEntity ent, string name);
	proto native external bool DeleteEntity(IEntity ent);
	proto native bool DeleteEntities(out array<IEntity> ents);
	proto native IEntity GetEntityUnderCursor();
	
	proto native external bool TraceWorldPos(int x, int y, int traceFlags, out vector traceStart, out vector traceEnd, out vector traceDir);

	proto native int GetSelectedEntitiesCount();
	proto native IEntity GetSelectedEntity(int n = 0);
	
	private void WorldEditorAPI() {}
	private void ~WorldEditorAPI() {}	
};

class WorldEditorTool
{
	//! Filled by workbench
	WorldEditorAPI m_API;
	
	void OnKeyPressEvent(int key) {}
	void OnKeyReleaseEvent(int key) {}
	void OnEnterEvent() {}
	void OnLeaveEvent() {}
	void OnMouseMoveEvent(float x, float y) {}
	void OnMouseDoubleClickEvent(float x, float y) {}
	void OnMousePressEvent(float x, float y) {}
	void OnMouseReleaseEvent(float x, float y) {}
	void OnWheelEvent(int delta) {}
	
	private void WorldEditorTool() {}
	private void ~WorldEditorTool() {}
};

class WorkbenchPlugin
{
	void Run() {}
	void RunCommandline() {}
	void Configure() {}
};

class ButtonAttribute
{
	string m_Label;
	bool m_Focused;
	
	void ButtonAttribute(string label = "ScriptButton", bool focused = false)
	{
		m_Label = label;
		m_Focused = focused;
	}
};

/*!
Attribute for Workbench plugin definition:
	name - ui name in Script Tools menu
	description - tooltip
	shortcut - shortcut in simple text form e.g. "ctrl+g"
	icon - relative path to icon file (32x32 png)
	wbModules - list of strings representing Workbench modules where this tool should be avalaible (e.g. {"ResourceManager", "ScriptEditor"}). Leave null or empty array for any module.
*/
class WorkbenchPluginAttribute
{
	string m_Name;
	string m_Icon;
	string m_Shortcut;
	string m_Description;
	ref array<string> m_WBModules;
		
	void WorkbenchPluginAttribute(string name, string description = "", string shortcut = "", string icon = "", array<string> wbModules = null)
	{
		m_Name = name;
		m_Icon = icon;
		m_Shortcut = shortcut;
		m_Description = description;
		m_WBModules = wbModules;
	}
};

/*!
Attribute for Workbench tool definition
*/
class WorkbenchToolAttribute: WorkbenchPluginAttribute
{
}
