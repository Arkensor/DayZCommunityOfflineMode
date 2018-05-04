class SliderOptionsAccessUI extends OptionsAccessUI
{
	const int SCROLLBAR_SLIDER = 204;
	const float SLIDER_WIDTH = 0.02; //with of the slider "handle" gizmo
	const float SLIDER_MIN = 0.01;
	const float SLIDER_MAX = 0.99;
	
	override void Init() 
	{
		Update();
	}
	
	override void Update()
	{
		NumericOptionsAccess numeric = NumericOptionsAccess.Cast( m_optionAccess );
		SliderWidget slider = SliderWidget.Cast( m_widget );
		
		if (numeric && slider)
		{
			float min = numeric.GetMin();
			float max = numeric.GetMax();
			float value = numeric.ReadValue();
			
			float val = value - min;
			val = val / (max - min) * 100.0;
			slider.SetCurrent(val);
			
			switch(slider.GetName())
			{
				case "FOVSliderWidget":
				{
					TextWidget text_value_widget = TextWidget.Cast( slider.FindWidget("FOVSliderValue") );
					float converted_value = Math.Round( 2 * Math.RAD2DEG * ( OPTIONS_FIELD_OF_VIEW_MIN + ( ( val / 100 ) * ( OPTIONS_FIELD_OF_VIEW_MAX - OPTIONS_FIELD_OF_VIEW_MIN ) ) ) );
					text_value_widget.SetText( converted_value.ToString()/*+" DEG"*/ ); //° symbol is not displayed, find workaround
					g_Game.SetUserFOV(value);
					break;
				}
				
				case "EffectsVolumeSliderWidget":
				{
					SetPercentageText(val,"EffectsVolumeValueTextWidget");
					break;
				}
				
				case "MusicVolumeSliderWidget":
				{
					SetPercentageText(val,"MusicVolumeValueTextWidget");
					break;
				}
				
				case "VoiceVolumeSliderWidget":
				{
					SetPercentageText(val,"VoiceVolumeValueTextWidget");
					break;
				}
				
				case "MasterVolumeSliderWidget":
				{
					SetPercentageText(val,"MasterVolumeValueTextWidget");
					break;
				}
				
				case "HeadBobSliderWidget":
				{
					SetPercentageText(val,"HeadBobAmountTextWidget");
					break;
				}
				
				case "BrightnessSliderWidget":
				{
					SetPercentageText(val,"BrightnessTextWidget");
					break;
				}
				
				case "GammaSliderWidget":
				{
					SetPercentageText(val,"GammaTextWidget");
					break;
				}
				
				case "BloomSliderWidget":
				{
					SetPercentageText(val,"BloomTextWidget");
					break;
				}
				
				case "BlurSliderWidget":
				{
					SetPercentageText(val,"BlurTextWidget");
					break;
				}
				
				case "MouseHorizontalSliderWidget":
				{
					SetPercentageText(val,"MouseHorizontalValueTextWidget");
					break;
				}
				case "MouseVerticalValueSliderWidget":
				{
					SetPercentageText(val,"MouseVerticalValueTextWidget");
					break;
				}
				case "SmoothingSliderWidget":
				{
					SetPercentageText(val,"SmoothingValueTextWidget");
					break;
				}
				case "AimingDeadzoneSliderWidget":
				{
					SetPercentageText(val,"AimingDeadzoneValueTextWidget");
					break;
				}
				case "ControllerSensitivityHorizontalSliderWidget":
				{
					SetPercentageText(val,"ControllerSensitivityHorizontalValueTextWidget");
					break;
				}
				case "ControllerSensitivityVerticalSliderWidget":
				{
					SetPercentageText(val,"ControllerSensitivityVerticalValueTextWidget");
					break;
				}
			}
			SetScrollingWidgetPos(val);
		}
	}
	
	void SetPercentageText(float val, string WidgetName)
	{
		SliderWidget slider = SliderWidget.Cast( m_widget );
		float RawValue = val;
		int RoundedValue = Math.Round(RawValue);
		
		TextWidget text_value_widget = TextWidget.Cast( slider.FindWidget(WidgetName) );
		text_value_widget.SetText( RoundedValue.ToString() + "%" );
	}
	
	void SetScrollingWidgetPos(float val)
	{
		float pos;
		SliderWidget slider = SliderWidget.Cast( m_widget );
		
		ImageWidget image = ImageWidget.Cast( GetChildByID(slider, SCROLLBAR_SLIDER) );
		
		if (image)
		{
			pos = Math.Clamp(val/100, SLIDER_MIN, SLIDER_MAX) - (SLIDER_WIDTH / 2); //slider width is currently 0.02
			image.SetPos(pos,0,true);
		}
	}
	
	Widget GetChildByID(Widget w,int id)
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

	override void SetValue()
	{
		NumericOptionsAccess numeric = NumericOptionsAccess.Cast( m_optionAccess );
		SliderWidget slider = SliderWidget.Cast( m_widget );

		if (numeric && slider)
		{
			float min = numeric.GetMin();
			float max = numeric.GetMax();
			float value = slider.GetCurrent() / 100.0;

			numeric.WriteValue(min + (value * (max - min)));
			Update();
		}
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		super.OnChange(w, x, y, finished);
		
		SetValue();
		return true;
	}
}
