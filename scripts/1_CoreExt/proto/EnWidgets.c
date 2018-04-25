/**
 * \defgroup WidgetAPI Widget UI system
 * @{
 */

#ifdef DOXYGEN
/** @name WidgetType
 *  Following WidgetType constants are available to script 
 */
///@{

	//!Single-line text. See enf::TextWidget
	TextWidgetTypeID,
	//!Multi-line text. See enf::MultilineTextWidget
	MultilineTextWidgetTypeID,
	//!Multi-line edit box. See enf::MultilineTextWidget
	MultilineEditBoxWidgetTypeID,
	//!Multi-line text with images in text. See enf::RichTextWidget
	RichTextWidgetTypeID,
	//! Render target for enf::BaseWorld. See enf::RenderTargetWidget
	RenderTargetWidgetTypeID,
	//! Picture, or multiple picture. See enf::ImageWidget
	ImageWidgetTypeID,
	//!Console. See enf::ConsoleWidget
	ConsoleWidgetTypeID,
	//!Video player. See enf::VideoWidget
	VideoWidgetTypeID,
	//! Texture used as render target for children widgets. See enf::RTTextureWidget
	RTTextureWidgetTypeID,
	//! Dummy frame, used as hierarchy node and clipper
	FrameWidgetTypeID,
	//! Dummy frame, used for embedding another layout and as hierarchy node and clipper
	EmbededWidgetTypeID,
	ButtonWidgetTypeID,
	CheckBoxWidgetTypeID,
	WindowWidgetTypeID,
	ComboBoxWidgetTypeID,
	SimpleProgressBarWidgetTypeID,
	ProgressBarWidgetTypeID,
	SliderWidgetTypeID,
	BaseListboxWidgetTypeID,
	TextListboxWidgetTypeID,
	GenericListboxWidgetTypeID,
	EditBoxWidgetWidgetTypeID,
	WorkspaceWidgetWidgetTypeID,
	CanvasWidgetWidgetTypeID,
///@}
#else
	typedef TypeID WidgetType;
#endif

typedef TypeID EventType;

enum WidgetFlags
{
	SOURCEALPHA, //< takes alpha from texture * alpha from color. If not set, considers texture as non-transparent and transparency is set only by color
	BLEND,	//< texture is blended with a surface base on alpha
	ADDITIVE,	//< texture is added to a surface based on alpha
	VISIBLE,	//<Widget is visible - ShowWidget(w, true)
	NOWRAP,	//< Do not do texture wrapping
	CENTER,	//< Centers TextWidgetTypeID
	VCENTER,	//< Centers TextWidgetTypeID verticaly
	HEXACTPOS,
	VEXACTPOS,
	EXACTPOS,	//< Uses physical resolution (g_iWidth, h_iHeight)
	HEXACTSIZE,	//< Uses physical resolution (g_iWidth)
	VEXACTSIZE,	//< Uses physical resolution (h_iHeight)
	EXACTSIZE,	//< Uses physical resolution
	NOFILTER,	//< no texture filtering (no blur)
	RALIGN,	//< Right alignment TextWidgetTypeID
	STRETCH,	//< Stretch texture to a full size
	FLIPU,	//< Flips texture in U axis
	FLIPV,	//< Flips texture in V axis
	CUSTOMUV,	//< ignores STRETCH/FLIPU/FLIPV and take custom UV set by SetWidgetUV()
	IGNOREPOINTER,
	DISABLED,
	NOFOCUS,
	CLIPCHILDREN,
	RENDER_ALWAYS,
	NOCLEAR,
	DRAGGABLE
};

//------------------------------------------
class Widget: Managed
{
	proto void ~Widget();
	proto private void Widget();
	proto static string TranslateString(string stringId);
	proto native owned string GetName();
	proto native void SetName(string name);
	proto native owned string GetTypeName();
	proto native WidgetType GetTypeID();
	proto native void Show(bool show);
	proto native void Enable(bool enable);
	proto native int GetFlags();
	proto native int SetFlags(int flags, bool immedUpdate = true);
	proto native int GetSort();
	proto native void SetSort(int sort, bool immedUpdate = true);
	proto native int ClearFlags(int flags, bool immedUpdate = true);
	proto native bool IsControlClass();
	proto native owned string GetStyleName();
	proto void GetUserData(out Class data);
	proto native void SetUserData(Class data);
	proto native int GetUserID();
	proto native void SetUserID(int id);
	proto native bool IsVisible();
	proto native bool IsVisibleHierarchy();
	proto native void SetPos(float x, float y, bool immedUpdate = true);
	proto native void SetSize(float x, float y, bool immedUpdate = true);
	proto native void SetColor(int color);
	proto native int GetColor();
	proto native void SetRotation(float roll, float pitch, float yaw, bool immedUpdate = true);
	proto native void SetAlpha(float alpha);
	proto native float GetAlpha();
	proto void GetPos(out float x, out float y);
	proto void GetSize(out float width, out float height);
	proto void GetScreenPos(out float x, out float y);
	proto void GetScreenSize(out float width, out float height);

