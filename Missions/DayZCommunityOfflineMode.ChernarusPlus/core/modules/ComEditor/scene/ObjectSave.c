
// Changed m_SceneObjects to Objects to match vanilla objectSpawnerArr json format.
// Changed param array to ObjectData array to allow proper objectSpawnerArr json format saving.
// - Brandon10x15

class ObjectSave
{
	ref array< ref ObjectData > Objects = new ref array< ref ObjectData >;
} 