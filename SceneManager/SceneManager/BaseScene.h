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
*�V�[���N���X�̃C���^�[�t�F�C�X�̃e���v���[�g.
*���G�ȏ������ƏI��������initialize()��finalize()�ōs��.
*�e���v���[�g����SceneID�̓V�[�����ʖ��ϐ��̌^��\��.(�񋓑�,������,�ʂ��ԍ��Ȃ�)
*�V�[���N���X��BaseScene���p������.
*finalize(), initialize(), doOneFrame(), decideNext()���I�[�o�[���C�h����.
*
*/

namespace jumpaku {
namespace scenemanager {

template<typename SceneID, typename SharedData>
class BaseScene
{
	friend SceneFactory < SceneID, SharedData > ;
public:
	/**�V�[�����ʖ��̌^*/
	typedef SceneID ID;
	/**�V�[���̑J�ڕ��@�N���X�̊�{�N���X*/
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
	*���L�f�[�^�ւ̃|�C���^���擾����
	*@return pointer of shared data
	*/
	SharedData *sharedData()
	{
		return sharedData_m;
	}
	/**
	*Transition = ResetScene : �V�[����S�č폜���Ă���V����id�̃V�[���𐶐������̃V�[���֑J�ڂ���
	*Transition = PushScene : id�̃V�[���𐶐����q�̃V�[���Ƃ��Ă����֑J�ڂ���
	*Transition = JumpScene : �S�ẴV�[���̒�����id�̃V�[����T���Ă����֑J�ڂ���,����������Ȃ����id�̃V�[����push����
	*Transition = ChildScene : �q�̃V�[���̒�����id�̃V�[����T���Ă����֑J�ڂ���
	*@param scene id of naxt sscene
	*@return instance of transition class
	*/
	template<template <typename, typename> class Transition>
	static SceneTransition getSceneTransition(SceneID id)
	{
		return SceneTransitionFactory<SceneID, SharedData>::get<Transition>(id);
	}
	/**
	*Transition = KeepScene : �V�[���J�ڂ��Ȃ��ō��̃V�[���̂܂܎��̃t���[���ɍs��
	*Transition = ClearScene : �V�[����S�č폜���ăQ�[�����I������
	*Transition = popScene : ���̃V�[�����폜����1�e�̃V�[���֑J�ڂ���
	*Transition = ParentScene : ���̃V�[�����c����1�e�̃V�[���֑J�ڂ���
	*@return instance of transition class
	*/
	template<template <typename, typename> class Transition>
	static SceneTransition getSceneTransition()
	{
		return SceneTransitionFactory<SceneID, SharedData>::get<Transition>();
	}
public:
	/**
	*1���[�v��1�xdoOneFrame()�̌�ɌĂ΂�,���̃V�[���ւ̑J�ڕ��@�N���X�̃C���X�^���X��Ԃ�.
	*������getSceneTransition���g���ăV�[���J�ڕ��@�𐶐�����
	*@return �V�[���J�ڕ��@
	*/ 
	virtual SceneTransition decideNext() = 0;
	/**
	*�Q�[���̃��C�����[�v�̖{�̂ł���,1���[�v��1�x�Ă΂��. 
	*/
	virtual void doOneFrame() = 0;
	/**
	*SceneTree����폜����鎞��1�x�����Ă΂�,�I���������s��. 
	*/
	virtual void finalize() = 0;
	/**
	*SceneTree�ɒǉ����ꂽ����1�x�����Ă΂�, �������������s��.
	*/
	virtual void initialize() = 0;
};

}
}

#include "SceneTransition.h"

