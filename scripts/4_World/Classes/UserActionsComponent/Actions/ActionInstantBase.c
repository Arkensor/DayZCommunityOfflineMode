class ActionInstantBase : ActionBase
{	
	override void WriteToContext (ParamsWriteContext ctx,ActionTarget target)
	{
		ctx.Write(INPUT_UDT_STANDARD_ACTION); // user data type
		ctx.Write(GetType()); // action id
	}
	
	void PerformAction( PlayerBase player, Param data ) //Setup and execution of action on client side
	{
	}
};