#pragma once

#include <map>
#include <memory>
#include <algorithm>
#include "BaseScene.h"
namespace projectj {
namespace scenemanager {
template<typename SceneID>
class BaseScene;
}
}

namespace projectj {
namespace scenemanager {

template<typename SceneID>
class BaseSceneGenerator
{
private:
	typedef SceneID ID_t;
	typedef std::shared_ptr<BaseScene<SceneID>> SharedScene_t;
public:
	BaseSceneGenerator() {};
	virtual ~BaseSceneGenerator() {};
	virtual SharedScene_t generateScene() const = 0;
};


template<typename SceneID, class DerivedScene>
class SceneGenerator : public BaseSceneGenerator<SceneID>
{
private:
	typedef SceneID ID_t;
	typedef std::shared_ptr<BaseScene<SceneID>> SharedScene_t;
public:
	SceneGenerator() {};
	~SceneGenerator() {};
public:
	SharedScene_t generateScene() const
	{
		return std::make_shared<DerivedScene>();
	}
};


template<typename SceneID>
class SceneIDGeneratorMap final
{
private:
	typedef SceneID ID_t;
	typedef BaseSceneGenerator<SceneID> Generator_t;

	typedef std::map<SceneID, BaseSceneGenerator<SceneID> *> Map_t;
	typedef std::pair<SceneID, BaseSceneGenerator<SceneID> *> Pair_t;
private:
	Map_t map_m;
private:
	SceneIDGeneratorMap(const SceneIDGeneratorMap &) = delete;
	SceneIDGeneratorMap(SceneIDGeneratorMap &&) = delete;
	SceneIDGeneratorMap &operator=(const SceneIDGeneratorMap &) = delete;
	SceneIDGeneratorMap &operator=(SceneIDGeneratorMap &&) = delete;
public:
	SceneIDGeneratorMap() {}
	~SceneIDGeneratorMap() {}
public:
	template<class DerivedScene>
	int insertGenerator(ID_t id)
	{
		return insertGenerator(id, new SceneGenerator<ID_t, DerivedScene>());
	}

	int insertGenerator(ID_t id, Generator_t *generator)
	{
		if(generator == nullptr) { return -1; }
		if(map_m.find(id) != map_m.end()) { return -1; }

		map_m.insert(std::make_pair(id, generator));

		return 0;
	}

	void clearMap()
	{
		std::for_each(map_m.begin(), map_m.end(), [](Pair_t pair)
		{
			delete pair.second;
			pair.second = nullptr;
		});
		map_m.clear();
	}

	Generator_t *getGenerator(ID_t &id) const
	{
		if(map_m.find(id) == map_m.end()) { return nullptr; }

		return map_m.at(id);
	}
};

}
}
