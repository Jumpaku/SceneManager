#pragma once

#include "SceneTree.h"
#include "SceneFactory.h"
#include "SceneTransition.h"

#include"SceneException.h"

/**
*シーンの実行とシーンの遷移,生成を管理する.
*テンプレート引数SceneIDはシーン識別名変数を表す.(列挙体,文字列,通し番号など)
*
*まずregisterScene<Scene>(id)を呼んで使用するシーンクラス(Scene)とそのシーンの識別名(id)を登録する
*そしてsetFirstScene(id)を呼んで最初のシーンを設定する
*メインループの中で1ループに1度excuteScene()を実行する
*excuteScene()の返り値がFINISH=0だったらループを抜ける
*registerScene()とsetFirstScene()とexecuteScene()がSceneLogicException例外を投げないようにSceneクラスを派生させる
*SceneRuntimeException例外が投げられたらゲームを終了させる
*
*使用例
int main()
{
	GameData gd;
	SceneManager<scenetest::TestSceneID, scenetest::GameData> manager(&gd);
	try {
		manager.registerScene<scenetest::SceneA>(scenetest::TestSceneID::SCENE_A);
		manager.registerScene<scenetest::SceneC>(scenetest::TestSceneID::SCENE_C);
		manager.registerScene<scenetest::SceneB>(scenetest::TestSceneID::SCENE_B);
		manager.setFirstScene(scenetest::TestSceneID::SCENE_A);

		while(true) {
			if(manager.executeScene() == SceneManager<scenetest::TestSceneID, scenetest::GameData>::FINISH) {
				break;
			}
		}
	}
	catch(SceneRuntimeException &e) {
		std::cout << e.what() << "\n";
		return -1;
	}
}
*/

namespace jumpaku {
namespace scenemanager {

template<typename SceneID,typename SharedData>
class SceneManager final
{
private:
	typedef SceneID ID_t;
	typedef SceneTree<SceneID, SharedData> Tree_t;
	typedef typename Tree<SceneNode<SceneID, SharedData>>::preorder_iterator Iterator_t;
	typedef SceneFactory<SceneID, SharedData> Factory_t;
	typedef std::unique_ptr<BaseSceneTransition<SceneID, SharedData>> Transition_t;
public:
	/**executeScene()がCONTINUEを返したらループを続ける*/
	static int const CONTINUE = 1;
	/**executeScene()がFINISHを返したらループを抜ける*/
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
	SceneManager(SharedData *data) :tree_m(), currentScene_m(tree_m.end()), factory_m(data) {}
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
	*idに対してDerivedSceneを登録する
	*すでに同じidが登録されていたら何もしない
	*@param DerivedScene
	*@param id
	*/
	template<class DerivedScene>
	void registerScene(ID_t id)
	{
		factory_m.insertGenerator<DerivedScene>(id);
	}
	/**
	*現在のシーンを1フレーム分実行し,次のシーンへ遷移させる
	*@return CONTINUE : 次のシーンがある時,FINISH : ない時
	*@exception SceneLogicException : シーン遷移に失敗した時,SceneRuntimeException : シーンの生成に失敗した時
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
	*終了処理を行う,デストラクタ内で呼ばれる
	*/
	void finalize()
	{
		currentScene_m = tree_m.end();
		tree_m.clear();
		factory_m.finalize();
	}

	/**
	*最初のシーンを設定する
	*@param id
	*@exception SceneLogicException : idがまだ登録されていない時,SceneRuntimeException : 最初のシーンの生成に失敗した時
	*/
	void setFirstScene(ID_t id)
	{
		try {
			currentScene_m = ResetScene<ID_t, SharedData>(id).transitionScene(factory_m, tree_m, currentScene_m);
		}
		catch(SceneLogicException &e) {
			throw SceneLogicException((std::string("!scene setting error : ") + e.what() + "!").c_str());
		}
	}
};

}
}
