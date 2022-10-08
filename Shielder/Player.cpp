#include "Pch.h"
#include "Player.h"

#include "Shield.h"

#include "ModelManager.h"
#include "KeyManager.h"
#include "DeltaTime.h"

using namespace My3dLib;

const float Player::HIT_POINT = 100.0f;
const float Player::DECREMENT_HIT_POINT = 5.0f;
const float Player::COLLIDE_RADIUS = 50.0f;
const float Player::NORMAL_SPEED = 3.0f;
const float Player::DEFENSE_SPEED = 1.0f;
const float Player::JUST_DEFENSE_TIME = 0.2f;
const float Player::NORMAL_DEFENSE_TIME = 0.5f;
const float Player::HIT_OTHER_CHARACTER_DIRECTION_Y = 1.5f;
const float Player::GRAVITY = 1.0f;
const float Player::FRICTION_FORCE = 0.1f;
const float Player::TRUNK_POINT = 100.0f;
const float Player::DECREMENT_TRUNK_POINT = 10.0f;
const float Player::INVINCIBLE_TIME = 5.0f;

/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
	if (modelHandle != NULL)
	{
		Finalize();
	}
	
}

/// <summary>
/// ����������
/// </summary>
void Player::Initialize()
{
	//���f���̓ǂݍ���
	modelHandle = MV1DuplicateModel(ModelManager::GetInstance().GetModelHandle(ModelManager::PLAYER));
	//�ǂݍ��ݎ��s�ŃG���[
	if (modelHandle < 0)
	{
		printfDx("���f���f�[�^�̓ǂݍ���_Player\n");
	}

	state = NORMAL;
	pUpdate = &Player::UpdateNormal;

	position = VGet(2000.0f, 0.0f, 100.0f);
	nextPosition = position;
	direction = ZERO_VECTOR;
	nextDirction = direction;
	
	isDefense = false;
	noDrawFrame = false;

	hitPoint = HIT_POINT;
	speed = NORMAL_SPEED;

	velocity = ZERO_VECTOR;
	inputDirection = ZERO_VECTOR;

	trunkPoint = TRUNK_POINT;

	//�ʒu��ݒ�
	MV1SetPosition(modelHandle, position);
	MV1SetScale(modelHandle, VGet(0.5f, 0.5f, 0.5f));


	//�����蔻�苅��ݒ�
	collisionSphere.localCenter = VGet(0.0f, 0.0f, 0.0f);
	collisionSphere.radius = COLLIDE_RADIUS;
	collisionSphere.worldCenter = position;

	shield = nullptr;
}

/// <summary>
/// �X�V����
/// </summary>
void Player::Update()
{
	if (pUpdate != nullptr)
	{
		(this->*pUpdate)();			//��Ԃ��Ƃ̍X�V����
	}

}

/// <summary>
/// �`�揈��
/// </summary>
void Player::Draw()
{
	//�`�悵�Ȃ��t���[���Ȃ�`�悵�Ȃ�
	if (noDrawFrame)
	{
		return;
	}

	MV1DrawModel(modelHandle);			//3D���f���̕`��
	if (shield != nullptr)
	{
		shield->Draw();					//���̕`��
		DrawFormatString(50, 50, GetColor(255, 255, 255), "DefenseCount : %f", shield->GetDefenseCount(), TRUE);
	}

	//�f�o�b�O�p
	

	//�����蔻��f�o�b�N�`��
	DrawSphere3D(collisionSphere.worldCenter, collisionSphere.radius,
		8, GetColor(0, 255, 0), 0, FALSE);


}

/// <summary>
/// ���̃L�����N�^�[�ƐڐG����
/// </summary>
/// <param name="forceDirection">������΂���</param>
void Player::OnHitOtherCharacter(const VECTOR& forceDirection)
{
	//�O���velocity�����Z�b�g����
	velocity = ZERO_VECTOR;

	VECTOR force = forceDirection;
	force.y = HIT_OTHER_CHARACTER_DIRECTION_Y;

	force = VScale(force, 8.0f);
	force.z = 0.0f;

	velocity = VAdd(velocity, force);
	invincibleTime = 0.0f;					//���G���Ԃ�������
	hitPoint -= DECREMENT_HIT_POINT;		//�̗͂�����������
	state = DAMAGE;
	pUpdate = &Player::UpdateDamage;
}