  proto native void SetTransform(vector mat[4], bool immedUpdate = true);

  proto native Widget GetParent();
	proto native Widget GetChildren();
	proto native Widget GetSibling();
	proto native void AddChild(Widget child, bool immedUpdate = true);
	proto native void RemoveChild(Widget child);

	proto native volatile void Update();

	proto void GetScript(out Class data);

	proto native Widget FindWidget(string pathname);	//find Widget by path. e.g FindWidget("widget1.widget2.widget3.mywidget")
	proto native Widget FindAnyWidget(string pathname);	//find Widget by name e.g. FindWidget("widget1")
	proto native Widget FindAnyWidgetById(int user_id);	//find Widget by userID

	proto native void SetHandler(ScriptedWidgetEventHandler eventHandler);
	proto native void Unlink(); //destroys widget and all its children
};

class WorkspaceWidget: Widget
{
	//!Create widgets by \ref WidgetType
	proto native external Widget CreateWidget(WidgetType type, int left, int top, int width, int height, WidgetFlags flags, int color, int sort, Widget parentWidget = NULL);
	//!Create widgets from *.layout file
	proto native external Widget CreateWidgets(string layout, Widget parentWidget = NULL);	
};

proto native Widget GetWidgetUnderCursor();
proto native void ReportMouse(int mousex, int mousey, Widget rootWidget);

class TextWidget extends Widget
{
  	proto native void SetTextSpacing(int horiz, int vert);
	//! set text exact size. Exact Text flag must be enabled. 0 equals original size.
	proto native void SetTextExactSize(int size);
  	proto native void SetTextOffset(int left, int top);
	proto native void SetText(string text);
	proto native void SetOutline(int outlineSize, int argb = 0xFF000000);
	proto native int GetOutlineSize();
	proto native int GetOutlineColor();
	proto native void SetShadow(int shadowSize, int shadowARGB = 0xFF000000, float shadowOpacity = 1, float shadowOffsetX = 0, float shadowOffsetY = 0);
	proto native int GetShadowSize();
	proto native int GetShadowColor();
	proto native float GetShadowOpacity();
	proto void GetShadowOffset(out float sx, out float sy);
	proto native void SetItalic(bool italic);
	proto native bool GetItalic();
	proto native void SetBold(bool bold);
	proto native bool GetBold();
	
	//! Returns text size in pixels
	proto void GetTextSize(out int sx, out int sy);
	proto void SetTextFormat(string text, void param1 = NULL, void param2 = NULL, void param3 = NULL, void param4 = NULL, void param5 = NULL, void param6 = NULL, void param7 = NULL, void param8 = NULL, void param9 = NULL);
};

class MultilineTextWidget extends TextWidget
{
};

class RichTextWidget extends TextWidget
{
	proto native float GetContentHeight();
	proto native float GetContentOffset();
	proto native void  SetContentOffset(float offset, bool snapToLine = false);
	proto native void  ElideText(int line, float maxWidth, string str);
	proto native int   GetNumLines();
	proto native void  SetLinesVisibility(int lineFrom, int lineTo, bool visible);
	proto native float GetLineWidth(int line);
};

class RenderTargetWidget extends Widget
{
//!when period > 1 then every n-th frame will be rendered. Offset is initial counter.
	proto native void SetRefresh(int period, int offset);
	proto native void SetResolutionScale(float xscale, float ycale);
};

class RTTextureWidget extends Widget
{
};

class ImageWidget extends Widget
{
	/*!
	Loads image. When image of this 'num' is already loaded, then is replaced
	by new one.
	\param num Number of image (0...7)
	\param name Name of image file
	\param noCache Do not cache the texture
	\return True when image is loaded, false otherwise
	*/
	proto native bool LoadImageFile(int num, string name, bool noCache = false);
	proto native void SetImageTexture(int image, RTTextureWidget texture);
	//! returns size of image
	proto void GetImageSize(int image, out int sx, out int sy);
	
