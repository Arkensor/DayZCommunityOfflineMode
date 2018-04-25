// -----------------------------------------------------------
class RadialProgressBar
{
	reference float speed;
	reference float start_rotation;

	protected string m_BarHider;
	protected string m_BarPart;
	protected Widget m_Root;	
	protected ref AnimatorTimer m_Anim;
	protected float x, y, z;
	protected float rotation = 0;
	protected int stage = 0;

	// -----------------------------------------------------------
	void RadialProgressBar()
	{
		m_Anim = new AnimatorTimer();
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(this.Update);
	}
	
	// -----------------------------------------------------------
	void ~RadialProgressBar()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(this.Update);
	}
	
	void SetProgress( float progress )
	{
		if( progress < 50 )
		{
			stage = 0;
		}
		rotation = 360 * ( progress / 100 );
	}
	
	// -----------------------------------------------------------
	protected void Update(float tDelta)
	{
		m_Anim.Tick(tDelta);
		
		Widget child = m_Root.GetChildren();
		
		int index = 0;
		while ( child )
		{
			UpdateChild( child, index );
			index++;
			child = child.GetSibling();
		}
	}
	
	protected void UpdateChild( Widget child, int index )
	{
		float rotation_value = ( m_Anim.GetTargetValue() * Math.RAD2DEG );
		if( child.GetName() == m_BarHider )
		{
			if( stage == 0 )
			{
				if( rotation > 0 )
				{
						child.GetChildren().Show(true);
				}
				else
				{
						child.GetChildren().Show(false);	
				}
				
				child.GetChildren().SetRotation( 0, 0, start_rotation + rotation);
				
				if(  rotation > 180 )
				{
					stage = 1;
					child.GetChildren().SetRotation(0, 0, 360);
				}
				
			}	
		}
		else if( child.GetName() == m_BarPart )
		{
			if( stage == 0 )
			child.Show( false );
			if( stage == 1 )
			{
				child.Show( true );
				child.SetRotation( 0, 0, start_rotation + rotation );
				
				if( rotation > 360 )
				{
					stage = 2;
					child.SetRotation( 0, 0, 180 );
				}
			}
		}
	}
	
	// -----------------------------------------------------------
	void OnWidgetScriptInit( Widget w )
	{
		m_Root = w;
		m_BarHider = "BarHider_" + m_Root.GetName();
		m_BarPart = "BarPart2_" + m_Root.GetName();
		m_Anim.AnimateLoop( speed );
	}
};