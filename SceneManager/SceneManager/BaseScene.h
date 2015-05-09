#pragma once

#include "SceneTransitionFactory.h"

namespace jumpaku {
namespace scenemanager {

template<typename SceneID>
class BaseSceneTransition;
template<typename SceneID>
class SceneTransitionFactory;

}
}

/**
*シーンクラスのインターフェイスのテンプレート.
*複雑な初期化と終了処理はinitialize()とfinalize()で行う.
*テンプレート引数SceneIDはシーン識別名変数の型を表す.(列挙体,文字列,通し番号など)
*シーンクラスはBaseSceneを継承する.
*finalize(), initialize(), doOneFrame(), decideNext()をオーバーライドする.
*/

namespace jumpaku {
namespace scenemanager {

	template<typename SceneID>
	class BaseScene
{
public:
	/**シーン識別名の型*/
	typedef SceneID ID;
	/**シーンの遷移方法クラスの基本クラス*/
	typedef std::unique_ptr<BaseSceneTransition<SceneID>> SceneTransition;
public:
	/**default constructor*/
	BaseScene() = default;
	/**default denstructor*/
	virtual ~BaseScene() = default;
protected:
	template<template <typename> class Method>
	static SceneTransition getSceneTransition(SceneID id)
	{
		return SceneTransitionFactory<SceneID>::get<Method>(id);
	}
	template<template <typename> class Method>
	static SceneTransition getSceneTransition()
	{
		return SceneTransitionFactory<SceneID>::get<Method>();
	}
public:
	/**
	*1ループに1度execute()の後に呼ばれ,次のシーンへの遷移方法クラスを返す.
	*@return シーン遷移方法
	*/ 
	virtual SceneTransition decideNext() = 0;
	/**
	*ゲームのメインループの本体であり,1ループに1度呼ばれる. 成功:0 / 失敗:-1
	*/
	virtual int doOneFrame() = 0;
	/**
	*SceneTreeから削除される時に呼ばれ,終了処理を行う. 成功:0 / 失敗:-1
	*/
	virtual int finalize() = 0;
	/**
	*SceneTreeに追加された時に呼ばれ, 初期化処理を行う.成功:0 / 失敗:-1
	*/
	virtual int initialize() = 0;
};

}
}

#include "SceneTransition.h"

