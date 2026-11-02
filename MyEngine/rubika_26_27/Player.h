#pragma once
#include <Entity.h>
#include <TransformComponent.h>
#include <Globals.h>
#include <InputMgr.h>

class Player : public Entity
{
public:
	Player(std::string friendlyName);

	enum dir { Up, Right, Down, Left };

	void SetUpInputs();
	void Move(dir);

private:
	TransformComponent* transform;
	float speed = 0.1;
};

