
static string worldMap = "Livonia"; // Chernarus, Livonia, Namalsk

static bool m_COM_GodMode; // move these to player saves? Edit: Jacob says "yes"
static bool m_COM_OldAiming;
static bool COM_bc_Visible;
static bool m_CameraTool;
static bool m_FreezeCam = false;
static int deleteAllClicks = 0;
static string currentGroup = "All";
static ref SettingsData settings;
static bool isSpawnMenuOpen = false;
static bool isEditorMenuOpen = false;
static string timeZone = "EST";
string lastEditorGroup = "All";
string lastEditorGroupInput = "None";
string lastObjectGroupInput = "None";
string lastObjectFilter = "All";
string lastObjectSearch = "";
string lastObjectSelection = "";
int chatLinesCount = 0;

string BASE_COM_DIR = "$saves:CommunityOfflineMode", BASE_SCENE_DIR = BASE_COM_DIR + "\\Scenes", BASE_BACKUP_DIR = BASE_COM_DIR + "\\Backups\\";
string objectsFile = "COMObjectEditorSave", undofilename = BASE_COM_DIR + "\\COMObjectMoves.json", filenameSettings = BASE_COM_DIR + "\\COMSettings.json";


static void startCOM() {
	if(worldMap == "Livonia") { 
		objectsFile = "COMObjectEditorSaveEnoch"; undofilename = BASE_COM_DIR + "\\COMObjectMovesEnoch.json"; filenameSettings = BASE_COM_DIR + "\\COMSettingsEnoch.json";
	} else if(worldMap == "Namalsk") { objectsFile = "COMObjectEditorSaveNamalsk"; undofilename = BASE_COM_DIR + "\\COMObjectMovesNamalsk.json"; filenameSettings = BASE_COM_DIR + "\\COMSettingsNamalsk.json"; }
    ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).loadSettings();
}
static void stopCOM() { ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).saveSettings(); }

static TVectorArray COM_GetSpawnPoints() {
    if(worldMap == "Livonia") return { "6296 0 9755" }; // Livonia spawn point
    if(worldMap == "Namalsk") return { "7000 0 11025" }; // Namalsk spawn point
    /*            Chernarus */return { "7025 0 8236" }; // Chernarus spawn point
}

void clearWeather(bool force = false) {
    if(settings.keepWeatherClear || force) { 
        GetGame().GetWeather().SetWindFunctionParams( 0, 0, 0 );
        GetGame().GetWeather().GetFog().Set( 0, 0, 0 );
        GetGame().GetWeather().SetStorm(0, 0, 99999);
        GetGame().GetWeather().GetOvercast().Set( 0, 0, 0 );
    }
}
void setDaytime(bool force = false) { if(settings.alwaysDay || force) { GetGame().GetWorld().SetDate( 2017, 7, 15, 12, 0 ); } }

static string getDateTime() {
    int yr, mo, dy, hr, mn, sc; GetHourMinuteSecondUTC(hr, mn, sc); GetYearMonthDayUTC(yr, mo, dy); string ampm = "AM";
    if(timeZone == "EST") { hr -= 4; if(hr < 0) { hr += 24; dy -= 1; } }
    string month = mo.ToString(), day = dy.ToString(), hour = hr.ToString(), min = mn.ToString(), sec = sc.ToString();
    if(mo < 10) { month = "0" + month; } if(dy < 10) { day = "0" + day; } if(mn < 10) { min = "0" + min; } if(sc < 10) { sec = "0" + sec; }
    if(hr > 12) { hour = (hr - 12).ToString(); ampm = "PM"; } else if (hr == 0) { hour = "12"; }
    return yr.ToString() + "/" + month + "/" + day + " " + hour + ":" + min + ":" + sec + ampm + " " + timeZone;
}

Object createObject(string type, vector pos, vector ypr = vector.Zero, bool snapToGround = false) {
    bool ai = false; if (GetGame().IsKindOf(type, "DZ_LightAI")) { ai = true; }
    Object obj = GetGame().CreateObject(type, pos, true, ai); obj.SetPosition(obj.GetPosition()); obj.SetOrientation(ypr);
    obj.Update(); obj.SetAffectPathgraph( true, false ); GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, obj);
	if(snapToGround) { COM_PlaceObjectOnGround(obj); } COM_ForceTargetCollisionUpdate(obj);
	return obj;
}
void objectDelete(Object obj) {
    obj.SetPosition(vector.Zero); GetGame().ObjectDelete(obj);
}

