#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\Inventory\\ItemSave.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\Inventory\\HandSave.c"
#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\Inventory\\InventorySave.c"

#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\modules\\SaveModule\\CharacterSave.c"

class SaveModule extends Module
{

	void SaveModule( CommunityOfflineMode mission )
	{
	}

	void ~SaveModule()
	{
	}
	
	override void Init() 
	{
		super.Init();
	}

	void SavePlayer() 
	{
		CharacterSave.SavePlayer("a", "b", m_Mission.m_oPlayer);
	}

	PlayerBase LoadPlayer()
	{
		return CharacterSave.LoadPlayer("a", "b");

		// return CreateDefaultCharacter();
	}

    // This is where we will show the Character Select GUI. 
	override void onMissionLoaded()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.SavePlayer, 1000, true);
	}
	
	override void onUpdate( int timeslice ) 
	{
	}

	override void onMissionFinish()
	{
		SavePlayer();
	}

	static PlayerBase CreateDefaultCharacter()
	{
		PlayerBase oPlayer = PlayerBase.Cast( GetGame().CreatePlayer( NULL, GetGame().CreateRandomPlayer(), GetSpawnPoints().GetRandomElement(), 0, "NONE") );

		EntityAI item = oPlayer.GetInventory().CreateInInventory( "AviatorGlasses" );

		item = oPlayer.GetInventory().CreateInInventory( "MilitaryBeret_UN" );
		item = oPlayer.GetInventory().CreateInInventory( "M65Jacket_Black" );
		item = oPlayer.GetInventory().CreateInInventory( "PlateCarrierHolster" );
		item = oPlayer.GetInventory().CreateInInventory( "TacticalGloves_Black" );
		item = oPlayer.GetInventory().CreateInInventory( "HunterPants_Autumn" );
		item = oPlayer.GetInventory().CreateInInventory( "MilitaryBoots_Black" );
		item = oPlayer.GetInventory().CreateInInventory( "AliceBag_Camo" );
		item = oPlayer.GetInventory().CreateInInventory( "M4A1_Black" );

		item.GetInventory().CreateAttachment( "M4_Suppressor" );
		item.GetInventory().CreateAttachment( "M4_RISHndgrd_Black" );
		item.GetInventory().CreateAttachment( "M4_MPBttstck_Black" );
		item.GetInventory().CreateAttachment( "ACOGOptic" );

		auto oMag = oPlayer.GetInventory().CreateInInventory( "Mag_STANAGCoupled_30Rnd" );
		oPlayer.GetInventory().CreateInInventory( "Mag_STANAGCoupled_30Rnd" );
		oPlayer.GetInventory().CreateInInventory( "Mag_STANAGCoupled_30Rnd" );

		oPlayer.LocalTakeEntityToHands( item );

		oPlayer.SetQuickBarEntityShortcut( item, 0, true );

		oPlayer.GetWeaponManager().AttachMagazine( oMag );

		return oPlayer;
	}

	private static TVectorArray GetSpawnPoints() {
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
	}
}