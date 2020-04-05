class CharacterData
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
    float FShock;

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
    ref InventoryData OInventory;

    bool HasItemInHands;
    ref HandData OHands;

    void CharacterData()
    {
        HasInventory = false;
        OInventory = new InventoryData;
        OHands = new HandData;
    }
}