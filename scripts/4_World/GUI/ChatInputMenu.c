class ChatInputMenu extends UIScriptedMenu
{
	private EditBoxWidget m_edit_box;
	private TextWidget m_channel_text;
	private ref Timer m_close_timer;

	void ChatInputMenu()
	{
		m_close_timer = new Timer();
	}

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_chat_input.layout");
		m_edit_box = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("InputEditBoxWidget") );
		m_channel_text = TextWidget.Cast( layoutRoot.FindAnyWidget("ChannelText") );
		
		UpdateChannel();
		return layoutRoot;
	}
	
	override bool UseKeyboard() 
	{ 
		return true; 
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		super.OnChange(w, x, y, finished);
		
		if (!finished) return false;
		
		string text = m_edit_box.GetText();

		if (text != "")
		{
			GetGame().ChatPlayer(/*GetGame().ChatGetChannel()*/0, text);
		}

		m_close_timer.Run(0.1, this, "Close");
		return true;
	}

	
	override void Refresh()
	{	
	}
	
	override void Update(float timeslice)
	{
		GetGame().GetInput().DisableKey(KeyCode.KC_RETURN);
	}
	
	void UpdateChannel()
	{
		m_channel_text.SetText(GetChannelName(/*GetGame().ChatGetChannel()*/0));	
	}
	
	static string GetChannelName(ChatChannel channel)
	{
		switch(channel)
		{
			case CCNone:
				return "None";
			case CCGlobal:
				return "Global";
			case CCItemTransmitter:
				return "Radio"; 
			case CCDirect:
				return "Direct";      
			case CCStatus:
				return "Status";   
			case CCSystem:
				return "System";   
		}	
		
		return "";
	}
}
