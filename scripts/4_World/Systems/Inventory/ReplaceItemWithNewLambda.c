/**@class		ReplaceItemWithNewLambda
 * @brief		adds automatic QuickBar handling
 **/
class ReplaceItemWithNewLambda : ReplaceItemWithNewLambdaBase
{
	PlayerBase m_Player;
	int m_IndexQB;

	void ReplaceItemWithNewLambda (EntityAI old_item, string new_item_type, PlayerBase player)
	{
		m_Player = player;
		m_IndexQB = -1;

		if (m_Player)
			m_IndexQB = m_Player.FindQuickBarEntityIndex(old_item);
	}

	/**@fn		CopyOldPropertiesToNew
	 * @brief	copy properties from old object to the created one
	 *
	 * @NOTE:	This is supposed to be overriden in derived classes, like for example right now.
	 **/
	override void CopyOldPropertiesToNew (notnull EntityAI old_item, notnull EntityAI new_item)
	{
		super.CopyOldPropertiesToNew(old_item, new_item);

		if (m_Player && m_IndexQB != -1)
		{
			m_Player.SetQuickBarEntityShortcut(new_item, m_IndexQB, true);
		}
	}
};

