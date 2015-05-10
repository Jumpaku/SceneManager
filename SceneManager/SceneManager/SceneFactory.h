#pragma once

#include "SceneIDGeneratorMap.h"
namespace jumpaku {
namespace scenemanager {

template<typename SceneID,typename SharedData>
class BaseScene;

}
}
//---------------------------------------------------------------------------------------
//機能, 役割         : シーンの生成をします。
//---------------------------------------------------------------------------------------

namespace jumpaku {
namespace scenemanager {

template<typename SceneID, typename SharedData>
class SceneFactory final
{
private:
	typedef SceneID ID_t;
	typedef SceneIDGeneratorMap<SceneID, SharedData> IDGenMap_t;
	typedef std::shared_ptr<BaseScene<SceneID, SharedData>> SharedScene_t;
	typedef std::shared_ptr<BaseSceneGenerator<SceneID, SharedData>> Generator_t;
private:
	IDGenMap_t idGeneratorMap_m;
	SharedData *sharedData_m;
private:
	SceneFactory(const SceneFactory &) = delete;
	SceneFactory(SceneFactory &&) = delete;
	SceneFactory &operator=(const SceneFactory &) = delete;
	SceneFactory &operator=(SceneFactory &&) = delete;
public:
	/***/
	SceneFactory(SharedData *data) :sharedData_m(data) {}
public:
	/**
	*
	*/
	SharedScene_t getScene(ID_t id) const
	{
		try {
			Generator_t generator = idGeneratorMap_m.getGenerator(id);

			SharedScene_t newScene = generator->generateScene();

			newScene->setSharedData(sharedData_m);

			return newScene;
		}
		catch(SceneLogicException &e) {
			throw SceneLogicException("cannot generate scene");
		}
	}

	/**
	*
	*/
	void finalize()
	{
		idGeneratorMap_m.clearMap();
	}

	/**
	*
	*/
	template<class DerivedScene>
	void insertGenerator(ID_t id)
	{
		idGeneratorMap_m.insertGenerator<DerivedScene>(id);
	}
};

}
}
