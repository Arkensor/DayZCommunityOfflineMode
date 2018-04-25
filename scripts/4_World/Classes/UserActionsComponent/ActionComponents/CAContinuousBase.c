class CAContinuousBase: CABase
{			
	override bool IsContinuousAction() //informs command callback whether action is looped or oneshot
	{
		return true;
	}
};