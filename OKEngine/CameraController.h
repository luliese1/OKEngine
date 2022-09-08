#pragma once
#include "ComponentBase.h"

class CameraController : public ComponentBase
{
public:
	CameraController() {}
	~CameraController() {}

	void Init() override;
	void Update() override;

};


