#include "EnginePCH.h"
#include "Camera.h"
#include "Transform.h"

using namespace DirectX;

Camera::Camera() :
	m_Near(0.1f), m_Far(50), m_FOV(60), m_ScreenWidth(980), m_ScreenHeight(560), 
	m_Projection(), m_LookAt(), m_Right(), m_Up(), m_ViewMatrix(DirectX::XMMatrixIdentity())
{
}

Camera::~Camera()
{

}

void Camera::Init()
{
	float r = (float)m_ScreenWidth / (float)m_ScreenHeight;

	float fov = 3.141592f * m_FOV / 180.f;
	m_Projection = DirectX::XMMatrixPerspectiveFovLH(fov, r, m_Near, m_Far);
	CalcLocalViewFrustum();

}

void Camera::Update()
{
	TransformUpdater::PushCamera(shared_from_this());
}

void Camera::CalcProjection(int Width, int Height)
{
	m_ScreenWidth = Width;
	m_ScreenHeight = Height;

	float r = (float)m_ScreenWidth / (float)m_ScreenHeight;
	float fov = (3.141592f) * m_FOV / 180;

	m_Projection = DirectX::XMMatrixPerspectiveFovLH(fov, r, m_Near, m_Far);

	CalcLocalViewFrustum();
}

void Camera::CalcViewMatrix()
{
	std::shared_ptr<Transform> transformComp = m_GameObject->GetComponent<Transform>();

	Vector3&& posf = transformComp->GetPosition();
	Vector3&& rotf = transformComp->GetRotation();

	Vector3 pos = XMLoadFloat3(&posf);
	Matrix&& rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rotf));
	Vector4 lookat = XMVector3Transform({ 0, 0, 1 }, rotation);

	////업벡터와 z와의 외적

	Vector4 EyeDirection = lookat;
	Vector4 UpDirection{ 0.f, 1.f, 0.f, 0.f };

	assert(!XMVector3Equal(EyeDirection, XMVectorZero()));
	assert(!XMVector3IsInfinite(EyeDirection));
	assert(!XMVector3Equal(UpDirection, XMVectorZero()));
	assert(!XMVector3IsInfinite(UpDirection));

	Vector4 LookAt = XMVector3Normalize(EyeDirection);
	Vector4 Right = XMVector3Transform({ 1, 0, 0 }, rotation);;
	Right = XMVector3Normalize(Right);
	Vector4 Up = XMVector3Cross(LookAt, Right);

	Vector4 NegEyePosition = XMVectorNegate(pos);

	Vector4 D0 = XMVector3Dot(Right, NegEyePosition);
	Vector4 D1 = XMVector3Dot(Up, NegEyePosition);
	Vector4 D2 = XMVector3Dot(LookAt, NegEyePosition);

	XMStoreFloat3(&m_LookAt, LookAt);
	XMStoreFloat3(&m_Right, Right);
	XMStoreFloat3(&m_Up, Up);

	DirectX::XMMATRIX ViewMatrix;

	ViewMatrix.r[0] = XMVectorSelect(D0, Right, g_XMSelect1110.v); //R0.x , R0.y, R0.z, D0
	ViewMatrix.r[1] = XMVectorSelect(D1, Up, g_XMSelect1110.v);
	ViewMatrix.r[2] = XMVectorSelect(D2, LookAt, g_XMSelect1110.v);
	ViewMatrix.r[3] = g_XMIdentityR3.v;

	m_ViewMatrix = DirectX::XMMatrixTranspose(ViewMatrix);

	CalcViewFrustumPlain();

	return;
}

