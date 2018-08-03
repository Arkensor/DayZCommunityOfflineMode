const string BASE_PLAYER_SAVE_DIR = "$saves:CommunityOfflineMode\\PlayerSaves";

class CharacterSave 
{
    string SModel;

    vector VecPosition;
    vector VecDirection;
    vector VecOrientation;

    float FHealth;
    float FBlood;

    int IBloodStatType;

    float FTemperature;
    float FEnergy;
    float FWater;

    float FStomachWater;
    float FStomachEnergy;

    float FHeatComfort;
    float FStomachSolid;

    float FWet;
    float FTremor;
    
    float FStamina;
    
    float FLastShaved;
    float FBloodyHands;
    
    float FPlayTime;

    bool HasInventory;
    InventorySave OInventory;

    static PlayerBase LoadPlayer(string sCharacter, string sSave, PlayerBase oPlayer = NULL) {
        ref CharacterSave oSave = new CharacterSave;

        JsonFileLoader<CharacterSave>.JsonLoadFile(BASE_PLAYER_SAVE_DIR + "\\" + sCharacter + "\\" + sSave + ".json", oSave);

        if (oPlayer == NULL)
        {
		    oPlayer = PlayerBase.Cast( GetGame().CreatePlayer( NULL, oSave.SModel, "0 0 0", 0, "NONE") );
        }

		oPlayer.SetPosition( oSave.VecPosition );
		oPlayer.SetDirection( oSave.VecDirection );
		oPlayer.SetOrientation( oSave.VecOrientation );
        /*
		oPlayer.SetHealth("","", oSave.FHealth );
		oPlayer.SetHealth("GlobalHealth", "FBlood", oSave.FBlood );
		oPlayer.GetStatBloodType().Set( oSave.IBloodStatType );
		oPlayer.GetStatTemperature().Set( oSave.FTemperature );
		oPlayer.GetStatEnergy().Set( oSave.FEnergy );
		oPlayer.GetStatWater().Set( oSave.FWater );
		oPlayer.GetStatStomachWater().Set( oSave.FStomachWater );
		oPlayer.GetStatStomachEnergy().Set( oSave.FStomachEnergy );
		oPlayer.GetStatHeatComfort().Set( oSave.FHeatComfort );
		oPlayer.GetStatStomachSolid().Set( oSave.FStomachSolid );
		oPlayer.GetStatWet().Set( oSave.FWet );
		oPlayer.GetStatTremor().Set( oSave.FTremor );
		oPlayer.GetStatStamina().Set( oSave.FStamina );
		oPlayer.StatRegister("playtime");
		oPlayer.StatUpdate("playtime", oSave.FPlayTime);
		oPlayer.SetLastShavedSeconds(oSave.FLastShaved);
		oPlayer.SetBloodyHands(oSave.FBloodyHands);
*/
        //if (HasInventory) {
        //    OInventory.Create(oPlayer);
        //}

        return oPlayer;
    }

    static void SavePlayer(string sCharacter, string sSave, PlayerBase oPlayer = NULL)
    {
        
        if (oPlayer == NULL)
        {
            return;
        }

        ref CharacterSave oSave = new CharacterSave;

        oSave.SModel = oPlayer.GetType();
        oSave.VecPosition = oPlayer.GetPosition();
        oSave.VecDirection = oPlayer.GetDirection();
        oSave.VecOrientation = oPlayer.GetOrientation();
        oSave.FHealth = oPlayer.GetHealth("","");
        oSave.FBlood = oPlayer.GetHealth("GlobalHealth", "FBlood");
        oSave.IBloodStatType = oPlayer.GetStatBloodType().Get();
        oSave.FTemperature = oPlayer.GetStatTemperature().Get();
        oSave.FEnergy = oPlayer.GetStatEnergy().Get();
        oSave.FWater = oPlayer.GetStatWater().Get();
        oSave.FStomachWater = oPlayer.GetStatStomachWater().Get();
        oSave.FStomachEnergy = oPlayer.GetStatStomachEnergy().Get();
        oSave.FHeatComfort = oPlayer.GetStatHeatComfort().Get();
        oSave.FStomachSolid = oPlayer.GetStatStomachSolid().Get();
        oSave.FWet = oPlayer.GetStatWet().Get();
        oSave.FTremor = oPlayer.GetStatTremor().Get();
        oSave.FStamina = oPlayer.GetStatStamina().Get();
        oSave.FLastShaved = oPlayer.GetLastShavedSeconds();
        oSave.FBloodyHands = oPlayer.HasBloodyHands();
        oSave.FPlayTime = oPlayer.StatGet("playtime");

        JsonFileLoader<CharacterSave>.JsonSaveFile(BASE_PLAYER_SAVE_DIR + "\\" + sCharacter + "\\" + sSave + ".json", oSave);
    }
}