	/*!
	Switches to another image, if it's loaded
	\param num Number of image.
	\return True when there is such image, false otherwise.
	*/
	proto native bool SetImage(int num);
	//!Returns active image
	proto native int GetImage();
	/*!
	When WF_CUSTOMUV is set, we can supply our own UV coords instead of computed ones.
	\param uv
	Pointer to array of at least 8 floats (4 corners * 2 floats for u/v)
	*/
	proto native void SetUV(Widget w, float uv[4][2]);
};

class MultilineEditBoxWidget extends TextWidget
{
	proto native int GetLinesCount();
	proto native int GetCarriageLine();
	proto native int GetCarriagePos();
	proto void GetText(out string text);
	proto native void SetLine(int line, string text);
	proto void GetLine(int line, out string text);
};

class UIWidget extends Widget
{
	proto native void SetTextColor(int color);
	proto native void SetTextOutline(int outlineSize, int argb = 0xFF000000);
	proto native int GetTextOutlineSize();
	proto native int GetTextOutlineColor();
	proto native void SetTextShadow(int shadowSize, int shadowARGB = 0xFF000000, float shadowOpacity = 1.0, float shadowOffsetX = 0.0, float shadowOffsetY = 0.0);
	proto native int GetTextShadowSize();
	proto native int GetTextShadowColor();
	proto native float GetTextShadowOpacity();
	proto native float GetTextShadowOffsetX();
	proto native float GetTextShadowOffsetY();
	proto native void SetTextItalic(bool italic);
	proto native bool GetTextItalic();
	proto native void SetTextBold(bool bold);
	proto native bool GetTextBold();
};

class CanvasWidget extends Widget
{
	proto native void DrawLine(float x1, float y1, float x2, float y2, float width, int color);
	proto native void Clear();
};

class EditBoxWidget extends UIWidget
{
	proto string GetText();
	proto native void SetText(string str);
};

class SliderWidget extends UIWidget
{
	proto native float GetMin();
	proto native float GetMax();
	proto native float GetCurrent();
	proto native void SetCurrent(float curr);
};

class SimpleProgressBarWidget extends UIWidget
{
	proto native float GetMin();
	proto native float GetMax();
	proto native float GetCurrent();
	proto native void SetCurrent(float curr);
};

class ProgressBarWidget extends SimpleProgressBarWidget
{
};

class ButtonWidget extends UIWidget
{
	proto native bool GetState();

	proto native bool SetState(bool state);

	proto native void SetText(string text);

	proto native void SetTextOffset(float xoffset, float yoffset);
/**
\param align ALIGN_CENTER, ALIGN_LEFT, ALIGN_RIGHT
*/
	proto native void SetTextHorizontalAlignment(int align);
/**
\param align ALIGN_CENTER, ALIGN_TOP, ALIGN_BOTTOM
*/
	proto native void SetTextVerticalAlignment(int align);

	//! Get text proportion - ratio between button height and button text height in interval <0,1>
	proto native float GetTextProportion();
	//! Set text proportion - ratio between button height and button text height in interval <0,1>
	proto native void SetTextProportion(float val);
};

class XComboBoxWidget extends UIWidget
{
	proto native int AddItem(string item);
	proto native void ClearAll();
	proto native void SetItem(int item, string value);
	proto native void RemoveItem(int item);
	proto native int GetNumItems();
	proto native int SetCurrentItem(int n);
	proto native int GetCurrentItem();
};

class CheckBoxWidget extends UIWidget
{
	proto native void SetText(string str);
	proto native bool IsChecked();
	proto native void SetChecked(bool checked);
};

class BaseListboxWidget extends UIWidget
{
	proto native void ClearItems();
	proto native int GetNumItems();
	proto native void SelectRow(int row);
	proto native int GetSelectedRow();
	proto native void RemoveRow(int row);
	proto native void EnsureVisible(int row);
};

class SimpleListboxWidget extends BaseListboxWidget
{
};

class TextListboxWidget extends SimpleListboxWidget
{
	//! Insert new Row, if row = -1, new Row is inserted at the end otherwise at row index.
	proto native int AddItem(string text, Class userData, int column, int row = -1);
	proto native void SetItem(int position, string text, Class userData, int column);
/**
\brief Get item
	\param row \p int Index of row
	\param column \p int Index of column
	\return \p string Value in widget on row and column
	@code
		string value;
		textListboxWidg.GetItemText(2, 0, value);
	@endcode
*/
	proto bool GetItemText(int row, int column, out string text);
	proto void GetItemData(int row, int column, out Class data);
	
