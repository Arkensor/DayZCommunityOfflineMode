// -----------------------------------------------------------
class OverrideVerticalSpacer : SpacerBase
{
	reference int border;
	reference int gap;
	reference int maxheight;

	override protected void UpdateChild(Widget child, float w, float h, int index)
	{
		float itemWidth = w - (2 * border);
		float itemHeight = (h - (border * 2) - ((m_count - 1) * gap)) / m_count;
	
		if ( itemHeight > maxheight ) 
		{
			itemHeight = maxheight;
		}

		child.SetPos(border, border + ((itemHeight + gap) * index));
		child.SetSize(itemWidth, itemHeight);
	}
}