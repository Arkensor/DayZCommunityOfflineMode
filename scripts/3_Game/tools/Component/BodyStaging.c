//-----------------------------
// BODY STAGING
//-----------------------------
/*
Every dead body will have this component which will control its various states.
Read "Dead bodies staging" design on Confluence.
*/

class ComponentBodyStaging : Component
{
	bool m_IsSkinned;
	
	void ComponentBodyStaging()
	{
		m_IsSkinned = false;
	}
	
	void ~ComponentBodyStaging()
	{
		
	}
	
	void SetAsSkinned()
	{
		m_IsSkinned = true;
		
		// Temporal! Body is moved into ground so we can see if it was already skinned or not until necesarry assets are developed.
		/*
		if ( !m_ThisEntityAI.IsKindOf("Animal_GallusGallusDomesticus")  &&  !m_ThisEntityAI.IsKindOf("Animal_GallusGallusDomesticusF") )
		{
			vector body_new_pos = m_ThisEntityAI.GetPosition() - "0 0.2 0";
			m_ThisEntityAI.SetPosition(body_new_pos);
		}
		else
		{
			vector body_new_pos2 = m_ThisEntityAI.GetPosition() - "0 0.08 0";
			m_ThisEntityAI.SetPosition(body_new_pos2);
		}
		*/
		
		SynchSkinnedState();
	}
	
	bool IsSkinned()
	{
		return m_IsSkinned;
	}
	
	void SetAsSkinnedClient()
	{
		m_IsSkinned = true;
	}
	
	/*====================================
	  SERVER --> CLIENT  SYNCHRONIZATION
	====================================*/
	
	// Synchronizes properties
	protected void SynchSkinnedState()
	{
		if ( GetGame().IsServer() )
		{
			Param1<bool> p = new Param1<bool>( IsSkinned() );
			GetGame().RPCSingleParam( m_ThisEntityAI, ERPCs.RPC_BS_SKINNED_STATE, p, true );
		}
	}
}