/**
 * \defgroup Tools
 * \desc Helpful functions & classes
 * @{
 */

//--------------------------------------------------------------------------
const int CALL_CATEGORY_SYSTEM = 0;
const int CALL_CATEGORY_GUI = 1;
const int CALL_CATEGORY_GAMEPLAY = 2;

const int CALL_CATEGORY_COUNT = 3;

// -------------------------------------------------------------------------
class CallQueueContext
{
	Class m_target;
	string m_function;
	ref Param m_params;
	bool m_valid;

	void CallQueueContext(Class target, string fn, Param params)
	{
		m_target = target;
		m_function = fn;
		m_params = params;
		m_valid = true;
	}
		
	void Call()
	{
		CallParams(m_params);
	}
		
	void CallParams(Param params)
	{
		if (params)
		{
			GetGame().GameScript.CallFunctionParams(m_target, m_function, NULL, params);
		}
		else
		{
			GetGame().GameScript.CallFunction(m_target, m_function, NULL, 0);
		}
	}
	
	void Invalidate() {
		m_valid = false;
	}

	bool IsValid(){
		return m_valid;
	}
};

//--------------------------------------------------------------------------
/**
 \brief CallQueue Class provide "lazy" calls - when we don't want to execute function immediately but later during frame update (used mainly in UI)
 \n usage:
 @code
 GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this, "Refresh"); // calls "Refresh" function on "this" with no arguments
 GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this, "Show", new Param1<bool>(true)); // calls "Show" function on "this" with one bool argument
 GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this, "SetPos", new Param2<float, float>(0.2, 0.5)); // calls "SetPos" function on "this" with two float arguments
 @endcode
 */
class CallQueue extends array<ref CallQueueContext>
{
	private bool m_processing;

	void CallQueue()
	{
		m_processing = false;
	}
		
	/**
	\brief System function, don't call it
	*/
	void Tick()
	{
		if (m_processing) return;
	
		m_processing = true;
	
		while(Count() > 0)
		{
			CallQueueContext ctx = Get(0);
			if (!ctx.IsValid())
			{
				Remove(0);
			}
			else
			{
				Remove(0);
				ctx.Call();
			}
		}
	
		m_processing = false;
	}

	/**
  \brief Creates new call request, add it on queue and execute during frame update (depends on call category)
  @param obj target object on which function will be executed
	@param fn_name name of function (on object "obj") which will be executed
	@param params function arguments see Param for usage, default NULL (no arguments)
	\warning When object "obj" is deleted prior call execution, don't forget to remove calls for this object with RemoveCalls
	*/
	void Call(Class obj, string fn_name, Param params = NULL)
	{
		Insert(new CallQueueContext(obj, fn_name, params));
	}


	/**
  \brief Removes all queued calls for object (call this function when object is going to be deleted)
  @param obj object for which you want remove all "lazy" calls
	*/
	void RemoveCalls(Class obj)
	{
		if (Count())
		{
			for (int i = Count() - 1; i >= 0; i--)
			{
				CallQueueContext ctx = Get(i);
				if (ctx.m_target == obj)
				{
					ctx.Invalidate();
				}
			}
		}
	}
};

//--------------------------------------------------------------------------
/**
 \brief DragQueue Class provide callbacks while mouse is dragging. Callback function must have exact arguments: 
 @code
 void AnyFunctionName(int mouse_x, int mouse_y, bool is_dragging);
 @endcode
 When mouse button is released, callback function is called one more time with is_dragging = false. Then all callbacks are automatically removed from queue.

 \n usage:
 @code
 class XYZ
 {
	void UpdateDrag(int mouse_x, int mouse_y, bool is_dragging);
 }
...
 GetGame().GetDragQueue().Call(this, "UpdateDrag"); // calls "UpdateDrag" function on "this" 

 @endcode
 */
class DragQueue extends CallQueue
{
	private ref Param3<int, int, bool> m_mouse_params; // x,y, is_holding_mb
	
	void DragQueue()
	{
		m_mouse_params = new Param3<int, int, bool>(0,0,true);
	}

