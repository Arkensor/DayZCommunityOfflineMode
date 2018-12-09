/*
	Manages all keybinds for COM
*/
class CustomDebugMonitor extends Module
{
	protected bool m_IsVisible = false;
	protected ref ArkDebugMonitor m_debugMonitor;

	void CustomDebugMonitor()
	{
	    m_debugMonitor = new ArkDebugMonitor;
	}

	void ~CustomDebugMonitor()
	{
	}
	
	override void Init() 
	{
		super.Init();

		m_debugMonitor.Init();
		m_debugMonitor.Hide();
	}
	
	override void onUpdate( float timeslice )
	{
        if( m_IsVisible )
        {
            PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );

            if ( player )
            {
                DebugMonitorValues values = player.GetDebugMonitorValues();

                if ( values )
                {
                    auto health = player.GetHealth("","Health");
                    auto blood = player.GetHealth("","Blood");
                    auto lastdamage = values.GetLastDamage();

                    if( lastdamage == "" )
                    {
                        lastdamage = "Unknown";
                    }

                    auto position = player.GetPosition();
                    auto orientation = GetGame().GetCurrentCameraDirection().VectorToAngles();

                    m_debugMonitor.SetHealth( health );
                    m_debugMonitor.SetBlood( blood );
                    m_debugMonitor.SetLastDamage( lastdamage );
                    m_debugMonitor.SetPosition( position );
                    m_debugMonitor.SetOrientation( orientation );

                    int year;
                    int month;
                    int day;
                    int hour;
                    int minute;
                    int second;

                    GetYearMonthDay(year, month, day);
                    GetHourMinuteSecond(hour, minute, second);

                    string date = day.ToStringLen(2) + "-" + month.ToStringLen(2) + "-" + year.ToStringLen(2) + " " + hour.ToStringLen(2) + ":" + minute.ToStringLen(2) + ":" + second.ToStringLen(2);

                    m_debugMonitor.SetDateTime( date );
                }
            }
        }
	}
	
	override void RegisterKeyMouseBindings() 
	{
		auto oToggleMonitor = new KeyMouseBinding( GetModuleType(), "ToggleMonitor", "Toggles the debug monitor." );

		oToggleMonitor.AddBinding( "kB" );

		RegisterKeyMouseBinding( oToggleMonitor );
	}

    void ToggleMonitor()
    {
        m_IsVisible = !m_IsVisible;

        if( m_IsVisible )
        {
            m_debugMonitor.Show();
        }
        else
        {
            m_debugMonitor.Hide();
        }
    }
}