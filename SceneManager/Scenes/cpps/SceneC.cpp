#include "../SceneC.h"

scenetest::SceneC::SceneC()
{
	name_m = "SceneC";
	std::cout << name_m << "()" << std::endl;
}

scenetest::SceneC::~SceneC()
{
	std::cout << "~" << name_m << "()" << std::endl;
}