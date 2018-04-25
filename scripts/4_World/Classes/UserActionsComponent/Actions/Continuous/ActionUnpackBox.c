class ActionUnpackBoxCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.UNPACK);
	}
};

class ActionUnpackBox: ActionContinuousBase
{	
	private const float PRECISE_SPECIALTY_WEIGHT = -0.01;

	void ActionUnpackBox()
	{
		m_CallbackClass = ActionUnpackBoxCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EMPTYMAG;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_EMPTYMAG;
		

		
		m_MessageStartFail = "I cannot unpack this.";
		m_MessageStart = "I have started unpacking the box.";
		m_MessageSuccess = "I have unpacked the box.";
		m_MessageFail = "I have moved and unpacking was canceled.";
		m_MessageCancel = "I stopped unpacking.";
		m_Sounds.Insert("bandage_0"); //sould be original sound effects?
		m_Sounds.Insert("bandage_1");
		m_Sounds.Insert("bandage_2");	
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	
	override bool HasProneException()
	{
		return true;
	}
		
	override int GetType()
	{
		return AT_UNPACK_BOX;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "unbox";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		if ( item && item.GetHierarchyRootPlayer() == player )
		{
			string path = "CfgVehicles " + item.GetType();
			string child_name = "";
			int count;
			array<string> resources = new array<string>;
			
			if( GetGame().ConfigIsExisting( path ) && GetGame().ConfigIsExisting( path + " Resources") )
			{
				path = path + " Resources";
				count = GetGame().ConfigGetChildrenCount ( path );
				for (int i = 0; i < count; i++)
				{
					GetGame().ConfigGetChildName ( path, i, child_name );
					
					if ( GetGame().ConfigGetInt( path + " " + child_name + " value" ) )
					{
						resources.Insert( child_name );
					}
				}

				//TODO modify to allow for multiple ammo types spawning (if needed??)
				string itemType = resources.Get(0);
				int itemCount = GetGame().ConfigGetInt( path + " " + itemType + " value" );
				
				UnboxLambda lambda = new UnboxLambda(item, itemType, player, itemCount);
				player.LocalReplaceItemInHandsWithNew(lambda);
				
				//spawns wrapping Paper
				ItemBase paper = ItemBase.Cast( GetGame().CreateObject("Paper", player.GetPosition(), false) );
			}
		}
	}
};

class UnboxLambda : ReplaceItemWithNewLambdaBase
{
	int m_ItemCount;
	void UnboxLambda (EntityAI old_item, string new_item_type, PlayerBase player, int count) { m_ItemCount = count; }

	override void CopyOldPropertiesToNew (notnull EntityAI old_item, notnull EntityAI new_item)
	{
		super.CopyOldPropertiesToNew(old_item, new_item);

		if ( GetGame().ConfigIsExisting( "CfgMagazines " + m_NewItemType ) )
		{
			Magazine pile;
			Class.CastTo(pile, new_item);
			pile.ServerSetAmmoCount(m_ItemCount);
			Print("UnboxLambda::SetupNewMagazine new_item=" + new_item + " count=" + m_ItemCount);
		}
		else
		{
			ItemBase unboxed;
			Class.CastTo(unboxed, new_item);
			unboxed.SetQuantity(m_ItemCount);
			Print("UnboxLambda::SetupNewItem new_item=" + new_item + " count=" + m_ItemCount);
		}
	}
};

