/** @addtogroup Debug
@{*/

/**
 * \defgroup DebugUI Debug UI API
	\brief Immediate mode debug UI API
 * @{
	Per frame usage example:
	@code
	bool m_ShowDbgUI = false;
	int m_DbgListSelection = 0;
	float m_DbgSliderValue = 0.0;
	autoptr array<string> m_DbgOptions = {"jedna", "dva", "tri"};
	
	void OnUpdate(float timeslice)
	{	
		DbgUI.Begin("Test");
		DbgUI.Check("Show DbgUI", m_ShowDbgUI);
		if (m_ShowDbgUI)
		{
			DbgUI.Text("DbgUI Test");		
	
			string name = "";
			DbgUI.InputText("name", name);		
			
			if (DbgUI.Button("Print name"))
			{
				Print(name);		
			}
			
			DbgUI.List("test list", m_DbgListSelection, m_DbgOptions);
			
			DbgUI.Text("Choice = " + m_DbgListSelection.ToString());	
			
			DbgUI.Spacer(10);
			DbgUI.SliderFloat("slider", m_DbgSliderValue, 0, 100);
			DbgUI.Text("Slider value = " + ftoa(m_DbgSliderValue));	
		}
		DbgUI.End();
	}
	@endcode

	For non-per frame usage example:
	@code
	int m_DbgEventCount = 0;
	void OnEvent(EventType eventTypeId, Param params)
	{
		m_DbgEventCount++;

		DbgUI.BeginCleanupScope();		
		DbgUI.Begin("events", 300, 0);
		DbgUI.Text("Events count = " + m_DbgEventCount.ToString());
		DbgUI.End();
		DbgUI.EndCleanupScope();
	}
	@endcode
*/

class DbgUI
{
	static proto native void DoUnitTest(); ///< Creates all possible DbgUI widgets. Just for the testing purposes.
	static proto native void Text(string label);
	static proto void Check(string label, out bool checked);
	static proto void Combo(string label, out int selection, TStringArray elems);
	static proto void List(string label, out int selection, TStringArray elems);
	static proto void SliderFloat(string label, out float value, float min, float max, int pxWidth = 150);
	static proto native void Spacer(int height);
	static proto native void Panel(string label, int width, int height, int color = 0xaa555555);
	static proto native bool Button(string txt, int minWidth = 0);
	static proto void InputText(string txt, out string value, int pxWidth = 150);
	static proto void InputInt(string txt, out int value, int pxWidth = 150);
	static proto void InputFloat(string txt, out float value, int pxWidth = 150);
			
	static proto native void PlotLive(string label, int sizeX, int sizeY, float val, int timeStep = 100, int historySize = 30, int color = 0xFFFFFFFF);
	
	static proto native void SameLine();
	static proto native void SameSpot();
	
	static proto native void PushID_Int(int int_id);
	static proto native void PushID_Str(string str_id);
	static proto native void PopID();
	
	static proto void BeginCleanupScope();
    static proto native void EndCleanupScope();
	
	static proto native void Begin(string windowTitle, float x = 0, float y = 0);
    static proto native void End();
};
//@}
//@}
