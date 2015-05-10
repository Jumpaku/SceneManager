#include "./../TestBaseScene.h"

using scenetest::TestBaseScene;
using namespace jumpaku::scenemanager;

void TestBaseScene::finalize()
{
	std::cout << name_m << "::finalize()" << std::endl;
}

void TestBaseScene::initialize()
{
	std::cout << name_m << "::initialize()" << std::endl;
}

void TestBaseScene::doOneFrame()
{
	std::cout << name_m << "::doOneFrame()" << std::endl;

	std::cout << "way (0:KEEP/ 1:CLEAR/ 2:POP/ 3:RESET/ 4:PUSH/ 5:JUMP/ 6:PARENT/ 7:CHILD)-->";
	std::cin >> sharedData()->transition_m;
	if(sharedData()->transition_m < 0 || 7 < sharedData()->transition_m) { sharedData()->transition_m = 0; return; }

	std::cout << "id (0:NULL/ 1:A/ 2:B/ 3:C)-->";
	int id; std::cin >> id;
	if(id < 0 || 3 < id) { sharedData()->id_m = TestSceneID::NULL_SCENE; return; }
	sharedData()->id_m = static_cast<TestSceneID>(id);
}

TestBaseScene::SceneTransition scenetest::TestBaseScene::decideNext()
{
	SceneTransition transition;
	std::cout << name_m << "::decideNext()" << std::endl;
	switch(sharedData()->transition_m) {
	case 0:
		transition = getSceneTransition<KeepScene>();
		break;
	case 1:
		transition = getSceneTransition<ClearScene>();
		break;
	case 2:
		transition = getSceneTransition<PopScene>(sharedData()->id_m);
		break;
	case 3:
		transition = getSceneTransition<ResetScene>(sharedData()->id_m);
		break;
	case 4:
		transition = getSceneTransition<PushScene>(sharedData()->id_m);
		break;
	case 5:
		transition = getSceneTransition<JumpScene>(sharedData()->id_m);
		break;
	case 6:
		transition = getSceneTransition<ParentScene>();
		break;
	case 7:
		transition = getSceneTransition<ChildScene>(sharedData()->id_m);
		break;
	default:
		transition = getSceneTransition<BaseSceneTransition>(sharedData()->id_m);
		break;
	}

	return transition;
}
