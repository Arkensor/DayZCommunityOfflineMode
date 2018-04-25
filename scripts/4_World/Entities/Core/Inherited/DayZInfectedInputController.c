class DayZInfectedInputController
{
	proto native float GetMovementSpeed();
	proto native int GetAlertLevel();
	proto native bool IsVault();
	proto native float GetVaultHeight();
	
	proto native int GetMindState();
	proto native EntityAI GetTargetEntity();
	proto native bool IsLookAtEnabled();
	proto native vector GetLookAtDirectionWS();
	
	//--------------------------------------------------------------
	//! never created by script
	private void DayZInfectedInputController()
	{
	}

	//! never destroyed by script
	private void ~DayZInfectedInputController()
	{
	}	
};