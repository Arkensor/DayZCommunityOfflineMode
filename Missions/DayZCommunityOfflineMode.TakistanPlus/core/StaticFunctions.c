static string COM_FormatFloat( float value, int decimals ) 
{
    if ( !value.ToString().Contains(".") ) return value.ToString();
	
    string result = "";
    array<string> output = new array<string>;

    value.ToString().Split(".", output);

    if ( output.Count() == 0 ) return value.ToString();

    if ( decimals == 0 ) return output.Get(0);

    string right = output.Get(1).Substring(0, decimals);
    result = output.Get(0) + "." + right;
    return result;
}

static string COM_VectorToString( vector vec )
{
    string result = vec.ToString();
    result.Replace( "<", "" );
    result.Replace( ">", "" );
    result.Replace( ",", "" );

    return result;
}

static string COM_VectorToString( vector vec, int decimals ) 
{
    string result = "";
    result = COM_FormatFloat(vec[0], decimals) + "|" + COM_FormatFloat(vec[1], decimals) + "|" + COM_FormatFloat(vec[2], decimals);

    return result;
}

static TStringArray COM_GetChildrenFromBaseClass( string strConfigName, string strBaseClass )
{
    string child_name = "";
    int count = GetGame().ConfigGetChildrenCount ( strConfigName );
    TStringArray class_names = new TStringArray;

    for (int p = 0; p < count; p++)
    {
        GetGame().ConfigGetChildName ( strConfigName, p, child_name );

        if ( GetGame().IsKindOf(child_name, strBaseClass ) && ( child_name != strBaseClass ) )
        {
            class_names.Insert(child_name);
        }
    }

    return class_names;
}

static TVectorArray COM_GetSpawnPoints()
{
    return { "1157.183587 0 1839.618937",  "1055.551387 0 1915.146107",  "1269.043688 0 1968.965239",
            "1413.761858 0 1966.684217",  "1559.747262 0 1903.829392",  "1643.638180 0 1770.009440",
            "1215.312950 0 1898.760454",  "1638.062348 0 1948.436042",  "1770.183620 0 1554.382588",
            "833.753696 0 2201.287632",  "10464.882027 0 10092.506911",  "10437.955830 0 10041.412021",
            "10428.710088 0 10073.853221",  "724.850750 0 2318.110802",  "803.557879 0 2178.517014",
            "10409.731986 0 10024.542597",  "8770.249227 0 8244.633210",  "1108.521785 0 1808.698418",
            "1497.145882 0 1951.730852",  "9939.707119 0 10203.334406",  "960.477124 0 2022.587782",
            "1344.063965 0 1994.309927",  "8874.507873 0 8235.546249",  "1593.709144 0 1845.283163",
            "8818.557856 0 8228.087387",  "641.688500 0 2379.987481",  "795.142651 0 2259.204203",
            "8919.354586 0 8243.518998",  "880.284955 0 2109.215133",  "9722.742584 0 9181.667328",
            "10014.159674 0 10037.722078",  "9749.993193 0 9114.027425",  "9634.468673 0 9482.650377",
            "10001.636009 0 7575.389661",  "10362.398833 0 9751.414388",  "10375.213107 0 9719.135394",
            "10383.972231 0 9686.207576",  "9963.389195 0 10157.592313",  "10026.000712 0 10119.149491",
            "9771.566591 0 8989.291010",  "9761.023201 0 9049.793849",  "9575.571233 0 9581.954201",
            "10392.569149 0 9663.012117",  "9282.713491 0 8198.371973",  "9989.990980 0 10102.928891",
            "9913.526688 0 7807.193722",  "10002.480842 0 10167.973497",  "9922.633464 0 7704.560944",
            "9357.702908 0 8029.893319",  "9152.998454 0 8496.323754",  "10048.060728 0 10059.782094",
            "9850.197056 0 9578.822074",  "8959.715411 0 8311.457879",  "9726.923956 0 9561.408582",
            "9500.922153 0 9549.766065",  "9866.634161 0 9434.710600",  "9770.211085 0 8420.967199",
            "9004.985979 0 8367.635147",  "9791.985081 0 9390.195093",  "9836.984344 0 7379.989218",
            "9762.506478 0 7541.785960",  "10175.130975 0 7848.258096",  "9068.206560 0 8420.497309",
            "10081.819626 0 7552.914836",  "9903.757603 0 8163.290906",  "9879.787715 0 8031.456524",
            "9230.730962 0 8263.323326",  "9989.364345 0 8165.003041",  "9783.052096 0 8355.906076",
            "9826.711535 0 8234.344502",  "10264.161987 0 7599.142477",  "9210.257382 0 8353.979933",
            "604.776744 0 2427.292362",  "553.275729 0 2488.655274",  "1093.221150 0 1717.313473",
            "1023.551219 0 2634.766286",  "974.741402 0 2618.215830",  "525.881572 0 2544.539355",
            "1111.599335 0 2581.279746",  "1353.106230 0 2998.109252",  "1356.225119 0 2412.245233",
            "1616.768543 0 2333.027460",  "1498.858133 0 1714.868452",  "1509.947983 0 2388.681873",
            "1367.132039 0 2747.397919",  "1865.642494 0 2394.965436",  "1735.258575 0 2368.933533",
            "1343.901793 0 2662.366453",  "1428.934911 0 2416.060253",  "1212.731642 0 1596.350369",
            "1173.838861 0 2575.143455",  "1415.755218 0 1659.115863",  "1460.491329 0 3026.160869",
            "1380.281235 0 2849.523340",  "1384.225994 0 2949.018921",  "1314.535256 0 2619.850720",
            "1239.146534 0 2616.782575",  "1261.120681 0 1749.231683",  "1142.251955 0 1656.661346",
            "1580.638059 0 2347.614302",  "1387.732446 0 3054.650793",  "1019.343476 0 2705.736882",
            "1321.431657 0 1608.622952",   };
}

