/** @file 

  this file is interface to AI Behaviour

*/

/*
class AIMessage: Managed

{
	Param m_Parameters;
	int m_CrcMessage;
	Param GetParameters();
	int GetTypeNameCRC();
}

class AIMessageTest : AIMessage
{
	int GetTypeNameCRC() {return testCRC;}
	//static int testCRC = 5;	
}

*/


class AIBehaviourHLData
{
	//void ParseConfig(ParamEntryPar param, AIWorld* world, AIAgentTemplate* agentTemplate) {};
	void OnParseConfig();	
	
	//Can be use only inside OnParseConfig function
	proto native void ParseBehaviourSlot(string name);
	proto native void ParseAlertLevel(string name);
	proto native float ReadParamValue(string paramName, float defValue);
	
}


class AIBehaviourHL
{
	void AIBehaviourHL()
	{
		//Print("A1");
	}
	
	proto native AIBehaviourHLData GetTemplateData();
	
	void OnInit(){};
	//private void AIBehaviourHL_script() {};
	//private void ~AIBehaviourHL_script() {};

	void Simulate (float timeDelta)
	{
		//time = time + timeDelta; 
		//Print("A2");
	}
	
	
	//void OnNoise(AINoiseInfo noise, EntityAI source){}
	void OnDamage(float damage, EntityAI source){}

	void OnDamageInflicted(){}
	void OnAnimationEvent(int nameCrc){}
	//void OnMessage(AIMessage message){}
	//void OnBehaviourSwitched(AIBehaviour* behaviour){}
	proto native void SetNextBehaviour(int BehaviourCrc);
	proto native void SwitchToNextBehaviour();
	
	static proto void RegAIBehaviour(string behname,typename behClass, typename behClassData);
};


class AIBehaviourHLZombie2 : AIBehaviourHL
{
	//AIBehaviourHLDataZombie2 data;
	//int state;
	//float time;
	AIBehaviourHLDataZombie2 GetData()
	{
		return AIBehaviourHLDataZombie2.Cast( GetTemplateData() );
	}
	void AIBehaviourHLZombie2()
	{

		//data = GetTemplateData();
		//int i = 5;
		//i++;
		//time = 0.0;
		//Print("B1");
	}
	
	override void OnInit()
	{
		SetNextBehaviour(0x7b9a4ee9);
		SwitchToNextBehaviour();
		//Print("OnInit");
		//data = GetData();
		//float a = data.m_fDamageToCrawl;
		//Print(a);
	}
	
	override void Simulate (float timeDelta)
	{
		//time = time + timeDelta; 
		//Print("B2");
	}
	
	void ShowDebugInfo() 
	{
		//Print(time);
	};
	
	//void OnMessage(AIMessage message)
	//{
		//int ID = message.GetTypeNameCRC();
		//Print(ID);
		
	//}
};

class AIBehaviourHLDataZombie2 : AIBehaviourHLData
{
	float m_fDamageToCrawl;
	float m_fCrawlProbability;
	
	override void OnParseConfig()
	{
		Print("zombie data parse config start");
		
		ParseBehaviourSlot("Calm");
		ParseBehaviourSlot("Attracted");
		ParseBehaviourSlot("Disturbed");
		ParseBehaviourSlot("Alerted");
		
		ParseAlertLevel("Calm");
		ParseAlertLevel("Disturbed");
		ParseAlertLevel("Attracted");
		ParseAlertLevel("Alerted");
		
		m_fDamageToCrawl = ReadParamValue("damageToCrawl",0.01);
		m_fCrawlProbability = ReadParamValue("crawlProbability",0.01);
		
		Print("zombie data parse config end");
	}
};

//NewNPC(model,behname,....)


