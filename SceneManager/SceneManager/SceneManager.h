#pragma once

#include "SceneTree.h"
#include "SceneFactory.h"
#include "SceneTransition.h"

#include"SceneException.h"

/**
*�V�[���̎��s�ƃV�[���̑J��,�������Ǘ�����.
*�e���v���[�g����SceneID�̓V�[�����ʖ��ϐ���\��.(�񋓑�,������,�ʂ��ԍ��Ȃ�)
*
*�܂�registerScene<Scene>(id)���Ă�Ŏg�p����V�[���N���X(Scene)�Ƃ��̃V�[���̎��ʖ�(id)��o�^����
*������setFirstScene(id)���Ă�ōŏ��̃V�[����ݒ肷��
*���C�����[�v�̒���1���[�v��1�xexcuteScene()�����s����
*excuteScene()�̕Ԃ�l��FINISH=0�������烋�[�v�𔲂���
*registerScene()��setFirstScene()��executeScene()��SceneLogicException��O�𓊂��Ȃ��悤��Scene�N���X��h��������
*SceneRuntimeException��O��������ꂽ��Q�[�����I��������
*
*�g�p��
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
	/**executeScene()��CONTINUE��Ԃ����烋�[�v�𑱂���*/
	static int const CONTINUE = 1;
	/**executeScene()��FINISH��Ԃ����烋�[�v�𔲂���*/
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
	*id�ɑ΂���DerivedScene��o�^����
	*���łɓ���id���o�^����Ă����牽�����Ȃ�
	*@param DerivedScene
	*@param id
	*/
	template<class DerivedScene>
	void registerScene(ID_t id)
	{
		factory_m.insertGenerator<DerivedScene>(id);
	}
	/**
	*���݂̃V�[����1�t���[�������s��,���̃V�[���֑J�ڂ�����
	*@return CONTINUE : ���̃V�[�������鎞,FINISH : �Ȃ���
	*@exception SceneLogicException : �V�[���J�ڂɎ��s������,SceneRuntimeException : �V�[���̐����Ɏ��s������
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
	*�I���������s��,�f�X�g���N�^���ŌĂ΂��
	*/
	void finalize()
	{
		currentScene_m = tree_m.end();
		tree_m.clear();
		factory_m.finalize();
	}

	/**
	*�ŏ��̃V�[����ݒ肷��
	*@param id
	*@exception SceneLogicException : id���܂��o�^����Ă��Ȃ���,SceneRuntimeException : �ŏ��̃V�[���̐����Ɏ��s������
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
