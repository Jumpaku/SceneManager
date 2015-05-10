
#include "SceneManager\SceneManager.h"
#include "Scenes\SceneA.h"
#include "Scenes\SceneB.h"
#include "Scenes\SceneC.h"
#include "Scenes\TestSceneID.h"

using namespace jumpaku::scenemanager;
using namespace scenetest;

int main()
{
	while(true) {
		GameData gd;
		SceneManager<scenetest::TestSceneID, scenetest::GameData> manager(&gd);
		try {
			manager.registerScene<scenetest::SceneA>(scenetest::TestSceneID::SCENE_A);
			manager.registerScene<scenetest::SceneC>(scenetest::TestSceneID::SCENE_C);
			manager.registerScene<scenetest::SceneB>(scenetest::TestSceneID::SCENE_B);
			manager.setFirstScene(scenetest::TestSceneID::SCENE_A);

			while(true) {
				if(manager.executeScene() == SceneManager<scenetest::TestSceneID, scenetest::GameData>::FINISH) {
					break;
				}
			}
		}
		catch(SceneLogicException &e) {
			std::cout << e.what() << "\n";
		}
		catch(SceneRuntimeException &e) {
			std::cout << e.what() << "\n";
			return -1;
		}
		std::cout << "---------------------------------\n";
	}
}
