#pragma once
#include "ComponentBase.h"

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
	//virtual void LateUpdate();
	virtual ~LightBase() ;

protected:
	LightBase(LightType type);
	const LightType m_LightType;
};

