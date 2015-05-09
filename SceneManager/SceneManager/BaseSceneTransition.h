#pragma once

#include "SceneFactory.h"
#include "SceneTree.h"
namespace jumpaku {
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

namespace jumpaku {
namespace scenemanager {

/**
*
*/
template<typename SceneID>
class BaseSceneTransition
{
protected:
	/***/
	typedef SceneID ID_t;
	/***/
	typedef BaseSceneTransition<SceneID> Base_t;
	/***/
	typedef typename Tree<SceneNode<SceneID>>::preorder_iterator Iterator_t;
	/***/
	typedef SceneFactory<SceneID> Factory_t;
	/***/
	typedef SceneTree<SceneID> Tree_t;
	/***/
	typedef SceneNode<SceneID> Node_t;
	/***/
	typedef std::shared_ptr<BaseScene<SceneID>> SharedScene_t;
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