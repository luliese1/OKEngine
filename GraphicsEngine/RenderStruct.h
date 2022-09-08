#pragma once

struct ScreenInfo
{
	int m_ScreenWidth;
	int m_ScreenHeight;
	bool m_4xMsaaFlag;
	UINT m_4xMsaaQuality;
};

struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT3 Tangent;
	DirectX::XMFLOAT2 Tex0;
	DirectX::XMFLOAT2 Tex1;
};

struct Material
{
	Material() { ZeroMemory(this, sizeof(this)); }

	DirectX::XMFLOAT4 Ambient;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular; // w = SpecPower
	DirectX::XMFLOAT4 Reflect;
};
inline DirectX::XMFLOAT3 QuatToEuler(DirectX::XMVECTOR rot)
{
	DirectX::XMFLOAT3 tempRot;

	//x 
	float q2sqr = rot.m128_f32[0] * rot.m128_f32[0];

	//pitch
	float t2 = 2.0f * (rot.m128_f32[0] * rot.m128_f32[3] - rot.m128_f32[2] * rot.m128_f32[1]);

	//yaw
	float t0 = 2.0f * (rot.m128_f32[2] * rot.m128_f32[0] + rot.m128_f32[1] * rot.m128_f32[3]); // wz , xy
	float t1 = 1.0f - 2.0f * (q2sqr + rot.m128_f32[1] * rot.m128_f32[1]); // yy , zz

	//roll
	float t3 = 2.0f * (rot.m128_f32[2] * rot.m128_f32[3] + rot.m128_f32[0] * rot.m128_f32[1]); //w*x , y*z
	float t4 = 1.0f - 2.0f * (rot.m128_f32[2] * rot.m128_f32[2] + q2sqr); //x*x y*y

	t2 = t2 > 1.0f ? 1.0f : t2;
	t2 = t2 < -1.0f ? -1.0f : t2;

	float pitch = asinf(t2);

	float yaw = atan2f(t0, t1);

	float roll = atan2f(t3, t4);

	tempRot = { pitch, yaw, roll };

	return tempRot;
}