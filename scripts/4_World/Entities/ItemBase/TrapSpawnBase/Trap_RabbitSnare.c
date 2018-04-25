class Trap_RabbitSnare extends TrapSpawnBase
{
	void Trap_RabbitSnare()
	{
		m_InitWaitTime = 120 + Math.RandomInt( 0, 300 );
		m_UpdateWaitTime = 15;
		m_BaitNeeded = false;
		m_IsFoldable = true;
		m_IsUsable = true;
		m_MinimalDistanceFromPlayersToCatch = 100;

		m_AnimationPhaseSet = "inventory";
		m_AnimationPhaseTriggered = "placing";
		m_AnimationPhaseUsed = "rabbit_snare_used";

		m_WaterSurfaceForSetup = false;

		m_SurfaceForSetup = new map<string, float>;
		m_SurfaceForSetup.Set("cp_concrete", 0.05 );
		m_SurfaceForSetup.Set("cp_dirt", 0.10 );
		m_SurfaceForSetup.Set("cp_broadleaf_dense1", 0.50 );
		m_SurfaceForSetup.Set("cp_broadleaf_dense2", 0.50 );
		m_SurfaceForSetup.Set("cp_broadleaf_sparse1", 0.50 );
		m_SurfaceForSetup.Set("cp_broadleaf_sparse2", 0.50 );
		m_SurfaceForSetup.Set("cp_conifer_common1", 0.50 );
		m_SurfaceForSetup.Set("cp_conifer_common2", 0.50 );
		m_SurfaceForSetup.Set("cp_conifer_moss1", 0.50 );
		m_SurfaceForSetup.Set("cp_conifer_moss2", 0.50 );
		m_SurfaceForSetup.Set("cp_grass", 0.35 );
		m_SurfaceForSetup.Set("cp_grass_tall", 0.35 );
		m_SurfaceForSetup.Set("cp_gravel", 0.10 );
		m_SurfaceForSetup.Set("cp_rock", 0.10 );
		
		m_CatchesPond = new multiMap<string, float>;
		
		m_CatchesSea = new multiMap<string, float>;
		
		m_CatchesGroundAnimal = new multiMap<string, float>;
		m_CatchesGroundAnimal.Insert("Animal_LepusEuropaeus",1);
		m_CatchesGroundAnimal.Insert("Animal_LepusEuropaeus",1);
		m_CatchesGroundAnimal.Insert("Animal_LepusEuropaeus",1);
	}
	
	override void SetupTrap()
	{
		if ( g_Game.IsServer() )
		{
			if ( this.GetHierarchyRootPlayer().CanDropEntity( this) )
			{
				if ( this.IsRuined() ) 
				{
					PlayerBase player = PlayerBase.Cast( this.GetHierarchyRootPlayer() );
					player.MessageImportant( "The trap is ruined." );
				}
				else
				{
					PlayerBase owner_player = PlayerBase.Cast( this.GetHierarchyRootPlayer() );
					Error("Mojmir: TODO");
					owner_player.LocalDropEntity( this );

					//GetDirection
					vector trapPos = ( owner_player.GetDirection() ); // * 0.5;
					trapPos[1] = 0;
					this.SetPosition( owner_player.GetPosition() + trapPos );
					
					owner_player.MessageStatus( m_InfoSetup );

					SetActive();
					
					this.SetOrientation( owner_player.GetOrientation() );
				}
			}
		}
	}
	
	override void AlignCatch( ItemBase obj, string catch_name )
	{
		if ( catch_name == "Animal_LepusEuropaeus" )
		{
			obj.SetOrientation( this.GetOrientation() );
			
			vector forward_vec = this.GetDirection();
			vector side_vec = forward_vec.Perpend(  ) * -0.22;
			forward_vec = forward_vec * -0.3;
			
			vector chatch_pos = obj.GetPosition() + forward_vec + side_vec;
			obj.SetPosition( chatch_pos );
		}
	}
}

class RabbitSnareTrap extends Trap_RabbitSnare 
{	

}