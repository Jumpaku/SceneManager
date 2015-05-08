#pragma once

#include "SceneTree.h"
#include "SceneFactory.h"
#include "SceneChangeMethod.h"

//����
/*---------------------------------------------------------------------------------------
�V�[���̎��s�ƃV�[���̈ڍs,�V�[�������N���X�̊Ǘ�������.
�e���v���[�g����SceneID�̓V�[�����ʖ��ϐ���\��.(�񋓑�,������,�ʂ��ԍ��Ȃ�)

�܂�registerScene<Scene>(id)���Ă�Ŏg�p����V�[���N���X(Scene)�Ƃ��̃V�[���̎��ʖ�(id)��o�^����.
������setFirstScene(id)���Ă�ōŏ��̃V�[����ݒ肷��.
���C�����[�v�̒���1���[�v��1�xexcuteScene()�����s����.
�I�������finalize()���Ă�.
---------------------------------------------------------------------------------------*/
//�g�p��
/*	
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
	typedef std::unique_ptr<BaseSceneChangeMethod<SceneID>> ChangeMethod_t;
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
	SceneManager() :tree_m(), currentScene_m(tree_m.end()) {}
	~SceneManager() {}
private:
	int changeScene()
	{
		if(currentScene_m != tree_m.end()) {
			if(currentScene_m->scene_m == nullptr) { return -1; }
			ChangeMethod_t changeMethod
				= currentScene_m->scene_m->decideNext();

			currentScene_m = changeMethod->changeScene(
				factory_m, tree_m, currentScene_m);
		}

		if(currentScene_m == tree_m.end()) { return -1; }

		return 0;
	}
public:
	template<class DerivedScene>
	int registerScene(ID_t id)
	{
		return factory_m.insertGenerator<DerivedScene>(id);
	}

	int executeScene()
	{
		if(currentScene_m == tree_m.end()) { return -1; }
		if(currentScene_m->scene_m == nullptr) { return -1; }
		if(currentScene_m->scene_m->doOneFrame() != 0) { return -1; }
		if(changeScene() != 0) { return -1; }

		return 0;
	}

	void finalize()
	{
		currentScene_m = tree_m.end();
		tree_m.clear();
		factory_m.finalize();
	}

	int setFirstScene(ID_t id)
	{
		currentScene_m = ResetScene<ID_t>(id).changeScene(factory_m, tree_m, currentScene_m);

		return 0;
	}
};

}
}
