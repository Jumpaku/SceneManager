#pragma once


#include "BaseSceneChangeMethod.h"

namespace projectj {
namespace scenemanager {

template<typename SceneID>
class SceneChangeMethodFactory
{
protected:
	typedef SceneID ID_t;
	typedef std::unique_ptr<BaseSceneChangeMethod<SceneID>> ChangeMethod_t;
public:
	template<template <typename> class Method>
	static ChangeMethod_t get(ID_t const&nextID)
	{
		return std::make_unique<Method<ID_t>>(nextID);
	}
	template<template <typename> class Method>
	static ChangeMethod_t get()
	{
		return std::make_unique<Method<ID_t>>();
	}
};

}
}