void Camera::CalcLocalViewFrustum()
{
	float verticalfov = 3.141592f * m_FOV / 180.f;
	/// 스케일은 width, height, far 이다.

	m_tanHalfVFov = tanf(verticalfov / 2);
	m_tanHalfHFov = (GetAspect() * m_tanHalfVFov);

	float FarHeight = m_Far * m_tanHalfHFov;
	float FarWidth = m_Far * m_tanHalfHFov;

	m_FrustumVertex[0] = { +FarWidth, +FarHeight, m_Far, 1.f };
	m_FrustumVertex[1] = { -FarWidth, +FarHeight, m_Far, 1.f };
	m_FrustumVertex[2] = { +FarWidth, -FarHeight, m_Far, 1.f };
	m_FrustumVertex[3] = { -FarWidth, -FarHeight, m_Far, 1.f };

	float NearHeight = m_Near * m_tanHalfVFov;
	float NearWidth = m_Near * m_tanHalfHFov;

	m_FrustumVertex[4] = { +NearWidth, +NearHeight, m_Near, 1.f };
	m_FrustumVertex[5] = { -NearWidth, +NearHeight, m_Near, 1.f };
	m_FrustumVertex[6] = { +NearWidth, -NearHeight, m_Near, 1.f };
	m_FrustumVertex[7] = { -NearWidth, -NearHeight, m_Near, 1.f };
}

void Camera::CalcViewFrustumPlain()
{
	//버텍스들을 모두 월드 좌표로 옮겨준다.

	std::shared_ptr<Transform> transform = GetComponent<Transform>();
	XMMATRIX cameraWorldTM = transform->GetWorldTM();

	XMVECTOR  WorldFrustumVertex[8];

	for (int cnt = 0; cnt < 8; cnt++)
	{
		WorldFrustumVertex[cnt] = XMVector3TransformCoord(m_FrustumVertex[cnt], cameraWorldTM);
	}

	//FarPlain 
	m_Plain[0] = XMPlaneFromPoints(WorldFrustumVertex[3], WorldFrustumVertex[2], WorldFrustumVertex[1]);
	//Top
	m_Plain[1] = XMPlaneFromPoints(WorldFrustumVertex[0], WorldFrustumVertex[4], WorldFrustumVertex[1]);
	//Bottom
	m_Plain[2] = XMPlaneFromPoints(WorldFrustumVertex[7], WorldFrustumVertex[6], WorldFrustumVertex[3]);
	//Right
	m_Plain[3] = XMPlaneFromPoints(WorldFrustumVertex[6], WorldFrustumVertex[4], WorldFrustumVertex[2]);
	//Left
	m_Plain[4] = XMPlaneFromPoints(WorldFrustumVertex[1], WorldFrustumVertex[5], WorldFrustumVertex[3]);
	//NearPlain
	m_Plain[5] = XMPlaneFromPoints(WorldFrustumVertex[5], WorldFrustumVertex[4], WorldFrustumVertex[7]);

}

Matrix& Camera::GetViewMatrix()
{
	return m_ViewMatrix;
}

void Camera::SetFarNearFOV(float ifar, float inputnear, float fov)
{
	m_Far = ifar;
	m_Near = inputnear;
	m_FOV = fov;

	CalcProjection(m_ScreenWidth, m_ScreenHeight);
}

void Camera::SetLookAt(Vector3 pos)
{
	std::shared_ptr<Transform> transform = GetComponent<Transform>();

	Vector3 rot = pos - transform->GetPosition();
	Vector3 vec = DirectX::XMLoadFloat3(&rot);

	XMVECTOR lookVec = DirectX::XMVector3Normalize(vec);
	XMVECTOR upVec = { 0.f , 1.f, 0.f, 0.f };

	XMVECTOR rightVec = DirectX::XMVector3Cross(upVec, lookVec);
	upVec = DirectX::XMVector3Cross(lookVec, rightVec);

	XMMATRIX rotmat;

	rotmat.r[2] = lookVec;
	rotmat.r[1] = upVec;
	rotmat.r[0] = rightVec;

	float pitch = (float)asin(-rotmat.r[2].m128_f32[1]);
	float yaw = (float)atan2(rotmat.r[2].m128_f32[0], rotmat.r[2].m128_f32[2]);
	float roll = (float)atan2(rotmat.r[0].m128_f32[1], rotmat.r[1].m128_f32[1]);

	transform->SetRotation({ pitch, yaw, roll });
}

Matrix& Camera::GetProjectionMatrix()
{
	return m_Projection;
}

Matrix Camera::GetOthogonalProjectionMatrix()
{
	return DirectX::XMMatrixOrthographicLH(m_ScreenWidth, m_ScreenHeight, m_Near, m_Far);
}
