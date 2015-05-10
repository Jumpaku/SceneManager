#include "../SceneB.h"

scenetest::SceneB::SceneB()
{
	name_m = "SceneB";
	std::cout << name_m << "()" << std::endl;
}

scenetest::SceneB::~SceneB()
{
	std::cout << "~" << name_m << "()" << std::endl;
}