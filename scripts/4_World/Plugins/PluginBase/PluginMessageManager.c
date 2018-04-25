/*! Class PluginMessageManager provides some basic Message Distribution mechanics, if you get instance of this plugin class on your object, 
you can use its methods to broadcast messages.
*/
class PluginMessageManager extends PluginBase
{
	int channelsUsed = 0;
	ref array<ref MessageReceiverBase>	channelList[NUM_OF_CHANNELS];
	
	void PluginMessageManager()
	{
		for (int i = 0; i < NUM_OF_CHANNELS; i++)
		{
			channelList[i] = new array<ref MessageReceiverBase>;
		}
	}

	//! Broadcasts an empty message on a channel provided in the 'index' parameter
	void Broadcast(int index)
	{
		//Debug.Log("Broadcasting message on a channel: "+ ToString(index), "Messaging System");
		array<ref MessageReceiverBase> x = channelList[index];
		//int test = channelList[index].Count();
		for(int i = 0; i < x.Count(); i++)
		{
			MessageReceiverBase mrb = x.Get(i);
			
			if(	mrb != NULL )
			{
				//string s = "Broadcasting message to: "+ToString(mrb);
				//Log(s, LogTemplates.TEMPLATE_BROADCAST);
				mrb.OnReceive(index);
			}
		}
	}

	//! Broadcasts a message on a channel provided in the 'index' parameter, passes the Int variable
	void BroadcastInt(int index, int value)
	{
		//Debug.Log("Broadcasting message on a channel: "+ ToString(index), "Messaging System");
		array<ref MessageReceiverBase> x = channelList[index];
		//int test = channelList[index].Count();
		for(int i = 0; i < x.Count(); i++)
		{
			MessageReceiverBase mrb = x.Get(i);
			
			if(	mrb )
			{
				//string s = "Broadcasting message to: "+ToString(mrb);
				//Log(s, LogTemplates.TEMPLATE_BROADCAST);
				mrb.OnReceiveInt(index, value);
			}
			
		}
	}

	void BroadcastFloat(int index, float value)
	{
		//Debug.Log("Broadcasting message on a channel: "+ ToString(index), "Messaging System");
		array<ref MessageReceiverBase> x = channelList[index];
		//int test = channelList[index].Count();
		for(int i = 0; i < x.Count(); i++)
		{
			MessageReceiverBase mrb = x.Get(i);
			
			if(	mrb )
			{
				//string s = "Broadcasting message to: "+ToString(mrb);
				//Log(s, LogTemplates.TEMPLATE_BROADCAST);
				mrb.OnReceiveFloat(index, value);
			}
			
		}
	}

	void BroadcastString(int index, string value)
	{
		//Debug.Log("Broadcasting message on a channel: "+ ToString(index), "Messaging System");
		array<ref MessageReceiverBase> x = channelList[index];
		//int test = channelList[index].Count();
		for(int i = 0; i < x.Count(); i++)
		{
			if(	x.Get(i) )
			{
				Print("broadcasting message to: ");
				Print(x.Get(i));
				x.Get(i).OnReceiveString(index, value);
			}
		}
	}

	//! Broadcasts a message on a channel provided in the 'index' parameter, passes the Param type object to the reciever
	void BroadcastParam(int index, Param params)
	{
		//Debug.Log("Broadcasting message on a channel: "+ ToString(index), "Messaging System");
		array<ref MessageReceiverBase> x = channelList[index];
		//int test = channelList[index].Count();
		for(int i = 0; i < x.Count(); i++)
		{
		
			if(	x.Get(i) )
			{
				x.Get(i).OnReceiveParam(index, params);
			}
			
		}
	}
	
	//! Subscribes an object to listen to messages on a channel provided in the 'index' parameter
	void Subscribe(MessageReceiverBase receiver, int index)
	{
		if(index > channelsUsed) //this is used to speed up the unsubscribeAll method, instead of all channels, we sweep just those in usage
		{
			channelsUsed = index;
		}
		array<ref MessageReceiverBase> chan = channelList[index];
		if( chan.Find(receiver) >= 0 ) return;
		chan.Insert(receiver);
	}

	void Unsubscribe(MessageReceiverBase receiver, int index)
	{
		array<ref MessageReceiverBase> chan = channelList[index];
		int i = chan.Find(receiver);
		if( i >= 0 )
		{
			chan.Remove(i);
		}
	}
	
	void UnsubscribeAll(MessageReceiverBase receiver)//REWORK.V: this is slow, should be made quicker(by registering all subscribers in a separate array upon their subscription and then going through this array instead)
	{
		//GetGame().ProfilerStart("UnsubscribeAll");
		for (int i = 0; i <= channelsUsed; i++)
		{
			array<ref MessageReceiverBase> chan = channelList[i];
			int c = chan.Find(receiver);
			if( c >= 0 )
			{
				chan.Remove(c);
			}
		}
		//GetGame().ProfilerStop("UnsubscribeAll");
	}
}