static string COM_FormatFloat(float value, int decimals)
{
    if (!value.ToString().Contains(".")) return value.ToString()+".000000";
    string result = "";
    array<string> output = new array<string>;
    value.ToString().Split(".", output);
    if (decimals == 0) return Math.Round(value).ToString();
    
    string right = output.Get(1).Substring(0, decimals);
    result = output.Get(0) + "." + right;
    return result;
}
/*
static string COM_FormatFloat(float value, int decimals)
{
    if (!value.ToString().Contains(".")) return value.ToString();

    string result = "";
    array<string> output = new array<string>;

    value.ToString().Split(".", output);

    if (output.Count() == 0) return value.ToString();

    if (decimals == 0) return output.Get(0);

    string right = output.Get(1).Substring(0, decimals);
    result = output.Get(0) + "." + right;
    return result;
}
*/
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

static string vecToString(vector vec, string divider)
{
    string result = vec.ToString();
    result.Replace("<", "");
    result.Replace(">", "");
    result.Replace(",", "");
    result.Replace(" ", divider);

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

static set< Object > COM_GetObjectsAt( vector from, vector to, Object ignore = NULL, float radius = 0.5, Object with = NULL )
{
    vector contact_pos, contact_dir; int contact_component;
    set<Object> geom = new set<Object>;
    DayZPhysics.RaycastRV(from, to, contact_pos, contact_dir, contact_component, geom, with, ignore, false, false, ObjIntersectGeom, radius);
    if(geom.Count() > 0) { return geom; }
    set<Object> view = new set<Object>;
    DayZPhysics.RaycastRV(from, to, contact_pos, contact_dir, contact_component, view, with, ignore, false, false, ObjIntersectView, radius);
    if(view.Count() > 0) { return view; }
    return NULL;
}

static Object COM_GetPointerObject( Object ignore = NULL, float radius = 0.5, Object with = NULL )
{
    vector dir = GetGame().GetPointerDirection(), from = GetGame().GetCurrentCameraPosition(), to = from + ( dir * 10000 );
    auto objs = COM_GetObjectsAt(from, to, ignore, radius, with);
    if(objs.Count() > 0) { return objs[0]; }
    return NULL;
}

static Object COM_GetCursorObject()
{
    vector rayStart = GetGame().GetCurrentCameraPosition(), rayEnd = rayStart + GetGame().GetCurrentCameraDirection() * 10000;
    auto objs = COM_GetObjectsAt( rayStart, rayEnd );
    if( objs.Count() > 0 ) { return objs[0]; }
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

static void COM_Message(string txt) {
    if(chatLinesCount == 8) { MissionGameplay.Cast(GetGame().GetMission()).m_Chat.Clear(); chatLinesCount = 0; }
    string name;
    GetGame().GetPlayerName(name);
    ChatMessageEventParams chat_params = new ChatMessageEventParams(CCDirect, name, txt, "");
    MissionGameplay.Cast(GetGame().GetMission()).m_Chat.Add(chat_params);
    chatLinesCount++;

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

static Weapon_Base COM_CreateWeaponTundra( PlayerBase oPlayer, string sWeapon )
{
    Weapon_Base oWpn = Weapon_Base.Cast(oPlayer.GetInventory().CreateInInventory( sWeapon ));
    EntityAI optic = oWpn.GetInventory().CreateAttachment( "HuntingOptic" );

    return oWpn;
}

static Weapon_Base COM_CreateWeapon( PlayerBase oPlayer, string sWeapon )
{
    Weapon_Base oWpn = Weapon_Base.Cast(oPlayer.GetInventory().CreateInInventory( sWeapon ));
    oWpn.GetInventory().CreateAttachment("Mag_FAL_20Rnd");
    oWpn.GetInventory().CreateAttachment("Fal_FoldingBttstck");
    EntityAI optic = oWpn.GetInventory().CreateAttachment( "ReflexOptic" );
    optic.GetInventory().CreateAttachment("Battery9V");

    return oWpn;
}

static PlayerBase COM_CreateCustomDefaultCharacter()
{
    vector pos = settings.spawnPosition; if(settings.spawnAtLastPosition == 1) { pos = settings.lastPosition; }
    PlayerBase oPlayer = PlayerBase.Cast(GetGame().CreatePlayer(NULL, GetGame().CreateRandomPlayer(), pos, 0, "NONE"));
    if(settings.spawnAtLastPosition == 1) { oPlayer.SetOrientation(settings.lastOrientation); }
    oPlayer.GetInventory().CreateInInventory( "BalaclavaMask_Blackskull" );
    oPlayer.GetInventory().CreateInInventory( "HuntingJacket_Winter" );
    oPlayer.GetInventory().CreateInInventory( "TacticalGloves_Black" );
    oPlayer.GetInventory().CreateInInventory( "HunterPants_Winter" );
    oPlayer.GetInventory().CreateInInventory( "MilitaryBoots_Black" );
    oPlayer.GetInventory().CreateInInventory("AliceBag_Black");
    oPlayer.GetInventory().CreateInInventory("HighCapacityVest_Black");
    oPlayer.GetInventory().CreateInInventory("Mag_FAL_20Rnd");
    oPlayer.GetInventory().CreateInInventory("Ammo_308Win");
    oPlayer.GetInventory().CreateInInventory("Binoculars");
    oPlayer.GetInventory().CreateInInventory("Vodka");
    Weapon_Base oWpn2 = Weapon_Base.Cast(oPlayer.GetInventory().CreateInInventory( "Shovel" ));

    Weapon_Base oWpn = COM_CreateWeapon( oPlayer, "FAL" );
    Weapon_Base oWpn1 = COM_CreateWeaponTundra( oPlayer, "Winchester70" );
    //oPlayer.PredictiveTakeEntityToHands( oWpn );
    oPlayer.SetQuickBarEntityShortcut(oWpn, 0, true);
    oPlayer.SetQuickBarEntityShortcut(oWpn1, 1, true);
    oPlayer.SetQuickBarEntityShortcut(oWpn2, 2, true);

    oPlayer.SetAllowDamage(false);
    m_COM_GodMode = true;

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

static vector COM_SnapToGround(vector pos) {
    float pos_x = pos[0], pos_z = pos[2], pos_y = GetGame().SurfaceY(pos_x, pos_z);
    vector tmp_pos = Vector(pos_x, pos_y, pos_z); tmp_pos[1] = tmp_pos[1] + pos[1]; return tmp_pos;
}

static void COM_SnapObjectToGround(Object object) {
    vector clippingInfo[2], pos = object.GetPosition(); 
    pos[1] = GetGame().SurfaceY(pos[0], pos[2]); object.ClippingInfo( clippingInfo ); pos[1] = pos[1] + clippingInfo[1][1] / 2;
    object.SetPosition(pos); COM_ForceTargetCollisionUpdate(object);
}
static void COM_PlaceObjectOnGround(Object object) {
    //vector clippingInfo[2], pos = object.GetPosition(); 
    //pos[1] = GetGame().SurfaceY(pos[0], pos[2]); object.ClippingInfo( clippingInfo ); pos[1] = pos[1] + clippingInfo[1][1] / 2;
    //object.SetPosition(pos); 
    object.PlaceOnSurface(); COM_ForceTargetCollisionUpdate(object);
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

int getKeyCode(string key) {
    key.ToLower();
	switch(key)
	{
		case "esc": { return KeyCode.KC_ESCAPE; }
		case "escape": { return KeyCode.KC_ESCAPE; }
		case "1": { return KeyCode.KC_1; }
		case "2": { return KeyCode.KC_2; }
		case "3": { return KeyCode.KC_3; }
		case "4": { return KeyCode.KC_4; }
		case "5": { return KeyCode.KC_5; }
		case "6": { return KeyCode.KC_6; }
		case "7": { return KeyCode.KC_7; }
		case "8": { return KeyCode.KC_8; }
		case "9": { return KeyCode.KC_9; }
		case "0": { return KeyCode.KC_0; }
		case "-": { return KeyCode.KC_MINUS; }
		case "minus": { return KeyCode.KC_MINUS; }
		case "subtract": { return KeyCode.KC_MINUS; }
		case "=": { return KeyCode.KC_EQUALS; }
		case "equal": { return KeyCode.KC_EQUALS; }
		case "equals": { return KeyCode.KC_EQUALS; }
		case "back": { return KeyCode.KC_BACK; }
		case "backspace": { return KeyCode.KC_BACK; }
		case "tab": { return KeyCode.KC_TAB; }
		case "q": { return KeyCode.KC_Q; }
		case "w": { return KeyCode.KC_W; }
		case "e": { return KeyCode.KC_E; }
		case "r": { return KeyCode.KC_R; }
		case "t": { return KeyCode.KC_T; }
		case "y": { return KeyCode.KC_Y; }
		case "u": { return KeyCode.KC_U; }
		case "i": { return KeyCode.KC_I; }
		case "o": { return KeyCode.KC_O; }
		case "p": { return KeyCode.KC_P; }
		case "[": { return KeyCode.KC_LBRACKET; }
		case "lbracket": { return KeyCode.KC_LBRACKET; }
		case "leftbracket": { return KeyCode.KC_LBRACKET; }
		case "left bracket": { return KeyCode.KC_LBRACKET; }
		case "]": { return KeyCode.KC_RBRACKET; }
		case "rbracket": { return KeyCode.KC_RBRACKET; }
		case "rightbracket": { return KeyCode.KC_RBRACKET; }
		case "right bracket": { return KeyCode.KC_RBRACKET; }
		case "enter": { return KeyCode.KC_RETURN; }
		case "return": { return KeyCode.KC_RETURN; }
		case "lctrl": { return KeyCode.KC_LCONTROL; }
		case "lcontrol": { return KeyCode.KC_LCONTROL; }
		case "leftctrl": { return KeyCode.KC_LCONTROL; }
		case "leftcontrol": { return KeyCode.KC_LCONTROL; }
		case "left ctrl": { return KeyCode.KC_LCONTROL; }
		case "left control": { return KeyCode.KC_LCONTROL; }
		case "a": { return KeyCode.KC_A; }
		case "s": { return KeyCode.KC_S; }
		case "d": { return KeyCode.KC_D; }
		case "f": { return KeyCode.KC_F; }
		case "g": { return KeyCode.KC_G; }
		case "h": { return KeyCode.KC_H; }
		case "j": { return KeyCode.KC_J; }
		case "k": { return KeyCode.KC_K; }
		case "l": { return KeyCode.KC_L; }
		case ";": { return KeyCode.KC_SEMICOLON; }
		case "semicolon": { return KeyCode.KC_SEMICOLON; }
		case "semi-colon": { return KeyCode.KC_SEMICOLON; }
		case "'": { return KeyCode.KC_APOSTROPHE ; }
		case "apostrophe": { return KeyCode.KC_APOSTROPHE ; }
		case "`": { return KeyCode.KC_GRAVE ; }
		case "grave": { return KeyCode.KC_GRAVE ; }
		case "backtick": { return KeyCode.KC_GRAVE ; }
		case "lshift": { return KeyCode.KC_LSHIFT; }
		case "leftshift": { return KeyCode.KC_LSHIFT; }
		case "left shift": { return KeyCode.KC_LSHIFT; }
		case "\\": { return KeyCode.KC_BACKSLASH; }
		case "backslash": { return KeyCode.KC_BACKSLASH; }
		case "z": { return KeyCode.KC_Z; }
		case "x": { return KeyCode.KC_X; }
		case "c": { return KeyCode.KC_C; }
		case "v": { return KeyCode.KC_V; }
		case "b": { return KeyCode.KC_B; }
		case "n": { return KeyCode.KC_N; }
		case "m": { return KeyCode.KC_M; }
		case ",": { return KeyCode.KC_COMMA; }
		case "comma": { return KeyCode.KC_COMMA; }
		case ".": { return KeyCode.KC_PERIOD; }
		case "period": { return KeyCode.KC_PERIOD; }
		case "decimal": { return KeyCode.KC_PERIOD; }
		case "/": { return KeyCode.KC_SLASH; }
		case "slash": { return KeyCode.KC_SLASH; }
		case "divide": { return KeyCode.KC_SLASH; }
		case "forwardslash": { return KeyCode.KC_SLASH; }
		case "forward slash": { return KeyCode.KC_SLASH; }
		case "rshift": { return KeyCode.KC_RSHIFT; }
		case "rightshift": { return KeyCode.KC_RSHIFT; }
		case "right shift": { return KeyCode.KC_RSHIFT; }
		case "*": { return KeyCode.KC_MULTIPLY; }
		case "multiply": { return KeyCode.KC_MULTIPLY; }
		case "times": { return KeyCode.KC_MULTIPLY; }
		case "lalt": { return KeyCode.KC_LMENU; }
		case "leftalt": { return KeyCode.KC_LMENU; }
		case "left alt": { return KeyCode.KC_LMENU; }
		case " ": { return KeyCode.KC_SPACE; }
		case "space": { return KeyCode.KC_SPACE; }
		case "spacebar": { return KeyCode.KC_SPACE; }
		case "cap": { return KeyCode.KC_CAPITAL; }
		case "caps": { return KeyCode.KC_CAPITAL; }
		case "capital": { return KeyCode.KC_CAPITAL; }
		case "caplock": { return KeyCode.KC_CAPITAL; }
		case "caps lock": { return KeyCode.KC_CAPITAL; }
		case "cap lock": { return KeyCode.KC_CAPITAL; }
		case "f1": { return KeyCode.KC_F1; }
		case "f2": { return KeyCode.KC_F2; }
		case "f3": { return KeyCode.KC_F3; }
		case "f4": { return KeyCode.KC_F4; }
		case "f5": { return KeyCode.KC_F5; }
		case "f6": { return KeyCode.KC_F6; }
		case "f7": { return KeyCode.KC_F7; }
		case "f8": { return KeyCode.KC_F8; }
		case "f9": { return KeyCode.KC_F9; }
		case "f10": { return KeyCode.KC_F10; }
		case "numlock": { return KeyCode.KC_NUMLOCK; }
		case "num lock": { return KeyCode.KC_NUMLOCK; }
		case "scroll": { return KeyCode.KC_SCROLL; }
		case "scrolllock": { return KeyCode.KC_SCROLL; }
		case "scroll lock": { return KeyCode.KC_SCROLL; }
		case "numpad7": { return KeyCode.KC_NUMPAD7; }
		case "numpad8": { return KeyCode.KC_NUMPAD8; }
		case "numpad9": { return KeyCode.KC_NUMPAD9; }
		case "numpad-": { return KeyCode.KC_SUBTRACT; }
		case "numpadminus": { return KeyCode.KC_SUBTRACT; }
		case "numpadsubtract": { return KeyCode.KC_SUBTRACT; }
		case "numpad4": { return KeyCode.KC_NUMPAD4; }
		case "numpad5": { return KeyCode.KC_NUMPAD5; }
		case "numpad6": { return KeyCode.KC_NUMPAD6; }
		case "numpad+": { return KeyCode.KC_ADD; }
		case "numpadplus": { return KeyCode.KC_ADD; }
		case "numpadadd": { return KeyCode.KC_ADD; }
		case "numpad1": { return KeyCode.KC_NUMPAD1; }
		case "numpad2": { return KeyCode.KC_NUMPAD2; }
		case "numpad3": { return KeyCode.KC_NUMPAD3; }
		case "numpad0": { return KeyCode.KC_NUMPAD0; }
		case "numpad.": { return KeyCode.KC_DECIMAL; }
		case "numpadperiod": { return KeyCode.KC_DECIMAL; }
		case "numpaddecimal": { return KeyCode.KC_DECIMAL; }
		case "<>": { return KeyCode.KC_OEM_102; }
		case "f11": { return KeyCode.KC_F11; }
		case "f12": { return KeyCode.KC_F12; }
		case "numpad=": { return KeyCode.KC_NUMPADEQUALS; }
		case "numpadequal": { return KeyCode.KC_NUMPADEQUALS; }
		case "numpadequals": { return KeyCode.KC_NUMPADEQUALS; }
		case "prevtrack": { return KeyCode.KC_PREVTRACK; }
		case "at": { return KeyCode.KC_AT; }
		case "colon": { return KeyCode.KC_COLON; }
		case "underline": { return KeyCode.KC_UNDERLINE; }
		case "stop": { return KeyCode.KC_STOP; }
		case "ax": { return KeyCode.KC_AX; }
		case "unlabeled": { return KeyCode.KC_UNLABELED; }
		case "nexttrack": { return KeyCode.KC_NEXTTRACK; }
		case "numpadenter": { return KeyCode.KC_NUMPADENTER; }
		case "numpadreturn": { return KeyCode.KC_NUMPADENTER; }
		case "rctrl": { return KeyCode.KC_RCONTROL; }
		case "rcontrol": { return KeyCode.KC_RCONTROL; }
		case "rightctrl": { return KeyCode.KC_RCONTROL; }
		case "rightcontrol": { return KeyCode.KC_RCONTROL; }
		case "right ctrl": { return KeyCode.KC_RCONTROL; }
		case "right control": { return KeyCode.KC_RCONTROL; }
		case "mute": { return KeyCode.KC_MUTE; }
		case "calculator": { return KeyCode.KC_CALCULATOR; }
		case "play": { return KeyCode.KC_PLAYPAUSE; }
		case "pausemedia": { return KeyCode.KC_PLAYPAUSE; }
		case "playpause": { return KeyCode.KC_PLAYPAUSE; }
		case "stop": { return KeyCode.KC_MEDIASTOP; }
		case "volume-": { return KeyCode.KC_VOLUMEDOWN; }
		case "volume+": { return KeyCode.KC_VOLUMEUP; }
		case "webhome": { return KeyCode.KC_WEBHOME; }
		case "numpad,": { return KeyCode.KC_NUMPADCOMMA; }
		case "numpadcomma": { return KeyCode.KC_NUMPADCOMMA; }
		case "numpad/": { return KeyCode.KC_DIVIDE; }
		case "numpadslash": { return KeyCode.KC_DIVIDE; }
		case "numpaddivide": { return KeyCode.KC_DIVIDE; }
		case "sysrq": { return KeyCode.KC_SYSRQ; }
		case "ralt": { return KeyCode.KC_RMENU; }
		case "rightalt": { return KeyCode.KC_RMENU; }
		case "right alt": { return KeyCode.KC_RMENU; }
		case "pause": { return KeyCode.KC_PAUSE; }
		case "home": { return KeyCode.KC_HOME; }
		case "up": { return KeyCode.KC_UP; }
		case "pageup": { return KeyCode.KC_PRIOR; }
		case "left": { return KeyCode.KC_LEFT; }
		case "right": { return KeyCode.KC_RIGHT; }
		case "end": { return KeyCode.KC_END; }
		case "down": { return KeyCode.KC_DOWN; }
		case "next": { return KeyCode.KC_NEXT; }
		case "insert": { return KeyCode.KC_INSERT; }
		case "delete": { return KeyCode.KC_DELETE; }
		case "del": { return KeyCode.KC_DELETE; }
		case "lwin": { return KeyCode.KC_LWIN; }
		case "leftwin": { return KeyCode.KC_LWIN; }
		case "left win": { return KeyCode.KC_LWIN; }
		case "rwin": { return KeyCode.KC_RWIN; }
		case "rightwin": { return KeyCode.KC_RWIN; }
		case "right win": { return KeyCode.KC_RWIN; }
		case "apps": { return KeyCode.KC_APPS; }
		case "power": { return KeyCode.KC_POWER; }
		case "sleep": { return KeyCode.KC_SLEEP; }
		case "wake": { return KeyCode.KC_WAKE; }
		case "mediaselect": { return KeyCode.KC_MEDIASELECT; }
	}
    return -1;
}
string getMouseCode(string key) {
    string original = key;
    key.ToLower();
	switch(key)
	{
		case "leftclick": { return "mBLeft"; }
		case "leftmousebutton": { return "mBLeft"; }
		case "lmbutton": { return "mBLeft"; }
		case "rightclick": { return "mBRight"; }
		case "rightmousebutton": { return "mBRight"; }
		case "rmbutton": { return "mBRight"; }
		case "middleclick": { return "mBMiddle"; }
		case "middlemousebutton": { return "mBMiddle"; }
		case "mmbutton": { return "mBMiddle"; }
		case "scrollup": { return "mWheelUp"; }
		case "mwu": { return "mWheelUp"; }
		case "mousewheelup": { return "mWheelUp"; }
		case "wheelup": { return "mWheelUp"; }
		case "scrolldown": { return "mWheelDown"; }
		case "mwd": { return "mWheelDown"; }
		case "mousewheeldown": { return "mWheelDown"; }
		case "wheeldown": { return "mWheelDown"; }
    }
    return original;
}