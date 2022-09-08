#pragma once
#include "LightBase.h"

class DirectionalLight : public LightBase
{
public:
	DirectionalLight();
	~DirectionalLight();

	void LoadTransformInfo();
	DirectionalLightInfo GetDirectionalLightInfo() const;

	void SetAmbient(Vector4 input);
	void SetDiffuse(Vector4 input);
	void SetSpecular(Vector4 input);
	void SetDirection(Vector4 input);

private:
	DirectionalLightInfo m_DirectionalLightInfo;
};

