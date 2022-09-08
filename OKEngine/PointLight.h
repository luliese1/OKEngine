#pragma once
#include "LightBase.h"

class PointLight : public LightBase
{
public:
	PointLight();
	~PointLight();

	PointLightInfo GetPointLIghtInfo() const { return m_PointLIghtInfo; }
private:
	PointLightInfo m_PointLIghtInfo;

};