	/**
	\brief System function, don't call it
	*/
	override void Tick()
	{
		if (m_processing) return;
	
		m_processing = true;
	
		int last_index = 0;
		int mouse_x;
		int mouse_y;
		bool is_holding = false;
		CallQueueContext ctx;
	
		if (GetMouseState(MouseState.LEFT) & 0x80000000)
		{
			is_holding = true;
		}
	
		GetMousePos(mouse_x, mouse_y);
	
		if (!is_holding || mouse_x != m_mouse_params.param1 || mouse_y != m_mouse_params.param2)
		{
			m_mouse_params.param1 = mouse_x;
			m_mouse_params.param2 = mouse_y;
			m_mouse_params.param3 = is_holding;
	
			while (Count() > last_index)
			{
				ctx = Get(last_index);
				if (!ctx.IsValid())
				{
					Remove(last_index);
				}
				else
				{
					ctx.CallParams(m_mouse_params);
					last_index++;
				}
			}
		}
	
		// clear queue when mouse button is released
		if (!is_holding)
		{
			Clear();
		}
	
		m_processing = false;
	}
}

//--------------------------------------------------------------------------
/**
 \brief TimerBase Class provide just interface for all Timer classes. Don't instance this class, use Timer class instead.
 */
class TimerBase: Managed
{
	protected bool m_running;
	protected bool m_loop;
	protected float m_duration;
	protected float m_time;
	protected array<TimerBase> m_timerQueue;

	void ~TimerBase()
	{
		if (!m_timerQueue) return;
	
		SetRunning(false);
	}


	/**
  \brief Pause Timer, internal counter is not restarted, so timer can continue later. Can be unpaused via Continue.
	*/
	void Pause()
	{
		SetRunning(false);
	}

	/**
  \brief Timer continue when it was paused.
	*/
	void Continue()
	{
		SetRunning(true);
	}

	/**
  \brief Stop Timer and restart internal counter. Cannot be unpaused, must be started again.
	*/
	void Stop()
	{
		SetRunning(false);
		m_time = 0;
	}
		
	/**
  \return True when Timer is running (not stopped, not paused)
	*/
	bool IsRunning()
	{
		return m_running;
	}
		
	/**
  \brief System function, don't call.
	*/
	void Tick(float timeslice)
	{
		if (IsRunning())
		{
			m_time = m_time + timeslice;
	
			if (m_time >= m_duration)
			{
				if (m_loop)
				{
					m_time = m_time - m_duration;
				}
				else
				{
					SetRunning(false);
					m_time = 0;
				}
	
				OnTimer();			
			}
			else
			{
				OnUpdate();
			}
		}
	}

	/**
  \brief System function, don't call.
	*/
	void OnTimerQueueDestoryed()
	{
		m_timerQueue = NULL;
	}

	float GetTime() {
			return m_time;
	}
		
	float GetDuration() {
			return m_duration;
	}
		
	float GetRemaining() {
			return m_duration - m_time;
	}
		
	protected void OnInit(int category)
	{
		m_duration = 1;
		m_loop = false;
		m_time = 0;
		m_running = false;
		m_timerQueue = GetGame().GetTimerQueue(category);
	}
		
	protected void OnStart(float duration, bool loop)
	{
		m_duration = duration;
		m_loop = loop;
		m_time = 0;
		SetRunning(true);
	}
		
	protected void OnUpdate()	{}
	protected void OnTimer() {}
	protected void SetRunning(bool running)
	{
		int index = -1;
	
		if (m_running == running) return;
	
			m_running = running;
		if (m_timerQueue == NULL) return;
		
		if (running)
		{
			index = m_timerQueue.Find(this);
			if (index == -1)
			{
				m_timerQueue.Insert(this);
			}
		}
		else
		{
			index = m_timerQueue.Find(this);
			if (index != -1)
			{
				m_timerQueue.Remove(index);
			}
		}
	}
};

//--------------------------------------------------------------------------
/**
 \brief TimerQueue Class using for system purpose only
 */
class TimerQueue extends array<TimerBase>
{
	private bool m_processing;

	// -------------------------------------------------------------------------
	void TimerQueue()
	{
		m_processing = false;
	}
	
