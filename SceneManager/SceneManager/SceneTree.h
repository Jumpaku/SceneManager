#pragma once

#include <memory>
#include <algorithm>
#include "../Tree/Tree.h"
#include "BaseScene.h"

namespace jumpaku {
namespace scenemanager {

template<typename SceneID, typename SharedData>
class BaseScene;

}
}

namespace jumpaku {
namespace scenemanager {

template<typename SceneID,typename SharedData>
class SceneNode
{
private:
	typedef SceneID ID_t;
	typedef std::shared_ptr<BaseScene<SceneID, SharedData>> SharedScene_t;
public:
	/***/
	ID_t id_m;
	/***/
	SharedScene_t scene_m;

	/***/
	SceneNode() = default;
	/***/
	SceneNode(ID_t const &id, SharedScene_t const &shared)
		:id_m(id), scene_m(shared)
	{}

	/***/
	bool operator==(const SceneNode scene)const { return id_m == scene.id_m; }
	/***/
	bool operator!=(const SceneNode scene)const { return id_m != scene.id_m; }
	/***/
	bool operator<=(const SceneNode scene)const { return id_m <= scene.id_m; }
	/***/
	bool operator>=(const SceneNode scene)const { return id_m >= scene.id_m; }
	/***/
	bool operator<(const SceneNode scene)const { return id_m < scene.id_m; }
	/***/
	bool operator>(const SceneNode scene)const { return id_m > scene.id_m; }
};


template<typename SceneID,typename SharedData>
class SceneTree final
{
private:
	typedef SceneID ID_t;
	typedef SceneNode<SceneID, SharedData>  Node_t;
	typedef std::shared_ptr<BaseScene<SceneID, SharedData>> SharedScene_t;
	typedef typename Tree<SceneNode<SceneID, SharedData>>::preorder_iterator Iterator_t;
	typedef Tree<SceneNode<SceneID, SharedData>> Tree_t;
private:
	Tree_t sceneTree_m;
public:
	/***/
	SceneTree() = default;
	/***/
	~SceneTree() = default;
private:
	SceneTree(const SceneTree &) = delete;
	SceneTree(SceneTree &&) = delete;
	SceneTree &operator=(const SceneTree &) = delete;
	SceneTree &operator=(SceneTree &&) = delete;
private:
	void deleteScene(Iterator_t const &pos)
	{
		auto begin = pos, end = pos;
		auto finalize = [](Node_t &node)
		{
			node.scene_m->finalize();
			node.scene_m = nullptr;
		};

		if(end.isTail()) { return; }
		while(end.hasParent() && end.isLast()) { end.goParent(); }
		end.goNextSibling();
		
		std::for_each(begin, end, finalize);
	}
public:
	/**
	*
	*/
	void erase(Iterator_t &pos)
	{
		deleteScene(pos);
		sceneTree_m.erase(pos);
	}

	/**
	*
	*/
	Iterator_t begin()const
	{
		return sceneTree_m.begin();
	}

	/**
	*
	*/
	Iterator_t end()const
	{
		return sceneTree_m.end();
	}

	/**
	*
	*/
	void clear()
	{
		deleteScene(begin());
		sceneTree_m.clear();
	}

	/**
	*
	*/
	Iterator_t setRoot(Node_t const &node)
	{
		return sceneTree_m.set_root(node);
	}

	/**
	*
	*/
	Iterator_t insertChild(
		Iterator_t &parent, Node_t const &node)
	{
		return sceneTree_m.insert_child(parent, node);
	}

	/**
	*
	*/
	Iterator_t find(ID_t const &id)
	{
		return sceneTree_m.find({ id, nullptr });
	}
};

}
}
