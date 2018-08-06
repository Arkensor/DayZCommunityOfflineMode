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
    ref InventorySave OPlayerInventory;

    bool HasItemInHands;
    ref HandSave OHandSave;

    void CharacterSave()
    {
        HasInventory = false;
        OPlayerInventory = new InventorySave;
        OHandSave = new HandSave;
    }

    static PlayerBase LoadPlayer(string sCharacter, string sSave) {
        ref CharacterSave oSave = new CharacterSave;

        JsonFileLoader<CharacterSave>.JsonLoadFile(BASE_PLAYER_SAVE_DIR + "\\" + sCharacter + "\\" + sSave + ".json", oSave);

		PlayerBase oPlayer = PlayerBase.Cast( GetGame().CreatePlayer( NULL, oSave.SModel, "0 0 0", 0, "NONE") );

        // Temporary for debugging purposes. Death checking not yet implemented.
        if (oSave.FHealth < 20) oSave.FHealth = 20;
        if (oSave.FBlood < 2000) oSave.FBlood = 2000;

		oPlayer.SetDirection( oSave.VecDirection );
		oPlayer.SetOrientation( oSave.VecOrientation );
        
		oPlayer.SetHealth("","", oSave.FHealth );
		oPlayer.SetHealth("GlobalHealth", "Blood", oSave.FBlood );
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

        if (oSave.HasInventory) {
            oSave.OPlayerInventory.Load(oPlayer, oPlayer);
        }

        if (oSave.HasItemInHands) {
            oSave.OHandSave.Load(oPlayer, oPlayer);
        }

		oPlayer.SetPosition( oSave.VecPosition );

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
        oSave.FBlood = oPlayer.GetHealth("GlobalHealth", "Blood");
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
        
        // TEMP
        oSave.HasInventory = true;

        if (oSave.HasInventory) {
            oSave.OPlayerInventory.Save(oPlayer, oPlayer);
        }

        ItemBase oHands = oPlayer.GetHumanInventory().GetEntityInHands();

        oSave.HasItemInHands = oHands != NULL;

        if (oSave.HasItemInHands) {
            oSave.OHandSave.Save(oHands, oPlayer);
        }

        JsonFileLoader<CharacterSave>.JsonSaveFile(BASE_PLAYER_SAVE_DIR + "\\" + sCharacter + "\\" + sSave + ".json", oSave);
    }
}