class LoadingMenu extends UIScriptedMenu
{
	void LoadingMenu()
	{
	}

	void ~LoadingMenu()
	{
	}
	
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/loading.layout");
		
		m_label = TextWidget.Cast( layoutRoot.FindAnyWidget("TextWidget") );
		m_progressBar = ProgressBarWidget.Cast( layoutRoot.FindAnyWidget("ProgressBarWidget") );
		m_image = ImageWidget.Cast( layoutRoot.FindAnyWidget("ImageBackground") );
		
		m_image.LoadImageFile( 0, GetRandomLoadingBackground() );
		
		return layoutRoot;
	}

	TextWidget m_label;
	ProgressBarWidget m_progressBar;
	ImageWidget m_image;
}
