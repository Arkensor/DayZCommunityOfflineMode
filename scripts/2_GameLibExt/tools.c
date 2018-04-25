/**
 \brief ScriptCallQueue Class provide "lazy" calls - when we don't want to execute function immediately but later during frame update (used mainly in UI)
 \n usage:
 @code
	class Arkanoid extends Game
	{
		ref ScriptCallQueue m_CallQueue = new ScriptCallQueue();
	
		ScriptCallQueue GetCallQueue() { 
			return m_CallQueue;
		}
	
		override void OnUpdate(float timeslice)
		{
			m_CallQueue.Tick(timeslice);
			...
		}
		...
	}

	class MyObject
	{
		int m_cnt = 0;
	
		void Hello(int p1, string p2)
		{
			Print("Hello( " + p1 + " , " + p2 + ")");
		}
	
		void Test()
		{
			Print(m_cnt);
			m_cnt++;
			
			if (m_cnt > 10)
			{
				ScriptCallQueue queue = GetGame().GetCallQueue();
				queue.Remove(Test);
			}
		}
	}

	void Test(MyObject obj)
	{
		ScriptCallQueue queue = GetGame().GetCallQueue(); 
		queue.CallLater(obj.Hello, 5000, false, 65, "world"); // adds call 'obj.Hello(65, "world")' into queue, and it will be executed once after 5s
		queue.CallLater(obj.Test, 3000, true); // adds call 'obj.Test()' into queue, and it will be executed each 3s
		queue.Call(obj.Hello, 72, "world 2"); // adds call 'obj.Hello(72, "world 2")' into queue, and it will be executed next frame (on next call of ScriptCallQueue.Tick)
	}
 @endcode
 */
class ScriptCallQueue
{
	//! executes calls on queue if their time is already elapsed, if 'repeat = false' call is removed from queue
	proto native void Tick(float timeslice);
	//! adds call into the queue with given parameters and arguments (arguments are holded in memory until the call is executed/removed or ScriptCallQueue is destroyed)
	proto void Call(func fn, void param1 = NULL, void param2 = NULL, void param3 = NULL, void param4 = NULL, void param5 = NULL, void param6 = NULL, void param7 = NULL, void param8 = NULL, void param9 = NULL);
	//! adds call into the queue with given parameters and arguments (arguments are holded in memory until the call is executed/removed or ScriptCallQueue is destroyed)
	proto void CallByName(Class obj, string fnName , Param params = NULL);
	//! adds call into the queue with given parameters and arguments (arguments are holded in memory until the call is executed/removed or ScriptCallQueue is destroyed)
	proto void CallLater(func fn, int delay = 0, bool repeat = false, void param1 = NULL, void param2 = NULL, void param3 = NULL, void param4 = NULL, void param5 = NULL, void param6 = NULL, void param7 = NULL, void param8 = NULL, void param9 = NULL);	
	//! adds call into the queue with given parameters and arguments (arguments are holded in memory until the call is executed/removed or ScriptCallQueue is destroyed)
	proto void CallLaterByName(Class obj, string fnName, int delay = 0, bool repeat = false, Param params = NULL);	
	//! remove specific call from queue	
	proto void Remove(func fn);
	//! return Remaining time to the call execution (in miliseconds)
	proto int GetRemainingTime(func fn);
	//! remove specific call from queue	
	proto void RemoveByName(Class obj, string fnName);
	//! return Remaining time to the call execution (in miliseconds)
	proto int GetRemainingTimeByName(Class obj, string fnName);
	//! remove all calls from queue
	proto native void Clear();
};

/**
 \brief ScriptInvoker Class provide list of callbacks
 \n usage:
 @code
	class Player
	{
		ref ScriptInvoker m_DeathInvoker	= new ScriptInvoker();
	
		void OnKilled()
		{
			m_DeathInvoker.Invoke(this);		
		}
	}

	void LogPlayerDeath(p)
	{
		Print("RIP " + p);
	}		

	class Game
	{
		void RemovePlayer(Player p)
		{
		}

		void GameOver()
		{
		}
	}

	void OnPlayerSpaned(Player p)
	{
		Game game = GetGame();
		p.m_DeathInvoker.Insert(LogPlayerDeath);
		p.m_DeathInvoker.Insert(game.RemovePlayer);
		p.m_DeathInvoker.Insert(game.GameOver);
	}
 @endcode
 */
class ScriptInvoker
{
	//! invoke call on all inserted methods with given arguments
	proto void Invoke(void param1 = NULL, void param2 = NULL, void param3 = NULL, void param4 = NULL, void param5 = NULL, void param6 = NULL, void param7 = NULL, void param8 = NULL, void param9 = NULL);
	//! insert method to list	
	proto void Insert(func fn);	
	//! remove specific call from list
	proto void Remove(func fn);
	//! remove all calls from list
	proto native void Clear();
};

