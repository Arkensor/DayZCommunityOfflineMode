// #include "Scripts\Classes\Gui\OptionsAccessUI.c"
// #include "Scripts\Classes\Gui\SliderOptionsAccessUI.c"
// #include "Scripts\Classes\Gui\ComboBoxOptionsAccessUI.c"
// #include "Scripts\Classes\Gui\ButtonOptionsAccessUI.c"

typedef Param2<int,string> TContextMenuItem;

typedef array<ref OptionsAccessUI> TOptionsAccessUIArray;

typedef array<ref ProfileOptionsUI> TProfileOptionsUIArray;

class OptionsMenu extends UIScriptedMenu
{
	int m_selected_tab = 0;
	
	private ref ContextMenu m_context_menu;
	ref array<string> m_languages;
	ref array<string> m_resolutions;
	ref array<string> m_smoothings;
	string m_OriginalLanguage;
	bool m_languageChanged;
	
	void OptionsMenu()
	{
		m_action_indices = new TIntArray;
		m_keys = new TIntArray;
		m_optionsUI = new TOptionsAccessUIArray;
		m_profile_optionsUI = new TProfileOptionsUIArray;
		m_languages = new array<string>;
		m_resolutions = new array<string>;
		m_smoothings = new array<string>;
	}
	
	void ~OptionsMenu()
	{
		PPEffects.SetBlurMenu(0);
		m_Options.Revert();
		GetGame().EndOptionsVideo();
	}
	
