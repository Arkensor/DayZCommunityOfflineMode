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

	private ButtonWidget m_btn_rot;
	private ButtonWidget m_btn_phi

	void CameraSettings()
	{
		Init();
	}	
	
	void ~CameraSettings()
	{
	}
	
	override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets( "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\CameraTool\\gui\\layouts\\CameraSettings.layout" );

		m_SldCamBlur = SliderWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_blur") );
		m_TxtCamBlur = TextWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_blur_value") );
	
		m_SldCamDist = SliderWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_dist") );
		m_TxtCamDist = TextWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_dist_value") );
		
		m_SldCamFLen = SliderWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_flen" ) );
		m_TxtCamFlen = TextWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_flen_value") );
		
		m_SldCamFnear = SliderWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_fnear") );
		m_TxtCamFnear = TextWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_fnear_value") );
		
		m_btn_rot = ButtonWidget.Cast( layoutRoot.FindAnyWidget("camera_btn_rot"));
		m_btn_phi = ButtonWidget.Cast( layoutRoot.FindAnyWidget("camera_btn_phi"));
		
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

		if ( w == m_btn_rot ) 
		{
			CameraTool.CAMERA_ROT.Show( !CameraTool.CAMERA_ROT.IsVisible() );
		}

		if ( w == m_btn_phi )
		{
			CameraTool.CAMERA_PHI.Show( !CameraTool.CAMERA_PHI.IsVisible() );
		}

		if ( w.GetName() == "camera_speed_btn_inc" ) 
		{
			CameraTool.CAMERA_SPEED += 0.25;
			CameraTool.CAMERA_MAXSPEED += 0.25;
		}

		if ( w.GetName() == "camera_speed_btn_dec" ) 
		{
			CameraTool.CAMERA_SPEED -= 0.25;
			CameraTool.CAMERA_MAXSPEED -= 0.25;
		}

		if ( w.GetName() == "camera_smooth_btn_inc" )
		{
			CameraTool.CAMERA_SMOOTH += 0.05;
			CameraTool.CAMERA_SMOOTH = Math.Clamp(CameraTool.CAMERA_SMOOTH, 0.0, 1.0); // ugh
		}

		if ( w.GetName() == "camera_smooth_btn_dec" )
		{
			CameraTool.CAMERA_SMOOTH -= 0.05;
			CameraTool.CAMERA_SMOOTH = Math.Clamp(CameraTool.CAMERA_SMOOTH, 0.0, 1.0);
		}

		if ( w.GetName() == "camera_msens_btn_inc" ) 
		{
			CameraTool.CAMERA_MSENS += 0.05;
			CameraTool.CAMERA_MSENS = Math.Clamp(CameraTool.CAMERA_MSENS, 0.0, 1.0);
		}

		if ( w.GetName() == "camera_msens_btn_dec" ) 
		{
			CameraTool.CAMERA_MSENS -= 0.05;
			CameraTool.CAMERA_MSENS = Math.Clamp(CameraTool.CAMERA_MSENS, 0.0, 1.0);
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

		string autoF = "";
		if ( CameraTool.CAMERA_AFOCUS ) 
		{
			autoF = " (AUTO)"
		}
		m_TxtCamDist.SetText(CameraTool.CAMERA_FDIST.ToString()+"m" + autoF);
		m_TxtCamFlen.SetText(CameraTool.CAMERA_FLENGTH.ToString());
		m_TxtCamFnear.SetText(CameraTool.CAMERA_FNEAR.ToString());
	}
	
	void UpdateSlider(TextWidget w, string text) 
	{
		w.SetText( text );
	}
	
}