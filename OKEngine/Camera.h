#pragma once
#include "ComponentBase.h"
#include "AlignedAllocationPolicy.h"

/// <summary>
/// 카메라 컴포넌트
/// </summary>

class Camera : public ComponentBase, public AlignedAllocationPolicy<16>
{
public:
	Camera();
	~Camera();

	void Init() override;
	void Update() override;

	void CalcProjection(int Width, int Height);
	void CalcViewMatrix();
	void CalcLocalViewFrustum();
	void CalcViewFrustumPlain();

	Vector3 GetRight() const { return m_Right; }
	void SetRight(Vector3 val) { m_Right = val; }
	Vector3 GetUp() const { return m_Up; }
	void SetUp(Vector3 val) { m_Up = val; }
	Vector3 GetLookAt() const { return m_LookAt; }
	void SetLookAt(Vector3 pos);

	Matrix& GetProjectionMatrix();
	Matrix GetOthogonalProjectionMatrix();

	Matrix& GetViewMatrix();

	int GetCameraID() const { return m_CameraID; }
	void SetCameraID(int val) { m_CameraID = val; }

	bool GetUsingNOW() const { return m_UsingNOW; }
	void SetUsingNOW(bool val) { m_UsingNOW = val; }

	void SetFarNearFOV(float ifar, float near, float fov);

	float GetFar() const { return m_Far; }
	float GetNear() const { return m_Near; }
	float GetFOV() const { return m_FOV; }
	float GetAspect() const { return (float)m_ScreenWidth / (float)m_ScreenHeight; }

	Vector4* GetViewFrustumPlain() { return m_Plain; }
	Vector4* GetLocalFrustumVertex() { return m_FrustumVertex; }

	float GetTanHalfHFov() const { return m_tanHalfHFov; }
	float GetTanHalfVFov() const { return m_tanHalfVFov; }
private:
	/// view Mat
	Vector3 m_LookAt;
	Vector3 m_Right;
	Vector3 m_Up;

	Matrix m_ViewMatrix;
	Matrix m_Projection;

	/// proj Mat
	float m_Near;
	float m_Far;
	//Field of View, Radian 값이다.
	float m_FOV;

	int m_ScreenWidth;
	int m_ScreenHeight;

	bool m_UsingNOW;

	//view Frustum
	//Horizontal
	float m_tanHalfHFov;
	//Vertical
	float m_tanHalfVFov;

	//(평면의 노말, D(거리))
	Vector4 m_FrustumVertex[8];
	Vector4 m_Plain[6];

	int m_CameraID;
};

