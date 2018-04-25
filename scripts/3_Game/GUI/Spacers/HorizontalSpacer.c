// -----------------------------------------------------------
class HorizontalSpacer : SpacerBase
{
	reference int Border;
	reference int Gap;
	
	override protected void UpdateChild(Widget child, float w, float h, int index)
	{
		float itemWidth = (w - (Border * 2) - ((m_count - 1) * Gap)) / m_count;
		float itemHeight = h - (2 * Border);
	
		child.SetPos(Border + ((itemWidth + Gap) * index), Border);
		child.SetSize(itemWidth, itemHeight);
		
		if(child.GetChildren())
		{
			Widget c = child.GetChildren();
			RightGap gap;
			c.GetScript(gap);
			if(gap)
			gap.OnUpdate(c);

			//gap.Update();
			
		}
	}
};