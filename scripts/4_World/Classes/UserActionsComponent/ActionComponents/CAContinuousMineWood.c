class CAContinuousMineWood : CAContinuousBase
{
	protected float 				m_TimeElpased;
	protected float 				m_TimeBetweenEachMaterialDrop;
	protected float					m_DamageToMiningItemEachDrop;
	protected int 					m_AmountOfDrops;
	protected string				m_Material;
	protected float 				m_AmountOfMaterialPerDrop;
	protected float					m_TimeToComplete;
	protected ref Param1<float>		m_SpentUnits;
	protected bool					m_DataLoaded = false;
	
	void CAContinuousMineWood(float time_between_drops)
	{
		m_TimeBetweenEachMaterialDrop = time_between_drops;
	}
	
	override void Setup( PlayerBase player, ActionTarget target, ItemBase item )
	{
		m_TimeElpased = 0;
		if ( !m_SpentUnits )
		{ 
			m_SpentUnits = new Param1<float>(0);
		}
		else
		{	
			m_SpentUnits.param1 = 0;
		}
		m_DataLoaded = GetMiningData(player,target,item);

		m_TimeBetweenEachMaterialDrop = player.GetSoftSkillManager().SubtractSpecialtyBonus( m_TimeBetweenEachMaterialDrop, m_Action.GetSpecialtyWeight(), true );		
		m_TimeToComplete = m_AmountOfDrops*m_TimeBetweenEachMaterialDrop;
	}
	
	override int Execute( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Plant targetObject;
		Class.CastTo(targetObject, target.GetObject());
		if ( !player || !m_DataLoaded )
		{
			return UA_ERROR;
		}
		
		if ( item.IsDamageDestroyed() || targetObject.IsDamageDestroyed() )
		{
			return UA_SETEND_2;
		}
		else
		{
			if ( m_TimeElpased < m_TimeBetweenEachMaterialDrop )
			{
				m_TimeElpased += player.GetDeltaT();
			}
			else
			{
				if ( GetGame().IsServer() )
				{
					ItemBase drop;
					Class.CastTo(drop, GetGame().CreateObject(m_Material,player.GetPosition(), false));
					drop.SetQuantity(m_AmountOfMaterialPerDrop,false);
					float damage = (1 / m_AmountOfDrops) * 100;
					targetObject.DecreaseHealth("","",damage);					
					item.DecreaseHealth ( "", "", m_DamageToMiningItemEachDrop );
				}
				if ( targetObject.IsDamageDestroyed() )
				{
					if ( m_SpentUnits )
					{
						m_SpentUnits.param1 = m_TimeElpased;
						SetACData(m_SpentUnits);	
					}
					return UA_FINISHED;
				}
				m_TimeElpased = 0;
			}
			return UA_PROCESSING;
		}
	}
	
	override float GetProgress()
	{	
		//float progress = m_TimeElpased/m_TimeBetweenEachMaterialDrop;
		return m_TimeElpased/m_TimeBetweenEachMaterialDrop;
	}
	
	//---------------------------------------------------------------------------
	
	bool GetMiningData(PlayerBase player, ActionTarget target, ItemBase item )
	{
		WoodBase ntarget;
		if ( Class.CastTo(ntarget, target.GetObject()) )
		{
			m_AmountOfDrops = Math.Max(1,ntarget.GetAmountOfDrops(item));
			m_Material = ntarget.GetMaterial(item);
			m_AmountOfMaterialPerDrop = Math.Max(1,ntarget.GetAmountOfMaterialPerDrop(item));
			m_DamageToMiningItemEachDrop = ntarget.GetDamageToMiningItemEachDrop(item);
			return true;
		}
		return false;
	}
	
};