	proto native void SetItemColor(int row, int column, int color );
};

//VideoWidget
enum VideoCommand
{
	PLAY,
	STOP,
	REWIND,
	POSITION,
	REPEAT,
	ISPLAYING
};

class VideoWidget extends Widget
{
	proto native int Play(VideoCommand cmd);
	proto native bool LoadVideo(string name, int soundScene);
};

/*! sets Widget typu RTTextureWidgetTypeID, to which it is possible to reference in shader as $rendertarget
it is posible to use only after object selection. When selecting another object, it is necessary to set GUI widget again
*/
proto native void SetGUIWidget(IEntity ent, int index, RTTextureWidget w);

enum ControlID
{
	CID_NONE = 0,
	CID_SELECT = 1,//select/use focused
	CID_BACK,
	CID_LEFT, //navigation
	CID_RIGHT,
	CID_UP,
	CID_DOWN,
	CID_MENU, //get to main menu
	CID_DRAG, //probably needed only for consoles
	CID_TABLEFT,
	CID_TABRIGHT,
	CID_RADIALMENU,
	CID_COUNT
};

class ScriptedWidgetEventHandler: Managed
{
	bool OnClick(Widget w, int x, int y, int button)
	{
		return false;
	}
	bool OnModalResult(Widget w, int x, int y, int code, int result)
	{
		return false;
	}
	bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		return false;
	}
	bool OnSelect(Widget w, int x, int y)
	{
		return false;
	}
	bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn)
	{
		return false;
	}
	bool OnFocus(Widget w, int x, int y)
	{
		return false;
	}
	bool OnFocusLost(Widget w, int x, int y)
	{
		return false;
	}
	bool OnMouseEnter(Widget w, int x, int y)
	{
		return false;
	}
	bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		return false;
	}
	bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		return false;
	}
	bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		return false;
	}
	bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		return false;
	}
	//! control is one of ControlID 
	bool OnController(Widget w, int control, int value)
	{
		return false;
	}
	bool OnKeyDown(Widget w, int x, int y, int key)
	{
		return false;
	}
	bool OnKeyUp(Widget w, int x, int y, int key)
	{
		return false;
	}
	bool OnKeyPress(Widget w, int x, int y, int key)
	{
		return false;
	}
	bool OnChange(Widget w, int x, int y, bool finished)
	{
		return false;
	}
	bool OnDrag(Widget w, int x, int y)
	{
		return false;
	}
	bool OnDragging(Widget w, int x, int y, Widget reciever)
	{
		return false;
	}
	bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
		return false;
	}
	bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		return false;
	}
	bool OnDropReceived(Widget w, int x, int y, Widget reciever)
	{
		return false;
	}
	bool OnResize(Widget w, int x, int y)
	{
		return false;
	}
	bool OnChildAdd(Widget w, Widget child)
	{
		return false;
	}
	bool OnChildRemove(Widget w, Widget child)
	{
		return false;
	}
	bool OnUpdate(Widget w)
	{
		return false;
	}
	bool OnEvent(EventType eventType, Widget target, int parameter0, int parameter1)
	{
		return false;
	}
};

//Common Widget API
proto native void SetCursorWidget(Widget cursor);
proto native void ShowCursorWidget(bool show);
proto native bool LoadWidgetImageSet(string filename);
proto native void LoadWidgetStyles(string filename);
// sets active window (widget which owns some control inputs like buttons, listboxes etc.). Necessary for controlling the focus by keyboard/joypad. In case when setFocus is set to true, it sets focus on a first child Widget, which may receive the focus (is not disabled, set as NoFocus etc.)
proto native bool SetActiveWindow(Widget w, bool resetFocus);

// sets focus (necessary when using keyboard/joyped) to a particular widget. Widget must have some inputs like button, listbox, checkbox, combobox etc.
proto native void SetFocus(Widget w);

proto native void SetModal(Widget w);

proto native Widget GetFocus();

//RenderTargetWidgetTypeID
proto native void SetWidgetWorld(RenderTargetWidget w, IEntity wrldEntity, int camera);


#ifdef PS3
	/*!
	circle and cross can be swapped when license area is japan/asia
	*/
	proto native bool IsCircleToCrossSwapped();
#endif

proto native bool ReloadTexture(string path);


//@}
