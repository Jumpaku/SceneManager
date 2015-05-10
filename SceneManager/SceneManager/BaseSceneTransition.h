#pragma once

#include "SceneFactory.h"
#include "SceneTree.h"
#include"SceneException.h"

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
class BaseSceneTransition
{
protected:
	/***/
	typedef SceneID ID_t;
	/***/
	typedef BaseSceneTransition<SceneID, SharedData> Base_t;
	/***/
	typedef typename Tree<SceneNode<SceneID, SharedData>>::preorder_iterator Iterator_t;
	/***/
	typedef SceneFactory<SceneID, SharedData> Factory_t;
	/***/
	typedef SceneTree<SceneID, SharedData> Tree_t;
	/***/
	typedef SceneNode<SceneID, SharedData> Node_t;
	/***/
	typedef std::shared_ptr<BaseScene<SceneID, SharedData>> SharedScene_t;
protected:
	/***/
	ID_t nextID_m;
public:
	/**
	*
	*/
	BaseSceneTransition(ID_t const &id) :nextID_m(id) {}
	/***/
	BaseSceneTransition() = default;
	/***/
	virtual ~BaseSceneTransition() = default;
public:
	/**
	*
	*/
	virtual Iterator_t transitionScene(
		Factory_t const &factory, Tree_t &tree, Iterator_t current) const
	{
		return tree.end();
	}
};

}
}