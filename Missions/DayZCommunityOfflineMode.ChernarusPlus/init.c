#define DISABLE_PERSISTENCY

#include "$mission:core\\BaseModuleInclude.c"

Mission CreateCustomMission(string path)
{
    if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
        return new CommunityOfflineServer();

    return new CommunityOfflineClient();
}
