class DebugMonitorPatched : DebugMonitor
{
	private Widget m_WidgetRootPatched;
	private TextWidget m_pWindowLabelTextPatched;
	private TextListboxWidget m_pNamesListboxWidgetPatched;
	private TextListboxWidget m_pValuesListboxWidgetPatched;
	private MultilineTextWidget m_pModifiersMultiTextWidgetPatched;
		
	void DebugMonitorPatched()
	{		
		m_WidgetRootPatched = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_debug_monitor.layout");
		m_WidgetRootPatched.Show(false);
		
		m_pWindowLabelTextPatched = TextWidget.Cast( m_WidgetRootPatched.FindAnyWidget("WindowLabel") );
		m_pNamesListboxWidgetPatched = TextListboxWidget.Cast( m_WidgetRootPatched.FindAnyWidget("NamesListboxWidget") );
		m_pValuesListboxWidgetPatched = TextListboxWidget.Cast( m_WidgetRootPatched.FindAnyWidget("ValuesListboxWidget") );
		m_pModifiersMultiTextWidgetPatched = MultilineTextWidget.Cast( m_WidgetRootPatched.FindAnyWidget("ModifiersMultilineTextWidget") );
	}
	
	void ~DebugMonitorPatched()
	{
	}
	
	void Init()
	{
		m_pNamesListboxWidgetPatched.AddItem("HEALTH:", NULL, 0);
		m_pValuesListboxWidgetPatched.AddItem("", NULL, 0);

		m_pNamesListboxWidgetPatched.AddItem("BLOOD:", NULL, 0);
		m_pValuesListboxWidgetPatched.AddItem("", NULL, 0);

		/*m_pNamesListboxWidgetPatched.AddItem("BODY TEMP:", NULL, 0);
		m_pValuesListboxWidgetPatched.AddItem("", NULL, 0);*/
		
		m_pNamesListboxWidgetPatched.AddItem("LAST DAMAGE:", NULL, 0);
		m_pValuesListboxWidgetPatched.AddItem("", NULL, 0);
		
		m_pNamesListboxWidgetPatched.AddItem("POSITION:", NULL, 0);
		m_pValuesListboxWidgetPatched.AddItem("", NULL, 0);

		/*m_pNamesListboxWidgetPatched.AddItem("MODIFIERS:", NULL, 0);
		m_pValuesListboxWidgetPatched.AddItem("", NULL, 0);*/
		
		//m_DebugFloatsListWidget.AddItem(key,NULL,0);
		//m_DebugFloatsListWidget.SetItem(i,value.ToString(),NULL,1);		
	
		m_WidgetRootPatched.Show(true);
	}

	void SetHealth(float value)
	{
		string health = string.Format(" %1", value.ToString());
		m_pValuesListboxWidgetPatched.SetItem(0, health, NULL, 0);
	}

	void SetBlood(float value)
	{
		string blood = string.Format(" %1", value.ToString());
		m_pValuesListboxWidgetPatched.SetItem(1, blood, NULL, 0);
	}
	
	void SetLastDamage(string value)
	{
		string lastDamage = string.Format(" %1", value);
		m_pValuesListboxWidgetPatched.SetItem(2, lastDamage, NULL, 0);
	}
	
	void SetPosition(vector value)
	{
		string position = string.Format(" %1 %2 %3", value[0].ToString(), value[1].ToString(), value[2].ToString());
		m_pValuesListboxWidgetPatched.SetItem(3, position, NULL, 0);
	}

	void Hide()
	{
		m_WidgetRootPatched.Show(false);
	}
	
	void Show()
	{
		m_WidgetRootPatched.Show(true);
	}
};
	
