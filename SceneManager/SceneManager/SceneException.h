#pragma once

#include<stdexcept>

namespace jumpaku{
namespace scenemanager {

struct SceneException : std::exception
{
	SceneException(char const *message) :exception(message) {}
};

}
}
