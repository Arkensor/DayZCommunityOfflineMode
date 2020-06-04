class BarrelCrosshair extends ScriptedWidgetEventHandler 
{
	protected Widget m_root;
	
	private Weapon		m_Weapon;
	private vector		m_Position;
	private PlayerBase  m_Player;
	
	float distance = 0;
	
	void BarrelCrosshair()
	{
		m_Player = NULL;
		m_Weapon = NULL;
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(this.Update);
	}
	
	void ~BarrelCrosshair()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(this.Update);
	}
	
	void OnWidgetScriptInit(Widget w)
	{
		m_root = w;
		m_root.SetHandler(this);
		m_root.Update();
	}
	
	private void Update()
	{
		if(!m_Player) Class.CastTo(m_Player, COM_GetPB());
		
		if ( COM_bc_Visible ) 
		{
			if( m_Player && m_Player.IsPlayerSelected() && !m_Player.IsInIronsights() && !GetGame().IsInventoryOpen() )
			{
				float sx, sy;

				GetCrosshairPosition();
				vector screenSpace = GetGame().GetScreenPos(m_Position);

				m_root.GetSize(sx, sy);
				screenSpace[0] = screenSpace[0] - sx/2.0;
				screenSpace[1] = screenSpace[1] - sy/2.0;

				/*
				float size = 0.005 - distance/100000.0;
				if ( size < 0.002 ) size = 0.002;
				m_root.SetSize(size, size);
				*/
				
				m_root.SetPos(screenSpace[0], screenSpace[1]);
				// m_root.SetAlpha(alpha);
				
				m_root.Show(true);
				
				return;
			}
		}
		
		if ( m_root.IsVisible() ) 
		{
			m_root.Show(false);
			m_Position = vector.Zero;
		}
	}
	
	Weapon COM_GetWeaponInHands()
	{
		Weapon weapon_in_hands;
		PlayerBase player = PlayerBase.Cast(COM_GetPB());
		if( player && player.GetItemInHands() ) Class.CastTo(weapon_in_hands,  player.GetItemInHands());
		
		return weapon_in_hands;
	}
	
	private void GetCrosshairPosition()
	{
		ItemBase itemInHands;
		itemInHands = m_Player.GetItemInHands();
		if( itemInHands && itemInHands.IsWeapon() )
		{
			if( Class.CastTo(m_Weapon, itemInHands) )
			{
				vector usti_hlavne_position = GetGame().ObjectGetSelectionPosition( m_Weapon, "usti hlavne" );
				vector konec_hlavne_position = GetGame().ObjectGetSelectionPosition( m_Weapon, "konec hlavne" );
				
				vector end_point = m_Weapon.ModelToWorld(usti_hlavne_position);
				vector begin_point = m_Weapon.ModelToWorld(konec_hlavne_position);
				
				vector contact_dir;
				int contact_component;
				
				vector aim_point = end_point - begin_point;
				aim_point = aim_point.Normalized() * 9999;
				aim_point = aim_point + end_point;
				
				if (DayZPhysics.RaycastRV(begin_point, aim_point, m_Position, contact_dir, contact_component, null, null, null, false, false, ObjIntersectFire)) 
				{
					distance = vector.Distance(begin_point, m_Position);
				}
			}
		}
	}
}