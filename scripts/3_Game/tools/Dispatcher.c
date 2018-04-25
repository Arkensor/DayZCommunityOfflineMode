typedef int CallID;

const CallID CALL_ID_SEND_LOG							= 0;
const CallID CALL_ID_SCR_CNSL_HISTORY_BACK				= 1;
const CallID CALL_ID_SCR_CNSL_HISTORY_NEXT				= 2;
const CallID CALL_ID_SCR_CNSL_ADD_PRINT					= 3;
const CallID CALL_ID_SCR_CNSL_GET_SQF_WATCHER			= 4;
const CallID CALL_ID_SCR_CNSL_SET_SQF_WATCHER_RESULT	= 5;
const CallID CALL_ID_SCENE_EDITOR_COMMAND				= 6;
const CallID CALL_ID_HIDE_INVENTORY						= 7;

class Dispatcher
{	
	Param CallMethod(CallID call_id, Param params){}
}

ref Dispatcher g_Dispatcher;

Dispatcher GetDispatcher()
{
	if ( g_Dispatcher == NULL )
	{
		//Print("[Dispatcher.h]=>GetDispatcher(): g_Dispatcher is NULL!!");
		return NULL;
	}
	
	return g_Dispatcher;
}

void SetDispatcher(Dispatcher dispatcher)
{
	g_Dispatcher = dispatcher;
}

Param CallMethod(CallID call_id, Param params)
{
	Dispatcher dis = GetDispatcher();
	if ( dis )
	{
		return dis.CallMethod(call_id, params);
	}
	
	return NULL;
}