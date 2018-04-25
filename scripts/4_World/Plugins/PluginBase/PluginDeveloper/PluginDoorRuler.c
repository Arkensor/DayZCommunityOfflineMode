class PluginDoorRuler extends PluginBase
{
	PluginDeveloper m_ModuleDeveloper;
	ItemBase m_Ruler;
	ref Timer m_Timer;
	bool m_Initialized;
	
	void CheckInit()
	{
		if( FreeDebugCamera.GetInstance().IsActive() ) 
			Init();
	}
	
	void Init()
	{
		if(m_Ruler)
			m_Ruler.Delete();
		
		m_Initialized = true;
		m_Ruler = ItemBase.Cast( GetGame().CreateObject("DoorTestCamera", FreeDebugCamera.GetInstance().GetPosition(), false, false/*ai*/) );
	}
	
	override void OnUpdate(float delta_time)
	{
		if(!m_Initialized) return;
		if(!FreeDebugCamera.GetInstance().IsActive()) return;
		if(!m_Ruler) return;
		vector cam_pos = FreeDebugCamera.GetInstance().GetPosition();
		vector ruler_pos = m_Ruler.GetPosition();
		vector camera_dir = FreeDebugCamera.GetInstance().GetDirection(); 
		camera_dir.Normalize();
		m_Ruler.SetPosition(FreeDebugCamera.GetInstance().GetPosition() + ( camera_dir * 2) );
		m_Ruler.SetAngles(FreeDebugCamera.GetInstance().GetAngles());
	}
}
