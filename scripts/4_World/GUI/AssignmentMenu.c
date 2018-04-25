// #include "Scripts\Classes\Gui\AssignmentTimer.c"

class AssignmentMenu extends UIScriptedMenu
{
	void AssignmentMenu()
	{
		m_keys = new TIntArray;
		m_specialKeys = new TIntArray;
		g_Game.SetKeyboardHandle(this);
	}

	void ~AssignmentMenu()
	{
		CancelCapture();
		g_Game.SetKeyboardHandle(NULL);
	}
	
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_assignments.layout");
		
		m_assignedList = TextListboxWidget.Cast( layoutRoot.FindAnyWidget("AssignedListboxWidget") );
		m_assignedList.ClearItems();

		m_specialKeysList = TextListboxWidget.Cast( layoutRoot.FindAnyWidget("SpecialKeysListboxWidget") );
		m_specialKeysList.ClearItems();

		m_actionNameText = TextWidget.Cast( layoutRoot.FindAnyWidget("ActionNameTextWidget") );

		m_MainWindow = (Widget)layoutRoot.FindAnyWidget("MainWindowWidget");
		m_MainWindow.Show(true);

		m_AssignWindow = (Widget)layoutRoot.FindAnyWidget("AssignWindowWidget");
		m_AssignWindow.Show(false);

