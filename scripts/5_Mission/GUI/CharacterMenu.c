
class CharacterMenu extends UIScriptedMenu
{
	ButtonWidget	m_genderButton;
	//---------------------------
	ButtonWidget	m_skinNextButton;
	ButtonWidget	m_skinPreviousButton;
	int				m_skinIndex;
	ButtonWidget	m_topNextButton;
	ButtonWidget	m_topPreviousButton;
	int				m_topIndex;
	ButtonWidget	m_pantsNextButton;
	ButtonWidget	m_pantsPreviousButton;
	int				m_pantsIndex;
	ButtonWidget	m_shoesNextButton;
	ButtonWidget	m_shoesPreviousButton;
	int				m_shoesIndex;
	//---------------------------
	EditBoxWidget 	m_name_widget;
	
	//string m_format;
	//bool m_scene.m_female;
	DayZIntroScene m_scene;

	void CharacterMenu()
	{
		
	}

	void ~CharacterMenu()
	{
		MainMenu menu;
		if (m_scene)
		{
			//m_scene.m_camera.LookAt(Vector(m_scene.m_demoPos[0] + Math.Cos(m_scene.angle + m_scene.angle_offset + Math.PI*4/3), m_scene.m_demoPos[1] + 0.75, m_scene.m_demoPos[2] + Math.Sin(m_scene.angle + m_scene.angle_offset + Math.PI*4/3)));
			m_scene.m_camera.LookAt(m_scene.target);
			//m_scene.angle_offset = 0;
			
			//sets name to widget in main menu
			if (g_Game.GetUIManager().FindMenu(MENU_MAIN))
			{
				MainMenu.CastTo( menu, g_Game.GetUIManager().FindMenu(MENU_MAIN) );
				//TextWidget m_name_widget = TextWidget.Cast( menu.layoutRoot.FindAnyWidget("CharacterNameText") );
				m_name_widget.SetText(g_Game.GetPlayerGameName());
			}
		}
	}

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_character_new.layout");

		Class.CastTo(m_genderButton, layoutRoot.FindAnyWidget("GenderButtonWidget"));
		//---------------------------
		Class.CastTo(m_skinNextButton, layoutRoot.FindAnyWidget("SkinNextButtonWidget"));
		Class.CastTo(m_skinPreviousButton, layoutRoot.FindAnyWidget("SkinPreviousButtonWidget"));
		Class.CastTo(m_topNextButton, layoutRoot.FindAnyWidget("TopNextButtonWidget"));
		Class.CastTo(m_topPreviousButton, layoutRoot.FindAnyWidget("TopPreviousButtonWidget"));
		Class.CastTo(m_pantsNextButton, layoutRoot.FindAnyWidget("PantsNextButtonWidget"));
		Class.CastTo(m_pantsPreviousButton, layoutRoot.FindAnyWidget("PantsPreviousButtonWidget"));
		Class.CastTo(m_shoesNextButton, layoutRoot.FindAnyWidget("ShoesNextButtonWidget"));
		Class.CastTo(m_shoesPreviousButton, layoutRoot.FindAnyWidget("ShoesPreviousButtonWidget"));
		//---------------------------
		
		MissionMainMenu mission;
		Class.CastTo(mission,  g_Game.GetMission() );
		m_scene = mission.GetIntroScene();
		m_scene.m_camera.LookAt(Vector(m_scene.m_demoPos[0],m_scene.m_demoPos[1] + 1,m_scene.m_demoPos[2]));
		//DefaultCharacterScreen();
		Class.CastTo(m_name_widget ,layoutRoot.FindAnyWidget("CharacterNameEdit"));
		m_name_widget.SetText(g_Game.GetPlayerGameName());
		m_name_widget.SetHandler(m_scene.m_player_name_handler);
		
		if (m_scene.m_female)
		{
			m_genderButton.SetText("F");
		}
		else
		{
			m_genderButton.SetText("M");
		}
		