	// -------------------------------------------------------------------------
	void ~TimerQueue()
	{
		if (Count())
		{
			for (int i = Count() - 1; i >= 0; i--)
			{
				Get(i).OnTimerQueueDestoryed();
			}
	
			Clear();
		}
	}
	
	// -------------------------------------------------------------------------
	void Tick(float timeslice)
	{
		if (m_processing) return;
	
		m_processing = true;
	
		if (Count())
		{
			for (int i = Count() - 1; i >= 0; i--)
			{
				Get(i).Tick(timeslice);
			}
		}
	
		m_processing = false;
	}
};

//--------------------------------------------------------------------------
/**
 \brief Simple class for fading Widgets
 */
class WidgetFadeTimer extends TimerBase
{
	private Widget m_widget;
	bool m_fadeIn;
	float m_alpha;

	void WidgetFadeTimer()
	{
		OnInit(CALL_CATEGORY_GUI);
		m_fadeIn = true;
	}

	/**
  \brief Make "fade in" effect on Widget (transparency goes from 0.0 to 1.0)
  @param w widget which will be faded
	@param time duration of effect
	@param continue - if True continue from current alpha value, otherwise always begin from 0.0 alpha
	*/
	void FadeIn(Widget w, float time, bool continue_ = false)
	{
		float alpha = w.GetAlpha();
		
		if (continue_ && alpha > 0.95)
		{
			w.SetAlpha(1.0);
			w.Show(true);
			return;
		}	
		
		m_widget = w;
		m_fadeIn = true;
	
		OnStart(time, false);
	
		if (m_widget)
		{
			alpha = m_widget.GetAlpha();
			m_widget.SetAlpha(0);
			m_widget.Show(true);
		}
	
		if (continue_)
		{
			m_time = alpha * time;
		}
	}

	/**
  \brief Make "fade out" effect on Widget (transparency goes from 1.0 to 0.0)
  @param w widget which will be faded
	@param time duration of effect
	@param continue - if True continue from current alpha value, otherwise always begin from 1.0 alpha
	*/
	void FadeOut(Widget w, float time, bool continue_ = false)
	{
		m_alpha = w.GetAlpha();
		
		if (continue_ && m_alpha < 0.05)
		{
			w.SetAlpha(0);
			w.Show(false);
			return;
		}	
		
		m_widget = w;
		m_fadeIn = false;
	
		OnStart(time, false);
		
		if (m_widget && !continue_)
		{
			m_alpha = 1.0;
			m_widget.SetAlpha(m_alpha);
			m_widget.Show(true);
		}	
	
		if (continue_)
		{
			m_time = (1.0 - m_alpha) * time;
		}
	}

	override private void OnTimer()
	{
		if (m_widget)
		{
			if (m_fadeIn)
			{
				m_widget.SetAlpha(1);
			}
			else
			{
				m_widget.SetAlpha(0);
				m_widget.Show(false);
			}
		}
	}

	override private void OnUpdate()
	{
		float timeDiff = m_duration - m_time;
		float progress = Math.Max( 0.0, ( 2.0 * m_alpha - timeDiff ) * 0.5 );
	
		if ( m_widget )
		{
			if ( m_fadeIn )
			{
				m_widget.SetAlpha( progress );
			}
			else
			{
				m_widget.SetAlpha( m_alpha - progress );
				Widget child = m_widget.GetChildren();
				while( child )
				{
					float alphaChild = child.GetAlpha();
					float progressChild = Math.Max( 0.0, ( 2.0 * alphaChild - timeDiff ) * 0.5 );
					child.SetAlpha( alphaChild - progressChild );
					child = child.GetChildren();
				}
			}
		}
	}
};

//--------------------------------------------------------------------------
/**
 \brief Timer class. Use when you want call function after some time, or repeatedly in time intervals. Call is not executed after the Timer object is deleted.
 \n usage:
 @code
 class MyObject
 {
	ref Timer myTimer1;
	ref Timer myTimer2;
	ref Timer myTimer3;

	void MyObject()
	{
		myTimer1 = new Timer();
		myTimer1.Run(10, this, "Refresh"); // calls "Refresh" on "this" after 10 seconds

		myTimer2 = new Timer();
		myTimer2.Run(10, this, "Refresh", NULL, true); // calls "Refresh" on "this" every 10 seconds, until Pause or Stop is called

		myTimer3 = new Timer();
		myTimer3.Run(15, this, "Show", new Param1<bool>(false)); // calls "Show" on "this" with one bool argument after 15 seconds
	}

	void Refresh();
	void Show(bool visible);
 }
 
 @endcode

 */
