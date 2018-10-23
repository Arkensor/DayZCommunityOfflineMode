class CharacterSave 
{   
    static void SavePlayer(string sCharacter, string sSave, PlayerBase oPlayer = NULL)
    {
        string character = sCharacter;
        string save = sSave;

        if ( character == "" )
        {
            return;
        }
        
        if ( save == "" )
        {
            save = "latest";
        }

        if ( oPlayer == NULL )
        {
            return;
        }

        ref CharacterData oData = new CharacterData;

        oData.SModel = oPlayer.GetType();
        oData.VecPosition = oPlayer.GetPosition();
        oData.VecDirection = oPlayer.GetDirection();
        oData.VecOrientation = oPlayer.GetOrientation();
        oData.FHealth = oPlayer.GetHealth("","");
        oData.FBlood = oPlayer.GetHealth("GlobalHealth", "Blood");
        oData.IBloodStatType = oPlayer.GetStatBloodType().Get();
        //oData.FTemperature = oPlayer.GetStatTemperature().Get();
        //oData.FShock = oPlayer.GetStatShock().Get();
        oData.FEnergy = oPlayer.GetStatEnergy().Get();
        oData.FWater = oPlayer.GetStatWater().Get();
        oData.FStomachWater = oPlayer.GetStatStomachWater().Get();
        oData.FStomachEnergy = oPlayer.GetStatStomachEnergy().Get();
        oData.FHeatComfort = oPlayer.GetStatHeatComfort().Get();
        oData.FStomachSolid = oPlayer.GetStatStomachSolid().Get();
        oData.FWet = oPlayer.GetStatWet().Get();
        oData.FTremor = oPlayer.GetStatTremor().Get();
        oData.FStamina = oPlayer.GetStatStamina().Get();
        oData.FLastShaved = oPlayer.GetLastShavedSeconds();
        oData.FBloodyHands = oPlayer.HasBloodyHands();
        oData.FPlayTime = oPlayer.StatGet("playtime");
        
        // TEMP
        oData.HasInventory = true;

        if (oData.HasInventory) {
            InventorySave.Save(oPlayer, oPlayer, oData.OInventory);
        }

        ItemBase oHands = oPlayer.GetHumanInventory().GetEntityInHands();
        
        oData.HasItemInHands = oHands != NULL;

        if (oData.HasItemInHands) {
            HandSave.Save(oHands, oPlayer, oData.OHands);
        }

        JsonFileLoader<CharacterData>.JsonSaveFile(BASE_PLAYER_SAVE_DIR + "\\" + character + "\\" + save + ".json", oData);
    }

    static void CreatePlayer(string sCharacter, PlayerBase oPlayer)
    {
        string character = sCharacter;
        string save = "latest";

        if ( character == "" )
        {
            return;
        }

        ref CharacterData oData = new CharacterData;

        oData.SModel = oPlayer.GetType();
        oData.VecPosition = oPlayer.GetPosition();
        oData.VecDirection = oPlayer.GetDirection();
        oData.VecOrientation = oPlayer.GetOrientation();
        
        oData.FHealth = oPlayer.GetHealth("","");
        oData.FBlood = oPlayer.GetHealth("GlobalHealth", "Blood");
        oData.IBloodStatType = oPlayer.GetStatBloodType().Get();
        //oData.FTemperature = oPlayer.GetStatTemperature().Get();
        //oData.FShock = oPlayer.GetStatShock().Get();
        oData.FEnergy = oPlayer.GetStatEnergy().Get();
        oData.FWater = oPlayer.GetStatWater().Get();
        oData.FStomachWater = oPlayer.GetStatStomachWater().Get();
        oData.FStomachEnergy = oPlayer.GetStatStomachEnergy().Get();
        oData.FHeatComfort = oPlayer.GetStatHeatComfort().Get();
        oData.FStomachSolid = oPlayer.GetStatStomachSolid().Get();
        oData.FWet = oPlayer.GetStatWet().Get();
        oData.FTremor = oPlayer.GetStatTremor().Get();
        oData.FStamina = oPlayer.GetStatStamina().Get();
        oData.FLastShaved = oPlayer.GetLastShavedSeconds();
        oData.FBloodyHands = oPlayer.HasBloodyHands();
        oData.FPlayTime = oPlayer.StatGet("playtime");
        
        // TEMP
        oData.HasInventory = true;

        if (oData.HasInventory) {
            InventorySave.Save(oPlayer, oPlayer, oData.OInventory);
        }

        ItemBase oHands = oPlayer.GetHumanInventory().GetEntityInHands();
        
        oData.HasItemInHands = oHands != NULL;

        if (oData.HasItemInHands) {
            HandSave.Save(oHands, oPlayer, oData.OHands);
        }

        JsonFileLoader<CharacterData>.JsonSaveFile(BASE_PLAYER_SAVE_DIR + "\\" + character + "\\" + save + ".json", oData);
    }
}