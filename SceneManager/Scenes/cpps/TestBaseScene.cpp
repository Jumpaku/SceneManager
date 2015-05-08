#include "./../TestBaseScene.h"

using scenetest::TestBaseScene;
using namespace jumpaku::scenemanager;

TestBaseScene::TestBaseScene() {}

TestBaseScene::~TestBaseScene() {}

int TestBaseScene::finalize()
{
	std::cout << name_m << "::finalize()" << std::endl;
	return 0;
}

int TestBaseScene::initialize()
{
	std::cout << name_m << "::initialize()" << std::endl;
	return 0;
}

int TestBaseScene::doOneFrame()
{
	std::cout << name_m << "::doOneFrame()" << std::endl;

	std::cout << "way (0:KEEP/ 1:CLEAR/ 2:POP/ 3:RESET/ 4:PUSH/ 5:JUMP/ 6:PARENT/ 7:CHILD)-->";
	std::cin >> method_m;
	if(method_m < 0 || 7 < method_m) { return -1; }

	std::cout << "id (0:NULL/ 1:A/ 2:B/ 3:C)-->";
	int id; std::cin >> id;
	if(id < 0 || 3 < id) { return -1; }
	id_m = static_cast<TestSceneID>(id);

	return 0;
}

TestBaseScene::ChangeMethod scenetest::TestBaseScene::decideNext()
{
	ChangeMethod method;
	std::cout << name_m << "::decideNext()" << std::endl;
	switch(method_m) {
	case 0:
		method = MethodFactory::get<KeepScene>();
		break;
	case 1:
		method = MethodFactory::get<ClearScene>();
		break;
	case 2:
		method = MethodFactory::get<PopScene>(id_m);
		break;
	case 3:
		method = MethodFactory::get<ResetScene>(id_m);
		break;
	case 4:
		method = MethodFactory::get<PushScene>(id_m);
		break;
	case 5:
		method = MethodFactory::get<JumpScene>(id_m);
		break;
	case 6:
		method = MethodFactory::get<ParentScene>();
		break;
	case 7:
		method = MethodFactory::get<ChildScene>(id_m);
		break;
	default:
		method = MethodFactory::get<BaseSceneChangeMethod>(id_m);
		break;
	}

	return method;
}
