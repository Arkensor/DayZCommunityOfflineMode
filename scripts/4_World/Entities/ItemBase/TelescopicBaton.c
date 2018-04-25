class TelescopicBaton : ItemBase
{
	protected int m_MeleeMode;
	protected int m_MeleeHeavyMode;
	protected int m_MeleeSprintMode;
	
	void TelescopicBaton()
	{
		Init();
	}
	
	protected void Init()
	{
		m_Opened = false;

		//!set default melee modes on init
		m_MeleeMode = 0;
		m_MeleeHeavyMode = 1;
		m_MeleeSprintMode = 2;
		
		//! sync the opened/closed state from server to clients
		RegisterNetSyncVariableBool("m_Opened");
	}
	
	override void Open()
	{
		super.Open();

		//! sets different set of melee modes for opened state
		m_MeleeMode = 3;
		m_MeleeHeavyMode = 4;
		m_MeleeSprintMode = 5;

		//! single player only
		if ( !GetGame().IsMultiplayer() && !GetGame().IsClient() )
			UpdateVisualState();

		SetSynchDirty();
	}

	override void Close()
	{
		super.Close();

		//! sets different set of melee modes for closed state
		m_MeleeMode = 0;
		m_MeleeHeavyMode = 1;
		m_MeleeSprintMode = 2;

		//! single player only		
		if ( !GetGame().IsMultiplayer() && !GetGame().IsClient() )
			UpdateVisualState();

		SetSynchDirty();
	}
	
	protected void UpdateVisualState()
	{
		if ( IsOpen() )
		{
			Print("set baton to OPENED state");
			SetAnimationPhase("Doors1", 0.0);
		}
		else
		{
			Print("set baton to CLOSED state");
			SetAnimationPhase("Doors1", 1.0);
		}
	}
	
	override int GetMeleeMode()
	{
		return m_MeleeMode;
	}

	override int GetMeleeHeavyMode()
	{
		return m_MeleeHeavyMode;
	}
	
	override int GetMeleeSprintMode()
	{
		return m_MeleeSprintMode;
	}

    override void OnVariablesSynchronized()
    {
        super.OnVariablesSynchronized();

        UpdateVisualState();
    }
}
