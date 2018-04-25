class DefaultRecoil: RecoilBase
{
	override void Init()
	{
		m_HandsCurvePoints.Insert("0 0 0");//forms a 2 dimentional spline(z is ignored)
		m_HandsCurvePoints.Insert("0 1 0");
		m_HandsCurvePoints.Insert("0 0 0");
		
		m_MouseOffsetRangeMin = 60;//in degrees min
		m_MouseOffsetRangeMax = 120;//in degrees max
		m_MouseOffsetDistance = 1;//how far should the mouse travel
		m_MouseOffsetRelativeTime = 1;//[0..1] a time it takes to move the mouse the required distance relative to the reload time of the weapon(firing mode)
	}
}