static set< Object > COM_GetObjectsAt( vector from, vector to, Object ignore = NULL, float radius = 0.5, Object with = NULL )
{
    vector contact_pos;
    vector contact_dir;
    int contact_component;

    set< Object > geom = new set< Object >;
    set< Object > view = new set< Object >;

    DayZPhysics.RaycastRV( from, to, contact_pos, contact_dir, contact_component, geom, with, ignore, false, false, ObjIntersectGeom, radius );
    DayZPhysics.RaycastRV( from, to, contact_pos, contact_dir, contact_component, view, with, ignore, false, false, ObjIntersectView, radius );

    if ( geom.Count() > 0 ) 
    {
        return geom;
    }
    if ( view.Count() > 0 ) 
    {
        return view;
    }
    return NULL;
}

static Object COM_GetPointerObject( Object ignore = NULL, float radius = 0.5, Object with = NULL )
{
    vector dir = GetGame().GetPointerDirection();

    vector from = GetGame().GetCurrentCameraPosition();

    vector to = from + ( dir * 10000 );

    auto objs = COM_GetObjectsAt( from, to, ignore, radius, with );

    if( objs.Count() > 0 )
    {
        return objs[ 0 ];
    }

    return NULL;
}

static Object COM_GetCursorObject()
{
    vector rayStart = GetGame().GetCurrentCameraPosition();
    vector rayEnd = rayStart + GetGame().GetCurrentCameraDirection() * 10000;

    auto objs = COM_GetObjectsAt( rayStart, rayEnd );

    if( objs.Count() > 0 )
    {
        return objs[ 0 ];
    }

    return NULL;
}

static vector COM_GetPointerPos()
{
    if ( !COM_GetPB() )
    {
        return "0 0 0";
    }

    vector dir = GetGame().GetPointerDirection();

    vector from = GetGame().GetCurrentCameraPosition();

    vector to = from + ( dir * 10000 );

    vector rayStart = from;
    vector rayEnd = to;
    vector hitPos;
    vector hitNormal;
    int hitComponentIndex;
    DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, NULL, NULL, COM_GetPB());

    return hitPos;
}

static vector COM_GetCursorPos()
{
    if ( !COM_GetPB() )
    {
        return "0 0 0";
    }

    vector rayStart = GetGame().GetCurrentCameraPosition();
    vector rayEnd = rayStart + GetGame().GetCurrentCameraDirection() * 10000;
    vector hitPos;
    vector hitNormal;
    int hitComponentIndex;
    DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, NULL, NULL, COM_GetPB());

    return hitPos;
}

