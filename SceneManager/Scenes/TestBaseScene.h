#pragma once

#include "./../SceneManager/BaseScene.h"
#include "TestSceneID.h"
#include <string>
#include <iostream>

namespace scenetest {

struct GameData
{
	int transition_m;
	TestSceneID id_m;
};

typedef jumpaku::scenemanager::BaseScene<scenetest::TestSceneID, GameData> Scene;

class TestBaseScene : public Scene
{
protected:
	std::string name_m = "TestBaseScene";
public:
	TestBaseScene() = default;
	virtual ~TestBaseScene() = default;
public:
	void finalize()override;
	void initialize()override;
	void doOneFrame()override;
	SceneTransition decideNext()override;
};

}

