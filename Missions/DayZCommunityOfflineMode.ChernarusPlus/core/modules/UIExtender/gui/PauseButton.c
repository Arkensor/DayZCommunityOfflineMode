class CustomPauseButton
{
	typename m_Object;
	string m_Function;
	string m_Text;

	int m_UserID;

	void CustomPauseButton(string text, int userId, typename object, string function)
	{
		m_Object = object;
		m_Function = function;

		m_Text = text;

		m_UserID = userId;
	}

	void RunFunction()
	{
		GetGame().GameScript.CallFunction(GetModuleManager().GetModule(m_Object), m_Function, NULL, 0 );
	}

}