class Timer extends TimerBase
{
	protected Managed m_target;
	protected string m_function;
	protected ref Param m_params;

	void Timer(int category = CALL_CATEGORY_SYSTEM)
	{
		OnInit(category);
	}

	/**
  \brief Starts timer.
	@param duration function is executed after this time (in seconds).
	@param obj target object on which function will be executed
	@param fn_name name of function (on object "obj") which will be executed
	@param params function arguments see Param for usage, default NULL (no arguments)
	@param loop when true, timer is looped endlessly and function is executed after every loop.
	*/
	void Run(float duration, Managed obj, string fn_name, Param params = NULL, bool loop = false)
	{
		m_target = obj;
		m_function = fn_name;
			
		m_params = params;
		OnStart(duration, loop);
	}
	
	override protected void OnTimer()
	{
		if (m_params)
		{
			GetGame().GameScript.CallFunctionParams(m_target, m_function, NULL, m_params);
			m_params = NULL;
		}
		else
		{
			GetGame().GameScript.CallFunction(m_target, m_function, NULL, 0);
		}
	}
		
	override void Stop()
	{
		super.Stop();
		m_params = NULL;
	}
};



//--------------------------------------------------------------------------
/**
 \brief AnimatorTimer class. This timer is for animate float value.
 \n usage:
 @code
 class MyObject
 {
	ref AnimatorTimer myAnim;
	
	void MyObject()
	{
		myAnim = new AnimatorTimer(this, "Refresh", CALL_CATEGORY_GUI);
		myAnim.SetValue(6.0);
		myAnim.Animate(10.0, 0.1);
	}

	void Refresh()
	{
		Print(myAnim.GetValue());
	}
 }; 
 @endcode

 */
class AnimatorTimer
{
	private bool m_active = false;
	private bool m_loop = false;
	private float m_target_value = 0;
	private float m_value = 0;
	private float m_time = 0;

	/**
  \brief Starts animate value until value reaches target value.
	@param val \p float target value
	@param speed \p float speed of animating, units per second
	*/
	void Animate(float val, float speed = 1.0)
	{
		m_target_value = val;
		m_loop = false;
		m_time = speed;
		m_active = true;
	}

	/**
  \brief Starts infinite animation loop <-1,1>. Based on sinus transition.
	@param speed \p float speed of animating , cycles per second (1.0 means one 360 degree sinus cycle per second: 0..1..0..-1..0)
	*/
	void AnimateLoop(float speed = 1.0)
	{
		m_value = 0;
		m_target_value = 0;
		m_loop = true;
		m_time = speed;
		m_active = true;
	}

	/**
  \brief Returns actual animated value.
	*/
	float GetValue() {
		return m_value;
	}	

	/**
  \brief Returns target value. While AnimateLoop returns angle of cycle in radians.
	*/
	float GetTargetValue() {
		return m_target_value;
	}

	/**
  \brief Sets both value and target value.
	*/
	void SetValue(float val) {
		m_value = val;
		m_target_value = val;
	}	

	bool IsRunning()
	{
		return m_active;
	}
	/**
  \brief Ticks the timer, is called by timer subsystem.
	*/
	void Tick(float timeslice)
	{
		if ( !m_active ) return;
		
		if (m_loop)
		{
			m_target_value += m_time * Math.PI2 * timeslice;
			while (m_target_value > Math.PI2) m_target_value -= Math.PI2;
			
			m_value = Math.Sin(m_target_value);
		}
		else
		{
			float diff = Math.AbsFloat(m_target_value - m_value);
			float step = m_time * timeslice;
	
			if (diff < step)
			{
				m_value = m_target_value;
				m_active = false;
			}
			else
			{
				if (m_target_value > m_value) 
				{
					m_value += step;
				}
				else
				{
					m_value -= step;
				}
			}
		}
	}
};

