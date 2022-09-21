#pragma once
#include "LightBase.h"

class DirectionalLight : public LightBase
{
public:
	DirectionalLight();
	~DirectionalLight();


	DirectionalLightInfo GetDirectionalLightInfo() ;

	virtual Matrix GetProjectionMatrix();
	virtual Matrix GetViewMatrix();


	void SetAmbient(Vector4 input);
	void SetDiffuse(Vector4 input);
	void SetSpecular(Vector4 input);
	void SetDirection(Vector3 input);

private:

private:
	DirectionalLightInfo m_DirectionalLightInfo;


};

