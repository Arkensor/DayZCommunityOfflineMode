class CustomBotHunt extends BotStateBase
{
	Man m_Target;
	float m_dtAccumulator = 0.0;

	ref CustomBotHunt_Tracking m_Tracking;
	ref CustomBotHunt_Hunting m_Hunting;

	void CustomBotHunt (Bot bot = NULL, BotStateBase parent = NULL)
	{
		// setup nested state machine
		m_FSM = new BotFSM(this); // @NOTE: set owner of the submachine fsm

		m_Tracking = new CustomBotHunt_Tracking(m_Bot, this);
		m_Hunting = new CustomBotHunt_Hunting(m_Bot, this);
		
		// events
		BotEventBase __InSight__ = new BotEventHuntedTargetInSight;
		BotEventBase __Lost__ = new BotEventHuntedTargetLost;

		// transitions
		m_FSM.AddTransition(new BotTransition(   m_Tracking, __InSight__, m_Hunting));
		//m_FSM.AddTransition(new BotTransition(   m_Hunting,  __Lost__   , m_Tracking));

		m_FSM.SetInitialState(m_Tracking);
	}

    void SetTarget(Man man)
    {
        m_Target = man;
		m_Tracking.m_Target = m_Target;
		m_Hunting.m_Target = m_Target;
		botDebugPrint("[bot] hunt SelectTarget target=" + man);
    }

	void SelectTarget ()
	{
		m_Target = BotSelectNearestTarget(GetPlayerOwner());
        SetTarget(m_Target);
	}

	override void OnEntry (BotEventBase e)
	{
		m_dtAccumulator = 0.0;
		SelectTarget();

		super.OnEntry(e);
	}

	override void OnExit (BotEventBase e)
	{
		m_dtAccumulator = 0.0;
		SetTarget( null );

		super.OnExit(e);
	}

	override void OnUpdate (float dt)
	{
		super.OnUpdate(dt);

		m_dtAccumulator += dt;
		
		/*float rescanTime = 3.0;
		if (m_dtAccumulator >= rescanTime)
			if (m_weapon.CanProcessWeaponEvents())
				m_Bot.ProcessEvent(new WeaponEventReloadTimeout(p));*/

		if (m_Target == null)
		{
			int acc = m_dtAccumulator;
			if (acc % 5 == 0)
			{
				Print("Searching...");
				// SelectTarget();
			}
		}
	}
};

class CustomBotHunt_Tracking extends BotStateBase
{
	EntityAI m_Target;
	bool m_TargetInSight = false;
	bool m_TargetLost = false;
	bool m_Tracking = true;

	override void OnEntry (BotEventBase e)
	{
		super.OnEntry(e);

		m_TargetLost = false;
		m_TargetInSight = false;
		m_Tracking = false;
	}

	override void OnAbort (BotEventBase e)
	{
		m_TargetLost = false;
		m_TargetInSight = false;
		m_Tracking = false;

		super.OnAbort(e);
	}

	override void OnExit (BotEventBase e)
	{
		m_TargetLost = false;
		m_TargetInSight = false;
		m_Tracking = false;

		super.OnExit(e);
	}

	float previousAngle = 0;

	override void OnUpdate (float dt)
	{
		if (m_Target)
		{
			m_Tracking = true;
			vector targetPos = m_Target.GetPosition();
			botDebugPrint("[bot] hunt Tracking target=" + m_Target + " pos=" + targetPos);

			float d = vector.Distance(m_Target.GetPosition(), GetPlayerOwner().GetPosition());
			if (d < 0.1)
			{
				m_TargetInSight = true;
			}
			else
			{
				m_TargetInSight = false;
			}

            //float angle = vector.Dot(, );

			vector a = m_Target.GetPosition();
			vector b = GetPlayerOwner().GetPosition();

			float angle = 0; //Math.Atan2(b[1], b[0]) - Math.Atan2(a[1], a[0]);

			angle = Math.Atan2( a[0] * b[2] - a[2]*b[0], a[0]*b[0] + a[2]*b[2] );

            angle = angle * Math.RAD2DEG;

			// angle = 180 - angle;

			// angle = angle - previousAngle;

/*
			if (!m_TargetInSight)
			{
				GetPlayerOwner().GetInputController().OverrideMovementSpeed(true, 2);
                GetPlayerOwner().GetInputController().OverrideAimChangeX(true, angle);
            }
			else
			{
			*/
			    GetPlayerOwner().GetInputController().OverrideMovementSpeed(false, 0);
			    GetPlayerOwner().GetInputController().OverrideAimChangeX(false, 0);
			//}

			GetPlayer().MessageStatus( "Angle: " + Math.Round(angle) + " Full: " + angle + " Pos1 " + a + " Pos2 " + b  );

			previousAngle = angle;
		}
		else
		{
			if (m_Tracking)
			{
				m_TargetLost = true;
				m_TargetInSight = false;
				m_Tracking = false;

				GetPlayerOwner().GetInputController().OverrideMovementSpeed(false, 0);
				GetPlayerOwner().GetInputController().OverrideAimChangeX(false, 0);
			}
		}
	}
};

class CustomBotHunt_Hunting extends BotStateBase
{
	EntityAI m_Target;

	override void OnEntry (BotEventBase e)
	{
		super.OnEntry(e);
	}

	override void OnAbort (BotEventBase e) { super.OnAbort(e); }

	override void OnExit (BotEventBase e)
	{
		super.OnExit(e);
	}

	override void OnUpdate (float dt)
	{
	}
};

