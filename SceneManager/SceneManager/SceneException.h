#pragma once

#include<stdexcept>
#include<string>

namespace jumpaku{
namespace scenemanager {

struct SceneException : std::exception
{
	SceneException(char const *message) :exception(message) {}
};

}
}
