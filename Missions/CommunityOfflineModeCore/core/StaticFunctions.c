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
    string currWorld = GetGame().GetWorldName();
    currWorld.ToLower();

    switch(currWorld)
    {
        case "chernarusplus":
            return { "15135.1 0 13901.1", "15017.8 0 13892.4", "14887.1 0 14547.9", "14749.7 0 13248.7",
                    "14697.6 0 13418.4", "14537.3 0 14755.7", "14415.3 0 14025.2", "14338.0 0 12859.5",
                    "14263.8 0 12748.7", "14172.2 0 12304.9", "14071.4 0 12033.3", "14054.9 0 11341.3",
                    "14017.8 0 2959.1", "13905.5 0 12489.7", "13852.4 0 11686.0", "13846.6 0 12050.0",
                    "13676.0 0 12262.1", "13617.4 0 12759.8", "13610.1 0 11223.6", "13594.3 0 4064.0",
                    "13587.8 0 6026.5", "13571.1 0 3056.8", "13552.6 0 4653.7", "13529.9 0 3968.3",
                    "13520.8 0 4223.7", "13504.0 0 5004.5", "13476.7 0 6136.3", "13441.6 0 5262.2",
                    "13426.6 0 5747.3", "13416.8 0 11840.4", "13400.8 0 4120.7", "13395.8 0 5902.8",
                    "13385.0 0 3946.6", "13374.4 0 6454.3", "13367.1 0 10837.1", "13366.3 0 4906.0",
                    "13337.1 0 5120.8", "13326.7 0 5489.1", "13312.7 0 6771.1", "13288.7 0 11415.1",
                    "13261.6 0 11785.2", "13171.6 0 6534.8", "13159.8 0 5401.7", "13155.2 0 5475.2",
                    "13084.9 0 7938.6", "13056.8 0 4848.5", "13048.1 0 8357.6", "13048.1 0 3867.7",
                    "12991.7 0 7287.1", "12983.0 0 5539.1", "12978.9 0 9727.8", "12950.2 0 5226.7",
                    "12942.1 0 8393.1", "12891.5 0 3673.9", "12628.7 0 10495.2", "12574.3 0 3592.8",
                    "12566.3 0 6682.6", "12465.2 0 8009.0", "12354.5 0 3480.0", "13262.8 0 7225.8" };
        break;
        case "enoch":
            return { "1509.023926 182.689377 7440.780762", "3993.01 0 10146.2" };
        break;
        case "namalsk":
            return { "4178.460938 390.396393 6588.150879" };
        break;
        case "banov":
            return { "1070 43.5625 1100" };
        break;
        case "deerisle":
            return { "10496 0 4292" };
        break;
        case "esseker":
            return { "10950.44 0 5496.32" };
        break;
        case "chiemsee":
            return { "18948.92 16.8124 4822.77" };
        break;
        case "takistanplus":
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
        break;
        case "rostow":
            return { "601.516326 0 12680.856730","918.302364 0 12262.054838","1691.475068 0 12174.612685",
                "1446.023416 0 12420.064343","1756.673164 0 12510.574642","842.365760 0 11827.145181",
                "1422.245288 0 12067.994621","1854.086788 0 12489.097622","1921.585992 0 12767.531847",
                "842.365760 0 11966.745812","1565.681097 0 11957.541375","1456.761926 0 12683.924876",
                "1372.387921 0 12929.376534","744.586449 0 9247.054844","739.792471 0 9090.771165",
                "658.277021 0 12518.245006","1860.990116 0 12683.157839","1317.928336 0 12865.712510",
                "1739.031326 0 12910.200623","605.351508 0 12602.619014","1288.780952 0 11784.958178",
                "821.655776 0 12332.622190","955.120112 0 11924.558808","1596.362553 0 12561.966083",
                "1631.646228 0 12828.894761","1469.801545 0 11919.189553","1548.039259 0 12992.273521",
                "1267.303932 0 11919.189553","1936.926721 0 12314.980352","1408.438632 0 12348.729955",
                "575.838428 0 9247.054844","1636.248447 0 12396.286214","746.486208 0 12407.791760",
                "1072.494535 0 7617.102363","1297.811495 0 8913.393983","623.778207 0 7936.381290",
                "1217.272667 0 8133.893179","1028.389939 0 9274.859916","814.578526 0 8924.899530",
                "1147.280590 0 8828.061177","620.901820 0 8326.611090","930.592790 0 9216.373386",
                "1315.069815 0 8793.544536","446.401026 0 8325.652295","655.418461 0 8137.728362",
                "1291.099926 0 8259.495400","520.228285 0 8618.084946","600.767113 0 8801.214901",
                "1402.320213 0 8559.598415","1482.859041 0 8362.086527","918.128448 0 8021.714097",
                "737.874880 0 8669.859907","885.529398 0 7907.617423","555.703721 0 8155.945477",
                "1230.695805 0 8401.397145","1562.439073 0 8217.308394","1634.348741 0 8111.840881",
                "1197.137960 0 8703.417752","1014.008005 0 7800.232318","2558.432775 0 13158.844987",
                "1233.572192 0 9009.273541","2672.289745 0 13229.556160","2697.458128 0 13360.192054",
                "628.572185 0 8521.246592","699.523057 0 7898.029467","1078.542551 0 8522.747224",
                "905.253685 0 7807.501064","2678.282217 0 12952.703943","2956.332923 0 13225.960676",
                "3007.868184 0 13042.591026","1101.615007 0 8301.840723","3047.418500 0 13132.478110",
                "2789.742199 0 13033.003070","840.454449 0 7816.337324","990.179957 0 7920.899734",
                "2789.742199 0 12909.558142","2764.573816 0 12969.482865","2890.415730 0 13309.855288",
                "2741.802422 0 13396.146888","2878.430786 0 13440.491182" };
        break;
    }
    return { "0 0 0" };
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
    return MissionBase.Cast( GetGame().GetMission() );
}

static CommunityOfflineClient COM_GetClientMission()
{
    return CommunityOfflineClient.Cast( GetGame().GetMission() );
}

static CommunityOfflineServer COM_GetServerMission()
{
    return CommunityOfflineServer.Cast( GetGame().GetMission() );
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