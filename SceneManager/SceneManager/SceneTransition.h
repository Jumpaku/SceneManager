#pragma once

#include "BaseSceneTransition.h"

namespace jumpaku {
namespace scenemanager {

template<typename SceneID, typename SharedData>
class BaseScene;
template<typename SceneID, typename SharedData>
class SceneTree;
template<typename SceneID, typename SharedData>
class SceneNode;
template<typename SceneID, typename SharedData>
class SceneFactory;

}
}

namespace jumpaku {
namespace scenemanager {

/**
*
*/
template<typename SceneID, typename SharedData>
class KeepScene : public BaseSceneTransition<SceneID, SharedData>
{
public:
	KeepScene() = default;
	KeepScene(ID_t const &id) :Base_t(id) {}
public:
	Iterator_t transitionScene(
		Factory_t const &factory, Tree_t &tree, Iterator_t current) const
	{
		return current;
	}
};

/**
*
*/
template<typename SceneID, typename SharedData>
class ClearScene : public BaseSceneTransition<SceneID, SharedData>
{
public:
	ClearScene() = default;
	ClearScene(ID_t const &id) :Base_t(id) {}
public:
	Iterator_t transitionScene(
		Factory_t const &factory, Tree_t &tree, Iterator_t current) const
	{
		tree.clear();
		return tree.end();
	}
};

/**
*
*/
template<typename SceneID, typename SharedData>
class PopScene : public BaseSceneTransition<SceneID, SharedData>
{
public:
	PopScene() = default;
	PopScene(ID_t const &id) :Base_t(id) {}
public:
	Iterator_t transitionScene(
		Factory_t const &factory, Tree_t &tree, Iterator_t current) const
	{
		if(current == tree.end()) { throw SceneLogicException("connot pop"); }
		if(current.isRoot()) { throw SceneLogicException("connot pop"); }

		Iterator_t poped = current;

		current.goParent();
		tree.erase(poped);

		return current;
	}
};

/**
*
*/
template<typename SceneID, typename SharedData>
class ResetScene : public BaseSceneTransition<SceneID, SharedData>
{
public:
	ResetScene(ID_t const &id) :Base_t(id) {}
public:
	Iterator_t transitionScene(
		Factory_t const &factory, Tree_t &tree, Iterator_t current) const
	{
		try {
			auto scene = factory.getScene(nextID_m);

			tree.clear();

			scene->initialize();

			return tree.setRoot({ nextID_m, scene });
		}
		catch(SceneLogicException &e) {
			throw SceneLogicException("cannot reset");
		}
	}
};

/**
*
*/
template<typename SceneID, typename SharedData>
class PushScene : public BaseSceneTransition<SceneID, SharedData>
{
public:
	PushScene(ID_t const &id) :Base_t(id) {}
public:
	Iterator_t transitionScene(
		Factory_t const &factory, Tree_t &tree, Iterator_t current) const
	{
		try {
			auto scene = factory.getScene(nextID_m);
			current = tree.insertChild(current, { nextID_m, scene });
		
			if(current == tree.end()) { throw SceneLogicException("connot push"); }

			current->scene_m->initialize();
		}
		catch(SceneLogicException &e) {
			throw SceneLogicException("connot push");
		}
		return current;
		
	}
};

/**
*
*/
template<typename SceneID, typename SharedData>
class JumpScene : public BaseSceneTransition<SceneID, SharedData>
{
public:
	JumpScene(ID_t const &id) :Base_t(id) {}
public:
	Iterator_t transitionScene(
		Factory_t const &factory, Tree_t &tree, Iterator_t current) const
	{
		Iterator_t found = tree.find(nextID_m);

		if(found != tree.end()) {
			return found;
		}
		else{
			try {
				return PushScene<SceneID, SharedData>(nextID_m).transitionScene(
					factory, tree, current);
			}
			catch(SceneLogicException &e) {
				throw SceneLogicException("cannot jump");
			}
		}
	}
};

/**
*
*/
template<typename SceneID, typename SharedData>
class ParentScene : public BaseSceneTransition<SceneID, SharedData>
{
public:
	ParentScene() = default;
	ParentScene(ID_t const &id) :Base_t(id) {}
public:
	Iterator_t transitionScene(
		Factory_t const &factory, Tree_t &tree, Iterator_t current) const
	{
		if(current.isRoot() || current == tree.end()) { throw SceneLogicException("connot go parent"); }
		current.goParent();

		return current;
	}
};

/**
*
*/
template<typename SceneID, typename SharedData>
class ChildScene : public BaseSceneTransition<SceneID, SharedData>
{
public:
	ChildScene(ID_t const &id) :Base_t(id) {}
public:
	Iterator_t transitionScene(
		Factory_t const &factory, Tree_t &tree, Iterator_t current) const
	{
		if(current.isLeaf()) { throw SceneLogicException("connot go child"); }

		current.goFirstChild();

		while(true) {
			if(current->id_m == nextID_m) { break; }
			else {
				if(current.isLast()) { throw SceneLogicException("connot go child"); }
				current.goNextSibling();
			}
		}

		return current;
	}
};

}
}
