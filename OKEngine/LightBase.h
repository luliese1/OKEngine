#pragma once
#include "ComponentBase.h"

class Transform;

class LightBase : public ComponentBase
{
public:
	enum class LightType
	{
		Directional,
		Spot,
		Point
	};

	const LightBase::LightType GetLightType() const { return m_LightType; }

public:
	virtual void Init();
	virtual void Update();
	virtual Matrix GetProjectionMatrix();
	virtual Matrix GetViewMatrix();

	//virtual void LateUpdate();
	virtual ~LightBase() ;

protected:
	LightBase(LightType type);
	std::weak_ptr<Transform> m_Transform;

	const LightType m_LightType;
};