		ShowMenuButtons(g_Game.IsNewCharacter());
		return layoutRoot;
	}
	
	override void OnShow()
	{
	}
		
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (m_scene.m_disableClick)
		{
			return false;
		} 
		super.OnClick(w, x, y, button);
		
		switch (w.GetUserID())
		{
		case IDC_OK:
			//m_scene.SaveDefaultCharacter();
			//Close();
			if (m_scene && m_scene.m_demoUnit)
			{
				//saves demounit for further use
				m_scene.SaveCharName();
				if (m_scene.m_demoUnit.GetInventory().FindAttachment(InventorySlots.BODY) && m_scene.CurrentCharacterID() == -1)		m_scene.SetCharacterInfo();
				
				if (!g_Game.IsNewCharacter()) 		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallByName(this, "ConnectLastSession");
				else 								GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallByName(this, "ConnectBestServer");
			}
			
			return true;
			
		case IDC_CANCEL:
			//g_Game.ObjectDelete(m_scene.m_demoUnit);
			//m_scene.m_demoUnit = NULL;
			m_scene.SaveCharName();
			Close();
			return true;
		
		case IDC_RESTART:
			RandomCharacter();
			return true;
		}
//------------------------------------------------------------------------
		switch (w)
		{
			case m_genderButton:
				if (!m_scene.m_female)
				{
					m_scene.m_female = true;
					m_genderButton.SetText("F");
				}
				else
				{
					m_scene.m_female = false;
					m_genderButton.SetText("M");
				}
				m_skinIndex = 0;
				
				SetCharacter();
			break;
			
			case m_skinNextButton:
				m_skinIndex++;
				SetCharacter();
			break;
			
			case m_skinPreviousButton:
				m_skinIndex--;
				SetCharacter();
			break;
			
			case m_topNextButton:
				m_topIndex++;
				if (m_topIndex < 0)									m_topIndex = m_scene.m_shirtList.Count() - 1;
				if (m_topIndex > m_scene.m_shirtList.Count() - 1)			m_topIndex = 0;
				m_scene.SetAttachment(m_scene.m_shirtList.Get(m_topIndex), InventorySlots.BODY);
			break;
			
			case m_topPreviousButton:
				m_topIndex--;
				if (m_topIndex < 0)									m_topIndex = m_scene.m_shirtList.Count() - 1;
				if (m_topIndex > m_scene.m_shirtList.Count() - 1)			m_topIndex = 0;
				m_scene.SetAttachment(m_scene.m_shirtList.Get(m_topIndex), InventorySlots.BODY);
			break
			
			case m_pantsNextButton:
				m_pantsIndex++;
				if (m_pantsIndex < 0)								m_pantsIndex = m_scene.m_pantsList.Count() - 1;
				if (m_pantsIndex > m_scene.m_pantsList.Count() - 1)			m_pantsIndex = 0;
				m_scene.SetAttachment(m_scene.m_pantsList.Get(m_pantsIndex), InventorySlots.LEGS);
			break;
			
			case m_pantsPreviousButton:
				m_pantsIndex--;
				if (m_pantsIndex < 0)								m_pantsIndex = m_scene.m_pantsList.Count() - 1;
				if (m_pantsIndex > m_scene.m_pantsList.Count() - 1)			m_pantsIndex = 0;
				m_scene.SetAttachment(m_scene.m_pantsList.Get(m_pantsIndex), InventorySlots.LEGS);
			break;
			
			case m_shoesNextButton:
				m_shoesIndex++;
				if (m_shoesIndex < 0)								m_shoesIndex = m_scene.m_shoesList.Count() - 1;
				if (m_shoesIndex > m_scene.m_shoesList.Count() - 1)			m_shoesIndex = 0;
				m_scene.SetAttachment(m_scene.m_shoesList.Get(m_shoesIndex), InventorySlots.FEET);
			break;
			
			case m_shoesPreviousButton:
				m_shoesIndex--;
				if (m_shoesIndex < 0)								m_shoesIndex = m_scene.m_shoesList.Count() - 1;
				if (m_shoesIndex > m_scene.m_shoesList.Count() - 1)			m_shoesIndex = 0;
				m_scene.SetAttachment(m_scene.m_shoesList.Get(m_shoesIndex), InventorySlots.FEET);
			break;
		}
