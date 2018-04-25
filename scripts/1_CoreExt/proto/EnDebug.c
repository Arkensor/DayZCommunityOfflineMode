/**
 * \defgroup Debug Debug utilities
 * @{
 */

 /**
\brief Prints current call stack (stack trace)
	\return \p void
	@code
		DumpStack();

	@endcode

	\verbatim
	Output:
		-- Stack trace --
	   SaveFile() Scripts\Entities\Modules\ModuleBase\ModuleFileHandler.c : 51
	   SaveConfigToFile() Scripts\Entities\Modules\ModuleBase\ModuleFileHandler\ModuleLocalProfile.c : 114
	   SaveParameterArray() Scripts\Entities\Modules\ModuleBase\ModuleFileHandler\ModuleLocalProfile.c : 133
	   SetParameterArray() Scripts\Entities\Modules\ModuleBase\ModuleFileHandler\ModuleLocalProfile.c : 231
	   PresetAdd() Scripts\Entities\Modules\ModuleBase\ModuleFileHandler\ModuleLocalProfile\ModuleLocalProfileUI.h : 46
	   OnKeyPress() Scripts/mission/missionGameplay.c : 215
	   OnKeyPress() Scripts/DayZGame.c : 334
	   -----------------
	\endverbatim
*/
proto void DumpStack();

//! Triggers breakpoint in C++ in run time(when app is running in debug enviroment)
proto void DebugBreak(bool condition = true, void param1 = NULL, void param2 = NULL, void param3 = NULL, void param4 = NULL, void param5 = NULL, void param6 = NULL, void param7 = NULL, void param8 = NULL, void param9 = NULL);

//! Triggers breakpoint in C++ in compile time(when app is running in debug enviroment)
void CompileBreak();

//!Prints content of variable to console/log. Should be used for critical messages so it will appear in debug log
proto void DPrint(string var);

//!Messagebox with error message
proto native void Error2(string title, string err);

//!Messagebox with error message
void Error(string err)
{
	Error2("", err);
}

//!Prints content of variable to console/log
proto void Print(void var);

	
/**
\brief Prints formated text to console/log
	@code
		string c = "Peter";
		PrintFormat("Hello %1, how are you?", c); // prints "Hello 'Peter', how are you?"
	@endcode
*/	
proto void PrintFormat(string fmt, void param1 = NULL, void param2 = NULL, void param3 = NULL, void param4 = NULL, void param5 = NULL, void param6 = NULL, void param7 = NULL, void param8 = NULL, void param9 = NULL);

	//------------------------------------------
/**
 * \defgroup DebugShape Debug Shape API definition
 * @{
 */
enum ShapeType
{
	BBOX,			//<	Just box
	LINE,			//<	One or more lines
	SPHERE,		//<	Sphere represented by triangle mesh
	CYLINDER,	//<	Cylinder represented by triangle mesh
	DIAMOND,	//< Eight faced pyramid. Defined by bound-box, where middle part is  equal to horizontal extents of box and top/bottom apogees lies on top/bottom side of box.
	PYRAMID	//< Four sided pyramid. Defined by bound-box, where base is equal to bottom side of box.
};

enum ShapeFlags
{
	NOZBUFFER,	//< Do not compare z-buffer when render
	NOZWRITE,		//< Do not update z-buffer when render
	WIREFRAME,	//< Render just wire-frame outline. No solid faces
	TRANSP,			//< Is translucent
	DOUBLESIDE,	//< Double-sided (do not cull back-faces)
	ONCE,				//< Rendered just once and then it's automatically destroyed. Do not keep pointer to these!!
	NOOUTLINE,	//< Render just solid faces. No wire-frame outline.
	BACKFACE,		//< Render just back faces
	NOCULL,			//< Do not cull shapes by view frustum
	VISIBLE,		//< Make it visible. Set by default
	ADDITIVE		//< Additive blending (works with ShapeFlags.TRANSP)
};

