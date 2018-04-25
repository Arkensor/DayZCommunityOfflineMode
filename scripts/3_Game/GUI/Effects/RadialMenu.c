class RadialMenu : ScriptedWidgetEventHandler
{
	protected Widget					m_Parent;
	protected Widget 					m_ItemCardsContainer;
	protected Widget					m_RadialSelector;
	protected Widget 					m_SelectedObject;
	protected ref map<Widget, float> 	m_RadialItemCards;
	
	protected float 					m_AngleRadOffset;
	protected ref Timer 				m_UpdateTimer;
	
	//widget
	static const string 				RADIAL_SELECTOR	 			= "RadialSelector";
	static const string 				RADIAL_DELIMITER_CONTAINER 	= "RadialDelimiterContainer";
	static const string 				RADIAL_ITEM_CARD_CONTAINER 	= "RadialItemCardContainer";

	//controls
	protected bool 						m_UsingMouse;
	protected int 						m_ControllerAngle;
	protected int 						m_ControllerTilt;
	
	//controller
	protected int 						m_ControllerTimout;
	protected bool						m_IsControllerTimoutEnabled 		= true;		//enables/disables controller deselect timeout reset
	protected const float				CONTROLLER_DESELECT_TIMEOUT 		= 1000;			//timeout [ms] after which selection is automatically deselect when controller is not active
	protected const int 				CONTROLLER_TILT_TRESHOLD			= 8;			//tilt value (0-10) for controller sticks after which the selection will be executed 
	
	//mouse
	protected const float 				MOUSE_SAFE_ZONE_RADIUS = 50;		//Radius [px] of safe zone where every previous selection is deselected
	
	//References
	protected float 					m_RadiusOffset;					//Radius [% of the main container size]
	protected float 					m_ExecuteDistanceOffset;		//Distance offset [% of the main container size] after which the selection will be automatically executed
	protected float						m_OffsetFromTop;				//first item in the menu won't be directly on top but offset by a rad angle value (clock-wise)
	protected float						m_ItemCardRadiusOffset;			//Radius [% of the main container size] for item cards
	protected string					m_DelimiterLayout;				//layout file name with path
	
	ref UIScriptedMenu					m_RegisteredClass;	
	ref static RadialMenu				m_Instance;
	
	//============================================
	// RadialMenu
	//============================================
	void RadialMenu()
	{
		m_Instance = this;
		
		m_RadialItemCards = new ref map<Widget, float>;
		m_UpdateTimer = new ref Timer();
		m_UpdateTimer.Run( 0.03, this, "Update", NULL, true );
	}

	void ~RadialMenu()
	{
	}
	
	static RadialMenu GetInstance()
	{
		return m_Instance;
	}
	
	//Set handler
	void OnWidgetScriptInit( Widget w )
	{
		m_ItemCardsContainer = w.FindAnyWidget( RADIAL_ITEM_CARD_CONTAINER );
		m_RadialSelector = w.FindAnyWidget( RADIAL_SELECTOR );
		
		//parent
		m_Parent = w;
		m_Parent.SetHandler( this );
		SetFocus( m_Parent );			//set focus to widget
	}

	//============================================
	// Setup
	//============================================	
	void RegisterClass( UIScriptedMenu class_name )
	{
		m_RegisteredClass = class_name;
	}	

	//Set PARAMs
	void AdjustRadialMenu( float radius_offset, float execute_dist_offset, float offset_top, float item_card_radius_offset, bool is_contr_timeout_enabled )
	{
		m_RadiusOffset 				= radius_offset;
		m_ExecuteDistanceOffset		= execute_dist_offset; 
		m_OffsetFromTop				= offset_top;
		m_ItemCardRadiusOffset 		= item_card_radius_offset;
		m_IsControllerTimoutEnabled = is_contr_timeout_enabled;
	}
	
	void SetWidgetProperties( string delimiter_layout )
	{
		m_DelimiterLayout = delimiter_layout;
	}
	
	//============================================
	// Visual
	//============================================	
	//hide_selector => shows/hides radial selector when refreshing radial menu
	void Refresh( bool hide_selector = true )
	{
		int item_cards_count = GetItemCardsCount();
		if ( item_cards_count > 0 ) m_AngleRadOffset = 2 * Math.PI / item_cards_count;		
		float angle_rad = -Math.PI / 2;
		
		//--PARAM top offset--
		if ( m_OffsetFromTop != 0 )
		{
			angle_rad = angle_rad + m_OffsetFromTop;
		}
		//--------------------
		
		//delete all delimiters
		Widget delimiters_panel = m_Parent.FindAnyWidget( RADIAL_DELIMITER_CONTAINER );
		if ( delimiters_panel )
		{
			Widget del_child = delimiters_panel.GetChildren();
			while ( del_child )
			{
				Widget child_to_destroy1 = del_child;
				del_child = del_child.GetSibling();
				
				delete child_to_destroy1;
			}
		}
		
		//Position item cards, crate radial delimiters
		Widget item_cards_panel = m_Parent.FindAnyWidget( RADIAL_ITEM_CARD_CONTAINER );
		Widget item_card = item_cards_panel.GetChildren();
		
		//get radius
		float original_r = GetRadius();
		float item_cards_r = original_r;
		
		//--PARAM top offset--....
		if ( m_ItemCardRadiusOffset != 0 )
		{
			item_cards_r = item_cards_r * m_ItemCardRadiusOffset;
			if ( item_cards_r < 0 ) item_cards_r = 0;				//min radius is 0
		}
		
		m_RadialItemCards.Clear();
		for ( int i = 0; i < item_cards_count; ++i )
		{
			//position item cards
			if ( item_card )
			{
				//creates circle from simple widget items
				float pos_x = item_cards_r * Math.Cos( angle_rad );
				float pos_y = item_cards_r * Math.Sin( angle_rad );
				
				pos_x = pos_x / original_r;
				pos_y =	pos_y / original_r;
				
				item_card.SetPos( pos_x, pos_y );
				
				//store item card
				m_RadialItemCards.Insert( item_card, angle_rad );
				
				//get next child
				item_card = item_card.GetSibling();
			}
			//-------------------------
			
			//create delimiter
			if ( item_cards_count > 1 && delimiters_panel && m_DelimiterLayout )
			{
				Widget delimiter_widget = GetGame().GetWorkspace().CreateWidgets( m_DelimiterLayout, delimiters_panel );
				float delim_angle_rad = angle_rad + ( m_AngleRadOffset / 2 );
				delimiter_widget.SetPos( 0, 0 );
				delimiter_widget.SetRotation( 0, 0, GetAngleInDegrees( delim_angle_rad ) + 90 );
			}
						
			//calculate next angle
			angle_rad += m_AngleRadOffset;
		}
		
		//hide selector on refresh
		if ( hide_selector )
		{
			HideRadialSelector();
		}
	}

	//Radial selector
	protected void ShowRadialSelector( Widget selected_item )
	{
		if ( m_RadialSelector && selected_item )
		{
			int angle_deg = GetAngleInDegrees( m_RadialItemCards.Get( selected_item ) );
			m_RadialSelector.SetRotation( 0, 0, angle_deg + 90 );			//rotate widget according to its desired rotation
			
			m_RadialSelector.Show( true );
		}		
	}
	
	protected void HideRadialSelector()
	{
		if ( m_RadialSelector )
		{
			m_RadialSelector.Show( false );
		}
	}
	
	//============================================
	// Widget size calculations
	//============================================	
	protected int GetItemCardsCount()
	{
		Widget child = m_ItemCardsContainer.GetChildren();
		int count = 0;
		
		while ( child )
		{
			++count;
			
			child = child.GetSibling();
		}
		
		return count;
	}
	
	protected float GetRadius()
	{
		float radius = Math.AbsFloat( GetParentMinSize() * 0.5 );
		
		//PARAM --radius--
		if ( m_RadiusOffset > 0 )
		{
			return radius * m_RadiusOffset;
		}
		//----------------
		
		return radius;
	}
	
	protected void GetParentCenter( out float center_x, out float center_y )
	{
		if ( m_Parent )
		{
			float wx;
			float wy;
			m_Parent.GetScreenPos( wx, wy );
			
			float ww;
			float wh;
			m_Parent.GetScreenSize( ww, wh );
			
			center_x = wx + ww / 2;	//center
			center_y = wy + wh / 2;
		}
	}
	
	protected float GetParentMinSize()
	{
		if ( m_Parent )
		{
			float size_x;
			float size_y;
			m_Parent.GetScreenSize( size_x, size_y );
			
			return Math.Min( size_x, size_y );
		}
		
		return 0;
	}

	//============================================
	// Angle calculations
	//============================================	
	//get object by angle (degrees)
	protected Widget GetObjectByDegAngle( float deg_angle )
	{
		for ( int i = 0; i < m_RadialItemCards.Count(); ++i )
		{
			Widget w = m_RadialItemCards.GetKey( i );
			int w_angle = GetAngleInDegrees( m_RadialItemCards.Get( w ) );
			int offset = GetAngleInDegrees( m_AngleRadOffset ) / 2;
			int min_angle = w_angle - offset;
			int max_angle = w_angle + offset;
			
			if ( min_angle < 0 ) min_angle += 360;	//clamp 0-360
			if ( max_angle > 360 ) max_angle -= 360;
			
			if ( min_angle > max_angle )		//angle radius is in the cycling point 360->
			{
				if ( min_angle <= deg_angle )	//is cursor position also before this point
				{
					if ( deg_angle > max_angle )
					{
						return w;
					}
				}
				else							//is cursor position after this point
				{
					if ( deg_angle < max_angle )
					{
						return w;
					}
				}
			}
			else
			{
				if ( deg_angle >= min_angle && deg_angle < max_angle )	//min, max angles are within 0-360 radius
				{
					return w;
				}				
			}
		}
		
		return NULL;
	}
	
	//returns GUI compatible mouse-to-parent angle
	protected float GetMousePointerAngle()
	{
		int mouse_x;
		int mouse_y;
		GetGame().GetMousePos( mouse_x, mouse_y );
		
		float center_x;
		float center_y;
		GetParentCenter( center_x, center_y );

		float tan_x = mouse_x - center_x;
		float tan_y = mouse_y - center_y;
		float angle = Math.Atan2( tan_y, tan_x );
		
		return angle;
	}
	
	//returns distance from parent center
	protected float GetMouseDistance()
	{
		int mouse_x;
		int mouse_y;
		GetGame().GetMousePos( mouse_x, mouse_y );
		
		float center_x;
		float center_y;
		GetParentCenter( center_x, center_y );

		float distance = vector.Distance( Vector( mouse_x, mouse_y, 0 ), Vector( center_x, center_y, 0 ) );
		
		return distance;
	}	
	
	protected int GetProperControllerStickAngle( int angle )
	{
		int proper_angle = ( 360 - angle ) % 360;	//rotation correction
		
		return proper_angle;
	}
	
	//return angle 0-360 deg
	protected float GetAngleInDegrees( float rad_angle )
	{
		int rad_deg = rad_angle * Math.RAD2DEG;
		
		if ( rad_deg < 0 )
		{
			rad_deg = ( rad_deg % 360 );
			rad_deg += 360;
		}
		
		return rad_deg;	
	}

	//============================================
	// Update
	//============================================
	//mouse
	int last_time = -1;
	protected void Update()
	{
		//get delta time
		if ( last_time < 0 )
		{
			last_time = GetGame().GetTime();
		}
		int delta_time = GetGame().GetTime() - last_time;
		last_time = GetGame().GetTime();
		
		//controls
		if ( this )
		{
			//mouse controls
			if ( m_UsingMouse )
			{
				
				float mouse_angle = GetMousePointerAngle();
				float mouse_distance = GetMouseDistance();
				
				//--PARAM --safe zone radius--
				if ( mouse_distance <= MOUSE_SAFE_ZONE_RADIUS )
				{
					//Deselect
					GetGame().GameScript.CallFunction( m_RegisteredClass, "OnSelectionDeselect", NULL, m_SelectedObject );
					m_SelectedObject = NULL;
					//hide selector
					HideRadialSelector();
				}
				//----------------------------
				else
				{
					//Deselect
					GetGame().GameScript.CallFunction( m_RegisteredClass, "OnSelectionDeselect", NULL, m_SelectedObject );
					
					//Select
					m_SelectedObject = GetObjectByDegAngle( GetAngleInDegrees( mouse_angle ) );
					GetGame().GameScript.CallFunction( m_RegisteredClass, "OnSelectionSelect", NULL, m_SelectedObject );				
					//show selector
					ShowRadialSelector( m_SelectedObject );
					
					//PARAM --execute distance--
					if ( mouse_distance >= GetRadius() * m_ExecuteDistanceOffset )
					{
						//Execute
						GetGame().GameScript.CallFunction( m_RegisteredClass, "OnSelectionExecute", NULL, m_SelectedObject );
					}
					//--------------------------
				}
			}
			//controller controls
			else
			{
				//if controller is in use
				if ( m_ControllerAngle > -1 && m_ControllerTilt > -1 )
				{
					//Right analogue stick
					int controller_stick_angle = GetProperControllerStickAngle( m_ControllerAngle );
					Widget w_selected = GetObjectByDegAngle( controller_stick_angle );
					
					//Select
					if ( w_selected )
					{
						if ( w_selected != m_SelectedObject )
						{
							//Deselect
							GetGame().GameScript.CallFunction( m_RegisteredClass, "OnSelectionDeselect", NULL, m_SelectedObject );
							
							//Select new object
							m_SelectedObject = w_selected;
							GetGame().GameScript.CallFunction( m_RegisteredClass, "OnSelectionSelect", NULL, m_SelectedObject );
							//show selector
							ShowRadialSelector( m_SelectedObject );							
						}
					}
					else
					{
						GetGame().GameScript.CallFunction( m_RegisteredClass, "OnSelectionDeselect", NULL, m_SelectedObject );
						m_SelectedObject = NULL;
						//hide selector
						HideRadialSelector();						
					}
					
					//Execute
					if ( m_ControllerTilt >= CONTROLLER_TILT_TRESHOLD )
					{
						//Execute
						GetGame().GameScript.CallFunction( m_RegisteredClass, "OnSelectionExecute", NULL, m_SelectedObject );
					}				
				}
				//if controller is giving no feedback
				else
				{
					if ( m_IsControllerTimoutEnabled )
					{
						m_ControllerTimout += delta_time;
						
						if ( m_ControllerTimout >= CONTROLLER_DESELECT_TIMEOUT )
						{
							GetGame().GameScript.CallFunction( m_RegisteredClass, "OnSelectionDeselect", NULL, m_SelectedObject );
							m_SelectedObject = NULL;
							//hide selector
							HideRadialSelector();						
							
							m_ControllerTimout = 0;		//reset controller timeout
						}
					}
				}
				
				m_ControllerAngle = -1;				//reset angle and tilt
				m_ControllerTilt = -1;
			}
		}
	}
	
	//============================================
	// Handler Events
	//============================================
	override bool OnController( Widget w, int control, int value )
	{
		super.OnController( w, control, value );

		//switch controls
		m_UsingMouse = false;
		
		//Right stick
		if ( control == ControlID.CID_RADIALMENU )
		{
			m_ControllerAngle 	= value >> 4; 	// <0,360>
			m_ControllerTilt	= value & 0xF; 	// <0,10>
			
			m_ControllerTimout = 0;				//reset controller timeout
			
			return true;
		}

		//A button
		if ( control == ControlID.CID_SELECT )
		{
			GetGame().GameScript.CallFunction( m_RegisteredClass, "OnControllerPressA", NULL, m_SelectedObject );
			
			return true;
		}	

		//B button
		if ( control == ControlID.CID_BACK )
		{
			GetGame().GameScript.CallFunction( m_RegisteredClass, "OnControllerPressB", NULL, m_SelectedObject );
			
			return true;
		}
				
		return false;
	}
		
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		m_UsingMouse = true;

		return false;
	}
}