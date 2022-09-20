#pragma once
#include "ComponentBase.h"
#include "AlignedAllocationPolicy.h"
#include "TransformUpdater.h"
//SRT연산을 기억합시다.

class Scene;

class Transform : public ComponentBase, public AlignedAllocationPolicy<16>
{
public:
	Transform();
	~Transform();

	virtual void Init() override;
	virtual void Update() override;

	Vector3 GetScale() const { return m_Scale; }
	void SetScale(Vector3 val) { m_Scale = val; }
	Vector3 GetRotation() const { return m_Rotation; }
	void SetRotation(Vector3 val) { m_Rotation = val; }
	Vector3 GetPosition() const { return m_Position; }
	void SetPosition(Vector3 val) { m_Position = val; }

	Matrix GetWorldTM() const { return m_WorldTM; }
	Matrix GetLocalTM() const { return m_LocalTM; }

	void Rotate(Vector3 val);
	void Translate(Vector3 val);
	void Scaling(Vector3 val);

	bool GetIsCalculated() const { return m_IsCalculated; }
	void SetIsCalculated(bool val) { m_IsCalculated = val; }

	Matrix GetLocalSclTM() const { return m_LocalSclTM; }
	void SetLocalSclTM(Matrix val) { m_LocalSclTM = val; }
	Matrix GetLocalRotTM() const { return m_LocalRotTM; }
	void SetLocalRotTM(Matrix val) { m_LocalRotTM = val; }
	Matrix GetLocalPosTM() const { return m_LocalPosTM; }
	void SetLocalPosTM(Matrix val) { m_LocalPosTM = val; }

	Vector3 GetForward();
	Vector3 GetUp();
	Vector3 GetRight();


protected:
	//SRT
	Vector3 m_Scale;
	Vector3 m_Rotation;
	Vector3 m_Position;

	Matrix m_LocalSclTM;
	Matrix m_LocalRotTM;
	Matrix m_LocalPosTM;

	Matrix m_WorldSclTM;
	Matrix m_WorldRotTM;
	Matrix m_WorldPosTM;

	Matrix m_LocalTM;
	Matrix m_WorldTM;

	bool m_IsCalculated;

private:
	void CalcLocalMatrix();
	DirectX::XMMATRIX CalcWorldMatrix();

	friend TransformUpdater;
};
