#pragma once

#include "SceneChangeMethodFactory.h"
namespace projectj {
namespace scenemanager {

template<typename SceneID>
class BaseSceneChangeMethod;
template<typename SceneID>
class SceneChangeMethodFactory;

}
}

//説明
/*--------------------------------------------------------------------------------------
シーンクラスのインターフェイスのテンプレート.
複雑な初期化と終了処理はinitialize()とfinalize()で行う.
テンプレート引数SceneIDはシーン識別名変数の型を表す.(列挙体,文字列,通し番号など)
シーンクラスはBaseSceneを継承する.
finalize(), initialize(), doOneFrame(), decideNext()をオーバーライドする.
---------------------------------------------------------------------------------------*/

namespace projectj {
namespace scenemanager {

	template<typename SceneID>
	class BaseScene
{
public:
	//シーン識別名変数
	typedef SceneID ID;
	//シーンの遷移方法クラスの基本クラス
	typedef std::unique_ptr<BaseSceneChangeMethod<SceneID>> ChangeMethod;
	//ChangeMethodFactory::get<遷移方法クラス>(次のシーン識別名)を呼んで次のシーンへの遷移方法クラスを得る事ができる.
	typedef SceneChangeMethodFactory<SceneID> MethodFactory;
public:
	BaseScene() {}
	virtual ~BaseScene() {}
public:
	//1ループに1度execute()の後に呼ばれ,次のシーンへの遷移方法クラスを返す. 
	virtual ChangeMethod decideNext() = 0;
	//ゲームのメインループの本体であり,1ループに1度呼ばれる. 成功:0 / 失敗:-1
	virtual int doOneFrame() = 0;
	//SceneTreeから削除される時に呼ばれ,終了処理を行う. 成功:0 / 失敗:-1
	virtual int finalize() = 0;
	//SceneTreeに追加された時に呼ばれ, 初期化処理を行う.成功:0 / 失敗:-1
	virtual int initialize() = 0;
};

}
}

#include "SceneChangeMethod.h"

