#ifdef GAME_TEMPLATE

[EditorAttribute("box", "GameLib/Scripted", "Render target", "-0.25 -0.25 -0.25", "0.25 0.25 0.25", "255 0 0 255")]
class RenderTargetClass
{

}

RenderTargetClass RenderTargetSource;

class RenderTarget: GenericEntity
{
	[Attribute("0", "slider", "Camera index", "0 31 1")]
	int CameraIndex;
	[Attribute("0", "editbox", "Position X <0, 1>")]
	float X;
	[Attribute("0", "editbox", "Position Y <0, 1>")]
	float Y;
	[Attribute("1", "editbox", "Render target width <0, 1>")]
	float Width;
	[Attribute("1", "editbox", "Render target height <0, 1>")]
	float Height;
	[Attribute("-1", "editbox", "Sort index (the lesser the more important)")]
	int Sort;
	[Attribute("0", "combobox", "Autoinit", "", { ParamEnum("No", "0"), ParamEnum("Yes", "1") } )]
	int AutoInit;
	[Attribute("0", "combobox", "Forcing creation of render target for camera #0 in Workbench", "", { ParamEnum("No", "0"), ParamEnum("Yes", "1") } )]
	bool ForceCreation;
	bool m_Show = true; // when autoinit, wait with showing the render target after all entities are created (EOnInit)
	ref RenderTargetWidget m_RenderWidget;
	
	void RenderTarget(IEntitySource src, IEntity parent)
	{
		SetFlags(EntityFlags.ACTIVE, false);

		if (AutoInit)
		{
			m_Show = false;
			SetEventMask(EntityEvent.INIT);
			Init();
		}
	}

	void ~RenderTarget()
	{
		delete m_RenderWidget;
	}
	
	void Init()
	{
		#ifdef WORKBENCH // Workbench is using its own renderer for main camera, it is not using render target widget.
			if (!ForceCreation && CameraIndex == 0)
				return;
		#endif

		int screenW, screenH;
		GetScreenSize(screenW, screenH);

		int posX = (float)(screenW * X);
		int posY = (float)(screenH * Y);
		int widthPix = (float)(screenW * Width);
		int heightPix = (float)(screenH * Height);
		if (Class.CastTo(m_RenderWidget, GetGame().GetWorkspace().CreateWidget(RenderTargetWidgetTypeID, posX, posY, widthPix, heightPix, WidgetFlags.VISIBLE | WidgetFlags.HEXACTSIZE | WidgetFlags.VEXACTSIZE | WidgetFlags.HEXACTPOS | WidgetFlags.VEXACTPOS, 0xffffffff, Sort)))
		{	
			m_RenderWidget.Show(m_Show);	
			SetWidgetWorld(m_RenderWidget, GetGame().GetWorldEntity(), CameraIndex);
		}
	}
	override void EOnInit(IEntity other, int extra) //!EntityEvent.INIT
	{
		if (m_RenderWidget)
		{
			m_Show = true;
			m_RenderWidget.Show(m_Show);
		}
	}
}

#endif