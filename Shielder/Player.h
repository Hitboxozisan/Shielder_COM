#pragma once

#include "Character.h"

class Shield;

class Player final : public Character
{
public:
	enum State
	{
		NONE,		//���݂��Ȃ�
		NORMAL,		//�ʏ�
		DEFENSE,	//�h��
		SLIDE,		//�h�䎞�ɐڐG
		DAMAGE,		//��e
		DEAD		//���S
	};
	
	Player();
	~Player();

	void Initialize();
	void Update();

	void Draw();

	void OnHitOtherCharacter(const VECTOR& forceDirection);
	void OnHitShield(const VECTOR& adjust);				//���ƃL�����N�^�[����������
	const bool IsDefense()const { return isDefense; }
	bool IsCollidableState() const;
	const float GetTrunk() const { return trunkPoint; }

	//�f�o�b�O�p
private:
	//Player();
	Player(const Player&);			//�R�s�[�R���X�g���N�^
	void operator=(const Player&);	//�R�s�[������Z�q

	static const float HIT_POINT;							//�̗�
	static const float DECREMENT_HIT_POINT;					//�̗͂̌�����
	static const float COLLIDE_RADIUS;
	static const float NORMAL_SPEED;
	static const float DEFENSE_SPEED;
	static const float HIT_OTHER_CHARACTER_DIRECTION_Y;		//�G�L�����Ɠ�����������Y�����̌���
	static const float JUST_DEFENSE_TIME;					//�W���X�g�K�[�h�Ɣ��f���鎞��
	static const float NORMAL_DEFENSE_TIME;					//�m�[�}���K�[�h�Ɣ��f���鎞��
	static const float GRAVITY;								//�d��
	static const float FRICTION_FORCE;						//���C��
	static const float TRUNK_POINT;							//�̊��Q�[�W
	static const float DECREMENT_TRUNK_POINT;				//�̊��Q�[�W������
	static const float INVINCIBLE_TIME;						//���G����

	State state;

	float justDefenceTime;			//�W���X�g�K�[�h�Ɣ��f���鎞��
	float normalDefenceTime;		//�m�[�}���K�[�h�Ɣ��f���鎞��
	float invincibleTime;			//���G����

	bool isDefense;

	void (Player::* pUpdate)();		//Update�֐��|�C���^

	//�e��Ԃ̍X�V����
	void UpdateNormal();
	void UpdateDefence();
	void UpdateSlide();
	void UpdateDamage();


	void Move();
	void MoveFinish();
	void Jump();
	void Defense();
	void Slide();		//�W���X�g�K�[�h�ȊO�ŐڐG
	void Damage();
	void InputAction();
	void CreateShield();
};
