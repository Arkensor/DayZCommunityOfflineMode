// -----------------------------------------------------------
class RightGap : ScriptedWidgetEventHandler
{
	reference int gap;
	bool kola;
	/*void OnWidgetScriptInit(Widget w)
	{
		float width;
		float height;
		w.SetFlags( WidgetFlags.EXACTSIZE, false );
		w.GetScreenSize( width, height );
		Print(w.GetName());
		Print(width);
		Print(height);
		w.SetSize( width-gap, height );
		w.Update();
	}*/
	override bool OnUpdate( Widget  w)
	{
		float width;
		float height;
		w.SetFlags( WidgetFlags.EXACTSIZE, false );
		w.GetParent().GetScreenSize( width, height );
		w.SetSize( width-gap, height );
		return false;
	}
};