#pragma once
#include "ComponentBase.h"

class Transform;

class Controller : public ComponentBase
{
public:
	Controller() {}
	~Controller() {}

	void Init() override;
	void Update() override;

private:
	std::weak_ptr<Transform> m_Transform;
	float m_speed = 5.f;
};
