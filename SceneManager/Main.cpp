
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
		SceneManager<scenetest::TestSceneID> manager;
		try {

			manager.registerScene<scenetest::SceneA>(scenetest::TestSceneID::SCENE_A);
			manager.registerScene<scenetest::SceneC>(scenetest::TestSceneID::SCENE_C);
			manager.registerScene<scenetest::SceneB>(scenetest::TestSceneID::SCENE_B);
			manager.setFirstScene(scenetest::TestSceneID::SCENE_A);

			while(true) {
				if(manager.executeScene() == SceneManager<scenetest::TestSceneID>::FINISH) {
					manager.finalize();
					break;
				}
			}
		}
		catch(SceneException &e) {
			std::cout << e.what() << "\n";
			manager.finalize();
		}
		std::cout << "---------------------------------\n";

	}
}
