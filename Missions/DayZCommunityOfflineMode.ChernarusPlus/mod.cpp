class CfgMissions
{
	class Missions
	{
		class DayZCommunityOfflineMode
		{
			directory="Missions\DayZCommunityOfflineMode.ChernarusPlus";
			dir = "Missions\DayZCommunityOfflineMode.ChernarusPlus";
			picture = "";
			action = "";
			hideName = 1;
			hidePicture = 1;
			name = "DayZ Community Offline Mode";
			credits = "";
			author = "Arkensor, Jacob_Mango, DannyDog";
			authorID = "0"; 
			version = "1.0"; 
			extra = 0;
			type = "mission";
			
			dependencies[]={"Game", "World", "Mission"};
			
			class defs
			{
				class gameScriptModule
				{
					value = "";
					files[] = {"nst/ns_dayz/scripts/3_Game"};
				};

				class worldScriptModule
				{
					value = "";
					files[] = {"nst/ns_dayz/scripts/4_World"};
				};

				class missionScriptModule
				{
					value = "";
					files[] = {"nst/ns_dayz/scripts/5_Mission"};
				};
			}
		};
	};
};
class CfgMods
{
	class DayZCommunityOfflineMode
	{
		directory="Missions\DayZCommunityOfflineMode.ChernarusPlus";
		dir = "Missions\DayZCommunityOfflineMode.ChernarusPlus";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "DayZ Community Offline Mode";
		credits = "";
		author = "Arkensor, Jacob_Mango, DannyDog";
		authorID = "0"; 
		version = "1.0"; 
		extra = 0;
		type = "mission";
		
		dependencies[]={"Game", "World", "Mission"};
		
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {"nst/ns_dayz/scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"nst/ns_dayz/scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"nst/ns_dayz/scripts/5_Mission"};
			};
		}
	};
};