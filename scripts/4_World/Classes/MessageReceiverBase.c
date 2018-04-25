class MessageReceiverBase
{
	// message system
	PluginMessageManager m_ModuleMessageManager;
	string m_System;

	void OnReceive(int channel) {}
	void OnReceiveInt(int channel, int value) {}
	void OnReceiveFloat(int channel, float value) {}
	void OnReceiveString(int channel, string value) {}
	//! This method is called when an object inheriting from this class subscribes to a channel, and a message along with an object reference is broadcast on that channel, if the object was created just for the purpose of relaying information and is not referenced anywhere else, it should be deleted after processing inside this method
	void OnReceiveParam(int channel, Param params) {}
	
	void MessageReceiverBase()
	{
		m_ModuleMessageManager = PluginMessageManager.Cast(GetPlugin(PluginMessageManager));
	}

	void Subscribe(int channel)
	{
		m_ModuleMessageManager.Subscribe(this,channel);
	}

	void Unsubscribe(int channel)
	{
		if( IsPluginManagerExists() ) m_ModuleMessageManager.Unsubscribe(this,channel);
	}

	void UnsubscribeAll()
	{
		if( IsPluginManagerExists() ) m_ModuleMessageManager.UnsubscribeAll(this);
	}

	void Broadcast(int channel)
	{
		m_ModuleMessageManager.Broadcast(channel);
	}

	void BroadcastInt(int channel, int value)
	{
		m_ModuleMessageManager.BroadcastInt(channel, value);
	}

	void BroadcastFloat(int channel, float value)
	{
		m_ModuleMessageManager.BroadcastFloat(channel, value);
	}

	void BroadcastString(int channel, string value)
	{
		m_ModuleMessageManager.BroadcastString(channel, value);
	}

	void BroadcastParam(int channel,Param params)
	{
		m_ModuleMessageManager.BroadcastParam(channel, params);
	}

	//Messaging system
	void Log(string message)
	{
		Debug.Log(message,m_System);
	}

}