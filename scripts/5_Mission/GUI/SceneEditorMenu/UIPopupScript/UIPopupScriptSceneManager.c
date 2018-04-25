class UIPopupScriptSceneManager extends UIPopupScript
{
	private ButtonWidget 		m_BtnCancel;
	private ButtonWidget 		m_BtnSceneNew;
	private ButtonWidget 		m_BtnSceneLoad;
	private ButtonWidget 		m_BtnSceneRename;
	private ButtonWidget 		m_BtnSceneDuplicate;
	private ButtonWidget 		m_BtnSceneDelete;
	private TextListboxWidget	m_LstListScenes;
	
	private static const int m_DaysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	
	private ButtonWidget	m_BtnSave;
	
	private TextWidget		m_TxtWeatherTime;
	private TextWidget		m_TxtAirTemperature;
	
	private SliderWidget 	m_SldStartTime;
	private TextWidget		m_TxtStartTimeValue;
	private SliderWidget 	m_SldStartDay;
	private TextWidget		m_TxtStartDayValue;
	private SliderWidget	m_SldOvercast;
	private TextWidget		m_TxtOvercastValue;
	private SliderWidget	m_SldRain;
	private TextWidget		m_TxtRainValue;
	private SliderWidget	m_SldFog;
	private TextWidget		m_TxtFogValue;
	private SliderWidget	m_SldWindForce;
	private TextWidget		m_TxtWindForceValue;
	
	private int				m_OrigYear;
	private int				m_OrigMonth;
	private int				m_OrigDay;
	private int				m_OrigHour;
	private int				m_OrigMinute;
	private float			m_OrigOvercast;
	private float			m_OrigRain;
	private float			m_OrigFog;
	private float			m_OrigWindForce;
	
	private int				m_CurrYear;
	private int				m_CurrMonth;
	private int				m_CurrDay;
	private int				m_CurrHour;
	private int				m_CurrMinute;
	private float			m_CurrOvercast;
	private float			m_CurrRain;
	private float			m_CurrFog;
	private float			m_CurrWindForce;
	
	//================================================
	// UIPopupScriptSceneManager
	//================================================	
	void UIPopupScriptSceneManager(Widget wgt)
	{
		m_BtnCancel			= ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_sm_cancel") );
		m_BtnSceneNew		= ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_sm_mission_new") );
		m_BtnSceneLoad		= ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_sm_mission_load") );
		m_BtnSceneRename	= ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_sm_mission_rename") );
		m_BtnSceneDelete	= ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_sm_mission_delete") );
		m_BtnSceneDuplicate	= ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_sm_mission_duplicate") );
		
		m_LstListScenes	= TextListboxWidget.Cast( wgt.FindAnyWidget("tls_ppp_sm_scene_list") );
		
		m_BtnSave			= ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_st_save") );
		
		m_TxtWeatherTime	= TextWidget.Cast( wgt.FindAnyWidget("txt_ppp_st_w_time_value") );
		m_TxtAirTemperature	= TextWidget.Cast( wgt.FindAnyWidget("txt_ppp_st_w_air_temp_value") );
		
		m_SldStartTime		= SliderWidget.Cast( wgt.FindAnyWidget("sld_ppp_st_start_time") );
		m_TxtStartTimeValue	= TextWidget.Cast( wgt.FindAnyWidget("txt_ppp_st_start_time_value") );
		
		m_SldStartDay		= SliderWidget.Cast( wgt.FindAnyWidget("sld_ppp_st_start_day") );
		m_TxtStartDayValue	= TextWidget.Cast( wgt.FindAnyWidget("txt_ppp_st_start_day_value") );
		
		m_SldOvercast		= SliderWidget.Cast( wgt.FindAnyWidget("sld_ppp_st_overcast") );
		m_TxtOvercastValue	= TextWidget.Cast( wgt.FindAnyWidget("txt_ppp_st_overcast_value") );
		
		m_SldRain			= SliderWidget.Cast( wgt.FindAnyWidget("sld_ppp_st_rain") );
		m_TxtRainValue		= TextWidget.Cast( wgt.FindAnyWidget("txt_ppp_st_rain_value") );
		
		m_SldFog			= SliderWidget.Cast( wgt.FindAnyWidget("sld_ppp_st_fog") );
		m_TxtFogValue		= TextWidget.Cast( wgt.FindAnyWidget("txt_ppp_st_fog_value") );
		
		m_SldWindForce		= SliderWidget.Cast( wgt.FindAnyWidget("sld_ppp_st_wind_force") );
		m_TxtWindForceValue	= TextWidget.Cast( wgt.FindAnyWidget("txt_ppp_st_wind_force_value") );
	}

	void ~UIPopupScriptSceneManager()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Remove(this.OnUpdate);
	}
	
	//================================================
	// OnClick
	//================================================	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		string scene_name;
		PluginSceneManager editor;
		
		if ( w == m_BtnCancel )
		{
			GetGame().GetWorld().SetDate( m_OrigYear, m_OrigMonth, m_OrigDay, m_OrigHour, m_OrigMinute );

			Weather weather = GetGame().GetWeather();
			weather.GetOvercast().Set( m_OrigOvercast, 0, 1000 );
			weather.GetRain().Set( m_OrigRain, 0, 1000 );
			weather.GetFog().Set( m_OrigFog, 0, 1000 );
			weather.SetWindSpeed( m_OrigWindForce );

			PopupBack();
			return true;
		}
		else if ( w == m_BtnSceneNew )
		{
			PopupOpen(SceneEditorMenu.POPUP_ID_SCENE_NEW, NULL);
			
			return true;
		}
		else if ( w == m_BtnSceneLoad )
		{
			if ( m_LstListScenes.GetSelectedRow() != -1 )
			{
				m_LstListScenes.GetItemText( m_LstListScenes.GetSelectedRow(), 0, scene_name );
				editor = PluginSceneManager.Cast( GetPlugin(PluginSceneManager) );
				
				editor.SceneLoad(scene_name);
				PopupBack();
				
				return true;
			}
		}
		else if ( w == m_BtnSceneRename )
		{
			m_LstListScenes.GetItemText( m_LstListScenes.GetSelectedRow(), 0, scene_name );
			
			UIPopupScriptSceneRename popup_rename = UIPopupScriptSceneRename.Cast( PopupOpen(SceneEditorMenu.POPUP_ID_SCENE_RENAME, NULL) );
			popup_rename.SetRenameName(scene_name);
			
			return true;
		}
		else if ( w == m_BtnSceneDuplicate )
		{
			if ( m_LstListScenes.GetSelectedRow() != -1 )
			{
				m_LstListScenes.GetItemText( m_LstListScenes.GetSelectedRow(), 0, scene_name );
				editor = PluginSceneManager.Cast( GetPlugin(PluginSceneManager) );
				
				editor.SceneDuplicate(scene_name);
				UpdateSceneList();
				
				return true;
			}
		}
		else if ( w == m_BtnSceneDelete )
		{
			if ( m_LstListScenes.GetSelectedRow() != -1 )
			{
				m_LstListScenes.GetItemText( m_LstListScenes.GetSelectedRow(), 0, scene_name );
				editor = PluginSceneManager.Cast( GetPlugin(PluginSceneManager) );
				
				editor.SceneDelete(scene_name);
				
				if ( editor.SceneCanDelete(scene_name) )
				{
					UIPopupScriptSceneDelete popup_delete = UIPopupScriptSceneDelete.Cast( PopupOpen(SceneEditorMenu.POPUP_ID_SCENE_DELETE, NULL) );
					popup_delete.SetDeleteName(scene_name);
				}
				else
				{
					UIPopupScriptNotify popup_notify = UIPopupScriptNotify.Cast( PopupOpen(SceneEditorMenu.POPUP_ID_NOTIFY, NULL) );
					popup_notify.SetLabelText("You cant delete current loaded scene!");
				}			
				
				UpdateSceneList();
				return true;
			}		
		}
		else if ( w == m_BtnSave )
		{		
			m_OrigYear = m_CurrYear;
			m_OrigMonth = m_CurrMonth;
			m_OrigDay = m_CurrDay;
			m_OrigHour = m_CurrHour;
			m_OrigMinute = m_CurrMinute;
			m_OrigOvercast = m_CurrOvercast;
			m_OrigRain = m_CurrRain;
			m_OrigFog = m_CurrFog;
			m_OrigWindForce = m_CurrWindForce;
			
			editor = PluginSceneManager.Cast( GetPlugin(PluginSceneManager) );
			editor.SetWeather(m_CurrOvercast, m_CurrRain, m_CurrFog, m_CurrWindForce);
			editor.SetDate(m_CurrYear, m_CurrMonth, m_CurrDay, m_CurrHour, m_CurrMinute);
			editor.SceneSave();
			
			PopupBack();
			
			return true;
		}
		
		return false;
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if ( w == m_SldStartTime )
		{		
			float slider_value_start_time = m_SldStartTime.GetCurrent() * 0.01;		
			float start_time_f = slider_value_start_time * 1439;
			int start_time = start_time_f;
			m_CurrHour = start_time / 60;
			m_CurrMinute = start_time % 60;
			
			UpdateSliderStartTime( m_CurrHour, m_CurrMinute );
			
			GetGame().GetWorld().SetDate( m_CurrYear, m_CurrMonth, m_CurrDay, m_CurrHour, m_CurrMinute );
			
			return true;
		}
		else if ( w == m_SldStartDay )
		{		
			float slider_value_start_day = m_SldStartDay.GetCurrent();
			float start_day_f = slider_value_start_day * 3.64 + 1;
			int start_day = start_day_f;
			
			for ( int i = 0; i < 12; i++ )
			{
				int days = m_DaysInMonth[i];
				if ( start_day <= days )
				{
					m_CurrMonth = i+1;
					m_CurrDay = start_day;
					break;
				}
				else
				{
					start_day -= days;
				}
			}
			
			UpdateSliderStartDay( m_CurrMonth, m_CurrDay );
			
			GetGame().GetWorld().SetDate( m_CurrYear, m_CurrMonth, m_CurrDay, m_CurrHour, m_CurrMinute );
			
			return true;
		}
		else if ( w == m_SldOvercast )
		{		
			m_CurrOvercast = m_SldOvercast.GetCurrent() * 0.01;		
			GetGame().GetWeather().GetOvercast().Set( m_CurrOvercast, 0, 1000 );

			UpdateSliderOvercast();
			
			return true;
		}
		else if ( w == m_SldRain )
		{		
			m_CurrRain = m_SldRain.GetCurrent() * 0.01;		
			GetGame().GetWeather().GetRain().Set( m_CurrRain, 0, 1000 );
			
			UpdateSliderRain();
			
			return true;
		}
		else if ( w == m_SldFog )
		{		
			m_CurrFog = m_SldFog.GetCurrent() * 0.01;		
			GetGame().GetWeather().GetFog().Set( m_CurrFog, 0, 1000 );

			UpdateSliderFog();
			
			return true;
		}
		else if ( w == m_SldWindForce )
		{		
			float wind_slider = m_SldWindForce.GetCurrent() * 0.01;
			m_CurrWindForce = GetGame().GetWeather().GetWindMaximumSpeed() * wind_slider;
			GetGame().GetWeather().SetWindSpeed( m_CurrWindForce );

			UpdateSliderWindForce();
			
			return true;
		}
		
		return false;
	}

	//================================================
	// UpdateSceneList
	//================================================	
	void UpdateSceneList()
	{
		m_LstListScenes.ClearItems();
		
		PluginSceneManager editor = PluginSceneManager.Cast( GetPlugin(PluginSceneManager) );
		
		TStringArray scene_list = editor.GetSceneList();
		string loaded_scene_name = editor.SceneGetName();
		
		for ( int i = 0; i < scene_list.Count(); ++i )
		{
			string scene_name = scene_list.Get(i);
			
			m_LstListScenes.AddItem(scene_name, NULL, 0);
			
			if ( scene_name == loaded_scene_name )
			{
				m_LstListScenes.SelectRow(i);
			}
		}
	}

	//================================================
	// UpdateSceneList
	//================================================	
	override void OnOpen(Param param)
	{
		//Log("Poup Scene Manager OnOpen");
		
		UpdateSceneList();
		GetGame().GetWorld().GetDate( m_OrigYear, m_OrigMonth, m_OrigDay, m_OrigHour, m_OrigMinute );

		Weather weather = GetGame().GetWeather();
		m_OrigOvercast = weather.GetOvercast().GetActual();
		m_OrigRain = weather.GetRain().GetActual();
		m_OrigFog = weather.GetFog().GetActual();
		m_OrigWindForce = weather.GetWindSpeed();

		m_CurrYear = m_OrigYear;
		m_CurrMonth = m_OrigMonth;
		m_CurrDay = m_OrigDay;
		m_CurrHour = m_OrigHour;
		m_CurrMinute = m_OrigMinute;
		m_CurrOvercast = m_OrigOvercast;
		m_CurrRain = m_OrigRain;
		m_CurrFog = m_OrigFog;
		m_CurrWindForce = m_OrigWindForce;
		
		Print( "Year" );
		Print( m_CurrYear );
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Insert(this.OnUpdate);
				
		ResetSliders();
	}
	
	//================================================
	// OnClose
	//================================================	
	override void OnClose()
	{		
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Remove(this.OnUpdate);
	}
	
	//================================================
	// OnUpdate
	//================================================	
	void OnUpdate()
	{
		m_TxtWeatherTime.SetText(GetGame().GetWeather().GetTime().ToString());
		m_TxtAirTemperature.SetText(GetGame().GetWeather().GetAirTemperature().ToString());
	}
	
	//================================================
	// ResetSliders
	//================================================	
	void ResetSliders()
	{
		int year, month, day, hour, minute;
		GetGame().GetWorld().GetDate( year, month, day, hour, minute );
		m_SldStartTime.SetCurrent( ((hour * 60) + minute) / 14.39 );
		UpdateSliderStartTime( hour, minute );
		
		float start_day = day;
		int month_tmp = month;
		while ( month_tmp > 1 )
		{
			month_tmp--;
			start_day += m_DaysInMonth[month];
		}
		m_SldStartDay.SetCurrent( start_day / 3.64 );
		UpdateSliderStartDay( month, day );

		Weather weather = GetGame().GetWeather();

		m_SldOvercast.SetCurrent(weather.GetOvercast().GetActual() * 100);
		UpdateSliderOvercast();

		m_SldRain.SetCurrent(weather.GetRain().GetActual() * 100);
		UpdateSliderRain();

		m_SldFog.SetCurrent(weather.GetFog().GetActual() * 100);
		UpdateSliderFog();

		float wind_slider = m_SldWindForce.GetCurrent() * 0.01;
		m_CurrWindForce = weather.GetWindMaximumSpeed() * wind_slider;
		weather.SetWindSpeed( m_CurrWindForce );
		UpdateSliderWindForce();
	}
	
	void UpdateSliderStartTime( int hour, int minute )
	{
		string label_text = hour.ToStringLen(2) + ":" + minute.ToStringLen(2);
		m_TxtStartTimeValue.SetText( label_text );
	}

	void UpdateSliderStartDay( int month, int day )
	{
		string label_text = day.ToString() + "." + month.ToString() + ". " + m_CurrYear.ToString();
		m_TxtStartDayValue.SetText( label_text );
	}

	void UpdateSliderOvercast()
	{
		string label_text = m_SldOvercast.GetCurrent().ToString()+"%";
		m_TxtOvercastValue.SetText( label_text );
	}

	void UpdateSliderRain()
	{
		string label_text = m_SldRain.GetCurrent().ToString()+"%";
		m_TxtRainValue.SetText( label_text );
	}

	void UpdateSliderFog()
	{
		string label_text = m_SldFog.GetCurrent().ToString()+"%";
		m_TxtFogValue.SetText( label_text );
	}

	void UpdateSliderWindForce()
	{
		string label_text = m_CurrWindForce.ToString()+"ms";
		m_TxtWindForceValue.SetText( label_text );
	}
}
