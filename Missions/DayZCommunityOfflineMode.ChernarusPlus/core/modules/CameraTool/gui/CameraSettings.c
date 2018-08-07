class CameraSettings extends UIScriptedMenu 
{
	
	private SliderWidget m_SldCamBlur;
	private TextWidget m_TxtCamBlur;
	
	private SliderWidget m_SldCamDist;
	private TextWidget m_TxtCamDist;
	
	private SliderWidget m_SldCamFLen;
	private TextWidget m_TxtCamFlen;
	
	private SliderWidget m_SldCamFnear;
	private TextWidget m_TxtCamFnear;
	
	
	protected CommunityOfflineMode m_Mission;
	
	void CameraSettings()
	{
		Init();
		
		m_Mission = CommunityOfflineMode.Cast( GetGame().GetMission() );
	}	
	
	void ~CameraSettings()
	{
	}
	
	override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\CameraTool\\gui\\layouts\\CameraSettings.layout" );

		m_SldCamBlur = SliderWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_blur") );
		m_TxtCamBlur = TextWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_blur_value") );
	
		m_SldCamDist = SliderWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_dist") );
		m_TxtCamDist = TextWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_dist_value") );
		
		m_SldCamFLen = SliderWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_flen" ) );
		m_TxtCamFlen = TextWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_flen_value") );

		m_SldCamFnear = SliderWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_fnear") );
		m_TxtCamFnear = TextWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_fnear_value") );
		
        return layoutRoot;

	}
	
	override bool UseKeyboard()	{
		return false;
	}
	
    override void OnShow()
    {
        super.OnShow();
    }

    override void OnHide()
    {
        super.OnHide();
    }
	
	override void Update(float timeslice)
	{
		UpdateSliders();
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if ( w.GetName() == "close_button" ) 
		{
			Close();
		}

		return false;
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if ( w == m_SldCamBlur ) 
		{
			CameraTool.CAMERA_BLUR = 4.0 * (m_SldCamBlur.GetCurrent() * 0.01); // percent
			
			if ( m_SldCamBlur.GetCurrent() == 0 ) 
			{
				CameraTool.CAMERA_DOF = false;
				PPEffects.ResetDOFOverride();
			} 
			else 
			{
				CameraTool.CAMERA_DOF = true;
			}
		} 
		else if ( w == m_SldCamDist ) 
		{
			if ( m_SldCamDist.GetCurrent() == 0 ) 
			{
				CameraTool.CAMERA_AFOCUS = true;
			} 
			else 
			{
				CameraTool.CAMERA_AFOCUS = false;
				CameraTool.CAMERA_FDIST = m_SldCamDist.GetCurrent() * 15;
			}
		}
		else if ( w == m_SldCamFLen ) 
		{
			CameraTool.CAMERA_FLENGTH = (m_SldCamFLen.GetCurrent());
		}
		else if ( w == m_SldCamFnear ) 
		{
			CameraTool.CAMERA_FNEAR = (m_SldCamFnear.GetCurrent());
		}
		return false;
	}
	
	void UpdateSliders() 
	{
		m_TxtCamBlur.SetText(((CameraTool.CAMERA_BLUR / 4.0) * 100.0).ToString() + "%");
		m_TxtCamDist.SetText(CameraTool.CAMERA_FDIST.ToString()+"m");
		m_TxtCamFlen.SetText(CameraTool.CAMERA_FLENGTH.ToString());
		m_TxtCamFnear.SetText(CameraTool.CAMERA_FNEAR.ToString());
	}
	
	void UpdateSlider(TextWidget w, string text) 
	{
		w.SetText( text );
	}
	
}