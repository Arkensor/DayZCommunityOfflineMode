
class CustomBotTrigger : BotTrigger
{
	PlayerBase m_Owner;
	void CustomBotTrigger (PlayerBase p) { m_Owner = p; }
	override bool CheckTrigger () { return true); }
};

class BotTest extends Module
{	
	PlayerBase customPlayer;
	bool spawned = false;
    ref CustomBot m_Bot;

	protected CameraTool m_CameraTool;

	void BotTest()
	{
	}

	void ~BotTest()
	{
	}
	
	override void Init() 
	{
		super.Init();
	}
	
	override void onUpdate( int timeslice )
	{
		if ( !spawned ) return;

		m_Bot.OnUpdate();

		customPlayer.GetInputController().SetDisabled( false );

		HumanCommandMove hcm = customPlayer.GetCommand_Move();
		if (!hcm)
		{
			customPlayer.StartCommand_Move();
		}

		// customPlayer.GetInputController().OverrideMovementSpeed(true, 1);

        //customPlayer.CommandHandler( timeslice, DayZPlayerConstants.COMMANDID_MOVE, true );

		//GetPlayer().MessageStatus( "CAlled!");
	}
	
	override void RegisterKeyMouseBindings() 
	{
	    KeyMouseBinding toggleSpawnMenu  = new KeyMouseBinding( GetModuleType(), "SpawnBot" , "[M]"    , "Toggle spawn menu."  );
		
		toggleSpawnMenu.AddKeyBind( KeyCode.KC_M    , KeyMouseBinding.KB_EVENT_RELEASE );
		
		RegisterKeyMouseBinding( toggleSpawnMenu );
	}

    void SpawnBot()
    {
        Print("BotTest::SpawnBot");

		// if ( m_Bot ) return;
		CustomBot bot = NULL;

		#ifdef MODULE_PERSISTENCY
		PersistencyModule pm = GetModuleManager().GetModuleByName("PersistencyModule");

		if ( pm )
		{
			customPlayer = CharacterLoad.LoadPlayer( pm.GetLoadedCharacter(), pm.GetLoadedSave(), true, new PlayerIdentity );
			spawned = true;
		}
		#endif


		customPlayer.GetInputController().SetDisabled( false );

		customPlayer.OnSelectPlayer();

		customPlayer.SetPosition( GetCursorPos() );

		customPlayer.SetHealth( "","", 0 );

        bot = new CustomBot( customPlayer );
        bot.Start(false);

		m_Bot = bot;
    }
}