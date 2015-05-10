#include "../SceneA.h"

scenetest::SceneA::SceneA()
{
	name_m = "SceneA";
	std::cout << name_m << "()" << std::endl;
}

scenetest::SceneA::~SceneA()
{
	std::cout << "~" << name_m << "()" << std::endl;
}