/**
 \brief Associative array template, with multiple values per key
 \n usage:
 @code
 ref multiMap<string, int>	myMap = new multiMap<string, int>;
 myMap.Insert("jozo", 7);
 myMap.Insert("jozo", 3);
 myMap.Insert("jozo", 5);
 myMap.Insert("fero", 98);
 myMap.Insert("fero", 35);
 myMap.Insert("fero", 17);

 Print("Jozo:");
 myMap.Get("jozo").Debug();
 Print("Fero:");
 myMap.Get("fero").Debug();

 @endcode

 */
class multiMap<Class K, Class V>
{
	private ref array<ref array<V> > m_values;
	private ref array<K> m_keys;

	bool HasKey(K key)
	{
		int index = -1;
		if (m_keys)
		{
			index = m_keys.Find(key);
		}

		if (index != -1)
		{
			return true;
		}

		return false;
	}

	array<V> Get(K key)
	{
		int index = -1;
		if (m_keys)
		{
			index = m_keys.Find(key);
		}

		if (index != -1)
		{
			return m_values.Get(index);
		}

		return NULL;
	}

	array<V> GetByIndex(int index)
	{
		return m_values.Get(index);
	}

	K GetKeyByIndex(int index)
	{
		return m_keys.Get(index);
	}

	void Insert(K key, V value)
	{
		int index = -1;
		
		if (!m_keys)
		{
			m_keys = new array<K>;
			m_values = new array<ref array<V> >;
		}
		else
		{
			index = m_keys.Find(key);
		}

		if (index == -1)
		{
			array<V> value_array = new array<V>;
			value_array.Insert(value);

			m_keys.Insert(key);
			m_values.Insert(value_array);

		}
		else
		{
			m_values.Get(index).Insert(value);
		}
	}

	void RemoveByIndex(int index)
	{
		m_keys.Remove(index);
		m_values.Remove(index);
	}

	void Remove(K key)
	{
		int index = -1;
		if (m_keys)
		{
			index = m_keys.Find(key);
		}

		if (index != -1)
		{
			RemoveByIndex(index);
		}
	}

	int Count()
	{
		if (m_keys)
		{
			return m_keys.Count();
		}

		return 0;
	}

	void Clear()
	{
		if ( m_keys && m_values)
		{
			m_keys.Clear();
			m_values.Clear();
		}
		
	}

	void ~multiMap()
	{
		Clear();
	}
};

// at last one template definition should be here, for template initialization in this script module
typedef map<string, string> TStringMap;

int GetTemperatureColor( int temperature )
{
	int alpha = 255;
	int red = 153;
	int green = 153;
	int blue = 153;
	if ( temperature < 20 )
	{
		temperature = temperature - 20;
		temperature = Math.Clamp( temperature, -50, 50);
		temperature = Math.AbsInt(temperature);

		red = Math.Clamp ( red - ((red/50 )*temperature), 0, 255 );
		green = Math.Clamp ( green - ((green/50 )*temperature), 0, 255 );
		blue = Math.Clamp ( blue+((blue/50)*temperature), 0, 255 );
	}
	else if ( temperature > 20 )
	{
		temperature = Math.Clamp( temperature, -100, 100);
		blue = Math.Clamp ( blue - ((blue/100 )*temperature), 0, 255 );
		green = Math.Clamp ( green - ((green/100 )*temperature), 0, 255 );
		red = Math.Clamp ( red+((red/100)*temperature), 0, 255 );
	}

	int color = ARGB( alpha, red, green, blue );
	return color;
}

//! Return value from profile variable, if variable with given name is not present, default value is returned
bool GetProfileValueBool(string name, bool def = false)
{
	string value;
	if (GetGame().GetProfileString(name, value))
	{
		if (value == "true" || value == "1")
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return def;
	}
}
	
//! Writes bool variable to profile, after write don't forget to call CGame::SaveProfile() to save profile to storage!
void SetProfileValueBool(string name, bool value)
{
	if (value)
	{
		GetGame().SetProfileString(name, "1");
	}
	else
	{
		GetGame().SetProfileString(name, "0");
	}
}


int GetNumberOfSetBits(int i)
{
     i = i - ((i >> 1) & 0x55555555);
     i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
     return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

/** @}*/