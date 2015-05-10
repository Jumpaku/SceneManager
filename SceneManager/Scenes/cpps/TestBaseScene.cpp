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
	std::cin >> method_m;
	if(method_m < 0 || 7 < method_m) { method_m = 0; return; }

	std::cout << "id (0:NULL/ 1:A/ 2:B/ 3:C)-->";
	int id; std::cin >> id;
	if(id < 0 || 3 < id) { id_m = TestSceneID::NULL_SCENE; return; }
	id_m = static_cast<TestSceneID>(id);
}

TestBaseScene::SceneTransition scenetest::TestBaseScene::decideNext()
{
	SceneTransition method;
	std::cout << name_m << "::decideNext()" << std::endl;
	switch(method_m) {
	case 0:
		method = getSceneTransition<KeepScene>();
		break;
	case 1:
		method = getSceneTransition<ClearScene>();
		break;
	case 2:
		method = getSceneTransition<PopScene>(id_m);
		break;
	case 3:
		method = getSceneTransition<ResetScene>(id_m);
		break;
	case 4:
		method = getSceneTransition<PushScene>(id_m);
		break;
	case 5:
		method = getSceneTransition<JumpScene>(id_m);
		break;
	case 6:
		method = getSceneTransition<ParentScene>();
		break;
	case 7:
		method = getSceneTransition<ChildScene>(id_m);
		break;
	default:
		method = getSceneTransition<BaseSceneTransition>(id_m);
		break;
	}

	return method;
}
