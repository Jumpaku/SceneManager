#pragma once

#include "./../SceneManager/BaseScene.h"
#include "TestSceneID.h"
#include <string>
#include <iostream>

namespace scenetest {

typedef projectj::scenemanager::BaseScene<scenetest::TestSceneID> Scene;

class TestBaseScene : public Scene
{
protected:
	std::string name_m = "TestBaseScene";
private:
	int method_m;
	ID id_m;
public:
	TestBaseScene();
	virtual ~TestBaseScene();
public:
	int finalize();
	int initialize();
	int doOneFrame();
	ChangeMethod decideNext();
};

}

