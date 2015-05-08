#pragma once

#include "SceneIDGeneratorMap.h"
namespace jumpaku {
namespace scenemanager {

template<typename SceneID>
class BaseScene;

}
}
//---------------------------------------------------------------------------------------
//�@�\, ����         : �V�[���̐��������܂��B
//---------------------------------------------------------------------------------------

namespace jumpaku {
namespace scenemanager {

template<typename SceneID>
class SceneFactory final
{
private:
	typedef SceneID ID_t;
	typedef SceneIDGeneratorMap<SceneID> IDGenMap_t;
	typedef std::shared_ptr<BaseScene<SceneID>> SharedScene_t;
	typedef BaseSceneGenerator<SceneID> Generator_t;
private:
	IDGenMap_t idGeneratorMap_m;
private:
	SceneFactory(const SceneFactory &) = delete;
	SceneFactory(SceneFactory &&) = delete;
	SceneFactory &operator=(const SceneFactory &) = delete;
	SceneFactory &operator=(SceneFactory &&) = delete;
public:
	/***/
	SceneFactory() = default;
public:
	/**
	*
	*/
	SharedScene_t getScene(ID_t id) const
	{
		Generator_t *generator = nullptr;
		SharedScene_t newScene = nullptr;

		generator = idGeneratorMap_m.getGenerator(id);
		if(generator != nullptr) {
			newScene = generator->generateScene();
		}

		return newScene;
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
	int insertGenerator(ID_t id)
	{
		return idGeneratorMap_m.insertGenerator<DerivedScene>(id);
	}
};

}
}
