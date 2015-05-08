#pragma once

#include "BaseSceneChangeMethod.h"

namespace projectj {
namespace scenemanager {

template<typename SceneID>
class BaseScene;
template<typename SceneID>
class SceneTree;
template<typename SceneID>
class SceneNode;
template<typename SceneID>
class SceneFactory;

}
}

namespace projectj {
namespace scenemanager {

template<typename SceneID>
class KeepScene : public BaseSceneChangeMethod<SceneID>
{
public:
	KeepScene() = default;
	KeepScene(ID_t const &id) :Base_t(id) {}
public:
	Iterator_t changeScene(
		Factory_t const &factory, Tree_t &tree, Iterator_t current) const
	{
		return current;
	}
};


template<typename SceneID>
class ClearScene : public BaseSceneChangeMethod<SceneID>
{
public:
	ClearScene() {}
	ClearScene(ID_t const &id) :Base_t(id) {}
public:
	Iterator_t changeScene(
		Factory_t const &factory, Tree_t &tree, Iterator_t current) const
	{
		tree.clear();
		return tree.end();
	}
};


template<typename SceneID>
class PopScene : public BaseSceneChangeMethod<SceneID>
{
public:
	PopScene() {}
	PopScene(ID_t const &id) :Base_t(id) {}
public:
	Iterator_t changeScene(
		Factory_t const &factory, Tree_t &tree, Iterator_t current) const
	{
		if(current == tree.end()) { return tree.end(); }
		if(current.isRoot()) { return tree.end(); }

		Iterator_t poped = current;

		current.goParent();
		tree.erase(poped);

		return current;
	}
};


template<typename SceneID>
class ResetScene : public BaseSceneChangeMethod<SceneID>
{
public:
	ResetScene(ID_t const &id) :Base_t(id) {}
public:
	Iterator_t changeScene(
		Factory_t const &factory, Tree_t &tree, Iterator_t current) const
	{
		auto scene = factory.getScene(nextID_m);
		if(scene == nullptr) { return tree.end(); }

		tree.clear();
		scene->initialize();

		return tree.setRoot({ nextID_m, scene });
	}
};


template<typename SceneID>
class PushScene : public BaseSceneChangeMethod<SceneID>
{
public:
	PushScene(ID_t const &id) :Base_t(id) {}
public:
	Iterator_t changeScene(
		Factory_t const &factory, Tree_t &tree, Iterator_t current) const
	{
		auto scene = factory.getScene(nextID_m);
		if(scene == nullptr) { return tree.end(); }

		current = tree.insertChild(current, { nextID_m, scene });
		if(current == tree.end()) { return tree.end(); }

		scene->initialize();

		return current;
	}
};


template<typename SceneID>
class JumpScene : public BaseSceneChangeMethod<SceneID>
{
public:
	JumpScene(ID_t const &id) :Base_t(id) {}
public:
	Iterator_t changeScene(
		Factory_t const &factory, Tree_t &tree, Iterator_t current) const
	{
		Iterator_t found = tree.find(nextID_m);

		if(found != tree.end()) {
			return found;
		}
		else{
			return PushScene<ID_t>(nextID_m).changeScene(
				factory, tree, current);
		}
	}
};


template<typename SceneID>
class ParentScene : public BaseSceneChangeMethod<SceneID>
{
public:
	ParentScene() {}
	ParentScene(ID_t const &id) :Base_t(id) {}
public:
	Iterator_t changeScene(
		Factory_t const &factory, Tree_t &tree, Iterator_t current) const
	{
		if(current.isRoot() || current == tree.end()) { return tree.end(); }
		current.goParent();

		return current;
	}
};


template<typename SceneID>
class ChildScene : public BaseSceneChangeMethod<SceneID>
{
public:
	ChildScene(ID_t const &id) :Base_t(id) {}
public:
	Iterator_t changeScene(
		Factory_t const &factory, Tree_t &tree, Iterator_t current) const
	{
		if(current.isLeaf()) { return tree.end(); }

		current.goFirstChild();

		while(true) {
			if(current->id_m == nextID_m) { break; }
			else {
				if(current.isLast()) { return tree.end(); }
				current.goNextSibling();
			}
		}

		return current;
	}
};

}
}