enum CollisionFlags
{
	FIRSTCONTACT,	//<In many cases only collided=true/false is enough
	NEARESTCONTACT	//<We want only one, the nearest contact
	ONLYSTATIC		//<Only static objects
	ONLYDYNAMIC		//<Only dynamic objects
	ONLYWATER		//<Only water components (legacy support for "walk on geometry")
	ALLOBJECTS		//<Valid when CF_FIRST_CONTACT, we get first contact for each object
}
		
/*!
Instance of created debug visualizer
*/
class Shape
{
	//!don't call destructor directly. Use Destroy() instead
	proto private void ~Shape();

	proto native void SetMatrix(vector mat[4]);
	proto native void SetColor(int color);
	proto native void SetFlags(ShapeFlags flags);
	proto native void Destroy();

	proto static native Shape Create(ShapeType type, int color, ShapeFlags flags, vector p1, vector p2);
	proto static native Shape CreateLines(int color, ShapeFlags flags, vector p[], int num);
	proto static native Shape CreateTris(int color, ShapeFlags flags, vector p[], int num);
	proto static native Shape CreateSphere(int color, ShapeFlags flags, vector origin, float radius);
	proto static native Shape CreateCylinder(int color, ShapeFlags flags, vector origin, float radius, float length);

	static Shape CreateArrow(vector from, vector to, float size, int color, ShapeFlags flags)
	{
		vector dir = to - from;
		dir.Normalize();
		vector dir1 = dir * size;
		size = size * 0.5;
	
		vector dir2 = dir.Perpend() * size;
	
		vector pts[5];
		pts[0] = from;
		pts[1] = to;
		pts[2] = to - dir1 - dir2;
		pts[3] = to - dir1 + dir2;
		pts[4] = to;
	
		return CreateLines(color, flags, pts, 5);
	}

	static Shape CreateBridgeArrow(vector from, vector to, float size, int color, ShapeFlags flags)
	{
		vector dir = to - from;
		dir.Normalize();
	
		vector dir1 = Vector(0, 0, -size);
		size = size * 0.5;
	
		vector dir2 = dir.Perpend() * size;
	
		vector pts[7];
		pts[0] = from;
		pts[1] = from + "0 0 1";
		pts[2] = to + "0 0 1";
		pts[3] = to;
		pts[4] = to - dir1 - dir2;
		pts[5] = to - dir1 + dir2;
		pts[6] = to;
	
		return CreateLines(color, flags, pts, 7);
	}

	static void CreateMatrix(vector mat[4])
	{
		vector org = mat[3];
		int flags = ShapeFlags.NOZWRITE|ShapeFlags.DOUBLESIDE|ShapeFlags.TRANSP|ShapeFlags.ONCE;
		Create(ShapeType.LINE, 0xffff0000, flags, org, mat[0] * 0.5 + org);
		Create(ShapeType.LINE, 0xff00ff00, flags, org, mat[1] * 0.5 + org);
		Create(ShapeType.LINE, 0xff0000ff, flags, org, mat[2] * 0.5 + org);
	}
};

//@}


//------------------------------------------
/**
 * \defgroup DiagMenu Diag menu API definition
 * @{
 */
class DiagMenu
{
	proto static native void RegisterMenu(int id, string name, string parent);
	proto static native void Unregister(int id);
	proto static native void RegisterItem(int id, string shortcut, string name, string parent, string values);
	/*!
	range value, are defined in format "min,max,startValue,step" -> e.g. "-2, 2, 0, 0.1"
	*/ 
	proto static native void RegisterRange(int id, string shortcut, string name, string parent, string valuenames);
	proto static native void RegisterBool(int id, string shortcut, string name, string parent, bool reverse = false);
	proto static native bool GetBool(int id, bool reverse = false);
	proto static native int GetValue(int id);
	proto static native void SetValue(int id, int value);
	proto static native float GetRangeValue(int id);
	proto static native void SetRangeValue(int id, float value);
};

//@}

//@}