static void COM_Message( string txt ) 
{
    COM_GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", txt, ""));
}

static Weapon COM_GetWeaponInHands()
{
    Weapon weapon_in_hands;
    if( COM_GetPB() && COM_GetPB().GetItemInHands() ) Class.CastTo(weapon_in_hands,  COM_GetPB().GetItemInHands());

    return weapon_in_hands;
}

static MissionBase COM_GetMission()
{
    return MissionBase.Cast( GetGame().COM_GetMission() );
}

static CommunityOfflineClient COM_GetClientMission()
{
    return CommunityOfflineClient.Cast( GetGame().COM_GetMission() );
}

static CommunityOfflineServer COM_GetServerMission()
{
    return CommunityOfflineServer.Cast( GetGame().COM_GetMission() );
}

static PlayerBase COM_GetPB()
{
    return PlayerBase.Cast( GetGame().GetPlayer() );
}

static bool COM_SHIFT()
{
    return( ( KeyState( KeyCode.KC_LSHIFT ) > 0 ) || ( KeyState( KeyCode.KC_RSHIFT ) > 0 ) );
}

static bool COM_CTRL()
{
    return( ( KeyState( KeyCode.KC_LCONTROL ) > 0 ) || ( KeyState( KeyCode.KC_RCONTROL ) > 0 ) );
}

static bool COM_ALT()
{
    return( ( KeyState( KeyCode.KC_LMENU ) > 0 ) || ( KeyState( KeyCode.KC_RMENU ) > 0 ) );
}

static bool COM_WINKEY()
{
    return( ( KeyState( KeyCode.KC_LWIN ) > 0 ) || ( KeyState( KeyCode.KC_RWIN ) > 0 ) );
}

/*
static Weapon_Base COM_CreateWeapon( PlayerBase oPlayer )
{
    Weapon_Base oWpn = Weapon_Base.Cast(oPlayer.GetInventory().CreateInInventory( "M4A1_Black" ));
    oWpn.GetInventory().CreateAttachment( "M4_Suppressor" );
    oWpn.GetInventory().CreateAttachment( "M4_RISHndgrd_Black" );
    oWpn.GetInventory().CreateAttachment( "M4_MPBttstck_Black" );
    oWpn.GetInventory().CreateAttachment( "ACOGOptic" );

    return oWpn;
}
*/

static Weapon_Base COM_CreateWeapon( PlayerBase oPlayer, string sWeapon )
{
    Weapon_Base oWpn = Weapon_Base.Cast(oPlayer.GetInventory().CreateInInventory( sWeapon ));
    oWpn.GetInventory().CreateAttachment( "PistolSuppressor" );
    EntityAI optic = oWpn.GetInventory().CreateAttachment( "ReflexOptic" );
    optic.GetInventory().CreateAttachment("Battery9V");

    return oWpn;
}

static PlayerBase COM_CreateCustomDefaultCharacter()
{
    PlayerBase oPlayer = PlayerBase.Cast( GetGame().CreatePlayer( NULL, GetGame().CreateRandomPlayer(), COM_GetSpawnPoints().GetRandomElement(), 0, "NONE") );

    oPlayer.GetInventory().CreateInInventory( "AviatorGlasses" );
    oPlayer.GetInventory().CreateInInventory( "MilitaryBeret_UN" );
    oPlayer.GetInventory().CreateInInventory( "M65Jacket_Black" );
    oPlayer.GetInventory().CreateInInventory( "TacticalGloves_Black" );
    oPlayer.GetInventory().CreateInInventory( "HunterPants_Autumn" );
    oPlayer.GetInventory().CreateInInventory( "MilitaryBoots_Black" );
    oPlayer.GetInventory().CreateInInventory( "AliceBag_Camo" );
    oPlayer.GetInventory().CreateInInventory( "Shovel" );

    Weapon_Base oWpn = COM_CreateWeapon( oPlayer, "UMP45" );
    oPlayer.PredictiveTakeEntityToHands( oWpn );

    Magazine oMag = Magazine.Cast( oPlayer.GetInventory().CreateInInventory( "Mag_UMP_25Rnd" ) );
    oPlayer.GetDayZPlayerInventory().PostWeaponEvent( new WeaponEventAttachMagazine( oPlayer, oMag ) );

    oPlayer.GetInventory().CreateInInventory( "Mag_UMP_25Rnd" );

    oPlayer.SetQuickBarEntityShortcut( oWpn, 0, true );
    oPlayer.SetQuickBarEntityShortcut( oMag, 1, true );

    return oPlayer;
}

