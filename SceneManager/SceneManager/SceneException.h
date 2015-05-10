#pragma once

#include<stdexcept>
#include<string>

namespace jumpaku{
namespace scenemanager {

struct SceneLogicException : std::logic_error
{
	SceneLogicException(char const *message) :logic_error(message) {}
};

struct SceneRuntimeException : std::runtime_error
{
	SceneRuntimeException(char const *message) :runtime_error(message) {}
};

}
}
