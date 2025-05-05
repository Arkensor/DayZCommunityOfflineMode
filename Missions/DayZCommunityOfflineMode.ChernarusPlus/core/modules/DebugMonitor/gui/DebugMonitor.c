class ArkDebugMonitor
{
	private Widget m_WidgetRoot;
	private TextWidget m_pWindowLabelText;
	private TextListboxWidget m_pNamesListboxWidget;
	private TextListboxWidget m_pValuesListboxWidget;
	private MultilineTextWidget m_pModifiersMultiTextWidget;

	void ArkDebugMonitor()
	{
		m_WidgetRoot = GetGame().GetWorkspace().CreateWidgets("$CurrentDir:missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\DebugMonitor\\gui\\layouts\\day_z_debug_monitor.layout");
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
		
		m_pNamesListboxWidget.AddItem("SHOCK:", NULL, 0); //Added shcok
		m_pValuesListboxWidget.AddItem("", NULL, 0);
		
		/* m_pNamesListboxWidget.AddItem("BODY t°:", NULL, 0); // Not working
		m_pValuesListboxWidget.AddItem("", NULL, 0); */

		m_pNamesListboxWidget.AddItem("AIR TEMP°:", NULL, 0); //Fixed air temp
		m_pValuesListboxWidget.AddItem("", NULL, 0);

		/*m_pNamesListboxWidget.AddItem("LAST DAMAGE:", NULL, 0); // Removed, not working.
		m_pValuesListboxWidget.AddItem("", NULL, 0);*/

		m_pNamesListboxWidget.AddItem("POSITION:", NULL, 0);
		m_pValuesListboxWidget.AddItem("", NULL, 0);

		m_pNamesListboxWidget.AddItem("ORIENTATION:", NULL, 0);
		m_pValuesListboxWidget.AddItem("", NULL, 0);

		m_pNamesListboxWidget.AddItem("TIME:", NULL, 0); //Shorter text, easier to read
		m_pValuesListboxWidget.AddItem("", NULL, 0);

        string version;
        GetGame().GetVersion(version);

		m_pNamesListboxWidget.AddItem("VERSION:", NULL, 0);
		m_pValuesListboxWidget.AddItem(" " + version, NULL, 0);

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

	void SetShock(float value)
	{
		string shock = string.Format(" %1", value.ToString());
		m_pValuesListboxWidget.SetItem(2, shock, NULL, 0);
	}

	/* void SetBodyTemp(float value)
	{
		string bodyTemp = string.Format(" %1", value.ToString());
		m_pValuesListboxWidget.SetItem(3, bodyTemp, NULL, 0);
	} */

	void SetAirTemp(float value)
	{
		string airTemp = string.Format(" %1", value.ToString());
		m_pValuesListboxWidget.SetItem(3, airTemp, NULL, 0);
	}

	/*void SetLastDamage(string value)
	{
		string lastDamage = string.Format(" %1", value);
		m_pValuesListboxWidget.SetItem(4, lastDamage, NULL, 0);
	}*/

	void SetPosition(vector value)
	{
		string position = string.Format(" %1 %2 %3", value[0].ToString(), value[1].ToString(), value[2].ToString());
		m_pValuesListboxWidget.SetItem(4, position, NULL, 0);
	}

	void SetOrientation(vector value)
	{
		string orientation = string.Format(" %1 %2 %3", value[0].ToString(), value[1].ToString(), value[2].ToString());
		m_pValuesListboxWidget.SetItem(5, orientation, NULL, 0);
	}

	void SetDateTime(string value)
	{
		string dateTime = string.Format(" %1", value);
		m_pValuesListboxWidget.SetItem(6, dateTime, NULL, 0);
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