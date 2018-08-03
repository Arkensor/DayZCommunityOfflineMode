const string BASE_PLAYER_SAVE_DIR = "$saves:CommunityOfflineMode\\PlayerSaves";

class CharacterSave 
{
    string Model;

    vector Position;
    vector Direction;
    vector Orientation;

    float Health;
    float Blood;

    int BloodStatType;

    float Temperature;
    float Energy;
    float Water;

    float StomachWater;
    float StomachEnergy;

    float HeatComfort;
    float StomachSolid;

    float Wet;
    float Tremor;
    
    float Stamina;
    
    float LastShaved;
    float BloodyHands;
    
    float PlayTime;

    bool HasInventory;
    InventorySave Inventory;

    static PlayerBase LoadPlayer(string character, string save) {
        CharacterSave oSave;

        JsonFileLoader<CharacterSave>.JsonLoadFile(BASE_PLAYER_SAVE_DIR + "\\" + character + "\\" + save + ".json", oSave);

		PlayerBase oPlayer = PlayerBase.Cast( GetGame().CreatePlayer( NULL, oSave.Model, "0 0 0", 0, "NONE") );
		
		oPlayer.SetPosition( oSave.Position );
		oPlayer.SetDirection( oSave.Direction );
		oPlayer.SetOrientation( oSave.Orientation );
		oPlayer.SetHealth("","", oSave.Health );
		oPlayer.SetHealth("GlobalHealth", "Blood", oSave.Blood );
		oPlayer.GetStatBloodType().Set( oSave.BloodStatType );
		oPlayer.GetStatTemperature().Set( oSave.Temperature );
		oPlayer.GetStatEnergy().Set( oSave.Energy );
		oPlayer.GetStatWater().Set( oSave.Water );
		oPlayer.GetStatStomachWater().Set( oSave.StomachWater );
		oPlayer.GetStatStomachEnergy().Set( oSave.StomachEnergy );
		oPlayer.GetStatHeatComfort().Set( oSave.HeatComfort );
		oPlayer.GetStatStomachSolid().Set( oSave.StomachSolid );
		oPlayer.GetStatWet().Set( oSave.Wet );
		oPlayer.GetStatTremor().Set( oSave.Tremor );
		oPlayer.GetStatStamina().Set( oSave.Stamina );
		oPlayer.StatRegister("playtime");
		oPlayer.StatUpdate("playtime", oSave.PlayTime);
		oPlayer.SetLastShavedSeconds(oSave.LastShaved);
		oPlayer.SetBloodyHands(oSave.BloodyHands);

        if (HasInventory) {
            Inventory.Create(oPlayer);
        }

        return oPlayer;
    }
}