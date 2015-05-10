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
*/

namespace jumpaku {
namespace scenemanager {

template<typename SceneID, typename SharedData>
class BaseScene
{
public:
	/**�V�[�����ʖ��̌^*/
	typedef SceneID ID;
	/**�V�[���̑J�ڕ��@�N���X�̊�{�N���X*/
	typedef std::unique_ptr<BaseSceneTransition<SceneID, SharedData>> SceneTransition;
protected:
	SharedData *sharedDate;
public:
	/**default constructor*/
	BaseScene() = default;
	/**default denstructor*/
	virtual ~BaseScene() = default;
protected:
	/**
	*
	*/
	template<template <typename, typename> class Transition>
	static SceneTransition getSceneTransition(SceneID id)
	{
		return SceneTransitionFactory<SceneID, SharedData>::get<Transition>(id);
	}
	/**
	*
	*/
	template<template <typename, typename> class Transition>
	static SceneTransition getSceneTransition()
	{
		return SceneTransitionFactory<SceneID, SharedData>::get<Transition>();
	}
public:
	void setSharedData(SharedData *data)
	{
		sharedDate = data;
	}
	/**
	*1���[�v��1�xexecute()�̌�ɌĂ΂�,���̃V�[���ւ̑J�ڕ��@�N���X��Ԃ�.
	*@return �V�[���J�ڕ��@
	*/ 
	virtual SceneTransition decideNext() = 0;
	/**
	*�Q�[���̃��C�����[�v�̖{�̂ł���,1���[�v��1�x�Ă΂��. ����:0 / ���s:-1
	*/
	virtual void doOneFrame() = 0;
	/**
	*SceneTree����폜����鎞�ɌĂ΂�,�I���������s��. ����:0 / ���s:-1
	*/
	virtual void finalize() = 0;
	/**
	*SceneTree�ɒǉ����ꂽ���ɌĂ΂�, �������������s��.����:0 / ���s:-1
	*/
	virtual void initialize() = 0;
};

}
}

#include "SceneTransition.h"

