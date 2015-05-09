#pragma once


#include "BaseSceneTransition.h"

namespace jumpaku {
namespace scenemanager {

template<typename SceneID>
class SceneTransitionFactory
{
protected:
	typedef SceneID ID_t;
	typedef std::unique_ptr<BaseSceneTransition<SceneID>> Transition_t;
public:
	/**
	*
	*/
	template<template <typename> class Method>
	static Transition_t get(ID_t const&nextID)
	{
		return std::make_unique<Method<ID_t>>(nextID);
	}
	/**
	*
	*/
	template<template <typename> class Method>
	static Transition_t get()
	{
		return std::make_unique<Method<ID_t>>();
	}
};

}
}
