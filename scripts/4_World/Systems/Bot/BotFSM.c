/**@class		BotFSM
 * @brief		Bot Finite State Machine (Hierarchical)
 **/
class BotFSM extends HFSMBase<BotStateBase, BotEventBase, BotActionBase, BotGuardBase>
{ };

typedef FSMTransition<BotStateBase, BotEventBase, BotActionBase, BotGuardBase> BotTransition; /// shorthand


