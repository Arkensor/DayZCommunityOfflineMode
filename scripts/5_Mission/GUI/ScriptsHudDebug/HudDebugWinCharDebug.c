class HudDebugWinCharDebug extends HudDebugWinBase
{
	private PluginDeveloper m_ModuleDeveloper;
	
	private TextWidget 		m_PlayerPosTextWidget;
	private TextWidget 		m_ClipboardTextWidget;
	private TextWidget 		m_GodModeTextWidget;

	//============================================
	// HudDebugWinCharDebug
	//============================================
	void HudDebugWinCharDebug(Widget widget_root)
	{	
		m_PlayerPosTextWidget = TextWidget.Cast( widget_root.FindAnyWidget("txt_PlayerPos") );
		m_ClipboardTextWidget = TextWidget.Cast( widget_root.FindAnyWidget("txt_Clipboard") );
		m_GodModeTextWidget = TextWidget.Cast( widget_root.FindAnyWidget("txt_GodMode") );
	}

	//============================================
	// ~HudDebugWinCharDebug
	//============================================
	void ~HudDebugWinCharDebug()
	{
	}
	
	//============================================
	// Update
	//============================================
	override void Update()
	{
		super.Update();
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( player != NULL )
		{
			vector pos = player.GetPosition();
			string pos_str = "Pos: " + pos[0].ToString() + " " + pos[2].ToString();
			m_PlayerPosTextWidget.SetText(pos_str);
		}
		
		string clipboard;
		GetGame().CopyFromClipboard(clipboard);
		clipboard = clipboard.Substring( 0, Math.Min( clipboard.Length(), 64 ) );	//max 64 chars
		clipboard = "Clipboard: " + clipboard;
		m_ClipboardTextWidget.SetText(clipboard);
		
		if ( PluginDeveloper.GetInstance().IsGodMode() )
		{
			m_GodModeTextWidget.SetText("God mode: ENABLED");
		}
		else
		{
			m_GodModeTextWidget.SetText("God mode: DISABLED");
		}
	}

	//============================================
	// GetWinType
	//============================================
	override int GetType()
	{
		return HudDebug.HUD_WIN_CHAR_DEBUG;
	}
}
