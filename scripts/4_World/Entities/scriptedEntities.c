#ifdef DEVELOPER
const bool DebugTrigger = true;
#else
const bool DebugTrigger = false;
#endif

class TriggerInsider
{
	ref OLinkT insider;
	int timeStamp;
	void TriggerInsider(Object obj)
	{
		insider = new OLinkT(obj);
	}

	Object GetObject()
	{
		return insider.Ptr();
	}
};

class Trigger extends ScriptedEntity
{
	const int TIMEOUT = 1000;
	ref array<ref TriggerInsider> m_insiders;
	Shape shape;

	protected void Cleanup()
	{
		if(shape)
		{
			shape.Destroy();
			shape = NULL;
		}			
	}

	void OnEnter(Object obj)
	{
		if(DebugTrigger)
		{		
			Print("entered");
			Print(obj);
		}
	}

	void OnLeave(Object obj)
	{
		if(DebugTrigger)
		{		
			Print("left");
			Print(obj);
		}
	}
	
	protected void UpdateInsiders(int timeout)
	{
		int timeStamp = g_Game.GetTime();

		for(int n = 0; n < m_insiders.Count(); )
		{
			TriggerInsider ins = m_insiders.Get(n);
			if(ins.insider == NULL)
			{
				//object has been deleted. Remove it
				m_insiders.Remove(n);
				continue;
			}
			
			int timeDiff = timeStamp - ins.timeStamp;
			if(timeDiff > timeout)
			{
				//object left. Remove it
				OnLeave(ins.insider.Ptr());
				m_insiders.Remove(n);
				continue;
			}
			 
			n++;			
		}		
	}

	protected void AddInsider(Object obj)
	{
		TriggerInsider ins;
		for(int n = 0; n < m_insiders.Count(); n++)
		{
			ins = m_insiders.Get(n);
			//already in?
			if(ins.insider.Ptr() == obj)
			{
				//just update timestamp
				ins.timeStamp = g_Game.GetTime();
				return;
			}
		}
		ins = new TriggerInsider(obj);
		ins.timeStamp = g_Game.GetTime();
		m_insiders.Insert(ins);
		OnEnter(obj);  				
	}
	
	override void EOnFrame(IEntity other, float timeSlice)
	{
		UpdateInsiders(TIMEOUT);
	}
	
	override void EOnTouch(IEntity other, int extra)
	{
		Object obj;
		if (Class.CastTo(obj, other))
			AddInsider(obj);
	}

	private float GetRadius(vector min, vector max)
	{
		vector v;
		for(int n = 0; n < 3; n++)
		{
			float m1 = Math.AbsFloat(min[n]);
			float m2 = Math.AbsFloat(max[n]);

			if(m1 > m2)
				v[n] = m1;
			else
				v[n] = m2;			
		}
		
		return v.Length();			
	}
	
	void SetExtents(vector mins, vector maxs)
	{
		SetCollisionBox(mins, maxs);

		if(DebugTrigger)
		{
			vector mat[4];
			GetTransform(mat);
			if(shape)
			{
				shape.Destroy();
				shape = NULL;
			}
			
			shape = Shape.Create(ShapeType.BBOX, 0x1fff7f7f, ShapeFlags.TRANSP|ShapeFlags.NOZWRITE, mins, maxs);
			
			if (shape) // Hackfix! Function AddDShape must return Shape! That's why this condition is probably never passed!
			{
				shape.SetMatrix(mat);
			}
		}
	}


	override void EOnInit(IEntity other, int extra)
	{
		const vector mins = "-2 -4 -2";
		const vector maxs = "2 4 2";
		SetExtents(mins, maxs);
	}

	private void ~Trigger()
	{
		Cleanup();
	}
			
	private void Trigger()
	{
		m_insiders = new array<ref TriggerInsider>;
		SetEventMask(EntityEvent.FRAME|EntityEvent.INIT|EntityEvent.TOUCH);
		SetFlags(EntityFlags.TRIGGER, false);
	}
};

class ManTrigger extends Trigger
{
	override void OnEnter(Object obj)
	{
	}

	override void EOnTouch(IEntity other, int extra)
	{
		Object obj;
		if(Class.CastTo(obj, other))
		{
			if(obj.IsMan()) AddInsider(obj);
		}
	}

	array<ref TriggerInsider> GetInsiders()
	{
		return m_insiders;
	}

	override void EOnInit(IEntity other, int extra)
	{
		const vector mins = "-2 -2 -2";
		const vector maxs = "2 2 2";
		SetExtents(mins, maxs);
	}
	
};