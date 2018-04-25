// quick and dirty way for displaying of checker overlay on screen
// - userd for camera settings primarily
class PluginDrawCheckerboard extends PluginBase
{
	private	ref Widget	m_MainWindow;
	private bool		m_IsActive;
	
	
	void PluginDrawCheckerboard()
	{
		CreateWidgetOverlay();
	}

	void ~PluginDrawCheckerboard() {}

	void CreateWidgetOverlay()
	{
#ifndef NO_GUI
		if (!m_MainWindow)
		{
			m_MainWindow = GetGame().GetWorkspace().CreateWidgets("gui/layouts/camera_checkerboard.layout");
			m_MainWindow.Show(false);
			int childId = 0;
			int row = 0;
			bool evenOrOdd;
			int tilesPerLine = 8;

			Widget child = m_MainWindow.GetChildren();
			while(child)
			{
				evenOrOdd = IsEven(childId);

				//! row counter
				if(childId > (tilesPerLine * row) - 1)
				{
					row++;
				}

				if(IsEven(row))
				{
					evenOrOdd = !evenOrOdd;
				}

				//! sets alpha level for even/odd child
				if(evenOrOdd)
					child.SetAlpha(0.15);
				else
					child.SetAlpha(0.05);

				child = child.GetSibling();
				childId++;
			}
		}
#endif
	}
	
	bool IsActive()
	{
		return m_IsActive;
	}

	//! even or odd
	bool IsEven(int num)
	{
		if((num % 2) == 0)
			return true;
		else
			return false;
	}

	void ShowWidgets(bool show)
	{
		if(m_MainWindow) 
		{
			m_MainWindow.Show(show);
		}
	}

	//!
	void Show()
	{
		ShowWidgets(true);
		m_IsActive = true;
	}

	//!
	void Hide()
	{
		ShowWidgets(false);
		m_IsActive = false;
	}
}