	override Widget Init()
	{
		//m_Options = new GameOptions;
		
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_optionsNEW.layout");
		//layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_options.layout");
		
		m_context_menu = new ContextMenu;
		m_context_menu.Init(layoutRoot);
		m_context_menu.ShowBackdrop(true);
		//m_context_menu.m_context_menu_root_widget.FindAnyWidget("BackdropImageWidget").Show(true);
		
		PPEffects.SetBlurMenu(0); //0.5
		
		GetGame().BeginOptionsVideo();	
		
		m_tabs[TAB_GAME] = layoutRoot.FindAnyWidget("GamePanelWidget");
		m_tabs[TAB_SOUND] = layoutRoot.FindAnyWidget("SoundPanelWidget");
		m_tabs[TAB_VIDEO] = layoutRoot.FindAnyWidget("VideoPanelWidget");
		m_tabs[TAB_CONTROLS] = layoutRoot.FindAnyWidget("ControlsPanelWidget");
		m_tabs[TAB_KEYBINDS]= layoutRoot.FindAnyWidget("KeyboardBindingPanelWidget");
		
		//m_keyboard_tab = layoutRoot.FindWidget("KeyboardBindingPanelWidget");
		
		m_tab_buttons[TAB_GAME] = ButtonWidget.Cast( layoutRoot.FindAnyWidget("GameButtonWidget") );
		m_tab_buttons[TAB_SOUND] = ButtonWidget.Cast( layoutRoot.FindAnyWidget("SoundButtonWidget") );
		m_tab_buttons[TAB_VIDEO] = ButtonWidget.Cast( layoutRoot.FindAnyWidget("VideoButtonWidget") );
		m_tab_buttons[TAB_CONTROLS] = ButtonWidget.Cast( layoutRoot.FindAnyWidget("ControlsButtonWidget") );
		
		m_tab_images[TAB_GAME] = ImageWidget.Cast( layoutRoot.FindAnyWidget("GameButtonBackdropImageWidget") );
		m_tab_images[TAB_SOUND] = ImageWidget.Cast( layoutRoot.FindAnyWidget("SoundButtonBackdropImageWidget") );
		m_tab_images[TAB_VIDEO] = ImageWidget.Cast( layoutRoot.FindAnyWidget("VideoButtonBackdropImageWidget") );
		m_tab_images[TAB_CONTROLS] = ImageWidget.Cast( layoutRoot.FindAnyWidget("ControlsButtonBackdropImageWidget") );
		
	//keybinds
		m_action_groups = XComboBoxWidget.Cast( layoutRoot.FindAnyWidget("ActionGroupsComboBoxWidget") );
		m_action_groups.ClearAll();
		
		m_actions = TextListboxWidget.Cast( layoutRoot.FindAnyWidget("ActionsListboxWidget") );
		m_actions.ClearItems();
		
		m_devices = TextListboxWidget.Cast( layoutRoot.FindAnyWidget("DevicesListboxWidget") ); //devices window, disconnected
		m_devices.ClearItems();
		
		m_presets_button = ButtonWidget.Cast( layoutRoot.FindAnyWidget("PresetsBtn") );
		m_assign_button = ButtonWidget.Cast( layoutRoot.FindAnyWidget("AssignBtn") );
	//end of keybinds
		m_device_button = ButtonWidget.Cast( layoutRoot.FindAnyWidget("EnableDeviceBtn") );
		m_device_custom_scheme = CheckBoxWidget.Cast( layoutRoot.FindAnyWidget("CustomSchemeCheckBoxWidget") );
		
		m_test_button = ButtonWidget.Cast( layoutRoot.FindAnyWidget("TestBtn") );
		m_quality_set_button = ButtonWidget.Cast( layoutRoot.FindAnyWidget("QualitySetBtn") );
		m_default_video_button = ButtonWidget.Cast( layoutRoot.FindAnyWidget("DefaultVideoBtn") );
		m_default_audio_button = ButtonWidget.Cast( layoutRoot.FindAnyWidget("DefaultSoundButtonWidget") );
		m_fxaa_video_button = ButtonWidget.Cast( layoutRoot.FindAnyWidget("PPAntialiasingFXAAButtonWidget") );
		m_smaa_video_button = ButtonWidget.Cast( layoutRoot.FindAnyWidget("PPAntialiasingSMAAButtonWidget") );
		m_Language_DDM_button = ButtonWidget.Cast(layoutRoot.FindAnyWidget("LanguageDDMButtonWidget"));
		m_Resolution_DDM_button = ButtonWidget.Cast( layoutRoot.FindAnyWidget("ResolutionDDMButtonWidget") );
		//m_Smoothing_DDM_button = ButtonWidget.Cast( layoutRoot.FindAnyWidget("EdgeSmoothingDDMButtonWidget") );
		m_controlls_keyboard = ButtonWidget.Cast( layoutRoot.FindAnyWidget("ConfigureKeyboardButtonWidget") );
		m_OK_keyboard = ButtonWidget.Cast( layoutRoot.FindAnyWidget("KeyboardOKButtonWidget"));
		
		m_system_cm_check = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("SystemCMCheckBox") );
		m_global_cm_check = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("GlobalCMCheckBox") );
		m_direct_cm_check = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("DirectCMCheckBox") );
		m_vehicle_cm_check = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("VehicleCMCheckBox") );
		m_radio_cm_check = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("RadioCMCheckBox") );
		m_ShowQuicBarCheck = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("ShowQuickBar") );
		//m_ShowHudCheck = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("ShowHud") );
		
		m_invert_mouse_off_button = ButtonWidget.Cast( layoutRoot.FindAnyWidget("InvertMouseOffButtonWidget"));
		m_invert_mouse_on_button = ButtonWidget.Cast( layoutRoot.FindAnyWidget("InvertMouseOnButtonWidget"));
		
		int i = 0;
		int c = g_Game.GetInput().GetActionGroupsCount();
		string name;
		
		m_original_widgets_y = new array<float>;
		m_array_buttons = new array<ButtonWidget>;
		
		for (i = 0; i < c ;i++)
		{
			g_Game.GetInput().GetActionGroupName(i, name);
			m_action_groups.AddItem(name);
		}
		SelectActionGroup(0);
		SelectTab(m_selected_tab);
		
		InitOptions();	
		UpdateDevices();
		InitSetInvertButtons();
		//UpdateChatMessages();
		
		RegisterProfileOptionWidgets();
		//InitProfileSettings();
		
		return layoutRoot;
	}
	
	void InitOptions()
	{
		int i;
		int c;
		string LanguageText;
		string ResolutionText;
		string text;
		OptionsAccessUI optionUI;
		OptionsAccess optionAccess;
		ListOptionsAccess loa;
		SwitchOptionsAccess soa;
		
		// cleanup first
		m_optionsUI.Clear();
		//m_profile_optionsUI.Clear();
		
		m_Options = new GameOptions;
		
		c = m_Options.GetOptionsCount();
		for (i = 0; i < c; i++)
		{
			optionUI = NULL;
			optionAccess = m_Options.GetOption(i);
			
			//TODO perform only once on init, if possible, save into some array for reference / revert values (?)
			switch (optionAccess.GetControlType())
			{
			case OA_CT_NUMERIC:
				optionUI = new SliderOptionsAccessUI;
			break;
			case OA_CT_SWITCH:
				optionUI = new ButtonOptionsAccessUI;
			break;
			case OA_CT_LIST:
				optionUI = new ListOptionsAccessUI; //ComboBoxOptionsAccessUI
			break;
			}
			
			switch (optionAccess.GetAccessType())
			{
			case AT_OPTIONS_FIELD_OF_VIEW:
				m_FOV_option = optionUI;
				NumericOptionsAccess noa = NumericOptionsAccess.Cast(optionAccess);
				m_FOV_original_value = noa.ReadValue();
			break;
			case AT_CONFIG_YREVERSED:
				m_invert_mouse_option = optionUI;
			break;
			case AT_OPTIONS_LANGUAGE:
				m_language_option = optionUI;
				
				ListAvailableLanguages(optionAccess,m_languages);
				soa = SwitchOptionsAccess.Cast(optionAccess);
				soa.GetItemText(LanguageText);
				m_Language_DDM_button.SetText(LanguageText);
				m_languageChanged = false;
			break;
			case AT_OPTIONS_SLIDER_FILLRATE:
				m_rend_res_option = optionUI;
			break;
			case AT_QUALITY_PREFERENCE:
				m_overall_quality_option = optionUI;
			break;
			case AT_OPTIONS_RESOLUTION:
				m_resolution_option = optionUI;
				
				ListListOptionAccess(optionAccess,m_resolutions);
				loa = ListOptionsAccess.Cast(optionAccess);
				loa.GetItemText(loa.GetIndex(),ResolutionText);
				m_Resolution_DDM_button.SetText(ResolutionText);
			break;
			case AT_OPTIONS_VIDEO_DEFAULT:
				m_default_video_access = SwitchOptionsAccess.Cast( optionAccess );
			break;
			case AT_FSAA_DETAIL:
				m_msaa_option = ListOptionsAccessUI.Cast( optionUI );
			break;
			case AT_POSTPROCESS_EFFECTS:
				m_postprocess_quality_option = ListOptionsAccessUI.Cast( optionUI );
			break;
			case AT_ATOC_DETAIL:
				m_atoc_option = optionUI;
			break;
			case AT_AMBIENT_OCCLUSION:
				m_ao_option = optionUI;
			break;
			case AT_BLOOM:
				m_bloom_option = optionUI;
			break;
			/*case AT_ROTATION_BLUR:
				m_rot_blur_option = optionUI;
			break;*/
			case AT_OPTIONS_FXAA_VALUE:	//PPAA
				m_ppaa_option = ListOptionsAccessUI.Cast( optionUI );
				
				/*ListListOptionAccess(optionAccess,m_smoothings);
				loa = ListOptionsAccess.Cast(optionAccess);
				loa.GetItemText(loa.GetIndex(),text);
				m_Smoothing_DDM_button.SetText(text);*/
			break;
			}
			
			if (optionUI)
			{
				if (optionUI.Type().ToString() != "ListOptionsAccessUI")
				{
					optionUI.Attach(optionAccess, layoutRoot.FindAnyWidgetById(WIDGET_USER_ID_OFFSET + optionAccess.GetAccessType()));
					m_optionsUI.Insert(optionUI);
				}
				else
				{
					ListOptionsAccessUI.Cast(optionUI).Attach(optionAccess, GetArrayOfWidgetsById(WIDGET_USER_ID_OFFSET + optionAccess.GetAccessType()));
					m_optionsUI.Insert(optionUI);
				}
			}
		}
		
		CheckOptionsDependency();
		//
		/*for (int cou = 0; cou < ListOptionsAccess.Cast(m_ppaa_option.GetOptionAccess()).GetItemsCount(); cou++)
		{
			Print( "" + m_ppaa_option.GetWidgetArray().Get(cou).GetName() );
		}*/
		//
	}
	
	array<Widget> GetArrayOfWidgetsById(int id) //Gets all button widgets, or one non-button widget into an array, whichever fits
	{
		array<Widget> arrayOfWidgets = new array<Widget>;
		Widget newWidget = layoutRoot.FindAnyWidgetById( id );
		
		if ( newWidget )
		{
			while ( newWidget )
			{
				if ( newWidget.GetTypeName() == "ButtonWidget" )
				{
					arrayOfWidgets.Insert(newWidget);
				}
				newWidget = newWidget.GetSibling();
			}
			if ( arrayOfWidgets.Count() == 0 )
			{
				arrayOfWidgets.Insert(layoutRoot.FindAnyWidgetById( id )); //insert single non-button widget (SliderWidget, for example)
			}
		}
		return arrayOfWidgets;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		int i;
		int selected;
		string text;
		OptionsAccessUI item;
		SwitchOptionsAccess soa;
		
		switch (w.GetUserID())
		{
			case IDC_OK:
				if (m_Options.NeedRestart() && m_languageChanged == true)
				{
					g_Game.GetUIManager().ShowDialog("#main_menu_configure", "#menu_restart_needed", 0, DBT_YESNO, DBB_YES, DMT_QUESTION, this);
				}
				else
				{
					GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this.ApplyFn);
				}
				return true;
				
			case IDC_CANCEL:
				g_Game.SetUserFOV(m_FOV_original_value);
				Close();
				return true;
		}
		
		if (w == m_device_button)
		{
			selected = m_devices.GetSelectedRow();
			if (selected > -1)
			{
				if (g_Game.GetInput().IsDeviceEnabled(selected))
				{
					g_Game.GetInput().SetDeviceEnabled(selected, false);
				}
				else
				{
					g_Game.GetInput().SetDeviceEnabled(selected, true);
				}
			}
			
			UpdateDevices();
			return true;
		}
		
		if (w == m_controlls_keyboard)
		{
			SelectTab(TAB_KEYBINDS);
		}
		
		if (w == m_OK_keyboard)
		{
			SelectTab(TAB_CONTROLS);
		}
		
		if (w == m_invert_mouse_off_button || w == m_invert_mouse_on_button)
		{
			soa = SwitchOptionsAccess.Cast( m_invert_mouse_option.GetOptionAccess() );
			soa.GetItemText(text);
			if (text == "Invert Mouse: Off" && w == m_invert_mouse_on_button)
			{
				m_invert_mouse_off_button.SetState(false);
				m_invert_mouse_on_button.SetState(true);
				soa.Switch();
			}
			else if (text == "Invert Mouse: On" && w == m_invert_mouse_off_button)
			{
				m_invert_mouse_off_button.SetState(true);
				m_invert_mouse_on_button.SetState(false);
				soa.Switch();
			}
		}
		
		if (w == m_device_custom_scheme)
		{
			if (g_Game.GetInput().IsCustomSchemeEnabled())
			{
				g_Game.GetInput().SetCustomSchemeEnabled(false);
			}
			else
			{
				g_Game.GetInput().SetCustomSchemeEnabled(true);
			}
			
			UpdateDevices();
			return true;
		}
		
		if (w == m_action_groups)
		{
			SelectActionGroup(m_action_groups.GetCurrentItem());
			return true;
		}
		
		if (w == m_test_button)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this.TestFn);
			return true;
		}
		
		if (w.GetParent().GetName() == "QualityPanelWidget")
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this.OverallQualityChanged);
			return true;
		}
		
		if (w == m_default_video_button)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this.DefaultVideoSettings);
			return true;
		}
		
		if (w == m_default_audio_button)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this.DefaultAudioSettings);
			return true;
		}
		
		if (w.GetParent().GetName() == "AntialiasingPanelWidget")
		{
			CheckOptionsDependency();
		}
		
		if (w.GetParent().GetName() == "EdgeSmoothingPanelWidget")
		{
			CheckOptionsDependency();
		}
		
		if (w.GetParent().GetName() == "PPAntialiasingPanelWidget")
		{
			CheckOptionsDependency();
		}
		
		if (w == m_postprocess_quality_option.GetWidget())
		{
			CheckOptionsDependency();
		}
		
		if (w == m_presets_button)
		{
			EnterScriptedMenu(MENU_CONTROLS_PRESET);
			return true;
		}
		
		if (w == m_assign_button)
		{
			selected = m_actions.GetSelectedRow();
			if (selected > -1)
			{
				int index = m_action_indices.Get(selected);
				
				AssignmentMenu assignmentsMenu = AssignmentMenu.Cast( EnterScriptedMenu(MENU_ASSIGNMENT) );
				if (assignmentsMenu)
				{
					assignmentsMenu.InitAssignment(index);
				}
				
				return true;
			}
		}
		
		//TODO prototype, CLEANUP!!
		//
		if (w == m_fxaa_video_button)
		{
			ListOptionsAccess loa;
			loa = ListOptionsAccess.Cast(m_ppaa_option.GetOptionAccess());
			loa.SetIndex(OPTION_FXAA_LOW);
			
			m_ppaa_option.SetButtonStates(OPTION_FXAA_LOW);
			
			CheckOptionsDependency();
			return true;
		}
		
		if (w == m_smaa_video_button)
		{
			loa = ListOptionsAccess.Cast(m_ppaa_option.GetOptionAccess());
			loa.SetIndex(OPTION_SMAA_LOW);
			
			m_ppaa_option.SetButtonStates(OPTION_SMAA_LOW);
			
			CheckOptionsDependency();
			return true;
		}
		//
		
		for (i = 0; i < TABS_COUNT; i++)
		{
			if (w == m_tab_buttons[i])
			{
				SelectTab(i);
				return true;
			}
		}
		
		if (w == m_Language_DDM_button )
		{
			return ShowDropdownMenu(w, m_languages, "SetLanguage");
		}
		
		if (w == m_Resolution_DDM_button )
		{
			return ShowDropdownMenu(w, m_resolutions, "SetResolution");
		}
		
		/*if (w == m_Smoothing_DDM_button )
		{
			return ShowDropdownMenu(w, m_smoothings, "SetSmoothing");
		}*/
		
		return false;
	}
	
	bool ShowDropdownMenu(Widget w, ref array<string> items, string FnName)
	{
		float x_out;
		float y_out;
		
		if (items.Count() > 0)
		{
			m_context_menu.Clear();
			for (int i = 0; i < items.Count(); i++)
			{
				TContextMenuItem ContextMenuItems = new TContextMenuItem(0,"");
				ContextMenuItems.param1 = i;
				ContextMenuItems.param2 = items.Get(i);
				m_context_menu.Add(items.Get(i),this,FnName,ContextMenuItems);
			}
			
			w.GetScreenPos(x_out,y_out);
			m_context_menu.Show(x_out,y_out);
			
			return true;
		}
		return false;
	}
	
	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		if (m_current_scrollbar.IsVisible())
		{
			ScrollWidget(wheel);
			return true;
		}
		
		return false;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		return FocusPanel(true,w);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		return FocusPanel(false,w);
	}
	
	override bool OnController(Widget w, int control, int value)
	{
		super.OnController(w, control, value);
		
		if(control == ControlID.CID_TABLEFT && value != 0)
		{
			SelectPreviousTab();
			return true;
		}
		
		if(control == ControlID.CID_TABRIGHT && value != 0)
		{
			SelectNextTab();
			return true;
		}
		return false;
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		super.OnChange(w, x, y, finished);
		
		if (w == m_system_cm_check)
		{
			m_system_cm = m_system_cm_check.IsChecked();
		}
		else if (w == m_global_cm_check)
		{
			m_global_cm  = m_global_cm_check.IsChecked();
		}
		else if (w == m_direct_cm_check)
		{
			m_direct_cm = m_direct_cm_check.IsChecked();
		}
		else if (w == m_vehicle_cm_check)
		{
			m_vehicle_cm = m_vehicle_cm_check.IsChecked();
		}
		else if (w == m_radio_cm_check)
		{
			m_radio_cm = m_radio_cm_check.IsChecked();
		}
		/*else if (w == m_ShowQuicBarCheck)
		{
			m_ShowQuickBar = m_ShowQuicBarCheck.IsChecked();
		}*/
		/*else if (w == m_ShowHudCheck)
		{
			m_ShowHud = m_ShowHudCheck.IsChecked();
		}*/
		else if (w == m_current_scrollbar)
		{
			m_scrollbar_value = m_current_scrollbar.GetCurrent();
			m_scrollbar_value = m_scrollbar_value / m_current_scrollbar.GetMax();
			//m_scrollbar_value = m_new_scrollbar_value;
			ScrollWidget(0); //0 value makes slider widget move widget Y positions
		}
		return false;
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		super.OnDoubleClick(w, x, y, button);
		
		int selected;
		
		if (w == m_actions)
		{
			selected = m_actions.GetSelectedRow();
			if (selected > -1)
			{
				int item = m_action_indices.Get(selected);
				
				AssignmentMenu assignmentsMenu = AssignmentMenu.Cast( EnterScriptedMenu(MENU_ASSIGNMENT) );
				if (assignmentsMenu)
				{
					assignmentsMenu.InitAssignment(item);
				}
				
				return true;
			}
		}
		
		if (w == m_devices)
		{
			selected = m_devices.GetSelectedRow();
			if (selected > -1)
			{
				m_device_button.Enable(g_Game.GetInput().IsCustomSchemeEnabled());
				
				if (g_Game.GetInput().IsDeviceEnabled(selected))
				{
					m_device_button.SetText("#menu_disable");
				}
				else
				{
					m_device_button.SetText("#menu_enable");
				}
			}
			else
			{
				m_device_button.Show(false);
			}
			return true;
		}
		return false;
	}
	
	override bool OnModalResult(Widget w, int x, int y, int code, int result)
	{
		super.OnModalResult(w, x, y, code, result);
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this.ApplyFn);
		
		if (result == DBB_YES)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this.RestartFn);
			return true;
		}
		return false;
	}
	
	override void OnShow()
	{
		super.OnShow();
		SelectActionGroup(m_action_groups.GetCurrentItem());
	}
	
	bool FocusPanel(bool show, Widget w)
	{
		Widget wid;
		Widget parent;
		if ( w.GetUserID() == PANEL_FOCUS_WIDGET )
		{
			wid = w;
		}
		if ( GetParentByID(w,PANEL_FOCUS_WIDGET) )
		{
			wid = GetParentByID(w,PANEL_FOCUS_WIDGET);
		}
		if ( wid )
		{
			parent = wid.GetParent();
			if ( parent )
			{
				GetChildByID(parent, PANEL_FOCUS_WIDGET).Show(show);
				return true;
			}
		}
		return false;
	}
	
	void SelectPreviousTab()
	{
		int currTab = m_selected_tab;
		currTab = (currTab - 1) % TABS_COUNT;
		if (currTab < 0)
		{
			currTab = TABS_COUNT - 1;
		}
		
		while(currTab != m_selected_tab)
		{
			if (m_tab_buttons[currTab] != NULL)
			{
				SelectTab(currTab);
				return;
			}
			
			currTab = (currTab - 1) % TABS_COUNT;
			if (currTab < 0)
			{
				currTab = TABS_COUNT - 1;
			}
		}
	}
	
	void SelectNextTab()
	{
		int currTab = m_selected_tab;
		currTab = (currTab + 1) % TABS_COUNT;
		
		while(currTab != m_selected_tab)
		{
			if (m_tab_buttons[currTab] != NULL)
			{
				SelectTab(currTab);	
				return;		
			}
			
			currTab = (currTab + 1) % TABS_COUNT;
		}
	}
	
	void SelectTab(int tab_id)
	{
		int i = 0;
		for (i = 0; i < TABS_COUNT; i++)
		{
			Widget tab = m_tabs[i];
			if (tab != NULL)
				tab.Show(i == tab_id);
			if (i == tab_id && tab_id != TAB_KEYBINDS) //determines which tab is open; exception for keybinds, may be remade
			{
				InitWidgetPosY(tab);
			}
		}
		
		if (tab_id != TAB_KEYBINDS)
		{
			for (i = 0; i < TABS_COUNT; i++)
			{
				ButtonWidget button = m_tab_buttons[i];
				ImageWidget backdrop = m_tab_images[i];
				if(button != NULL)
				{
					button.SetState(i == tab_id);
					backdrop.Show(i != tab_id);
				}
			}
			
		 	m_selected_tab = tab_id;
		}
	}
	
	void SetLanguage(int index, string language)
	{
		string setLanguage;
		SwitchOptionsAccess soa = SwitchOptionsAccess.Cast(m_language_option.GetOptionAccess());
		soa.GetItemText(setLanguage);
		m_OriginalLanguage = setLanguage;
		while (setLanguage != language)
		{
			soa.Switch();
			soa.GetItemText(setLanguage);
		}
		m_Language_DDM_button.SetText(setLanguage);
		if (setLanguage != m_OriginalLanguage)
		{
			m_languageChanged = true;
		}
	}
	
	void SetResolution(int index, string resolution)
	{
		ListOptionsAccess loa = ListOptionsAccess.Cast(m_resolution_option.GetOptionAccess());
		loa.SetIndex(index);
		m_Resolution_DDM_button.SetText(resolution);
	}
	
	/*void SetSmoothing(int index, string resolution)
	{
		ListOptionsAccess loa = ListOptionsAccess.Cast(m_ppaa_option.GetOptionAccess());
		loa.SetIndex(index);
		m_Smoothing_DDM_button.SetText(resolution);
	}*/
	
	void ScrollWidget(int wheel)
	{
		int i = 0;
		float pos_x;
		float pos_y;
		float bottom_x;
		float bottom_y;
		float top_x;
		float top_y;
		float scroll_speed = wheel/20;
		//float slider_min;
		float slider_max = m_current_scrollbar.GetMax();
		float adjusted_value;
		float scrolling_limit;
		
		Widget Child = Widget.Cast(m_current_scrolling_panel.GetChildren());
		
		m_current_bottom_offset.GetPos(bottom_x,bottom_y);
		m_current_top_offset.GetPos(top_x,top_y);
		scrolling_limit = -top_y + bottom_y - 1;
		
		if ((bottom_y < (1 + scroll_speed) && wheel < 0) || (top_y >= (0 - scroll_speed) && wheel > 0))
		{
			return;
		}
		
		if (wheel != 0)
		{
			m_scrollbar_value = m_current_scrollbar.GetCurrent() / slider_max;
			adjusted_value = (m_scrollbar_value * scrolling_limit + scroll_speed) / scrolling_limit * slider_max;//m_scrollbar_value/(scrolling_limit + 1)
			m_current_scrollbar.SetCurrent(adjusted_value);
			m_scrollbar_value = adjusted_value;
		}
		else //scrolls by scrollbar dragging
		{
			//slider_max = m_video_scrollbar.GetMax(); //100
			adjusted_value = m_scrollbar_value * scrolling_limit - scrolling_limit;
		}
		
		while (Child)
		{
			if (wheel != 0)
			{
				Child.GetPos(pos_x,pos_y);
				Child.SetPos(pos_x, Math.Clamp(pos_y + scroll_speed, (-scrolling_limit - 1),(scrolling_limit + 1)) ,true);
			}
			else if (adjusted_value != 0) //scrolls by scrollbar dragging
			{
				Child.GetPos(pos_x,pos_y);
				Child.SetPos(pos_x,m_original_widgets_y.Get(i) + adjusted_value,true);
				i++;
			}
			Child = Child.GetSibling();
		}
		if (m_current_scroll_slider.IsVisible())
		{
			float junk;
			float y_clamp;
			m_current_scroll_slider.GetSize(junk, y_clamp);
			float positionY = m_current_scrollbar.GetCurrent() / m_current_scrollbar.GetMax();
			positionY = Math.Clamp((1 - positionY),0,(1-y_clamp));
			m_current_scroll_slider.SetPos(0,positionY,true);
		}
	}
	
	void InitSetInvertButtons()
	{
		bool state;
		string text;
		SwitchOptionsAccess soa = SwitchOptionsAccess.Cast( m_invert_mouse_option.GetOptionAccess() );
		soa.GetItemText(text);
		state = (text != "Invert Mouse: Off");
		m_invert_mouse_off_button.SetState(!state);
		m_invert_mouse_on_button.SetState(state);
	}
	
	void SelectActionGroup(int group_index)
	{
		m_actions.ClearItems();
		g_Game.GetInput().GetActionGroupItems(group_index, m_action_indices);
		string desc;
		
		for (int i = 0; i < m_action_indices.Count(); i++)
		{
			int action_index = m_action_indices.Get(i);
			g_Game.GetInput().GetActionDesc(action_index, desc);
			int row = m_actions.AddItem(desc, NULL, 0);
			m_actions.SetItem(row, GetActionKeysString(action_index), NULL, 1);
		}
	}
	
	string GetActionKeysString(int action_index)
	{
		int key;
		string name;
		string output;
		output = "";
		g_Game.GetInput().GetActionKeys(action_index, m_keys);
		
		for (int i = 0; i < m_keys.Count(); i++)
		{
			key = m_keys.Get(i);
			g_Game.GetInput().GetActionKeyName(key, name);
			if (i > 0)
			{
				output += ",";
			}
			
			output += "'";
			output += name;
			output += "'";
			
			int conflictAction = g_Game.GetInput().CheckKeyCollision(action_index, key);
			if (conflictAction != -1)
			{
				output += "!";
			}
		}
		
		return output;
	}
	
	void UpdateChatMessages()
	{
		m_system_cm = GetProfileValueBool(SYSTEM_CHAT_MSG, true);
		m_global_cm = GetProfileValueBool(GLOBAL_CHAT_MSG, true);
		m_direct_cm = GetProfileValueBool(DIRECT_CHAT_MSG, true);
		m_vehicle_cm = GetProfileValueBool(VEHICLE_CHAT_MSG, true);
		m_radio_cm = GetProfileValueBool(RADIO_CHAT_MSG, true);	
	}
	
	//!Secondary init method for non-C++ based options (currently unused)
	void InitProfileSettings()
	{
	}
	
	//!Registers and inits non-C++ based options
	void RegisterProfileOptionWidgets()
	{
		int max = EDayZProfilesOptions.MAX;
		string str;
		ref array<Widget> buttonsArray;
		ProfileOptionsUI profileOptionUI;
		for (int i = 0; i < max; i++)
		{
			profileOptionUI = new ProfileOptionsUI;
			str = g_Game.GetProfileOptionMap().Get(i).param1;
			buttonsArray = new array<Widget>;
			
			switch (str)
			{
				case SHOW_CROSSHAIR:
					buttonsArray = GetArrayOfWidgetsById(WIDGET_PROFILE_USER_ID_OFFSET + EDayZProfilesOptions.CROSSHAIR);
				break;
				case SHOW_HUD:
					buttonsArray = GetArrayOfWidgetsById(WIDGET_PROFILE_USER_ID_OFFSET + EDayZProfilesOptions.HUD);
					/*buttonsArray.Insert(m_HudOffButton);
					buttonsArray.Insert(m_HudOnButton);*/
				break;
				case SHOW_QUICKBAR:
					buttonsArray = GetArrayOfWidgetsById(WIDGET_PROFILE_USER_ID_OFFSET + EDayZProfilesOptions.QUICKBAR);
					/*buttonsArray.Insert(m_QuickbarOffButton);
					buttonsArray.Insert(m_QuickbarOnButton);*/
				break;
				case SYSTEM_CHAT_MSG:
					buttonsArray = GetArrayOfWidgetsById(WIDGET_PROFILE_USER_ID_OFFSET + EDayZProfilesOptions.SERVER_MESSAGES);
				break;
				case DIRECT_CHAT_MSG:
					buttonsArray = GetArrayOfWidgetsById(WIDGET_PROFILE_USER_ID_OFFSET + EDayZProfilesOptions.USERS_CHAT);
				break;
				case RADIO_CHAT_MSG:
					buttonsArray = GetArrayOfWidgetsById(WIDGET_PROFILE_USER_ID_OFFSET + EDayZProfilesOptions.RADIO_CHAT);
				break;
			}
			
			if (buttonsArray.Count() > 0)
			{
				profileOptionUI.Attach(i, buttonsArray);
				m_profile_optionsUI.Insert(profileOptionUI);
			}
		}
		//Print(str);
	}
	
	//!Saves non-C++ based options to profile file
	void SaveProfileOptions()
	{
		bool state;
		ButtonWidget offButton;
		array<Widget> wArray = new array<Widget>;
		for (int i = 0; i < m_profile_optionsUI.Count(); i++)
		{
			wArray.Clear();
			wArray = m_profile_optionsUI.Get(i).GetWidgetArray();
			offButton = ButtonWidget.Cast( wArray.Get(0) ); //OFF button
			state = offButton.GetState();
			g_Game.SetProfileOption(i,!state);
		}
	}
	
	void UpdateDevices()
	{
		// devices
		int i;
		int c;
		int lastSelection = m_devices.GetSelectedRow();
		string name;
		
		m_devices.ClearItems();
		
		c = g_Game.GetInput().GetDevicesCount();
		for (i = 0; i < c; i++)
		{
			g_Game.GetInput().GetDeviceName(i, name);
			m_devices.AddItem(name, NULL, 0);
			int enabled = g_Game.GetInput().IsDeviceEnabled(i);
			
			if (enabled)
			{
				m_devices.SetItem(i, "#menu_enabled", NULL, 1);
			}
			else
			{
				m_devices.SetItem(i, "#menu_disabled", NULL, 1);
			}
		}
		
		if (c)
		{
			if (lastSelection >= 0 && lastSelection < c)
			{
				m_devices.SelectRow(lastSelection);
			}
			else
			{
				m_devices.SelectRow(0);
			}
			
			m_device_button.Enable(true);
			
			if (g_Game.GetInput().IsDeviceEnabled(m_devices.GetSelectedRow()))
			{
				m_device_button.SetText("#menu_disable");
			}
			else
			{
				m_device_button.SetText("#menu_enable");
			}
		}
		else
		{
			m_device_button.Enable(false);
		}
		
		int IsCustomSchemeEnabled = g_Game.GetInput().IsCustomSchemeEnabled();
		
		if (g_Game.GetInput().IsCustomSchemeEnabled())
		{
			m_device_custom_scheme.SetChecked(false);
			m_device_button.Show(true);
		}
		else
		{
			m_device_custom_scheme.SetChecked(true);
			m_device_button.Show(false);
		}
	}
	
	ref GameOptions m_Options;
	
	void TestFn()
	{
		m_Options.Test();
	
		// reload fillrate option
		m_rend_res_option.Init();
		m_rend_res_option.Update();
	}
	
	void ApplyFn()
	{/*
		//SetProfileValueBool(GLOBAL_CHAT_MSG, m_global_cm);
		//SetProfileValueBool(VEHICLE_CHAT_MSG, m_vehicle_cm);
		SetProfileValueBool(SHOW_HUD, m_ShowHud);
		SetProfileValueBool(SHOW_CROSSHAIR, m_ShowHud);
		SetProfileValueBool(SYSTEM_CHAT_MSG, m_system_cm);
		SetProfileValueBool(DIRECT_CHAT_MSG, m_direct_cm);
		SetProfileValueBool(RADIO_CHAT_MSG, m_radio_cm);
		SetProfileValueBool(SHOW_QUICKBAR, m_ShowQuickBar);
		*/
		string value;
		GetGame().GetProfileString("usenewui", value);
		
		SaveProfileOptions();
		
		GetGame().SaveProfile();
		m_Options.Apply();
		//
		m_hud = IngameHud.Cast( GetGame().GetMission().GetHud() );
		if (m_hud)
		{
			m_hud.Init(m_hud.GetHudPanelWidget());
		}
		//
		NumericOptionsAccess noa = NumericOptionsAccess.Cast(m_FOV_option.GetOptionAccess());
		float valueFOV = noa.ReadValue();
		g_Game.SetUserFOV(valueFOV);
		Close();
	}
	
	void RestartFn()
	{
		g_Game.RequestRestart(IDC_MAIN_QUIT);
	}
	
	void OverallQualityChanged()
	{
		m_overall_quality_option.GetOptionAccess().Test();
		m_overall_quality_option.GetOptionAccess().Apply();
		
		m_current_quality_value = ListOptionsAccess.Cast( m_overall_quality_option.GetOptionAccess() ).GetIndex();
	
		InitOptions();
		
		// workaround for quality presets
		ListOptionsAccessUI.Cast( m_overall_quality_option ).SetButtonStates( m_current_quality_value );
	}
	
	void DefaultVideoSettings()
	{
		if (m_default_video_access)
		{
			m_default_video_access.Switch();
			InitOptions();
		}
	}
	
	//TODO add functionality; set some reasonable default values either here (via NumericOptionsAccess) or in code
	void DefaultAudioSettings()
	{
		InitOptions();
	}
	
	void CheckOptionsDependency()
	{
		ListOptionsAccess listAccess;
		
		//ATOC hiding
		listAccess = ListOptionsAccess.Cast( m_msaa_option.GetOptionAccess() );
		SetMultipleWidgetFlags(m_atoc_option,WidgetFlags.DISABLED,listAccess.GetIndex() == 0);
		layoutRoot.FindAnyWidget("ATOCCoverPanelWidget").Show( listAccess.GetIndex() == 0); //0 == disabled
		
		//PPAA hiding
		listAccess = ListOptionsAccess.Cast( m_ppaa_option.GetOptionAccess() );
		SetMultipleWidgetFlags(m_ppaa_option,WidgetFlags.DISABLED,listAccess.GetIndex() == 0);
		layoutRoot.FindAnyWidget("PPAACoverPanelWidget").Show( listAccess.GetIndex() == 0);
		if ( listAccess.GetIndex() == 0 )
		{
			for (int i = 1; i < listAccess.GetItemsCount(); i++)
			{
				m_ppaa_option.GetWidgetArray().Get( i ).Show(false);
			}
			
			m_smaa_video_button.SetState(false);
			m_fxaa_video_button.SetState(false);
			//m_ppaa_option.SetButtonStates(0); //?
		}
		else if (listAccess.GetIndex() > 0 && listAccess.GetIndex() < OPTION_SMAA_LOW) //FXAA
		{
			for (i = 0; i < 4; i++)
			{
				m_ppaa_option.GetWidgetArray().Get( OPTION_SMAA_LOW + i ).Show(false);
				m_ppaa_option.GetWidgetArray().Get( OPTION_FXAA_LOW + i ).Show(true);
			}
			/*if (listAccess.GetIndex() < OPTION_FXAA_LOW || listAccess.GetIndex() >= OPTION_SMAA_LOW)
			{
				m_ppaa_option.SetButtonStates(OPTION_FXAA_LOW);
			}*/		
			m_fxaa_video_button.SetState(true);
			m_smaa_video_button.SetState(false);
		}
		else //SMAA
		{
			for (i = 0; i < 4; i++)
			{
				m_ppaa_option.GetWidgetArray().Get( OPTION_FXAA_LOW + i ).Show(false);
				m_ppaa_option.GetWidgetArray().Get( OPTION_SMAA_LOW + i ).Show(true);
			}
			/*if (listAccess.GetIndex() < OPTION_SMAA_LOW)
			{
				m_ppaa_option.SetButtonStates(OPTION_SMAA_LOW);
			}*/
			m_fxaa_video_button.SetState(false);
			m_smaa_video_button.SetState(true);
		}
		
		//Postprocessing hiding
		listAccess = ListOptionsAccess.Cast( m_postprocess_quality_option.GetOptionAccess() );
		layoutRoot.FindAnyWidget("PPCoverPanelWidget").Show( listAccess.GetIndex() == 0);
		layoutRoot.FindAnyWidget("AOCoverPanelWidget").Show( listAccess.GetIndex() < 2);
		SetMultipleWidgetFlags(m_bloom_option,WidgetFlags.DISABLED,listAccess.GetIndex() == 0);
		SetMultipleWidgetFlags(m_ao_option,WidgetFlags.DISABLED,listAccess.GetIndex() < 2);
	}
	
	void SetMultipleWidgetFlags(OptionsAccessUI oaUI, int flag, bool setflags)
	{
		array<Widget> temparray = new array<Widget>;
		temparray = oaUI.GetWidgetArray();
		if (!temparray) 	return;
		
		int count = temparray.Count();
		if (setflags)
		{
			for (int i = 0; i < count; i++)
			{
				temparray.Get(i).SetFlags(flag);
			}
		}
		else
		{
			for (i = 0; i < count; i++)
			{
				temparray.Get(i).ClearFlags(flag);
			}
		}
	}
	
	void ListAvailableLanguages(OptionsAccess optionAccess, out array<string> languages) //TODO extend to fill arrays for all further dropdown menus? Should differentiate between ListListOptionAccess and SwitchOptionsAccess
	{
		string OaValue;
		string OriginalLanguage;
		SwitchOptionsAccess soa = SwitchOptionsAccess.Cast(optionAccess);
		m_languages.Clear();
		//m_languages = new array<string>;
		
		soa.GetItemText(OaValue);
		if (OaValue != "English")
		{
			OriginalLanguage = OaValue;
			while(OaValue != "English") //switches to "English"
			{
				soa.Switch();
				soa.GetItemText(OaValue);
			}
		}
								
		languages.Insert(OaValue); //inserts "English" at 0
		soa.Switch();
		soa.GetItemText(OaValue);
		while(OaValue != "English")
		{
			languages.Insert(OaValue);
			soa.Switch();
			soa.GetItemText(OaValue);
		}
		if (OriginalLanguage != "") //returns language setting to its original value. Can be changed later by dropdown menu.
		{
			while(OaValue != OriginalLanguage)
			{
				soa.Switch();
				soa.GetItemText(OaValue);
			}
		}
	}
	
	void ListListOptionAccess(OptionsAccess optionAccess, out array<string> output)
	{
		if (optionAccess.GetControlType() == OA_CT_LIST )
		{
			string str;
			ListOptionsAccess loa = ListOptionsAccess.Cast(optionAccess);
			output.Clear();
			for (int i = 0; i < loa.GetItemsCount(); i++)
			{
				loa.GetItemText(i,str);
				output.Insert(str);
			}
		}
	}
	
	void InitWidgetPosY(Widget w) //CAUTION, used to initialize all panels and scrolling stuff...I hope...
	{
		bool scrollbar = false;
		int i = 0;
		float pos_x;
		float pos_y;
		float pos_y_2;
		array <float> widgets_zero_positions = new array <float>;
		Widget Child;
		
		m_current_scrolling_panel = NULL;
		m_current_top_offset = NULL;
		m_current_bottom_offset = NULL;
		m_current_scrollbar = NULL;
		m_current_scrolling_backdrop_image = NULL;
		
		m_original_widgets_y.Clear();
		
		Child = Widget.Cast(w.GetChildren());
		while (Child)
		{
			if (Child.GetUserID() == PANEL_WIDGET && !m_current_scrolling_panel)
			{
				m_current_scrolling_panel = Child;
				Child = m_current_scrolling_panel.GetChildren();
				while (Child) //widgets to be scrollable
				{
					if (Child.GetUserID() == TOP_OFFSET_WIDGET)
					{
						m_current_top_offset = Child;
					}
					if (Child.GetUserID() == BOTTOM_OFFSET_WIDGET)
					{
						m_current_bottom_offset = Child;
					}
					Child.GetPos(pos_x,pos_y);
					m_original_widgets_y.Insert(pos_y);
					Child = Child.GetSibling();
				}
				Child = Widget.Cast(w.GetChildren());
			}
			if (Child.GetUserID() == SCROLLBAR_BACKDROP_WIDGET)
			{
				m_current_scrolling_backdrop_image = Child;
				m_current_scrollbar = SliderWidget.Cast(Child.GetChildren());
				m_current_scroll_slider = GetChildByID(m_current_scrollbar, SCROLLBAR_SLIDER);
			}
			Child = Child.GetSibling();
		}
		//adjust widget location, hide/show scrollbar if needed
		m_current_top_offset.GetPos(pos_x, pos_y);
		m_current_bottom_offset.GetPos(pos_x, pos_y_2);
		
		m_current_scrolling_backdrop_image.Show((pos_y - pos_y_2) < -1);
		
		if (m_current_scrolling_backdrop_image.IsVisible() && pos_y != 0) //sets widgets to original positions, if needed
		{
			Child = m_current_scrolling_panel.GetChildren();
			while (Child)
			{
				Child.SetPos(pos_x,m_original_widgets_y.Get(i) - pos_y,true);
				Child.GetPos(pos_x,pos_y_2);
				widgets_zero_positions.Insert(pos_y_2);
				Child = Child.GetSibling();
				i++;
			}
			m_original_widgets_y.Clear();
			m_original_widgets_y.Copy(widgets_zero_positions);
		}
		m_scrollbar_value = 1;
	}
	
	Widget GetParentByID(Widget w, int id)
	{
		Widget parent;
		parent = w.GetParent();
		while (parent && parent.GetUserID() != id)
		{
			parent = parent.GetParent();
		}
		if (parent && parent.GetUserID() == id)
		{
			return parent;
		}
		else
		{
			return NULL;
		}
	}
	
	Widget GetChildByID(Widget w, int id)
	{
		Widget other;
		Widget child = w.GetChildren();
		while (child && child.GetUserID() != id)
		{
			if (child.GetChildren() == NULL)
			{
				child = child.GetSibling();
			}
			else
			{
				other = GetChildByID(child,id);
				if (other.GetUserID() == id)
				{
					return other;
				}
			}
		}
		if (child && child.GetUserID() == id)
		{
			return child;
		}
		else
		{
			return NULL;
		}
	}
	
	const int TAB_GAME = 0;
	const int TAB_SOUND = 1;
	const int TAB_VIDEO = 2;
	const int TAB_CONTROLS = 3;
	const int TAB_KEYBINDS = 4;
	const int TABS_COUNT = 5;
	
	const int WIDGET_USER_ID_OFFSET = 10000;
	const int WIDGET_PROFILE_USER_ID_OFFSET = 20000; //associated widget are indexed same as EDayZProfilesOptions enum
	
	const int PANEL_WIDGET = 200;
	const int SCROLLBAR_BACKDROP_WIDGET = 201;
	const int TOP_OFFSET_WIDGET = 202;
	const int BOTTOM_OFFSET_WIDGET = 203;
	const int SCROLLBAR_SLIDER = 204;
	//shared between "focus" effect panel widget (1st) and panel widget, containing all buttons/content (2nd). Both are inserted into container widget. 
	const int PANEL_FOCUS_WIDGET = 205;

	const int OPTION_FXAA_LOW = 1;
	const int OPTION_SMAA_LOW = 5;
	
	int 	m_current_quality_value; //used to set preset buttons (hack-ish stuff)
	
	float 	m_original_bottom_offset_y;
	float 	m_scrollbar_value;
	float	m_FOV_original_value;
	
	Widget 	m_tabs[TABS_COUNT];
	Widget 	m_keyboard_tab;
	
	Widget 	m_current_scrolling_panel;
	Widget 	m_current_scrolling_backdrop_image;
	Widget 	m_current_top_offset;
	Widget 	m_current_bottom_offset;
	Widget 	m_current_scroll_slider;
	
	ref TIntArray m_action_indices;
	ref TIntArray m_keys;
	
	bool	m_system_cm;
	bool	m_global_cm;
	bool	m_direct_cm;
	bool	m_vehicle_cm;
	bool	m_radio_cm;
	//bool	m_ShowQuickBar;
	//bool	m_ShowCrosshair;
	//int	m_ShowHud;
	
	CheckBoxWidget	m_system_cm_check;
	CheckBoxWidget	m_global_cm_check;
	CheckBoxWidget	m_direct_cm_check;
	CheckBoxWidget	m_vehicle_cm_check;
	CheckBoxWidget	m_radio_cm_check;
	CheckBoxWidget	m_ShowQuicBarCheck;
	//CheckBoxWidget	m_ShowHudCheck;
	
	ButtonWidget m_tab_buttons[TABS_COUNT];
	ImageWidget m_tab_images[TABS_COUNT];
	ButtonWidget m_controlls_keyboard;
	ButtonWidget m_OK_keyboard;
	ref array<ButtonWidget> m_array_buttons;
	ref array<float> m_original_widgets_y;
	
	XComboBoxWidget m_action_groups;
	TextListboxWidget m_actions;
	TextListboxWidget m_devices;
	ButtonWidget m_device_button;
	ButtonWidget m_presets_button;
	ButtonWidget m_assign_button;
	ButtonWidget m_test_button;
	ButtonWidget m_quality_set_button;
	ButtonWidget m_default_video_button;
	ButtonWidget m_default_audio_button;
	ButtonWidget m_fxaa_video_button;
	ButtonWidget m_smaa_video_button;

	ButtonWidget m_invert_mouse_off_button;
	ButtonWidget m_invert_mouse_on_button;
	
	//Dropdown Menus
	ButtonWidget m_Language_DDM_button;
	ButtonWidget m_Resolution_DDM_button;
	//ButtonWidget m_Smoothing_DDM_button;
	
	CheckBoxWidget m_device_custom_scheme;
	
	SliderWidget m_current_scrollbar;
	
	OptionsAccessUI m_invert_mouse_option;
	OptionsAccessUI m_rend_res_option;
	OptionsAccessUI m_language_option;
	OptionsAccessUI m_overall_quality_option;
	OptionsAccessUI m_resolution_option;
	SwitchOptionsAccess m_default_video_access;
	ListOptionsAccessUI m_ppaa_option;
	ListOptionsAccessUI m_msaa_option; // AT_FSAA_DETAIL
	ListOptionsAccessUI m_postprocess_quality_option; // AT_POSTPROCESS_EFFECTS
	OptionsAccessUI m_FOV_option;
	
	OptionsAccessUI m_atoc_option; // AT_ATOC_DETAIL
	OptionsAccessUI m_ao_option; // AT_AMBIENT_OCCLUSION
	OptionsAccessUI m_bloom_option; // AT_BLOOM
	//OptionsAccessUI m_rot_blur_option; // AT_ROTATION_BLUR
	
	ref TOptionsAccessUIArray m_optionsUI;
	ref TProfileOptionsUIArray m_profile_optionsUI;
	
	ref IngameHud	m_hud;
}
