#pragma once
#include "LightBase.h"

class SpotLIght : public LightBase
{
public:
	SpotLIght();
	~SpotLIght();

	SpotLightInfo GetSpotLightInfo() const { return m_SpotLightInfo; }
private:
	SpotLightInfo m_SpotLightInfo;
};