//------------------------------------------------------------------------
		return false;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		super.OnMouseButtonDown(w, x, y, button);
		
		if (w.GetName() == "CharacterRotationFrame")
		{
			m_scene.CharacterRotationStart();
			return true;
		}

		return false;
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		super.OnMouseButtonUp(w, x, y, button);
		
		m_scene.CharacterRotationStop();
		return true;
	}
	
	override bool OnMouseWheel(Widget  w, int  x, int  y, int wheel)
	{
	}
	
	void SetCharacter()
	{
		string character;
		//string params[2];

		if (m_scene.m_female)
		{
			if (m_skinIndex < 0)											m_skinIndex = m_scene.m_personalityFemaleList.Count() - 1;
			if (m_skinIndex > m_scene.m_personalityFemaleList.Count() - 1)	m_skinIndex = 0;
			
			/*params[0] = "F";
			params[1] = m_scene.m_personalityFemaleList.Get(m_skinIndex);*/
			character = m_scene.m_personalityFemaleList.Get(m_skinIndex);
		}
		else
		{
			if (m_skinIndex < 0)											m_skinIndex = m_scene.m_personalityMaleList.Count() - 1;
			if (m_skinIndex > m_scene.m_personalityMaleList.Count() - 1)	m_skinIndex = 0;
			
			/*params[0] = "M";
			params[1] = m_scene.m_personalityMaleList.Get(m_skinIndex);*/
			character = m_scene.m_personalityMaleList.Get(m_skinIndex);
		}
		
		//g_Game.FormatString(m_scene.m_format, params, character);
		m_scene.CreateNewCharacter(character);
		if (m_scene.m_demoUnit)
		{
			m_scene.SetAttachment(m_scene.m_shirtList.Get(m_topIndex), InventorySlots.BODY);
			m_scene.SetAttachment(m_scene.m_pantsList.Get(m_pantsIndex), InventorySlots.LEGS);
			m_scene.SetAttachment(m_scene.m_shoesList.Get(m_shoesIndex), InventorySlots.FEET);
			/////////allows for attachment of items to other slots, if needed
			if (g_Game.IsNewCharacter())
			{
				m_scene.SetAttachment("", InventorySlots.SHOULDER);
				m_scene.SetAttachment("", InventorySlots.BOW);
				m_scene.SetAttachment("", InventorySlots.MELEE);
				m_scene.SetAttachment("", InventorySlots.VEST);
				m_scene.SetAttachment("", InventorySlots.HIPS);
				m_scene.SetAttachment("", InventorySlots.BACK);
				m_scene.SetAttachment("", InventorySlots.HEADGEAR);
				m_scene.SetAttachment("", InventorySlots.MASK);
				m_scene.SetAttachment("", InventorySlots.EYEWEAR);
				m_scene.SetAttachment("", InventorySlots.GLOVES);
				m_scene.SetAttachment("", InventorySlots.ARMBAND);
				m_scene.SetAttachment("", InventorySlots.HANDS); //5 = hands slot
				//Print(InventorySlots.GetSlotIdFromString("BACK"));
			}
			
			m_scene.m_disableClick = true;
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(m_scene.SceneCharacterSetPos, 250);
		}
	}

	void RandomCharacter()
	{
		g_Game.SetNewCharacter(true);
		g_Game.SetPlayerGameName("Survivor");
		ShowMenuButtons(true);
		m_scene.m_currentCharacterID = -1;
		m_name_widget.SetText(g_Game.GetPlayerGameName());		
		
		// make random selection
		m_scene.RandomSelectGender();
		
		if (m_scene.m_female)
		{
			m_genderButton.SetText("F");
			m_skinIndex =  m_scene.RandomSelectIndex(m_scene.m_personalityFemaleList);
		}
		else
		{
			m_genderButton.SetText("M");
			m_skinIndex =  m_scene.RandomSelectIndex(m_scene.m_personalityMaleList);
		}
		
		m_topIndex = m_scene.RandomSelectIndex(m_scene.m_shirtList);
		m_pantsIndex = m_scene.RandomSelectIndex(m_scene.m_pantsList);
		m_shoesIndex = m_scene.RandomSelectIndex(m_scene.m_shoesList);
		
		// update character
		SetCharacter();
	}
	
	int ListLoad(string path, out TStringArray list);
	
	void ConnectLastSession()
	{
		//TODO fix code-side
		if ( !g_Game.ConnectLastSession(this, m_scene.CurrentCharacterID()) )
		{
			g_Game.GetUIManager().EnterServerBrowser(this);
		}
	}
	
	void ConnectBestServer()
	{
		//TODO add functionality! For now works as follows:
		ConnectLastSession();
	}
	
	void ShowMenuButtons(bool show)
	{
		layoutRoot.FindAnyWidget("CharacterPanel").Show(show);
	}
}