		m_addButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("AddButton") );
		
		m_addSpecialButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("AddSpecialButton") );
		m_addSpecialButton.Show(false);

		m_removeButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("RemoveButton") );
		m_removeButton.Show(false);

		m_resetButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("ResetButton") );
		m_defaultButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("DefaultButton") );

		// init special keys
		g_Game.GetInput().GetSpecialKeys(m_specialKeys);

		m_specialKeysList.ClearItems();

		string name;
		for (int i = 0; i < m_specialKeys.Count(); i++)
		{
			int key = m_specialKeys.Get(i);
			g_Game.GetInput().GetActionKeyName(key, name);
			m_specialKeysList.AddItem(name, NULL, 0);
		}

		CancelCapture();
		return layoutRoot;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick( w, x, y, button);
		
		switch (w.GetUserID())
		{
		case IDC_OK:
			g_Game.GetInput().SetActionKeys(m_action_index, m_keys);
			Close();
			return true;
			
		case IDC_CANCEL:
			Close();
			return true;
		}

		if (w == m_addButton)
		{
			AddKey();
			return true;
		}

		if (w == m_addSpecialButton)
		{
			AddSpecialKey();
			return true;
		}
		
		if (w == m_removeButton)
		{
			RemoveKey();
			return true;
		}

		if (w == m_resetButton)
		{
			ResetAssignments();
			return true;
		}

		if (w == m_defaultButton)
		{
			DefaultAssignments();
			return true;
		}

		if (w == m_specialKeysList)
		{
			if (m_specialKeysList.GetSelectedRow() != -1)
			{
				m_addSpecialButton.Show(true);
			}
			else
			{
				m_addSpecialButton.Show(false);
			}
			return true;
		}

		return false;
	}
		
	float m_doubletapTime = 0.3;
	bool CanProcessCapture()
	{
		return ((m_first_key != -1) && (m_doubletapTime <= 0));
	}
	
	override void Update(float timeslice)
	{
		if(!IsCapturing())
			return;
		
		int dik = g_Game.GetInput().GetSpecialKeyPressed();
		if (dik != -1)
		{		
			OnKeyDown(NULL, 0, 0, dik);
			OnKeyUp(NULL, 0, 0, dik);
		}

		if(CanProcessCapture())
		{
			ProcessCapture();
		}

		if(m_first_key_double_tap_enabled || m_second_key_double_tap_enabled)
		{
			m_doubletapTime -= timeslice;
		}
	}
	
	override bool OnKeyDown(Widget w, int x, int y, int key)
	{
		super.OnKeyDown( w, x, y, key);
		
		if (!IsCapturing())
			return false;
		
		if (key == KeyCode.KC_ESCAPE)
		{
			CancelCapture();
			return true;
		}

		if (m_first_key == -1)
		{
			m_first_key = key;
			return true;
		}
		
		if (m_first_key == key && m_first_key_double_tap_enabled) // double tap
		{
			m_first_key = key + INPUT_ACTION_TYPE_DOUBLETAP;
			ProcessCapture();
			return true;
		}

		if (g_Game.GetInput().IsKeyReserved(key)) 
			return false;

		if (m_second_key == -1 && m_first_key != key)
		{
			m_second_key = key;
			return true;
		}
		
		if (m_second_key == key && m_second_key_double_tap_enabled) // double tap
		{
			m_second_key = key + INPUT_ACTION_TYPE_DOUBLETAP;
			ProcessCapture();
			return true;
		}

		return false;
	}
	
	override bool OnKeyUp(Widget w, int x, int y, int key)
	{
		super.OnKeyUp(w, x, y, key);
		
		if (!IsCapturing()) 
			return false;
		
		if (m_first_key == key && m_second_key != -1)
		{
			ProcessCapture();
			return true;
		}

		if (m_first_key == key)
		{
			m_doubletapTime = 0.3;
			m_first_key_double_tap_enabled = true;
		}

		if (m_second_key == key)
		{
			m_doubletapTime = 0.3;
			m_second_key_double_tap_enabled = true;
		}

		return true;
	}
	
	void InitAssignment(int action_index)
	{
		m_action_index = action_index;
		ResetAssignments();

		string desc;
		g_Game.GetInput().GetActionDesc(m_action_index, desc);
		m_actionNameText.SetText("#options_controls_action_label " + desc);
	}

	void AddKey()
	{
		if (IsCapturing()) return;

		StartCapture();
	}

	void AddSpecialKey()
	{
		int specialKeyIndex = m_specialKeysList.GetSelectedRow();
		if (specialKeyIndex != -1)
		{
			int specialKey = m_specialKeys.Get(specialKeyIndex);
			m_keys.Insert(specialKey);
		}
		RefreshUI();
	}
	
	void RemoveKey()
	{
		int keyIndex = m_assignedList.GetSelectedRow();
		if (keyIndex != -1 && keyIndex < m_keys.Count())
		{
			m_keys.Remove(keyIndex);
		}

		RefreshUI();
	}

	void ResetAssignments()
	{
		g_Game.GetInput().GetActionKeys(m_action_index, m_keys);
		RefreshUI();
	}

	void DefaultAssignments()
	{
		g_Game.GetInput().GetActionDefaultKeys(m_action_index, m_keys);
		RefreshUI();
	}

	void RefreshUI()
	{
		string name;
		int i;
		int key;

		int lastKeyIndex = m_assignedList.GetSelectedRow();

		m_assignedList.ClearItems();

		if (m_keys.Count() == 0) 
		{
			m_removeButton.Show(false);
			return;
		}

		for (i = 0; i < m_keys.Count(); i++)
		{
			key = m_keys.Get(i);
			g_Game.GetInput().GetActionKeyName(key, name);
			int conflictAction = g_Game.GetInput().CheckKeyCollision(m_action_index, key);
			if (conflictAction != -1)
			{
				string conflictActionName;
				g_Game.GetInput().GetActionDesc(conflictAction, conflictActionName);
				name += "(";
				name += conflictActionName;
				name += ")";
			}
			
			m_assignedList.AddItem(name, NULL, 0);
			
			if (conflictAction != -1)
			{
				m_assignedList.SetItemColor(i,0,ARGBF(1, 1, 0, 0) );
			}			
			
		}

		if (lastKeyIndex != -1 && lastKeyIndex < m_keys.Count())
		{
			m_assignedList.SelectRow(lastKeyIndex);
		}
		else
		{
			m_assignedList.SelectRow(m_keys.Count() - 1);
		}

		m_removeButton.Show(true);
	}


	void StartCapture()
	{
		m_MainWindow.Show(false);
		m_AssignWindow.Show(true);
		
		m_first_key = -1;
		m_second_key = -1;
		m_capturing = true;
		m_first_key_double_tap_enabled = false;
		m_second_key_double_tap_enabled = false;
		m_doubletapTime = 0.3;
	}

	bool IsCapturing()
	{
		return m_capturing;
	}

	int CreateBind(int firstKey, int secondKey)	
	{
		if(firstKey == -1)
			return -1;
				
		if(secondKey != -1)
		{
			int comboBind = (firstKey & INPUT_KEY_MASK) * INPUT_COMBO_KEY_OFFSET;
			if(firstKey & INPUT_AXIS)
			{
				comboBind |= INPUT_COMBO_AXIS;
			}
			
			int comboType = INPUT_ACTION_TYPE_COMBO;
			if((firstKey & INPUT_MODULE_TYPE_MASK) == INPUT_DEVICE_KEYBOARD && (secondKey & INPUT_MODULE_TYPE_MASK) == INPUT_DEVICE_MOUSE)
			{
				comboType = INPUT_ACTION_TYPE_SPECIALCOMBO;
			}

			comboBind |= (secondKey | comboType);
			
			return comboBind;
		}

		return firstKey;
	}
	
	void ProcessCapture()
	{
		if (m_second_key != -1) // combo
		{
			int combo = CreateBind(m_first_key, m_second_key);
			if ( !IsAlreadyAdded( combo ) )
			{
				m_keys.Insert( combo );
			}
		}
		else
		{
			if ( !IsAlreadyAdded( m_first_key ) && !g_Game.GetInput().IsKeyReserved( m_first_key ) )
			{
				m_keys.Insert( m_first_key );
			}
		}
		
		RefreshUI();
		CancelCapture();
	}
	
	bool IsAlreadyAdded( int m_first_key )
	{
		int key;
		for ( int i = 0; i < m_keys.Count(); i++ )
		{
			key = m_keys.Get(i);
			if( key == m_first_key )
			{
				return true;
			}
		}
		
		return false;
	}
	
	void CancelCapture()
	{
		m_MainWindow.Show(true);
		m_AssignWindow.Show(false);

		m_first_key = -1;
		m_second_key = -1;
		m_capturing = false;
	}

	TextListboxWidget m_assignedList;
	TextListboxWidget m_specialKeysList;
	TextWidget m_actionNameText;
	Widget m_MainWindow;
	Widget m_AssignWindow;
	ButtonWidget m_addButton;
	ButtonWidget m_addSpecialButton;
	ButtonWidget m_removeButton;
	ButtonWidget m_resetButton;
	ButtonWidget m_defaultButton;
	
	ref TIntArray m_keys;
	ref TIntArray m_specialKeys;

	int m_action_index;
	int m_first_key;
	int m_second_key;
	bool m_capturing;
	bool m_first_key_double_tap_enabled;
	bool m_second_key_double_tap_enabled;
}
