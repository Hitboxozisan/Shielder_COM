#include "Pch.h"

#include "Character.h"
#include "ModelManager.h"
#include "Shield.h"

using namespace My3dLib;

//const float Character::HIT_OTHER_CHARACTER_DIRECTION_Y = 5.0f;
//const float Character::GRAVITY = 0.1f;

Character::Character()
	:speed(0.0f)
	,velocity()
	,nextPosition()
	,nextDirction()
	,prevPosition()
	,noDrawFrame()
{
}

Character::~Character()
{
	if (modelHandle != NULL)
	{
		Finalize();
	}
}

void Character::Initialize()
{
	
}

void Character::Finalize()
{
	MV1DeleteModel(modelHandle);
	modelHandle = NULL;
}

void Character::Update()
{

}

void Character::Draw()
{
	
}

void Character::OnHitOtherCharacter(const VECTOR& forceDirection)
{
	//VECTOR force = forceDirection;
	//force.y = HIT_OTHER_CHARACTER_DIRECTION_Y;
	//
	//force = VScale(force, 5.0f);
	//
	//velocity = VAdd(velocity, force);
	//state = DAMAGE;

	////�y����΂�
	////velocity = VAdd(velocity, force);

	////nextPosition = VAdd(nextPosition, forceDirection);
}

void Character::MoveFinish()
{
	/*prevPosition = position;

	position = nextPosition;
	direction = nextPosition;

	MV1SetPosition(modelHandle, position);
	MV1SetRotationYUseDir(modelHandle, direction, 0.0f);*/

	prevPosition = position;
	prevDirection = direction;
	position = nextPosition;
	direction = nextDirction;

	//�ʒu��ݒ�
	MV1SetPosition(modelHandle, position);
	//�����蔻�苅�ړ�
	collisionSphere.Move(position);

	//�ړ��L�[�������Ă��Ȃ��ꍇ�͌������Œ肷��
	if (!VSquareSize(inputDirection) == 0.0f)
	{
		MV1SetRotationYUseDir(modelHandle, direction, 0.0f);
	}
}

bool Character::IsCollidableState() const
{
	/*if (state == NORMAL)
	{
		return true;
	}

	return false;*/
	return false;
}

