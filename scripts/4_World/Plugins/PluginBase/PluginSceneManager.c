class PluginSceneManager extends PluginBase
{
	static string		SCENE_DEFAULT_NAME		= "default";
	static string		SCENE_SUFIX				= "scene";
	static PlayerBase	PLAYER;
	static PluginSceneManager instance;
	
	bool			m_RulerActivated;
	ref array<vector>	m_RulerPoints;
	ref array<Shape>	m_RulerLines;
	ref array<Shape>	m_RulerSpheres;
	
	static const int SCENE_EDITOR_CMD_REFRESH	= 0;
	static const int SCENE_EDITOR_CMD_POPUP		= 1;
	static const int SCENE_EDITOR_CMD_SAVE		= 2;
	
	//---------------------------------------------------------------------------------
	// >> Public scope

	// System Events
	void PluginSceneManager()
	{
		if ( instance == NULL )
		{
			instance = this;
		}
	}
	
	// System Events
	void ~PluginSceneManager()
	{
		if ( instance == NULL )
		{
			instance = this;
		}
	}
	
	static PluginSceneManager GetInstance()
	{
		return PluginSceneManager.Cast( GetPlugin( PluginSceneManager ) );
	}

	//==========================================
	// OnInit (System Event)
	//==========================================
	override void OnInit()
	{
		m_ModuleDeveloper			= PluginDeveloper.Cast( GetPlugin(PluginDeveloper) );
		m_ModuleConfigScene			= PluginConfigScene.Cast( GetPlugin(PluginConfigScene) );
		m_ModuleConfigDebugProfile	= PluginConfigDebugProfile.Cast( GetPlugin(PluginConfigDebugProfile) );
		
		//Ruler
		m_RulerActivated = false;
		m_RulerPoints	= new array<vector>;
		m_RulerLines	= new array<Shape>;
		m_RulerSpheres	= new array<Shape>;
	}

	//==========================================
	// OnDestroy (destructor)
	//==========================================
	override void OnDestroy()
	{
		SceneUnload();
	}
	
	//==========================================
	// OnMouseWheel
	//==========================================
	void OnMouseWheel(int wheel)
	{
		if ( !IsMouseInScene() )
		{
			return;
		}
		
		if ( GetSelectedSceneObject() )
		{
			GetSelectedSceneObject().AddRotation(GetRotationAngle() * wheel);
		}
		
		EditorUpdate();
	}

	//==========================================
	// OnUIEditorOpened
	//==========================================
	void OnUIEditorOpened()
	{	
		m_IsOpen = true;

		DeveloperFreeCamera.EnableFreeCameraSceneEditor( PlayerBase.Cast( GetGame().GetPlayer() ) );
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.FreeCameraControlDisable);
		
		EditorUpdate();
	}

	//==========================================
	// OnUIEditorClosed
	//==========================================
	void OnUIEditorClosed()
	{
		m_IsOpen = false;
		
		FreeCameraControlEnable();
		
		m_ModuleDeveloper.ToggleFreeCameraBackPos();
		
		EditorUpdate();
	}
	
	//::::::::::::::::::::::::::::::::::::::::::
	// Event_OnClick
	//::::::::::::::::::::::::::::::::::::::::::
	void Event_OnClick()
	{
		if ( !IsMouseInScene() )
		{
			return;
		}
			
		// Direction Vector from current active Camera
		vector dir = GetGame().GetPointerDirection();
		
		// Raycast from
		vector from = FreeDebugCamera.GetInstance().GetPosition(); 
		// Raycast to
		vector to = from + ( dir * 10000 );
		// Raycast out parameter
		vector contact_pos;
		vector contact_dir;
		int contact_component;
		set<Object> objects = new set<Object>;
		bool selected = false;
		
		if ( DayZPhysics.RaycastRV(from, to, contact_pos, contact_dir, contact_component, objects) )   
		{
			for ( int i = 0; i < objects.Count(); ++i )
			{
				Object obj = objects.Get(i);
				
				if ( obj.IsInherited( EntityAI ) )
				{
					SceneObject sc_object = SceneObject.Cast( m_SceneData.GetSceneObjectByEntityAI(EntityAI.Cast( obj ) ) );
					
					if ( sc_object == NULL )
					{
						//sc_object = m_SceneData.AddObject(obj);
						return;
					}
					
					SelectObject( sc_object );
					selected = true;
					
					break;
				}
			}
		}
		
		if ( !selected )
		{
			DeselectObject();
			
			//	Ruler
			if ( IsRulerActivated() && GetSelectedSceneObject() == NULL )
			{
				RulerAddPoint();
			}
		}
		
		EditorUpdate();
	}

	//::::::::::::::::::::::::::::::::::::::::::
	// Event_OnClickMiddle
	//::::::::::::::::::::::::::::::::::::::::::
	void Event_OnClickMiddle()
	{
		if ( !IsMouseInScene() )
		{
			return;
		}
		
		if ( GetSelectedSceneObject() != NULL )
		{
			// Direction Vector from current active Camera
			vector dir = GetGame().GetPointerDirection();
			
			// Raycast from
			vector from = FreeDebugCamera.GetInstance().GetPosition(); 
			// Raycast to
			vector to = from + ( dir * 10000 );   
			// Raycast out parameter
			vector contact_pos;
			vector contact_dir;
			int contact_component;
			set<Object> objects = new set<Object>;
			bool selected = false;
			
			if ( DayZPhysics.RaycastRV(from, to, contact_pos, contact_dir, contact_component, objects) )   
			{
				for ( int i = 0; i < objects.Count(); ++i )
				{
					Object obj = objects.Get(i);
					
					if ( obj.IsInherited( EntityAI ) )
					{
						EntityAI eai = EntityAI.Cast( obj );
						SceneObject scene_object = m_SceneData.GetSceneObjectByEntityAI(eai);
						
						if ( !scene_object.IsPlayer() )
						{
							if ( scene_object != NULL && scene_object != GetSelectedSceneObject() )
							{
								if ( !AreSceneObjectsLinked(GetSelectedSceneObject(), scene_object) ) 
								{
									LinkSceneObjects( GetSelectedSceneObject(), scene_object );
								}
								else
								{
									UnlinkSceneObjects( GetSelectedSceneObject(), scene_object );
								}
							}
						}
						else
						{
							SceneEditorMenuCmdPopup("I am sorry. You cant create link on player!");
						}
					}
				}
			}
		}
		
		EditorUpdate();
	}

	//::::::::::::::::::::::::::::::::::::::::::
	// Event_OnDrag
	//::::::::::::::::::::::::::::::::::::::::::
	void Event_OnDrag()
	{
		if ( !IsMouseInScene() )
		{
			return;
		}
		
		if ( GetSelectedSceneObject() != NULL )
		{
			// Direction Vector from current active Camera
			vector dir = GetGame().GetPointerDirection();
			
			// Raycast from
			vector from = FreeDebugCamera.GetInstance().GetPosition(); 
			// Raycast to
			vector to = from + ( dir * 10000 );   
			// Raycast out parameter
			vector contact_pos;
			vector contact_dir;
			int contact_component;
			
			//Raycast(vector begPos, vector endPos, out vector contactPos, out vector contactDir, out int contactComponent, out set<Object> results = NULL, Object with = NULL, Object ignore = NULL,  bool sorted = false,  bool ground_only = false,  int iType = ObjIntersectView, float radius = 0.0);
			
			if ( DayZPhysics.RaycastRV(from, to, contact_pos, contact_dir, contact_component, NULL, NULL, NULL, false, true) )   
			{
				GetSelectedSceneObject().SetPosition(contact_pos);
			}
			
			EditorUpdate();
		}
	}

	//::::::::::::::::::::::::::::::::::::::::::
	// Event_OnDoubleClick
	//::::::::::::::::::::::::::::::::::::::::::
	SceneObject Event_OnDoubleClick()
	{
		if ( !IsMouseInScene() )
		{
			return NULL;
		}
		
		if ( m_ClassBrowserSelected == "" )
		{
			return NULL;
		}	
		
		// Direction Vector from current active Camera
		vector dir = GetGame().GetPointerDirection();
		
		// Raycast from
		vector from = FreeDebugCamera.GetInstance().GetPosition(); 
		// Raycast to
		vector to = from + ( dir * 10000 );   
		// Raycast out parameter
		vector contact_pos;
		vector contact_dir;
		int contact_component;
		
		if ( DayZPhysics.RaycastRV(from, to, contact_pos, contact_dir, contact_component) )   
		{
			SceneObject obj = m_SceneData.CreateSceneObject(m_ClassBrowserSelected, contact_pos);
			
			if ( obj != NULL )
			{
				SelectObject(obj);
				return obj;
			}			
		}
		
		return NULL;
	}
	
	//==========================================
	// Load
	//==========================================
	void InitLoad()
	{		
		PLAYER = PlayerBase.Cast( GetGame().GetPlayer() );
		
		SceneLoad( PluginMissionConfig.GetInstance().GetSceneEditorName() );
		
		RestoreSceneObjectsLinks();
		
		ExecuteInitScripts();
	}

	//==========================================
	// RestoreSceneObjectsLinks
	//==========================================
	void RestoreSceneObjectsLinks()
	{
		array<ref SceneObject> scene_objects = GetSceneObjects();
			
		for ( int i = 0; i < scene_objects.Count(); i++ )
		{
			SceneObject scene_object = scene_objects.Get(i);
			
			array<int> linked_scene_objects_indices = scene_object.m_LinkedSceneObjectsIndices;
			if (linked_scene_objects_indices)
			{
				for ( int j = 0; j < linked_scene_objects_indices.Count(); j++ )
				{
					int index = linked_scene_objects_indices.Get(j);
					if ( index >= 0 && index < scene_objects.Count() )
					{
						scene_object.LinkSceneObject( scene_objects.Get(index), false );
						
//PrintString("Load link: "+ ToString(scene_object.GetObject()) +" <-> "+ ToString(scene_objects.Get(index).GetObject()) );
					}
				}
			}
			scene_object.m_LinkedSceneObjectsIndices = NULL;
		}
	}

	//==========================================
	// ExecuteInitScripts
	//==========================================
	void ExecuteInitScripts()
	{
		array<ref SceneObject> scene_objects = GetSceneObjects();	
		for ( int i = 0; i < scene_objects.Count(); i++ )
		{
			SceneObject scene_object = scene_objects.Get(i);
			ExecuteEnforceScript( i, scene_object, scene_object.GetInitScript() );
		}
	}
	
	//==========================================
	// EditorToggle
	//==========================================
	void EditorToggle()
	{	
		//Log("EditorToggle");

		if ( GetGame().IsMultiplayer() )
		{
			return;
		}

		if ( IsOpened() )
		{
			// Close UI Scene Editor Menu
			GetGame().GetUIManager().Back();
		}
		else
		{
			// Open UI Scene Editor Menu
			GetGame().GetUIManager().EnterScriptedMenu(MENU_SCENE_EDITOR, NULL);
			EditorUpdate();
		}
	}
	
	//==========================================
	// IsOpened
	//==========================================
	bool IsOpened()
	{
		return m_IsOpen;
	}

	//==========================================
	// SelectClassName
	//==========================================
	void SelectClassName(string class_name)
	{
		m_ClassBrowserSelected = class_name;
	}

	//==========================================
	// FreeCameraControlEnable
	//==========================================
	void FreeCameraControlEnable()
	{
		// Enable Camera control
		GetGame().GetInput().ChangeGameFocus(-1);
		
		// Disable Cursor
		GetGame().GetUIManager().ShowUICursor(false);
	}

	//==========================================
	// FreeCameraControlDisable
	//==========================================
	void FreeCameraControlDisable()
	{
		// Disable Camera control
		GetGame().GetInput().ChangeGameFocus(1);
		
		// Enable Cursor
		GetGame().GetUIManager().ShowUICursor(true);
	}
	
	//==========================================
	// DeleteSelectedObject
	//==========================================
	void DeleteSelectedObject()
	{
		SceneObject selected_obj = GetSelectedSceneObject();
		
		if ( selected_obj != NULL )
		{
			if ( !selected_obj.IsPlayer() )
			{
				array<ref SceneObject> scene_objects = GetSceneObjects();
				for ( int i = 0; i < scene_objects.Count(); i++ )
				{
					if ( AreSceneObjectsLinked(scene_objects.Get(i), selected_obj) )
					{
						UnlinkSceneObjects(scene_objects.Get(i), selected_obj);
					}
				}
				
				m_SceneData.DeleteSceneObject(selected_obj);
				
				scene_objects.RemoveItem(selected_obj);
				
				DeselectObject();
			}
			else
			{
				SceneEditorMenuCmdPopup("You cant delete player!");
			}
		}
		
		EditorUpdate();
	}
	
	//==========================================
	// SelectObject
	//==========================================
	void SelectObject(SceneObject obj)
	{
		if ( GetSelectedSceneObject() != NULL && GetSelectedSceneObject() == obj )
		{
			return;
		}
		
		DeselectObject();

		m_SelectedObject = obj;
		
		EditorUpdate();
	}
	
	//==========================================
	// GetSelectedSceneObject
	//==========================================
	SceneObject GetSelectedSceneObject()
	{
		return m_SelectedObject;
	}

	//==========================================
	// SelectedObjectDuplicate
	//==========================================
	void SelectedObjectDuplicate()
	{
		SceneObject selected_obj = GetSelectedSceneObject();
		
		if ( selected_obj )
		{
			if ( selected_obj.GetObject() == PLAYER )
			{
				return;
			}
			
			vector size = GetSelectedSceneObject().GetSize();
			
			vector new_pos = selected_obj.GetPosition() + Vector(1, 0, 1) + size;
			
			SceneObject obj = m_SceneData.CreateSceneObject(selected_obj.GetTypeName(), new_pos );
			obj.SetRotation(selected_obj.GetRotation());
			
			array<EntityAI> attachments = selected_obj.GetAttachments();
			for ( int i = 0; i < attachments.Count(); i++ )
			{
				obj.AddAttachment( attachments.Get(i).GetType() );
			}
			
			SelectObject(obj);
		}
	}

	//==========================================
	// SelectedObjectSetRot
	//==========================================
	void SelectedObjectSetRot(float dir)
	{
		if ( GetSelectedSceneObject() )
		{
			GetSelectedSceneObject().SetRotation(dir);
			
			EditorUpdate();
		}
	}

	//==========================================
	// SelectedObjectSetPosX
	//==========================================
	void SelectedObjectSetPosX(float value)
	{
		SceneObject selected_obj = GetSelectedSceneObject();
		
		if ( selected_obj )
		{
			vector v = selected_obj.GetPosition();
			v[0] = value;
			selected_obj.SetPosition(v);
			
			EditorUpdate();
		}
	}

	//==========================================
	// SelectedObjectSetPosY
	//==========================================
	void SelectedObjectSetPosY(float value)
	{
		SceneObject selected_obj = GetSelectedSceneObject();
		
		if ( selected_obj )
		{
			vector v = selected_obj.GetPosition();
			v[1] = value;
			selected_obj.SetPosition(v);
			
			EditorUpdate();
		}
	}

	//==========================================
	// SelectedObjectSetPosZ
	//==========================================
	void SelectedObjectSetPosZ(float value)
	{
		SceneObject selected_obj = GetSelectedSceneObject();
		
		if ( GetSelectedSceneObject() )
		{
			vector v = selected_obj.GetPosition();
			v[2] = value;
			selected_obj.SetPosition(v);
			
			EditorUpdate();
		}
	}

	//==========================================
	// SelectedObjectSetDamage
	//==========================================
	void SelectedObjectSetDamage(float value)
	{
		SceneObject selected_obj = GetSelectedSceneObject();
		EntityAI selected_obj_EAI = selected_obj.GetObject();
		
		if ( selected_obj )
		{
			selected_obj.SetHealth(Math.Clamp ( value, 0, selected_obj_EAI.GetMaxHealth("", "") ));
			
			EditorUpdate();
		}
	}

	//==========================================
	// SelectedObjectFocus
	//==========================================
	void SelectedObjectFocus()
	{
		SceneObject selected_obj = GetSelectedSceneObject();
		
		if ( selected_obj )
		{
			vector		cam_pos = FreeDebugCamera.GetInstance().GetPosition();
			vector		obj_pos = selected_obj.GetPosition();
			vector		view_target = obj_pos;
			
			vector obj_size = selected_obj.GetSize();
			
			cam_pos = obj_pos;
			
			cam_pos[0] = cam_pos[0] + ( obj_size[0] * 2 );
			cam_pos[1] = cam_pos[1] + obj_size[1] + 0.5;
			cam_pos[2] = cam_pos[2] + ( obj_size[2] * 2 );		
			
			view_target[1] = view_target[1] + (obj_size[1] * 0.5);
			
			vector cam_dir = view_target - cam_pos;
			cam_dir.Normalize();
			
			FreeDebugCamera.GetInstance().SetPosition(cam_pos);
			FreeDebugCamera.GetInstance().SetDirection(cam_dir);
			
			//vector debug_pos = cam_pos + (cam_dir * 50);
			//Debug.DrawLine(cam_pos, debug_pos);
			//Debug.DrawSphere(cam_pos);
		}
	}

	//==========================================
	// SelectedObjectAddAttachment
	//==========================================
	void SelectedObjectAddAttachment(string att_name)
	{
		if ( GetSelectedSceneObject() )
		{
			GetSelectedSceneObject().AddAttachment(att_name);
		}
		
		EditorUpdate();
	}

	//==========================================
	// SelectedObjectRemoveAttachment
	//==========================================
	void SelectedObjectRemoveAttachment(EntityAI e)
	{
		if ( GetSelectedSceneObject() )
		{
			GetSelectedSceneObject().RemoveAttachment(e);
		}
		
		EditorUpdate();
	}

	//==========================================
	// LinkSceneObjects
	//==========================================
	void LinkSceneObjects(SceneObject scene_object1, SceneObject scene_object2)
	{
		scene_object1.LinkSceneObject(scene_object2);
		scene_object2.LinkSceneObject(scene_object1);
	}

	//==========================================
	// UnlinkSceneObjects
	//==========================================
	void UnlinkSceneObjects(SceneObject scene_object1, SceneObject scene_object2)
	{
		scene_object1.UnlinkSceneObject(scene_object2);
		scene_object2.UnlinkSceneObject(scene_object1);
	}

	//==========================================
	// AreSceneObjectsLinked
	//==========================================
	bool AreSceneObjectsLinked(SceneObject scene_object1, SceneObject scene_object2)
	{
		if ( scene_object1.IsLinkedWithSceneObject(scene_object2) && scene_object2.IsLinkedWithSceneObject(scene_object1) )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	//==========================================
	// SetDrawSelection
	//==========================================
	void SetDrawSelection(bool draw)
	{
		m_ModuleConfigDebugProfile.SetSceneEditorDrawSelection(draw);
		
		EditorUpdate();
	}

	//==========================================
	// GetDrawSelection
	//==========================================
	bool GetDrawSelection()
	{
		return m_ModuleConfigDebugProfile.GetSceneEditorDrawSelection();
	}

	//==========================================
	// SetLoadPlayerPos
	//==========================================
	void SetLoadPlayerPos(bool save)
	{
		m_ModuleConfigDebugProfile.SetSceneEditorLoadPlayerPos(save);
	}

	//==========================================
	// SetLoadPlayerPos
	//==========================================
	bool GetLoadPlayerPos()
	{
		return m_ModuleConfigDebugProfile.GetSceneEditorLoadPlayerPos();
	}
	
	//==========================================
	// SetRotationAngle
	//==========================================
	void SetRotationAngle(int angle)
	{
		m_ModuleConfigDebugProfile.SetSceneEditorRotationAngle(angle);
	}

	//==========================================
	// GetRotationAngle
	//==========================================
	int	GetRotationAngle()
	{
		return m_ModuleConfigDebugProfile.GetSceneEditorRotationAngle();
	}

	//==========================================
	// SceneLoad
	//==========================================
	void SceneLoad(string scene_name)
	{
		SceneUnload();
		
		PluginMissionConfig.GetInstance().SetSceneEditorName(scene_name);
		m_SceneData = m_ModuleConfigScene.SceneDataLoad(scene_name);
		
		if ( !(GetGame().IsMultiplayer() && GetGame().IsServer()) )
		{
			EditorUpdate();
		}
	}

	//==========================================
	// SceneUnload
	//==========================================
	void SceneUnload()
	{
		m_SceneData = NULL;
		DeselectObject();
	}

	//==========================================
	// SceneSave
	//==========================================
	void SceneSave()
	{
		m_ModuleConfigScene.ClearParams();
		m_ModuleConfigScene.SceneDataSave(m_SceneData);
		SceneEditorMenuCmdSave();
		EditorUpdate();
	}

	//==========================================
	// SceneDelete
	//==========================================
	bool SceneCanDelete(string scene_name)
	{
		if ( scene_name == m_SceneData.GetNameScene() )
		{
			return false;
		}
		
		return true;
	}

	//==========================================
	// SceneDelete
	//==========================================
	bool SceneDelete(string scene_name)
	{
		if ( scene_name == m_SceneData.GetNameScene() )
		{
			return false;
		}
		
		m_ModuleConfigScene.SceneDelete(scene_name);
		
		EditorUpdate();
		
		return true;
	}	

	//==========================================
	// SceneDelete
	//==========================================
	void SceneDuplicate(string scene_name)
	{
		m_ModuleConfigScene.SceneDuplicate(scene_name);
		
		EditorUpdate();
	}

	//==========================================
	// SceneRename
	//==========================================
	void SceneRename(string scene_name, string new_name)
	{
		m_ModuleConfigScene.SceneRename(scene_name, new_name);
		
		if ( scene_name == m_SceneData.GetNameScene() )
		{
			m_SceneData.SetNameScene(new_name);
		}
		
		EditorUpdate();
	}

	//==========================================
	// SceneGetName
	//==========================================
	string SceneGetName()
	{
		if ( m_SceneData )
		{
			return m_SceneData.GetNameScene();
		}
		
		return STRING_EMPTY;
	}
	
	//==========================================
	// SetWeather
	//==========================================
	void SetWeather(float overcast, float rain, float fog, float wind_force)
	{
		m_SceneData.SetInitOvercast(overcast);
		m_SceneData.SetInitRain(rain);
		m_SceneData.SetInitFog(fog);
		m_SceneData.SetInitWindForce(wind_force);
	}

	//==========================================
	// SetDate
	//==========================================
	void SetDate( int year, int month, int day, int hour, int minute )
	{
		m_SceneData.SetInitYear(year);
		m_SceneData.SetInitMonth(month);
		m_SceneData.SetInitDay(day);
		m_SceneData.SetInitHour(hour);
		m_SceneData.SetInitMinute(minute);
	}

	//==========================================
	// SelectAndFocusPlayerObject
	//==========================================
	void SelectAndFocusPlayerObject()
	{
		array<ref SceneObject> scene_objects = GetSceneObjects();
		for ( int i = 0; i < scene_objects.Count(); i++ )
		{
			if ( scene_objects.Get(i).GetObject() == GetGame().GetPlayer() )
			{
				SelectObject( scene_objects.Get(i) );
				SelectedObjectFocus();
				break;
			}
		}
	}
		
	//------------------------------------------
	// ExecuteEnforceScript
	//------------------------------------------
	void ExecuteEnforceScript( int index, SceneObject scene_object, string script )
	{
		if ( script != "" )
		{
			string cls = scene_object.GetObject().ClassName();
			string wrapped_script = "void scMngMain() \n{\n PluginSceneManager se = GetPlugin(Type(\"PluginSceneManager\")); \n SceneObject this_obj = se.GetSceneObjectByIndex(" + index.ToString() + "); \n " + cls + " this_eai = this_obj.GetObject(); \n " + script + "\n}\n";
			
			GetGame().ExecuteEnforceScript(wrapped_script, "scMngMain");
		}
	}
	
	//==========================================
	// RulerToggle
	//==========================================
	void RulerToggle()
	{
		if ( IsRulerActivated() )
		{
			m_RulerActivated = false;
		}
		else
		{
			m_RulerActivated = true;
		}
		
		SceneEditorMenuCmdRefresh();
	}

	//==========================================
	// ActivateRuler
	//==========================================
	bool IsRulerActivated()
	{
		return m_RulerActivated;
	}

	//==========================================
	// RulerAddPoint
	//==========================================
	void RulerAddPoint()
	{
		vector point_pos = GetMousePositionInScene();
		
		m_RulerPoints.Insert( point_pos );
		
		EditorUpdateRuler();
	}

	//------------------------------------------
	// RulerClearVisuals
	//------------------------------------------
	void RulerClearVisuals()
	{
		for ( int i = 0; i < m_RulerLines.Count(); ++i )
		{
			m_RulerLines.Get(i).Destroy();
		}
		m_RulerLines.Clear();
		
		for ( int j = 0; j < m_RulerSpheres.Count(); ++j )
		{
			m_RulerSpheres.Get(j).Destroy();
		}
		m_RulerSpheres.Clear();
	}

	//------------------------------------------
	// RulerDelete
	//------------------------------------------
	void RulerDelete()
	{
		m_RulerPoints.Clear();
		
		EditorUpdateRuler();
	}

	//==========================================
	// GetLoadedScene
	//==========================================
	SceneData GetLoadedScene()
	{
		return m_SceneData;
	}

	//==========================================
	// GetSelectedSceneObjectIndex
	//==========================================
	int GetSelectedSceneObjectIndex()
	{
		array<ref SceneObject> scene_objects = GetSceneObjects();
		for ( int i = 0; i < scene_objects.Count(); i++ )
		{
			if ( GetSelectedSceneObject() == scene_objects.Get(i) )
			{
				return i;
			}
		}
		
		return -1;
	}

	//==========================================
	// GetSceneObjects
	//==========================================
	array<ref SceneObject> GetSceneObjects()
	{
		if ( m_SceneData )
		{
			return m_SceneData.GetSceneObjects();
		}
		
		return NULL;
	}
	
	//==========================================
	// GetScenePlayers
	//==========================================
	array<ref ScenePlayer> GetScenePlayers()
	{
		if ( m_SceneData )
		{
			return m_SceneData.GetScenePlayers();
		}
		
		return NULL;
	}
	
	//==========================================
	// GetSceneObjectsAll
	//==========================================
	array<ref SceneObject> GetSceneObjectsAll()
	{
		if ( m_SceneData )
		{
			return m_SceneData.GetSceneObjectsAll();
		}
		
		return NULL;
	}

	//==========================================
	// GetSceneObjectByIndex
	//==========================================
	SceneObject GetSceneObjectByIndex( int index )
	{
		array<ref SceneObject> scene_objects = GetSceneObjects();
		
		if ( scene_objects != NULL )
		{
			if ( index > -1 && index < scene_objects.Count() )
			{
				return scene_objects.Get( index );
			}
		}
		
		return NULL;
	}

	//==========================================
	// GetSceneObjectByEntity
	//==========================================
	SceneObject GetSceneObjectByEntity( EntityAI entity )
	{
		array<ref SceneObject> scene_objects = GetSceneObjects();
		
		if ( scene_objects != NULL )
		{
			for ( int i = 0; i <= scene_objects.Count(); ++i )
			{
				SceneObject obj = scene_objects.Get(i);
				EntityAI e = obj.GetObject();		
				
				if ( e == entity )
				{
					return obj;
				}
			}
		}
		
		return NULL;
	}
	
	//==========================================
	// GetSceneList
	//==========================================
	TStringArray GetSceneList()
	{
		return m_ModuleConfigScene.GetSceneList();
	}

	//==========================================
	// DeleteSceneObject
	//==========================================
	void DeleteSceneObject(SceneObject obj)
	{		
		if ( obj != NULL )
		{
			if ( obj.GetObject() != GetGame().GetPlayer() )
			{
				obj.UnlinkAll();
				
				if ( GetSelectedSceneObject() != NULL && GetSelectedSceneObject() == obj )
				{
					DeselectObject();
				}
				
				m_SceneData.DeleteSceneObject(obj);
			}
			else
			{
				SceneEditorMenuCmdPopup("You cant delete player!");
			}
		}
			
		EditorUpdate();
	}
	
	//---------------------------------------------------------------------------------
	// >> Protected	scope
	protected PluginDeveloper			m_ModuleDeveloper;
	protected PluginConfigScene			m_ModuleConfigScene;
	protected PluginConfigDebugProfile	m_ModuleConfigDebugProfile;

	//---------------------------------------------------------------------------------
	// >> protected Scope
	protected float					m_TimeLastClickRelease;
	protected float					m_TimeLastClickPress;
	protected int					m_PressedMouseID;
	protected bool					m_IsDragging;
	protected ref array<Shape>	m_ShapesSceneObjects;
	protected ref map<int, KeyBinding>	m_KeyBindings;
	protected ref SceneData		m_SceneData;
	protected SceneObject			m_SelectedObject;
	protected string				m_ClassBrowserSelected;
	protected bool					m_IsOpen;
	
	//------------------------------------------
	// DeselectObject
	//------------------------------------------
	private void DeselectObject()
	{
		m_SelectedObject = NULL;
	}
	
	//------------------------------------------
	// EditorUpdate	
	//------------------------------------------
	protected void EditorUpdate()
	{
		if ( GetGame().IsMultiplayer() )
		{
			return;
		}
		
		array<ref SceneObject> objects = GetSceneObjects();
		
		for ( int i = 0; i < objects.Count(); ++i )
		{
			SceneObject obj = objects.Get(i);
			
			array<SceneObject> linked_scene_objects = obj.GetLinkedSceneObjects();
			int j;
			
			if ( IsOpened() )
			{
				obj.EditorShapeAdd();
				
				if ( GetSelectedSceneObject() && GetSelectedSceneObject() == obj )
				{
					if ( GetDrawSelection() )
					{
						obj.EditorShapeSelect();
					}
				}
				else
				{
					obj.EditorShapeDeselect();
				}
				
				for ( j = 0; j < linked_scene_objects.Count(); j++ )
				{					
					obj.EditorLineAdd( linked_scene_objects.Get(j) );
				}
			}
			else
			{
				obj.EditorShapeRemove();
				
				if (linked_scene_objects)
				{
					for ( j = 0; j < linked_scene_objects.Count(); j++ )
					{
						obj.EditorLineRemove( linked_scene_objects.Get(j) );
					}
				}
			}
		}

		SceneEditorMenuCmdRefresh();
	}
	
	//------------------------------------------
	// SceneEditorMenuRefresh
	//------------------------------------------
	private void SceneEditorMenuCmdRefresh()
	{
		CallMethod(CALL_ID_SCENE_EDITOR_COMMAND, new Param2<int, Param>(SCENE_EDITOR_CMD_REFRESH, NULL));
	}

	//------------------------------------------
	// SceneEditorMenuCmdPopup
	//------------------------------------------
	private void SceneEditorMenuCmdPopup(string message)
	{
		CallMethod(CALL_ID_SCENE_EDITOR_COMMAND, new Param2<int, Param1<string>>(SCENE_EDITOR_CMD_POPUP, new Param1<string>(message)));
	}

	//------------------------------------------
	// SceneEditorMenuCmdSave
	//------------------------------------------
	private void SceneEditorMenuCmdSave()
	{
		CallMethod(CALL_ID_SCENE_EDITOR_COMMAND, new Param2<int, Param>(SCENE_EDITOR_CMD_SAVE, NULL));
	}
	
	//------------------------------------------
	// IsMouseInScene
	//------------------------------------------
	private bool IsMouseInScene()
	{
		Widget widget_under_mouse = GetWidgetUnderCursor();
		
		if ( widget_under_mouse != NULL && widget_under_mouse.GetName() == "pnl_SceneControl" )
		{
			return true;
		}
		
		return false;
	}
	
	//------------------------------------------
	// EditorUpdateRuler
	//------------------------------------------
	private void EditorUpdateRuler()
	{
		RulerClearVisuals();
		
		for ( int i = 0; i < m_RulerPoints.Count(); ++i )
		{
			vector pos1 = m_RulerPoints.Get(i);
			
			m_RulerSpheres.Insert(Debug.DrawSphere(pos1, 0.1));
			
			if ( i < m_RulerPoints.Count() - 1 )
			{
				vector pos2 = m_RulerPoints.Get(i + 1);
				m_RulerLines.Insert(Debug.DrawLine(pos1, pos2));
			}
		}
	}
	
	//------------------------------------------
	// GetMousePositionInScene
	//------------------------------------------
	private vector GetMousePositionInScene()
	{
		vector dir = GetGame().GetPointerDirection();
		vector from = FreeDebugCamera.GetInstance().GetPosition(); 
		vector to = from + ( dir * 10000 );   
		vector contact_pos;
		vector contact_dir;
		int contact_component;
		
		if ( DayZPhysics.RaycastRV(from, to, contact_pos, contact_dir, contact_component) )   
		{
			return contact_pos;
		}
		
		return "0 0 0";
	}
}
