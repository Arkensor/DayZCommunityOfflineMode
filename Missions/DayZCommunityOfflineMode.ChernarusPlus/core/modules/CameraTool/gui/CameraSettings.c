class CameraSettings extends PopupMenu 
{
	
	private SliderWidget m_SldCamBlur;
	private TextWidget m_TxtCamBlur;
	
	private SliderWidget m_SldCamDist;
	private TextWidget m_TxtCamDist;
	
	private SliderWidget m_SldCamFLen;
	private TextWidget m_TxtCamFlen;
	
	private SliderWidget m_SldCamFnear;
	private TextWidget m_TxtCamFnear;

	private SliderWidget m_SldCamExp;
	private TextWidget m_TxtCamExp;

	private ButtonWidget m_btn_rot;
	private ButtonWidget m_btn_phi

	static Widget CAMERA_ROT;
	static Widget CAMERA_PHI;

	private SliderWidget m_SldChromX;
	private SliderWidget m_SldChromY;

	private TextWidget m_TxtChromX;
	private TextWidget m_TxtChromY;

	private ref WidgetStore widgetStore;

	void CameraSettings()
	{
	}	
	
	void ~CameraSettings()
	{
	}
	
	void Init()
    {
    	widgetStore = new WidgetStore( layoutRoot );

		m_SldCamBlur = SliderWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_blur") );
		m_TxtCamBlur = TextWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_blur_value") );
	
		m_SldCamDist = SliderWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_dist") );
		m_TxtCamDist = TextWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_dist_value") );
		
		m_SldCamFLen = SliderWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_flen" ) );
		m_TxtCamFlen = TextWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_flen_value") );
		
		m_SldCamFnear = SliderWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_fnear") );
		m_TxtCamFnear = TextWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_fnear_value") );
		
		m_SldCamExp = layoutRoot.FindAnyWidget("camera_slider_exp");
		m_TxtCamExp = layoutRoot.FindAnyWidget("camera_slider_exp_value");

		m_btn_rot = ButtonWidget.Cast( layoutRoot.FindAnyWidget("camera_btn_rot"));
		m_btn_phi = ButtonWidget.Cast( layoutRoot.FindAnyWidget("camera_btn_phi"));

		m_TxtChromX = layoutRoot.FindAnyWidget("camera_slider_chrom_value_x");
		m_TxtChromY = layoutRoot.FindAnyWidget("camera_slider_chrom_value_y");

		CAMERA_ROT = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\CameraTool\\gui\\layouts\\CameraROT.layout" );
		CAMERA_PHI = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\CameraTool\\gui\\layouts\\CameraPHI.layout" );
	}
	
	
    override void OnShow()
    {
        super.OnShow();

        GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert( this.Update );

        UpdateEditBox();
    }

    override void OnHide()
    {
        super.OnHide();

    }
	
	void Update()
	{
		UpdateSliders();
	}

	void UpdateEditBox() 
	{
		widgetStore.GetEditBoxWidget("camera_input_chrom_x").SetText( CameraTool.CHROMABERX.ToString());
		widgetStore.GetEditBoxWidget("camera_input_chrom_y").SetText( CameraTool.CHROMABERY.ToString());
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		SetFocus( layoutRoot );
		return false;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if ( w.GetName() == "close_button" ) 
		{
			GetLayoutRoot().Show( false );
			OnHide();
		}

		if ( w.GetName() == "camera_toggle" ) 
		{
			ref CameraTool cmt = GetModuleManager().GetModule( CameraTool );
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Call( cmt.ToggleCamera ); // Fix crash
		}

		Widget effectsFrame = widgetStore.GetWidget( "camera_effects_frame" );
		Widget settingsFrame = widgetStore.GetWidget( "camera_settings_frame" );

		if ( w.GetName() == "camera_slider_tab_effects" ) 
		{
			if ( !effectsFrame.IsVisible() ) 
			{
				effectsFrame.Show(!effectsFrame.IsVisible());
				settingsFrame.Show(!effectsFrame.IsVisible());
			}
		}
		if ( w.GetName() == "camera_slider_tab_settings" ) 
		{
			if ( !settingsFrame.IsVisible() ) 
			{
				settingsFrame.Show(!settingsFrame.IsVisible());
				effectsFrame.Show(!settingsFrame.IsVisible());
			}
		}

		if ( w == m_btn_rot ) 
		{
			CAMERA_ROT.Show( !CAMERA_ROT.IsVisible() );
		}

		if ( w == m_btn_phi )
		{
			CAMERA_PHI.Show( !CAMERA_PHI.IsVisible() );
		}

		if ( w.GetName() == "camera_fov_speed_btn_inc" ) 
		{
			CameraTool.CAMERA_FOV_SPEED_MODIFIER += 1;
		}

		if ( w.GetName() == "camera_fov_speed_btn_dec" ) 
		{
			CameraTool.CAMERA_FOV_SPEED_MODIFIER -= 1;
			if ( CameraTool.CAMERA_FOV_SPEED_MODIFIER < 0 ) CameraTool.CAMERA_FOV_SPEED_MODIFIER = 0;
		}

		if ( w.GetName() == "camera_smooth_btn_inc" )
		{
			CameraTool.CAMERA_SMOOTH += 0.025;
			CameraTool.CAMERA_SMOOTH = Math.Clamp(CameraTool.CAMERA_SMOOTH, 0.0, 1.0); // ugh
		}

		if ( w.GetName() == "camera_smooth_btn_dec" )
		{
			CameraTool.CAMERA_SMOOTH -= 0.025;
			CameraTool.CAMERA_SMOOTH = Math.Clamp(CameraTool.CAMERA_SMOOTH, 0.0, 1.0);
		}

		if ( w.GetName() == "camera_msens_btn_inc" ) 
		{
			CameraTool.CAMERA_MSENS += 0.05;
			CameraTool.CAMERA_MSENS = Math.Clamp(CameraTool.CAMERA_MSENS, 0.0, 1.5);
		}

		if ( w.GetName() == "camera_msens_btn_dec" ) 
		{
			CameraTool.CAMERA_MSENS -= 0.05;
			CameraTool.CAMERA_MSENS = Math.Clamp(CameraTool.CAMERA_MSENS, 0.0, 1.5);
		}
		if ( w.GetName() == "camera_msmooth_btn_inc" ) 
		{
			CameraTool.CAMERA_VELDRAG += 0.01; // percent
			CameraTool.CAMERA_VELDRAG = Math.Clamp(CameraTool.CAMERA_VELDRAG, 0.9, 1.0);
		}
		if ( w.GetName() == "camera_msmooth_btn_dec" ) 
		{
			CameraTool.CAMERA_VELDRAG -= 0.01;
			CameraTool.CAMERA_VELDRAG = Math.Clamp(CameraTool.CAMERA_VELDRAG, 0.9, 1.0); // 10 clicks
		}

		return false;
	}

	override bool OnChange( Widget w, int x, int y, bool finished )
	{
		Material chromAber = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/chromaber");
		Material matColors = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/glow");
		Material rotBlur = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/rotblur");
		Material radBlur = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/radialblur");

		SliderWidget changeSlider = widgetStore.GetSliderWidget( w.GetName() );
		EditBoxWidget editBox = widgetStore.GetEditBoxWidget( w.GetName() );

		if ( editBox ) 
		{
			string text = editBox.GetText();
			float value = text.ToFloat();

			if ( editBox.GetName() == "camera_input_chrom_x" ) 
			{
				CameraTool.CHROMABERX = value;
				chromAber.SetParam( "PowerX", CameraTool.CHROMABERX );
			}

			if ( editBox.GetName() == "camera_input_chrom_y" ) 
			{
				CameraTool.CHROMABERY = value;
				chromAber.SetParam( "PowerY", CameraTool.CHROMABERY );
			}
			return false;
		}
		/*
		switch ( changeSlider.GetName() ) 
		{
			case 
		}
		*/

		if ( w == m_SldCamBlur ) 
		{
			CameraTool.CAMERA_BLUR = m_SldCamBlur.GetCurrent() * 0.01; // percent
			
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
			CameraTool.CAMERA_FLENGTH = (m_SldCamFLen.GetCurrent() * 2.0);
		}
		else if ( w == m_SldCamFnear ) 
		{
			CameraTool.CAMERA_FNEAR = (m_SldCamFnear.GetCurrent() * 2.0);
		}
		else if ( w == m_SldCamExp ) 
		{
			CameraTool.EXPOSURE = (m_SldCamExp.GetCurrent() * 0.1) - 5.0;
			GetGame().SetEVUser( CameraTool.EXPOSURE );
			//PPEffects.SetBlur( exposure );
			
			//float color[4];
			//matColors.SetParam("LensDistort", CameraTool.EXPOSURE );
			//GetPlayer().MessageStatus( CameraTool.EXPOSURE.ToString() );
			//matColors.SetParam("PowerX", exposure);
			// matColors.SetParam("PowerY", exposure);
		}
		else if ( changeSlider.GetName() == "camera_slider_hue" ) 
		{
			CameraTool.HUESHIFT = (changeSlider.GetCurrent() * 0.1) - 5.0;
			matColors.SetParam( "Saturation", CameraTool.HUESHIFT );
		}
		else if ( changeSlider.GetName() == "camera_slider_rotblur_power" ) 
		{
			CameraTool.ROTBLUR = 0.5 * (changeSlider.GetCurrent() * 0.01);
			rotBlur.SetParam( "Power", CameraTool.ROTBLUR );
		}
		else if ( changeSlider.GetName() == "camera_slider_rotblur_mindepth" ) 
		{
			CameraTool.MINDEPTH = 2.5 * (changeSlider.GetCurrent() * 0.01);
			rotBlur.SetParam( "MinDepth", CameraTool.MINDEPTH );
		}
		else if ( changeSlider.GetName() == "camera_slider_rotblur_maxdepth" ) 
		{
			CameraTool.MAXDEPTH = (changeSlider.GetCurrent() * 0.1);
			rotBlur.SetParam( "MaxDepth", CameraTool.MAXDEPTH );
		}
		else if ( changeSlider.GetName() == "camera_slider_radblur_x" ) 
		{
			CameraTool.RADBLURX = (changeSlider.GetCurrent() * 0.01);
			radBlur.SetParam( "PowerX", CameraTool.RADBLURX );
		}
		else if ( changeSlider.GetName() == "camera_slider_radblur_y" ) 
		{
			CameraTool.RADBLURY = (changeSlider.GetCurrent() * 0.01);
			radBlur.SetParam( "PowerY", CameraTool.RADBLURY );
		}
		else if ( changeSlider.GetName() == "camera_slider_radblur_offsetx" ) 
		{
			CameraTool.RADBLUROFFX = (changeSlider.GetCurrent() * 0.01);
			radBlur.SetParam( "OffsetX", CameraTool.RADBLUROFFX );
		}
		else if ( changeSlider.GetName() == "camera_slider_radblur_offsety" ) 
		{
			CameraTool.RADBLUROFFY = (changeSlider.GetCurrent() * 0.01);
			radBlur.SetParam( "OffsetY", CameraTool.RADBLUROFFY );
		}
		else if ( changeSlider.GetName() == "camera_slider_vign" ) 
		{
			CameraTool.VIGNETTE = (changeSlider.GetCurrent() * 0.02);
			matColors.SetParam( "Vignette", CameraTool.VIGNETTE );
		}
		else if ( changeSlider.GetName() == "camera_slider_vign_r" ) 
		{
			CameraTool.VARGB[0] = ((changeSlider.GetCurrent() * 0.1) - 5.0);
			matColors.SetParam("VignetteColor", CameraTool.VARGB );
		}
		else if ( changeSlider.GetName() == "camera_slider_vign_g" ) 
		{
			CameraTool.VARGB[1] = ((changeSlider.GetCurrent() * 0.1) - 5.0);
			matColors.SetParam("VignetteColor", CameraTool.VARGB );
		}
		else if ( changeSlider.GetName() == "camera_slider_vign_b" ) 
		{
			CameraTool.VARGB[2] = ((changeSlider.GetCurrent() * 0.1) - 5.0);
			matColors.SetParam("VignetteColor", CameraTool.VARGB );
		}
		else if ( changeSlider.GetName() == "camera_slider_color_r" ) 
		{
			CameraTool.CARGB[0] = ((changeSlider.GetCurrent() * 0.1) - 5.0);
			matColors.SetParam("OverlayColor", CameraTool.CARGB );
		}
		else if ( changeSlider.GetName() == "camera_slider_color_g" ) 
		{
			CameraTool.CARGB[1] = ((changeSlider.GetCurrent() * 0.1) - 5.0);
			matColors.SetParam("OverlayColor", CameraTool.CARGB );
		}
		else if ( changeSlider.GetName() == "camera_slider_color_b" ) 
		{
			CameraTool.CARGB[2] = ((changeSlider.GetCurrent() * 0.1) - 5.0);
			matColors.SetParam("OverlayColor", CameraTool.CARGB );
		}
		else if ( changeSlider.GetName() == "camera_slider_color_a" ) 
		{
			CameraTool.CARGB[3] = ((changeSlider.GetCurrent() * 0.05) - 4.0);
			matColors.SetParam("OverlayFactor", CameraTool.CARGB[3] );
		}
		else if ( changeSlider.GetName() == "camera_slider_view" )
		{
			CameraTool.VIEWDISTANCE = 100*changeSlider.GetCurrent();
			GetGame().GetWorld().SetPreferredViewDistance(CameraTool.VIEWDISTANCE);
		}
		return false;
	}

	override bool OnMouseWheel( Widget w, int x, int y, int wheel )
	{
		SliderWidget slider = widgetStore.GetSliderWidget( w.GetName() );

		if ( slider ) 
		{
			bool up = wheel < 0;
			int value = 1;

			if ( up ) value = -1;

			float current = slider.GetCurrent();
			slider.SetCurrent( current + value );

			OnChange( slider, x, y, false );
		}
		return false;
	}

	void UpdateSliders() 
	{
		string cameraTarget = "None";

		Object targetObject = CameraTool.Cast(GetModuleManager().GetModule(CameraTool)).GetTargetObject();

		if ( targetObject ) 
		{
			cameraTarget = targetObject.GetType();
		}

		vector targetPos = CameraTool.Cast(GetModuleManager().GetModule(CameraTool)).GetTargetPos();
		if ( targetPos != vector.Zero ) 
		{
			cameraTarget = VectorToString( targetPos, 1 );
		}

		widgetStore.GetTextWidget("camera_target_txt").SetText("Target: " + cameraTarget );

		m_TxtCamBlur.SetText((CameraTool.CAMERA_BLUR * 100.0).ToString() + "%");

		string autoF = "";
		if ( CameraTool.CAMERA_AFOCUS ) 
		{
			autoF = " (AUTO)";
		}
		m_TxtCamDist.SetText(CameraTool.CAMERA_FDIST.ToString()+"m" + autoF);
		m_TxtCamFlen.SetText(CameraTool.CAMERA_FLENGTH.ToString());
		m_TxtCamFnear.SetText(CameraTool.CAMERA_FNEAR.ToString());
		m_TxtCamExp.SetText( CameraTool.EXPOSURE.ToString() );

		widgetStore.GetTextWidget("camera_slider_hue_value").SetText( CameraTool.HUESHIFT.ToString() ); // make script param that updates child textwidget of slider based on vvariable?
		widgetStore.GetTextWidget("camera_slider_rotblur_power_value").SetText( CameraTool.ROTBLUR.ToString() );
		widgetStore.GetTextWidget("camera_slider_rotblur_mindepth_value").SetText( CameraTool.MINDEPTH.ToString() );
		widgetStore.GetTextWidget("camera_slider_rotblur_maxdepth_value").SetText( CameraTool.MAXDEPTH.ToString() );
		widgetStore.GetTextWidget("camera_slider_radblur_value_x").SetText( CameraTool.RADBLURX.ToString() );
		widgetStore.GetTextWidget("camera_slider_radblur_value_y").SetText( CameraTool.RADBLURY.ToString() );
		widgetStore.GetTextWidget("camera_slider_radblur_value_offsetx").SetText( CameraTool.RADBLUROFFX.ToString() );
		widgetStore.GetTextWidget("camera_slider_radblur_value_offsety").SetText( CameraTool.RADBLUROFFY.ToString() );
		widgetStore.GetTextWidget("camera_slider_vign_value").SetText( CameraTool.VIGNETTE.ToString() );
		widgetStore.GetTextWidget("camera_slider_vign_value_r").SetText( CameraTool.VARGB[0].ToString() );
		widgetStore.GetTextWidget("camera_slider_vign_value_g").SetText( CameraTool.VARGB[1].ToString() );
		widgetStore.GetTextWidget("camera_slider_vign_value_b").SetText( CameraTool.VARGB[2].ToString() );
		widgetStore.GetTextWidget("camera_slider_color_value_r").SetText( CameraTool.CARGB[0].ToString() );
		widgetStore.GetTextWidget("camera_slider_color_value_g").SetText( CameraTool.CARGB[1].ToString() );
		widgetStore.GetTextWidget("camera_slider_color_value_b").SetText( CameraTool.CARGB[2].ToString() );
		widgetStore.GetTextWidget("camera_slider_color_value_a").SetText( CameraTool.CARGB[3].ToString() );
		widgetStore.GetTextWidget("camera_slider_view_value").SetText( CameraTool.VIEWDISTANCE.ToString() );

		// get child and update text?

		// Set slider current to value
		widgetStore.GetSliderWidget("camera_slider_hue").SetCurrent( (CameraTool.HUESHIFT + 5.0) / 0.1); // this could probably be moved to a proper system
		widgetStore.GetSliderWidget("camera_slider_rotblur_power").SetCurrent( (CameraTool.ROTBLUR / 0.5) / 0.01);
		widgetStore.GetSliderWidget("camera_slider_rotblur_mindepth").SetCurrent( (CameraTool.MINDEPTH / 0.01) / 2.5);
		widgetStore.GetSliderWidget("camera_slider_rotblur_maxdepth").SetCurrent( CameraTool.MAXDEPTH / 0.1);
		widgetStore.GetSliderWidget("camera_slider_radblur_x").SetCurrent( CameraTool.RADBLURX / 0.01);
		widgetStore.GetSliderWidget("camera_slider_radblur_y").SetCurrent( CameraTool.RADBLURY / 0.01);
		widgetStore.GetSliderWidget("camera_slider_radblur_offsetx").SetCurrent( CameraTool.RADBLUROFFX / 0.01);
		widgetStore.GetSliderWidget("camera_slider_radblur_offsety").SetCurrent( CameraTool.RADBLUROFFY / 0.01);
		widgetStore.GetSliderWidget("camera_slider_view").SetCurrent( CameraTool.VIEWDISTANCE / 100.0);

		// fk me ok im lazy. cbf doing this for all the sliders.

		TextWidget speedTxt = layoutRoot.FindAnyWidget( "camera_fov_speed_text" );
		speedTxt.SetText("FOV Smooth: " + CameraTool.CAMERA_FOV_SPEED_MODIFIER );

		TextWidget smoothTxt = layoutRoot.FindAnyWidget( "camera_smooth_text" );
		smoothTxt.SetText("Cam Smooth: " + CameraTool.CAMERA_SMOOTH );

		TextWidget sensTxt = layoutRoot.FindAnyWidget( "camera_msens_text" );
		sensTxt.SetText("Cam Sens: " + CameraTool.CAMERA_MSENS );

		TextWidget flySpeed = layoutRoot.FindAnyWidget("camera_msmooth_text");
		flySpeed.SetText("Fly smooth: " + (CameraTool.CAMERA_VELDRAG - 0.9) / 0.1);
	}	
}