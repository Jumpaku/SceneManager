
#include "SceneManager\SceneManager.h"
#include "Scenes\SceneA.h"
#include "Scenes\SceneB.h"
#include "Scenes\SceneC.h"
#include "Scenes\TestSceneID.h"

using namespace projectj::scenemanager;
using namespace scenetest;

int main()
{
	SceneManager<scenetest::TestSceneID> manager;
	
	manager.registerScene<scenetest::SceneA>(scenetest::TestSceneID::SCENE_A);
	manager.registerScene<scenetest::SceneC>(scenetest::TestSceneID::SCENE_C);
	manager.registerScene<scenetest::SceneB>(scenetest::TestSceneID::SCENE_B);
	manager.setFirstScene(scenetest::TestSceneID::SCENE_A);

	while (true){
		if (manager.executeScene() != 0){
			break;
		}
	}

	manager.finalize();

}
