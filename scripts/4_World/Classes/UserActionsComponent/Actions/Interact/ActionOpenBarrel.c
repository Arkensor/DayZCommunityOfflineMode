class ActionOpenBarrel: ActionInteractBase
{
	void ActionOpenBarrel()
	{
		m_MessageSuccess = "I have opened it.";
		//m_Animation = "open";	
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENLID;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_HUDCursorIcon = CursorIcons.OpenHood;
	}

	override int GetType()
	{
		return AT_OPEN_BARREL;
	}

	override string GetText()
	{
		return "open";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object target_object = target.GetObject();
		if ( target_object.IsItemBase() )
		{
			Barrel_ColorBase ntarget = Barrel_ColorBase.Cast( target_object );
			if( ntarget )
			{
				if( !ntarget.IsLocked() && !ntarget.IsOpen() )
				{
					return true;
				}
			}
		}
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Object target_object = target.GetObject();
		Barrel_ColorBase ntarget = Barrel_ColorBase.Cast( target_object );
		if( ntarget )
		{
			ntarget.Open();
		}
	}
}