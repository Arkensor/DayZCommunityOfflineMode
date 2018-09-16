modded class DayZPlayerCamera1stPerson
{   
    void DayZPlayerCamera1stPerson( DayZPlayer pPlayer, HumanInputController pInput )
    {
        m_iBoneIndex = pPlayer.GetBoneIndexByName("Face_Forehead");
        if (m_iBoneIndex == -1)
        {
            Print("modded DayZPlayerCamera1stPerson: main bone not found");
        }
    }
}