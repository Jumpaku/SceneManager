#pragma once

#include "SceneChangeMethodFactory.h"
namespace jumpaku {
namespace scenemanager {

template<typename SceneID>
class BaseSceneChangeMethod;
template<typename SceneID>
class SceneChangeMethodFactory;

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

	template<typename SceneID>
	class BaseScene
{
public:
	/**�V�[�����ʖ��̌^*/
	typedef SceneID ID;
	/**�V�[���̑J�ڕ��@�N���X�̊�{�N���X*/
	typedef std::unique_ptr<BaseSceneChangeMethod<SceneID>> ChangeMethod;
	/**ChangeMethodFactory::get<�J�ڕ��@�N���X>(���̃V�[�����ʖ�)�Ŏ��̃V�[���ւ̑J�ڕ��@�N���X�𓾂鎖���ł���N���X*/
	typedef SceneChangeMethodFactory<SceneID> MethodFactory;
public:
	/**default constructor*/
	BaseScene() = default;
	/**default denstructor*/
	virtual ~BaseScene() = default;
public:
	/**
	*1���[�v��1�xexecute()�̌�ɌĂ΂�,���̃V�[���ւ̑J�ڕ��@�N���X��Ԃ�.
	*@return �V�[���J�ڕ��@
	*/ 
	virtual ChangeMethod decideNext() = 0;
	/**
	*�Q�[���̃��C�����[�v�̖{�̂ł���,1���[�v��1�x�Ă΂��. ����:0 / ���s:-1
	*/
	virtual int doOneFrame() = 0;
	/**
	*SceneTree����폜����鎞�ɌĂ΂�,�I���������s��. ����:0 / ���s:-1
	*/
	virtual int finalize() = 0;
	/**
	*SceneTree�ɒǉ����ꂽ���ɌĂ΂�, �������������s��.����:0 / ���s:-1
	*/
	virtual int initialize() = 0;
};

}
}

#include "SceneChangeMethod.h"

