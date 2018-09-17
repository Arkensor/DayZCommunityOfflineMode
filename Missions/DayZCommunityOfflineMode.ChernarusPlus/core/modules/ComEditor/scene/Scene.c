class Scene //temp class
{
	string name;

	ref array<ref Param3<string, vector, vector>> m_SceneObjects; // class name, position, orientation

	void Scene() 
	{
		m_SceneObjects = new array<ref Param3<string, vector, vector>>;
	}
}