/// <summary>
/// �������̃L�����N�^�[�ƐڐG����
/// </summary>
/// <param name="adjust">������΂���</param>
void Player::OnHitShield(const VECTOR& adjust)
{
	//�O���velocity�����Z�b�g����
	velocity = ZERO_VECTOR;
	
	VECTOR force = adjust;
	force.y = 0.0f;			//�ςȕ����ɓ����Ȃ��悤�ɂ���
	force.z = 0.0f;			//�ςȕ����ɓ����Ȃ��悤�ɂ���

	//�K�[�h�����^�C�~���O�ɂ���Č�ނ�����ʂ�ω�������
	if (shield->GetDefenseCount() <= JUST_DEFENSE_TIME)
	{
		//�W���X�g�K�[�h�Ȃ珬������ނ�����
		force = VScale(force, 3.0f);
		//�̊��Q�[�W�����炷
		trunkPoint -= DECREMENT_TRUNK_POINT;
	}
	else
	{
		//�W���X�g�K�[�h����Ȃ��Ȃ�傫����ނ�����
		force = VScale(force, 8.0f);
		//�̊��Q�[�W�����炷
		trunkPoint -= DECREMENT_TRUNK_POINT;
	}

	velocity = VAdd(velocity, force);
	state = SLIDE;
	pUpdate = &Player::UpdateSlide;
}

/// <summary>
/// �ڐG�ł����Ԃ�
/// </summary>
/// <returns></returns>
bool Player::IsCollidableState() const
{
	//�ڐG�ł����ԂȂ�
	if (state == NORMAL ||
		state == DEFENSE)
	{
		return true;
	}

	return false;
}

/// <summary>
/// Normal��Ԃ̍X�V����
/// </summary>
void Player::UpdateNormal()
{
	InputAction();
	Move();
}

/// <summary>
/// Defence��Ԃ̍X�V����
/// </summary>
void Player::UpdateDefence()
{
	InputAction();
	Move();

	Defense();
}

void Player::UpdateSlide()
{
	Defense();
	Slide();
}

void Player::UpdateDamage()
{
	Damage();
}

void Player::Move()
{
	if (!VSquareSize(inputDirection) == 0.0f)
	{
		nextDirction = VNorm(inputDirection);
	}
	else
	{
		nextDirction = inputDirection;
	}

	nextPosition = VAdd(position, VScale(nextDirction, speed));
}

void Player::MoveFinish()
{
	//�O�t���[���̈ʒu���X�V
	prevPosition = position;
	//�O�t���[���̌������X�V
	prevDirection = direction;
	position = nextPosition;
	direction = nextDirction;

	//�ʒu��ݒ�
	MV1SetPosition(modelHandle, position);
	
	//�ړ��L�[�������Ă��Ȃ��ꍇ�͌������Œ肷��
	if (!VSquareSize(inputDirection) == 0.0f)
	{
		MV1SetRotationYUseDir(modelHandle, direction, 0.0f);
	}
}

void Player::Jump()
{
	
}

void Player::Defense()
{
	if (shield != nullptr)
	{
		//���𐶐�
		shield->Activate(position, direction, prevDirection);
		shield->Update();
		shield->OnHitOtherCharacter();
	}
}

/// <summary>
/// �K�[�h������
/// </summary>
void Player::Slide()
{
	//�G�t�F�N�g����

	//���点��
	velocity.x += FRICTION_FORCE;

	//�~�܂�����ʏ��Ԃɖ߂�
	if (velocity.x >= 0.0f)
	{
		velocity = ZERO_VECTOR;
		state = NORMAL;
		pUpdate = &Player::UpdateNormal;
	}

	nextPosition = VAdd(nextPosition, velocity);
}

/// <summary>
/// �_���[�W����
/// </summary>
void Player::Damage()
{
	noDrawFrame = !noDrawFrame;		//2���1��`�悵�Ȃ�

	//�G�t�F�N�g����


	nextPosition = VAdd(nextPosition, velocity);

	velocity.y -= GRAVITY;

	if (nextPosition.y <= 0.0f)
	{
		nextPosition.y = 0.0f;
		velocity = ZERO_VECTOR;
		state = NORMAL;
		pUpdate = &Player::UpdateNormal;
		noDrawFrame = false;
	}
}

void Player::InputAction()
{
	inputDirection = ZERO_VECTOR;
	//���E�ړ�
	if (KeyManager::GetInstance().CheckPressed(KEY_INPUT_A))
	{
		inputDirection += LEFT * speed;
	}
	if (KeyManager::GetInstance().CheckPressed(KEY_INPUT_D))
	{
		inputDirection += RIGHT * speed;
	}

	if (KeyManager::GetInstance().CheckPressed(KEY_INPUT_SPACE))
	{
		//�W�����v

	}

	//�h��
	if (KeyManager::GetInstance().CheckPressed(KEY_INPUT_LSHIFT))
	{
		speed = DEFENSE_SPEED;
		
		isDefense = true;		//�h��
		CreateShield();

		pUpdate = &Player::UpdateDefence;
	}
	else
	{
		speed = NORMAL_SPEED;
		isDefense = false;
		
		pUpdate = &Player::UpdateNormal;
		shield = nullptr;			//�������ł�����
	}

	
}

void Player::CreateShield()
{
	if (shield == nullptr)
	{
		shield = new Shield;
		shield->Initialize(prevDirection);
		state = DEFENSE;
	}
}