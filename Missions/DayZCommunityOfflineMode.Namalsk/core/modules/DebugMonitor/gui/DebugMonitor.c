class ArkDebugMonitor
{
	private Widget m_WidgetRoot;
	private TextWidget m_pWindowLabelText;
	private TextListboxWidget m_pNamesListboxWidget;
	private TextListboxWidget m_pValuesListboxWidget;
	private MultilineTextWidget m_pModifiersMultiTextWidget;

	void ArkDebugMonitor()
	{
		m_WidgetRoot = GetGame().GetWorkspace().CreateWidgets("$CurrentDir:missions\\DayZCommunityOfflineMode.Namalsk\\core\\modules\\DebugMonitor\\gui\\layouts\\day_z_debug_monitor.layout");
		m_WidgetRoot.Show(false);

		m_pWindowLabelText = TextWidget.Cast( m_WidgetRoot.FindAnyWidget("WindowLabel") );
		m_pNamesListboxWidget = TextListboxWidget.Cast( m_WidgetRoot.FindAnyWidget("NamesListboxWidget") );
		m_pValuesListboxWidget = TextListboxWidget.Cast( m_WidgetRoot.FindAnyWidget("ValuesListboxWidget") );
		m_pModifiersMultiTextWidget = MultilineTextWidget.Cast( m_WidgetRoot.FindAnyWidget("ModifiersMultilineTextWidget") );
	}

	void Init()
	{
		m_pNamesListboxWidget.AddItem("HEALTH:", NULL, 0);
		m_pValuesListboxWidget.AddItem("", NULL, 0);

		m_pNamesListboxWidget.AddItem("BLOOD:", NULL, 0);
		m_pValuesListboxWidget.AddItem("", NULL, 0);

		/*m_pNamesListboxWidget.AddItem("BODY TEMP:", NULL, 0);
		m_pValuesListboxWidget.AddItem("", NULL, 0);*/

		m_pNamesListboxWidget.AddItem("LAST DAMAGE:", NULL, 0);
		m_pValuesListboxWidget.AddItem("", NULL, 0);

		m_pNamesListboxWidget.AddItem("POSITION:", NULL, 0);
		m_pValuesListboxWidget.AddItem("", NULL, 0);

		m_pNamesListboxWidget.AddItem("ORIENTATION:", NULL, 0);
		m_pValuesListboxWidget.AddItem("", NULL, 0);

		m_pNamesListboxWidget.AddItem("DATETIME:", NULL, 0);
		m_pValuesListboxWidget.AddItem("", NULL, 0);

        string version;
        GetGame().GetVersion(version);

		m_pNamesListboxWidget.AddItem("VERSION:", NULL, 0);
		m_pValuesListboxWidget.AddItem( " " + version, NULL, 0);

		/*m_pNamesListboxWidget.AddItem("MODIFIERS:", NULL, 0);
		m_pValuesListboxWidget.AddItem("", NULL, 0);*/

		//m_DebugFloatsListWidget.AddItem(key,NULL,0);
		//m_DebugFloatsListWidget.SetItem(i,value.ToString(),NULL,1);

		m_WidgetRoot.Show(true);
	}

	void SetHealth(float value)
	{
		string health = string.Format(" %1", value.ToString());
		m_pValuesListboxWidget.SetItem(0, health, NULL, 0);
	}

	void SetBlood(float value)
	{
		string blood = string.Format(" %1", value.ToString());
		m_pValuesListboxWidget.SetItem(1, blood, NULL, 0);
	}

	void SetLastDamage(string value)
	{
		string lastDamage = string.Format(" %1", value);
		m_pValuesListboxWidget.SetItem(2, lastDamage, NULL, 0);
	}

	void SetPosition(vector value)
	{
		string position = string.Format(" %1 %2 %3", value[0].ToString(), value[1].ToString(), value[2].ToString());
		m_pValuesListboxWidget.SetItem(3, position, NULL, 0);
	}

	void SetOrientation(vector value)
	{
		string orientation = string.Format(" %1 %2 %3", value[0].ToString(), value[1].ToString(), value[2].ToString());
		m_pValuesListboxWidget.SetItem(4, orientation, NULL, 0);
	}

	void SetDateTime(string value)
	{
		string orientation = string.Format(" %1", value);
		m_pValuesListboxWidget.SetItem(5, orientation, NULL, 0);
	}

	void Hide()
	{
		m_WidgetRoot.Show(false);
	}

	void Show()
	{
		m_WidgetRoot.Show(true);
	}
};