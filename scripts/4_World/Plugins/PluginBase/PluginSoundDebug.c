class PluginSoundDebug extends PluginBase
{
	void OnInit()
	{
		m_TickTimer = new Timer();
		m_TickTimer.Run(0.1, this, "OnGUITimer", NULL, true);
	}
	
	void OnUpdate(float delta_time)
	{
		;
	}
	
	void OnDestroy()
	{
		m_TickTimer = NULL;
	}
	
	/*void Show()
	{
		m_TickTimer = new Timer();
		m_TickTimer.Run(0.1, this, "OnGUITimer", NULL, true);
	}
	
	void Hide()
	{
		m_TickTimer = NULL;
	}*/
	
	void OnGUITimer()
	{
		DbgUI.BeginCleanupScope();
		DbgUI.Begin("Sound debug", 10, 10);
		
		DbgUI.Text("SoundParams: ");
		DbgUI.SameLine();			
		string soundsetName = "BearGrowl_SoundSet";
		DbgUI.InputText("", soundsetName, 200);
		DbgUI.SameLine();
		if(DbgUI.Button("Create"))
		{
			m_soundParams = new SoundParams(soundsetName);
			m_soundBuilder = new SoundObjectBuilder(m_soundParams);
			m_soundObject = m_soundBuilder.BuildSoundObject();
			m_soundObject.SetPosition(GetGame().GetPlayer().GetPosition());
		}
		
		DbgUI.Text("SoundObjectBuilder: ");
		
		DbgUI.Text("SoundObject: ");
		
		DbgUI.Text("AbstractWave: ");
		
		if(m_soundParams != NULL && DbgUI.Button("Create and play"))
		{
			m_wave = m_soundObject.Play3D();
		}
		
		if(m_wave != NULL)
		{
			DbgUI.Text("Wave length: " + m_wave.GetLength() + "s");
			
			DbgUI.Text("Wave position: " + Math.Round(m_wave.GetCurrPosition() * 100) + "%");
			
			if(DbgUI.Button("Play"))
				m_wave.Play();
			if(DbgUI.Button("Stop"))
				m_wave.Stop();
			if(DbgUI.Button("Repeat"))
				m_wave.Repeat(2);
			if(DbgUI.Button("StopRepeat"))
				m_wave.Repeat(0);
		}
		
		DbgUI.End();
		DbgUI.EndCleanupScope();
	}
	
	
	ref Timer m_TickTimer;
	
	ref SoundParams m_soundParams;
	ref SoundObjectBuilder m_soundBuilder;
	ref SoundObject m_soundObject;
	AbstractWave m_wave;
}