static string COM_FileAttributeToString( FileAttr attr )
{
    string fileType = "";
    if ( attr & FileAttr.DIRECTORY )
    {
        fileType = fileType + "DIRECTORY";
    }
    if ( attr & FileAttr.HIDDEN )
    {
        fileType = fileType + "HIDDEN";
    }
    if ( attr & FileAttr.READONLY )
    {
        fileType = fileType + "READONLY";
    }
    if ( attr & FileAttr.INVALID )
    {
        fileType = fileType + "INVALID";
    }
    return fileType;
}

static vector COM_SnapToGround(vector pos)
{
    float pos_x = pos[0];
    float pos_z = pos[2];
    float pos_y = GetGame().SurfaceY( pos_x, pos_z );
    vector tmp_pos = Vector( pos_x, pos_y, pos_z );
    tmp_pos[1] = tmp_pos[1] + pos[1];

    return tmp_pos;
}

static bool m_COM_GodMode; // move these to player saves? Edit: Jacob says "yes"
static bool m_COM_OldAiming;
static bool COM_bc_Visible;

static void COM_SnapToGroundNew( Object object ) 
{
    vector pos = object.GetPosition();
    pos[1] = GetGame().SurfaceY(pos[0], pos[2]);
    
    vector clippingInfo[2];
    vector objectBBOX[2];
    
    object.GetCollisionBox( objectBBOX );
    object.ClippingInfo( clippingInfo );

    //float clipY = objectBBOX[1][1] / 2.0//- clippingInfo[0][1];
    //pos[1] = pos[1] + objectBBOX[1][1] - clipY;
    pos[1] = pos[1] + clippingInfo[1][1] / 2.0;//objectBBOX[0][1] - clipY

    object.SetPosition(pos);

    COM_ForceTargetCollisionUpdate( object );
}

static void COM_ForceTargetCollisionUpdate( Object oObj )
{
    if ( !oObj ) return;

    vector roll = oObj.GetOrientation();
    roll [ 2 ] = roll [ 2 ] - 1;
    oObj.SetOrientation( roll );
    roll [ 2 ] = roll [ 2 ] + 1;
    oObj.SetOrientation( roll );
}

static void COM_ToggleCursor()
{
    if ( GetGame().GetInput().HasGameFocus( INPUT_DEVICE_MOUSE ) )
    {
        GetGame().GetInput().ChangeGameFocus( 1 );
        GetGame().GetUIManager().ShowUICursor( true );
    }
    else
    {
        GetGame().GetUIManager().ShowUICursor( false );
        GetGame().GetInput().ResetGameFocus();
    }
}

/*
    Token types:
     0 - error, no token
     1 - defined token (special characters etc. . / * )
     2 - quoted string. Quotes are removed -> TODO
     3 - alphabetic string
     4 - number
     5 - end of line -> TODO
*/
static bool COM_CheckStringType( string str, int type ) 
{
    for(int i = 0; i<str.Length(); i++ ) 
    {
        string character = str.Get(i);
        string token;
        int result = character.ParseStringEx(token);
        if ( result == type ) return true;
    }
    return false;
}

string COM_GetRandomChildFromBaseClass( string strConfigName, string strBaseClass, int minScope = -1 )
{
    string child_name = "";
    int count = GetGame().ConfigGetChildrenCount ( strConfigName );
    array< string > class_names = new array<string>;

    for ( int p = 0; p < count; p++ )
    {
        GetGame().ConfigGetChildName( strConfigName, p, child_name );

        if( ( minScope != -1 ) && ( GetGame().ConfigGetInt( strConfigName + " " + child_name + " scope" ) < minScope ) ) continue;

        if ( GetGame().IsKindOf( child_name, strBaseClass ) && ( child_name != strBaseClass ) )
        {
            class_names.Insert( child_name );
        }
    }

    return class_names.GetRandomElement();
}