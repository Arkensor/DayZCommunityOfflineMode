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
    auto obj = GetGame().CreateObject( type, position, ECE_CREATEPHYSICS );
    obj.SetFlags( EntityFlags.STATIC, false );
    obj.SetPosition( position );
    obj.SetOrientation( orientation );
    obj.SetOrientation( obj.GetOrientation() ); //Collision fix - Фикс Коллизии объектов
    obj.Update();
    obj.SetAffectPathgraph( true, false );
    if( obj.CanAffectPathgraph() ) GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().UpdatePathgraphRegionByObject, 100, false, obj );
}
void main()
{   
/* 

                    // ENG: name file init (); - sample
                    // RU : имя файла init (); - пример 
                    
                    // ENG: How could you optimize your mapping:
                    // RU : Как бы вы могли оптимизировать ваш маппинг:


                

                SpawnObject( "Classname_1", "x.x y.y z.z", "0.000000 0.000000 0.000000" );
                SpawnObject( "Classname_2", "x.x y.y z.z", "0.000000 0.000000 0.000000" );
                    {//comment - Name location

                        SpawnObject( "Classname_3", "x.x y.y z.z", "0.000000 0.000000 0.000000" );
                        SpawnObject( "Classname_4", "x.x y.y z.z", "0.000000 0.000000 0.000000" );
                            {//Subname - sublocation

                                SpawnObject( "Classname_5", "x.x y.y z.z", "0.000000 0.000000 0.000000" );
                                SpawnObject( "Classname_6", "x.x y.y z.z", "0.000000 0.000000 0.000000" );
                            }
                    }
*/


};