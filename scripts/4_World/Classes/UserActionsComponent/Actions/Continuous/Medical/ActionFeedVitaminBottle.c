class ActionFeedVitaminBottleCB : ActionContinuousBaseCB
{
	private const float QUANTITY_USED_PER_SEC = 1;
	
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantityEdible(QUANTITY_USED_PER_SEC,UATimeSpent.DEFAULT);
	}
};

class ActionFeedVitaminBottle: ActionForceConsume
{
	void ActionFeedVitaminBottle()
	{
		m_CallbackClass = ActionFeedVitaminBottleCB;
	}

	override int GetType()
	{
		return AT_FEED_VITAMIN_BOTTLE;
	}
};