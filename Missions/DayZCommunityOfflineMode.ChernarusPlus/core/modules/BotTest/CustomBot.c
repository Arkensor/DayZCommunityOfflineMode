/**@class		Bot
 **/
class CustomBot extends Bot
{
    ref CustomBotHunt oBotHunt;
    
    void CustomBot (PlayerBase ow)
	{
		m_Owner = ow;
	}

	override void InitFSM ()
	{
		m_FSM = new BotFSM();

		// basic states
		BotStateBase BotIdle = new BotStateIdle(this, NULL);
        oBotHunt = new CustomBotHunt(this, NULL);
		// unstable (intermediate) states
		//m_BotTest = new BotTestAttachAndDropCycle(this, NULL);
		//m_BotTest = new BotTestSwapInternalC2H(this, NULL);
		//m_BotTest = new BotTestSwapG2H(this, NULL);
		//m_BotTest = new BotTestSwapC2H(this, NULL);
		//m_BotTest = new BotTestSwapInternal(this, NULL);

		// events
		BotEventBase __Start__ = new BotEventTestStart;
		BotEventBase __Stop__ = new BotEventTestStop;
		BotEventBase __OK__ = new BotEventTestEndOK;
		BotEventBase __Fail__ = new BotEventTestEndFail;
		BotEventBase __Tout__ = new BotEventTestEndTimeout;

		///@{ transition table
		//m_FSM.AddTransition(new BotTransition(  BotIdle     , __Start__,   BotIdle));
		//m_FSM.AddTransition(new BotTransition(  BotIdle     , __Stop__,      BotIdle));

		m_FSM.AddTransition(new BotTransition(  oBotHunt     , __Start__,   oBotHunt));
		m_FSM.AddTransition(new BotTransition(  oBotHunt     , __Stop__,      oBotHunt));

		//m_FSM.AddTransition(new BotTransition(m_BotTest     , __OK__,        BotIdle));
		//m_FSM.AddTransition(new BotTransition(m_BotTest     , __Fail__,      BotIdle));
		//m_FSM.AddTransition(new BotTransition(m_BotTest     , __Tout__,      BotIdle));
		///@} transition table

		m_FSM.SetInitialState(oBotHunt);
		m_FSM.Start();
	}

	override void OnUpdate ()
	{
		m_FSM.GetCurrentState().OnUpdate(c_UpdateMS / 1000.0);

        // oBotHunt.SetTarget(m_Target);

		// OnTrigger(); // to detect trigger stop
	}
};