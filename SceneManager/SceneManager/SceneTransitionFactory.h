#pragma once


#include "BaseSceneTransition.h"

namespace jumpaku {
namespace scenemanager {

template<typename SceneID, typename SharedData>
class SceneTransitionFactory
{
protected:
	typedef SceneID ID_t;
	typedef std::unique_ptr<BaseSceneTransition<SceneID, SharedData>> Transition_t;
public:
	/**
	*
	*/
	template<template <typename, typename> class Transition>
	static Transition_t get(ID_t const&nextID)
	{
		return std::make_unique<Transition<ID_t, SharedData>>(nextID);
	}
	/**
	*
	*/
	template<template <typename, typename> class Transition>
	static Transition_t get()
	{
		return std::make_unique<Transition<ID_t, SharedData>>();
	}
};

}
}
