#pragma once

#include <map>
#include <memory>
#include <algorithm>
#include "BaseScene.h"
#include"SceneException.h"

namespace jumpaku {
namespace scenemanager {

template<typename SceneID>
class BaseScene;

}
}

namespace jumpaku {
namespace scenemanager {

template<typename SceneID>
class BaseSceneGenerator
{
private:
	typedef SceneID ID_t;
	typedef std::shared_ptr<BaseScene<SceneID>> SharedScene_t;
public:
	virtual ~BaseSceneGenerator() = default;
	virtual SharedScene_t generateScene() const = 0;
};


template<typename SceneID, class DerivedScene>
class SceneGenerator : public BaseSceneGenerator<SceneID>
{
private:
	typedef SceneID ID_t;
	typedef std::shared_ptr<BaseScene<SceneID>> SharedScene_t;
public:
	SharedScene_t generateScene() const
	{
		try {
			return std::make_shared<DerivedScene>();
		}
		catch(std::bad_alloc &e) {
			throw SceneRuntimeException("scene make_shared bad_alloc");
		}
	}
};


template<typename SceneID>
class SceneIDGeneratorMap final
{
private:
	typedef SceneID ID_t;
	typedef std::shared_ptr<BaseSceneGenerator<SceneID>> Generator_t;

	typedef std::map<SceneID, std::shared_ptr<BaseSceneGenerator<SceneID>>> Map_t;
	typedef std::pair<SceneID, std::shared_ptr<BaseSceneGenerator<SceneID>>> Pair_t;
private:
	Map_t map_m;
private:
	SceneIDGeneratorMap(const SceneIDGeneratorMap &) = delete;
	SceneIDGeneratorMap(SceneIDGeneratorMap &&) = delete;
	SceneIDGeneratorMap &operator=(const SceneIDGeneratorMap &) = delete;
	SceneIDGeneratorMap &operator=(SceneIDGeneratorMap &&) = delete;
public:
	SceneIDGeneratorMap() = default;
	~SceneIDGeneratorMap() = default;
public:
	template<class DerivedScene>
	void insertGenerator(ID_t id)
	{
		try {
			Generator_t generator = std::make_shared<SceneGenerator<ID_t, DerivedScene>>();
		
			if(map_m.find(id) == map_m.end()) {
				map_m.insert(std::make_pair(id, generator));
			}
		}
		catch(std::bad_alloc &e) {
			throw SceneRuntimeException("scene generator make_shared bad_alloc");
		}
	}

	void clearMap()
	{
		map_m.clear();
	}

	Generator_t getGenerator(ID_t &id) const
	{
		if(map_m.find(id) == map_m.end()) { throw SceneLogicException("generator not found"); }

		return map_m.at(id);
	}
};

}
}
