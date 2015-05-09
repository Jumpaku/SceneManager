#pragma once

#include "SceneTree.h"
#include "SceneFactory.h"
#include "SceneTransition.h"

#include"SceneException.h"

/**
*シーンの実行とシーンの移行,シーン生成クラスの管理をする.
*テンプレート引数SceneIDはシーン識別名変数を表す.(列挙体,文字列,通し番号など)
*
*まずregisterScene<Scene>(id)を呼んで使用するシーンクラス(Scene)とそのシーンの識別名(id)を登録する.
*そしてsetFirstScene(id)を呼んで最初のシーンを設定する.
*メインループの中で1ループに1度excuteScene()を実行する.
*終わったらfinalize()を呼ぶ.
*/
/*使用例
int main()
{
	SceneManager manager;
	
	manager.registerScene<scenetest::SceneA>(scenetest::TestSceneID::SCENE_A);
	manager.registerScene<scenetest::SceneC>(scenetest::TestSceneID::SCENE_C);
	manager.registerScene<scenetest::SceneB>(scenetest::TestSceneID::SCENE_B);
	manager.setFirstScene(scenetest::TestSceneID::SCENE_A);

	while (true){
		if (manager.executeScene() != 0){
			break;
		}
	}

	manager.finalize();
}
*/

namespace jumpaku {
namespace scenemanager {

template<typename SceneID>
class SceneManager final
{
private:
	typedef SceneID ID_t;
	typedef SceneTree<SceneID> Tree_t;
	typedef typename Tree<SceneNode<SceneID>>::preorder_iterator Iterator_t;
	typedef SceneFactory<SceneID> Factory_t;
	typedef std::unique_ptr<BaseSceneTransition<SceneID>> Transition_t;
public:
	/***/
	static int const CONTINUE = 1;
	/***/
	static int const FINISH = 0;
private:
	Tree_t tree_m;
	Iterator_t currentScene_m;
	Factory_t factory_m;
private:
	SceneManager(const SceneManager &) = delete;
	SceneManager(SceneManager &&) = delete;
	SceneManager &operator=(const SceneManager &) = delete;
	SceneManager &operator=(SceneManager &&) = delete;
public:
	/**constructor*/
	SceneManager() :tree_m(), currentScene_m(tree_m.end()) {}
	/**destructor*/
	~SceneManager() { finalize(); }
private:
	int transition()
	{
		if(currentScene_m == tree_m.end()) {
			throw SceneLogicException("!transition error : current scene itertor is end of tree!");
		}

		Transition_t transition = currentScene_m->scene_m->decideNext();

		try {
			currentScene_m = transition->transitionScene(
				factory_m, tree_m, currentScene_m);
		}
		catch(SceneLogicException &e) {
			throw SceneLogicException((std::string("!transition error : ") + e.what() + "!").c_str());
		}

		return currentScene_m == tree_m.end() ? FINISH : CONTINUE;
	}
public:
	/**
	*
	*/
	template<class DerivedScene>
	void registerScene(ID_t id)
	{
		factory_m.insertGenerator<DerivedScene>(id);
	}
	/**
	*
	*/
	int executeScene()
	{
		if(currentScene_m == tree_m.end()) { throw SceneLogicException("!execution error : current scene iterator is end of tree!"); }
		currentScene_m->scene_m->doOneFrame();
		try{
			return transition();
		}
		catch(SceneLogicException &e) {
			throw;
		}
	}

	/**
	*
	*/
	void finalize()
	{
		currentScene_m = tree_m.end();
		tree_m.clear();
		factory_m.finalize();
	}

	/**
	*
	*/
	void setFirstScene(ID_t id)
	{
		try {
			currentScene_m = ResetScene<ID_t>(id).transitionScene(factory_m, tree_m, currentScene_m);
		}
		catch(SceneLogicException &e) {
			throw SceneLogicException((std::string("!setting scene error : ") + e.what() + "!").c_str());
		}
	}
};

}
}
