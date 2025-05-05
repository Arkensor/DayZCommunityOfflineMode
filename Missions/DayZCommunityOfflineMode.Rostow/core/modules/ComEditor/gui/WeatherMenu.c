class WeatherMenu extends PopupMenu
{
	private static const int m_DaysInMonth [ 12 ] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    protected ButtonWidget m_BtnSave;
    protected ButtonWidget m_BtnCancel;
	protected TextWidget m_TxtWeatherTime;
	protected TextWidget m_TxtAirTemperature;
	protected SliderWidget m_SldStartTime;
	protected TextWidget m_TxtStartTimeValue;
	protected SliderWidget m_SldStartDay;
	protected TextWidget m_TxtStartDayValue;
	protected SliderWidget m_SldOvercast;
	protected TextWidget m_TxtOvercastValue;
	protected SliderWidget m_SldRain;
	protected TextWidget m_TxtRainValue;
	protected SliderWidget m_SldFog;
	protected TextWidget m_TxtFogValue;
	protected SliderWidget m_SldWindForce;
	protected TextWidget m_TxtWindForceValue;

	private int	  m_OrigYear;
	private int	  m_OrigMonth;
	private int	  m_OrigDay;
	private int	  m_OrigHour;
	private int	  m_OrigMinute;
	private float m_OrigOvercast;
	private float m_OrigRain;
	private float m_OrigFog;
	private float m_OrigWindForce;

	private int	  m_CurrYear;
	private int	  m_CurrMonth;
	private int	  m_CurrDay;
	private int	  m_CurrHour;
	private int	  m_CurrMinute;
	private float m_CurrOvercast;
	private float m_CurrRain;
	private float m_CurrFog;
	private float m_CurrWindForce;

	void WeatherMenu()
	{
	}

	void ~WeatherMenu()
	{
	}

	void Init()
	{

        m_BtnSave			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_save" ) );
        m_BtnCancel			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_cancel" ) );

		m_TxtWeatherTime	= TextWidget.Cast( layoutRoot.FindAnyWidget( "txt_ppp_st_w_time_value" ) );
		m_TxtAirTemperature	= TextWidget.Cast( layoutRoot.FindAnyWidget( "txt_ppp_st_w_air_temp_value" ) );

		m_SldStartTime		= SliderWidget.Cast( layoutRoot.FindAnyWidget( "sld_ppp_st_start_time" ) );
		m_TxtStartTimeValue	= TextWidget.Cast( layoutRoot.FindAnyWidget( "txt_ppp_st_start_time_value" ) );

		m_SldStartDay		= SliderWidget.Cast( layoutRoot.FindAnyWidget( "sld_ppp_st_start_day" ) );
		m_TxtStartDayValue	= TextWidget.Cast( layoutRoot.FindAnyWidget( "txt_ppp_st_start_day_value" ) );

		m_SldOvercast		= SliderWidget.Cast( layoutRoot.FindAnyWidget( "sld_ppp_st_overcast" ) );
		m_TxtOvercastValue	= TextWidget.Cast( layoutRoot.FindAnyWidget( "txt_ppp_st_overcast_value" ) );

		m_SldRain			= SliderWidget.Cast( layoutRoot.FindAnyWidget( "sld_ppp_st_rain" ) );
		m_TxtRainValue		= TextWidget.Cast( layoutRoot.FindAnyWidget( "txt_ppp_st_rain_value" ) );

		m_SldFog			= SliderWidget.Cast( layoutRoot.FindAnyWidget( "sld_ppp_st_fog" ) );
		m_TxtFogValue		= TextWidget.Cast( layoutRoot.FindAnyWidget( "txt_ppp_st_fog_value" ) );

		m_SldWindForce		= SliderWidget.Cast( layoutRoot.FindAnyWidget( "sld_ppp_st_wind_force" ) );
		m_TxtWindForceValue	= TextWidget.Cast( layoutRoot.FindAnyWidget( "txt_ppp_st_wind_force_value" ) );
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{

		if ( w == m_BtnSave )
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

			PluginSceneManager editor = PluginSceneManager.Cast( GetPlugin(PluginSceneManager) );
			editor.SetWeather(m_CurrOvercast, m_CurrRain, m_CurrFog, m_CurrWindForce);
			editor.SetDate(m_CurrYear, m_CurrMonth, m_CurrDay, m_CurrHour, m_CurrMinute);

            GetGame().GetWeather().SetWindFunctionParams( m_OrigWindForce, m_CurrWindForce, 1 );

			return true;
		}

		return false;
	}

	override bool OnChange( Widget w, int x, int y, bool finished )
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
			UpdateSliderOvercast();

			m_CurrOvercast = m_SldOvercast.GetCurrent() * 0.01;
			GetGame().GetWeather().GetOvercast().Set( m_CurrOvercast, 0, 0 );
			GetGame().GetWorld().SetDate( m_CurrYear, m_CurrMonth, m_CurrDay, m_CurrHour, m_CurrMinute+5 );

			return true;
		}
		else if ( w == m_SldRain )
		{
			UpdateSliderRain();

			m_CurrRain = m_SldRain.GetCurrent() * 0.01;
			GetGame().GetWeather().GetRain().Set( m_CurrRain, 0, 0 );

			float density = m_CurrRain * m_CurrRain; 
			float threshold = 0.8; 
			float timeOut = 4000;

			GetGame().GetWeather().SetStorm(density, threshold, timeOut);

			return true;
		}
		else if ( w == m_SldFog )
		{
			UpdateSliderFog();

			m_CurrFog = m_SldFog.GetCurrent() * 0.01;
			GetGame().GetWeather().GetFog().Set( m_CurrFog, 0, 0 );

			return true;
		}
		else if ( w == m_SldWindForce )
		{
			UpdateSliderWindForce();

			m_CurrWindForce = m_SldWindForce.GetCurrent() * 0.01;

            GetGame().GetWeather().SetWindFunctionParams( m_OrigWindForce, m_CurrWindForce, 1 );

			return true;
		}

		return false;
	}

	override void OnShow()
	{
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

		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);

		ResetSliders();
	}

	override void OnHide()
	{
		/*
		Weather weather = GetGame().GetWeather();

		GetGame().GetWorld().SetDate( m_OrigYear, m_OrigMonth, m_OrigDay, m_OrigHour, m_OrigMinute );
		weather.GetOvercast().Set( m_OrigOvercast, 0, 1000 );
		weather.GetRain().Set( m_OrigRain, 0, 1000 );
		weather.GetFog().Set( m_OrigFog, 0, 1000 );
		weather.SetWindSpeed( m_OrigWindForce );

		*/
	}

	void Update()
	{
		m_TxtWeatherTime.SetText( GetGame().GetWeather().GetTime().ToString() );
		// m_TxtAirTemperature.SetText( GetGame().GetWeather().GetAirTemperature().ToString() );
	}

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

		float slider_wind_value = ( weather.GetWindSpeed() / weather.GetWindMaximumSpeed() ) * 100;

		if( weather.GetWindSpeed() < 0.01 )
		{
		    slider_wind_value = 0.0;
		}

		m_SldWindForce.SetCurrent( slider_wind_value );

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
		string label_text = m_SldWindForce.GetCurrent().ToString()+"%";
		m_TxtWindForceValue.SetText( label_text );
	}
}