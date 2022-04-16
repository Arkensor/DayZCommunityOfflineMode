#define DISABLE_PERSISTENCY

#include "$CurrentDir:Missions\\DayZCommunityOfflineMode.ExclusionZone\\core\\BaseModuleInclude.c"

//#include "$CurrentDir:Missions\\DayZCommunityOfflineMode.MapName\\custom_objects\\sample.c"
  
Mission CreateCustomMission(string path)
{	
    if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
        return new CommunityOfflineServer();

    return new CommunityOfflineClient();
}


//Spawn helper function - Спавн объектов
void SpawnObject( string type, vector position, vector orientation )
{
    auto obj = GetGame().CreateObject( type, position );
    obj.SetPosition( position );
    obj.SetOrientation( orientation );
    obj.SetOrientation( obj.GetOrientation() ); //Collision fix
    obj.Update();
    obj.SetAffectPathgraph( true, false );
    if( obj.CanAffectPathgraph() ) GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().UpdatePathgraphRegionByObject, 100, false, obj );
}

void main()
{   
    //name file init (); - sample

};