class ToggleSelections
{
	protected string m_ToggleOff;
	protected string m_ToggleOn;
	
	void ToggleSelections( string toggle_off, string toggle_on )
	{
		m_ToggleOff = toggle_off;
		m_ToggleOn = toggle_on;
	}

	string GetToggleOff()
	{
		return m_ToggleOff;
	}

	string GetToggleOn()
	{
		return m_ToggleOn;
	}
}
