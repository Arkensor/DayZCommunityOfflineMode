class ChatLine
{
	const float FADE_TIMEOUT = 30;
	const float FADE_OUT_DURATION = 3;
	const float FADE_IN_DURATION = 0.5;
	TextWidget m_text_widget;
	TextWidget m_text_widget_bg;
	private ref WidgetFadeTimer m_fade_timer;
	private ref WidgetFadeTimer m_fade_timer_bg;
	private ref Timer m_timeout_timer;
	private ref Timer m_timeout_timer_bg;

	void ChatLine(Widget root_widget)
	{
		m_text_widget_bg = TextWidget.Cast( GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_chat_item.layout", root_widget) );
		m_text_widget_bg.Show(false);
		m_text_widget_bg.SetColor(0xFF000000);
		
		m_text_widget = TextWidget.Cast( GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_chat_item.layout", root_widget) );
		m_text_widget.Show(false);
		
		m_fade_timer = new WidgetFadeTimer;
		m_fade_timer_bg = new WidgetFadeTimer;
		m_timeout_timer = new Timer(CALL_CATEGORY_GUI);
		m_timeout_timer_bg  = new Timer(CALL_CATEGORY_GUI);
	}

	void ~ChatLine()
	{
		delete m_text_widget;
		delete m_text_widget_bg;
	}


	void Set(ChatMessageEventParams params)
	{
		if (params.param2 != "")
		{
			m_text_widget.SetText(params.param2 + ": " + params.param3);
			m_text_widget_bg.SetText(params.param2 + ": " + params.param3);
		}
		else
		{
			m_text_widget.SetText(params.param3);
			m_text_widget_bg.SetText(params.param3);
		}

		ref TFloatArray color = new TFloatArray;
		
		if (params.param1 != CCStatus || params.param4 == "")
		{
			m_text_widget.SetColor(0xFFFFFFFF);
			switch(params.param1)
			{
				case CCGlobal:
					//GetGame().ConfigGetFloatArray("RscChatListDefault colorGlobalChannel", color);
					color.Insert(1.000);
					color.Insert(1.000);
					color.Insert(1.000);
					color.Insert(1.000);
				break;
				case CCDirect:
					GetGame().ConfigGetFloatArray("RscChatListDefault colorDirectChannel", color);
				break;
				case CCStatus:
					GetGame().ConfigGetFloatArray("RscChatListDefault colorStatusChannel", color);
				break;
				case CCSystem:
					GetGame().ConfigGetFloatArray("RscChatListDefault colorSystemChannel", color);
				break;				
			}
			
			if (color.Count() == 4)
			{
				m_text_widget.SetColor(ARGBF(color.Get(3), color.Get(0), color.Get(1), color.Get(2)));
			}
			else
			{
				m_text_widget.SetColor(0xFFFFFFFF);
			}
		}
		else
		{
			GetGame().ConfigGetFloatArray("RscChatListDefault " + params.param4, color);
		
			if (color.Count() == 4)
			{
				m_text_widget.SetColor(ARGBF(color.Get(3), color.Get(0), color.Get(1), color.Get(2)));
			}
		}
		m_fade_timer.FadeIn(m_text_widget, FADE_IN_DURATION);
		m_fade_timer_bg.FadeIn(m_text_widget_bg, FADE_IN_DURATION);
		
		m_timeout_timer.Run(FADE_TIMEOUT, m_fade_timer, "FadeOut", new Param2<Widget, float>(m_text_widget, FADE_OUT_DURATION));
		m_timeout_timer_bg.Run(FADE_TIMEOUT, m_fade_timer_bg, "FadeOut", new Param2<Widget, float>(m_text_widget_bg, FADE_OUT_DURATION));
	}

	void Clear()
	{
		m_text_widget.Show(false);
		m_text_widget_bg.Show(false);
		m_timeout_timer.Stop();
		m_timeout_timer_bg.Stop();
		m_fade_timer.Stop();
		m_fade_timer_bg.Stop();
	}
}
