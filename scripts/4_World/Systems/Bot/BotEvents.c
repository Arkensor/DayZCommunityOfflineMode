/**@class	BotEventBase
 * @brief	represents event that triggers transition from state to state
 **/
class BotEventBase
{
	PlayerBase m_Player;

	void BotEventBase (PlayerBase p = NULL) { m_Player = p; }
};

//class BotEventXXX : BotEventBase { void BotEventXXX (PlayerBase p = NULL) { } };


class BotEventTestStart : BotEventBase { };
class BotEventTestStop : BotEventBase { };

class BotEventTestEndOK : BotEventBase { };
class BotEventTestEndFail : BotEventBase { };
class BotEventTestEndTimeout : BotEventBase { };

