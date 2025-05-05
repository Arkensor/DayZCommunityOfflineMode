/**
\brief Represents active loaded scenes in the game	

*/
class Scene 
{
	ref array<Object> s_Objects = new ref array<Object>; // All active spawned objects
	ref SceneData sceneData;

	void Scene( SceneData data ) 
	{
		sceneData = data;
	}

	void ~Scene() 
	{

	}
}