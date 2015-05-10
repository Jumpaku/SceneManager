#pragma once

#include "SceneTransitionFactory.h"
#include"SceneException.h"

namespace jumpaku {
namespace scenemanager {

template<typename SceneID, typename SharedData>
class BaseSceneTransition;
template<typename SceneID, typename SharedData>
class SceneTransitionFactory;

}
}

/**
*シーンクラスのインターフェイスのテンプレート.
*複雑な初期化と終了処理はinitialize()とfinalize()で行う.
*テンプレート引数SceneIDはシーン識別名変数の型を表す.(列挙体,文字列,通し番号など)
*シーンクラスはBaseSceneを継承する.
*finalize(), initialize(), doOneFrame(), decideNext()をオーバーライドする.
*
*/

namespace jumpaku {
namespace scenemanager {

template<typename SceneID, typename SharedData>
class BaseScene
{
	friend SceneFactory < SceneID, SharedData > ;
public:
	/**シーン識別名の型*/
	typedef SceneID ID;
	/**シーンの遷移方法クラスの基本クラス*/
	typedef std::unique_ptr<BaseSceneTransition<SceneID, SharedData>> SceneTransition;
private:
	SharedData *sharedData_m;
public:
	/**default constructor*/
	BaseScene() = default;
	/**default denstructor*/
	virtual ~BaseScene() = default;
private:
	void setSharedData(SharedData *data)
	{
		sharedData_m = data;
	}
protected:
	/**
	*共有データへのポインタを取得する
	*@return pointer of shared data
	*/
	SharedData *sharedData()
	{
		return sharedData_m;
	}
	/**
	*Transition = ResetScene : シーンを全て削除してから新しくidのシーンを生成しそのシーンへ遷移する
	*Transition = PushScene : idのシーンを生成し子のシーンとしてそこへ遷移する
	*Transition = JumpScene : 全てのシーンの中からidのシーンを探してそこへ遷移する,もし見つからなければidのシーンをpushする
	*Transition = ChildScene : 子のシーンの中からidのシーンを探してそこへ遷移する
	*@param scene id of naxt sscene
	*@return instance of transition class
	*/
	template<template <typename, typename> class Transition>
	static SceneTransition getSceneTransition(SceneID id)
	{
		return SceneTransitionFactory<SceneID, SharedData>::get<Transition>(id);
	}
	/**
	*Transition = KeepScene : シーン遷移しないで今のシーンのまま次のフレームに行く
	*Transition = ClearScene : シーンを全て削除してゲームを終了する
	*Transition = popScene : 今のシーンを削除して1つ親のシーンへ遷移する
	*Transition = ParentScene : 今のシーンを残して1つ親のシーンへ遷移する
	*@return instance of transition class
	*/
	template<template <typename, typename> class Transition>
	static SceneTransition getSceneTransition()
	{
		return SceneTransitionFactory<SceneID, SharedData>::get<Transition>();
	}
public:
	/**
	*1ループに1度doOneFrame()の後に呼ばれ,次のシーンへの遷移方法クラスのインスタンスを返す.
	*内部でgetSceneTransitionを使ってシーン遷移方法を生成する
	*@return シーン遷移方法
	*/ 
	virtual SceneTransition decideNext() = 0;
	/**
	*ゲームのメインループの本体であり,1ループに1度呼ばれる. 
	*/
	virtual void doOneFrame() = 0;
	/**
	*SceneTreeから削除される時に1度だけ呼ばれ,終了処理を行う. 
	*/
	virtual void finalize() = 0;
	/**
	*SceneTreeに追加された時に1度だけ呼ばれ, 初期化処理を行う.
	*/
	virtual void initialize() = 0;
};

}
}

#include "SceneTransition.h"

