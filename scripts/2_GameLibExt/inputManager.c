#ifdef GAME_TEMPLATE

enum InputTrigger
{
	UP, 			///< call listener when button/key is released
	DOWN,		 	///< call listener when button/key is pressed
	PRESSED, 		///< call listener in each tick when button/key is pressed
	VALUE		 	///< call listener in each tick with current value
};

class ActionManager
{
	void ActionManager(ActionManager parent);
	proto native external bool RegisterAction(string actionName);
	proto native external bool RegisterContext(string contextName);
	
	proto native external float GetActionValue(string actionName);
	proto native external bool GetActionTriggered(string actionName);
	
	proto native external bool ActivateAction(string actionName, int duration = 0);	
	proto native external bool IsActionActive(string actionName);
	
	proto native external bool ActivateContext(string contextName, int duration = 0);	
	proto native external bool IsContextActive(string contextName);
	
	proto external void AddActionListener(string actionName, InputTrigger trigger,  func callback);
	
	proto native external void SetContextDebug(string contextName, bool bDebug);
	
	proto native void SetParent(ActionManager parent);
	proto native void SetDebug(bool bDebug);
};

class InputManager: ActionManager
{
	private void InputManager(ActionManager parent) {};
	private void ~InputManager() {};
	
	proto native external void ResetAction(string actionName);
	proto native void SetCursorPosition(int x, int y);
	proto native external bool RegisterActionManager(ActionManager pManager);
	proto native external bool UnregisterActionManager(ActionManager pManager);
};

#endif

