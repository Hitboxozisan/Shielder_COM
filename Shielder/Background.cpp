#include "Pch.h"
#include <string>
#include "Background.h"

const float Background::SIZE = 1.0f;
const VECTOR Background::INITIAL_POSITION = VGet(0.0f, 0.0f, 0.0f);

Background::Background()
{
}

Background::~Background()
{
}

void Background::Initialize()
{
	std::string fullPath = "Data/Img/background.png";
	img = LoadGraph(fullPath.c_str());
	position = INITIAL_POSITION;
}

void Background::Draw()
{
	DrawGraph(position.x, position.y, img, TRUE);
}
