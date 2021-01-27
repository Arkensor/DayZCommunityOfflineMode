#define DISABLE_PERSISTENCY

#include "$CurrentDir:Missions\\DayZCommunityOfflineMode.Rostow\\core\\BaseModuleInclude.c"
  
Mission CreateCustomMission(string path)
{	
    if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
        return new CommunityOfflineServer();

    return new CommunityOfflineClient();
}

void main()
{
    //In Beta We Trust!
}