#define DISABLE_PERSISTENCY

#include "$CurrentDir:Missions\\CommunityOfflineModeCore\\core\\BaseModuleInclude.c"
  
Mission CreateCustomMission(string path)
{
    if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
        return new CommunityOfflineServer();

    return new CommunityOfflineClient();
};
