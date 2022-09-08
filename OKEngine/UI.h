#pragma once
#include "ComponentBase.h"
#include "AlignedAllocationPolicy.h"

//UI�� BASECOMPONENT
//��ư, �ؽ�Ʈ.. �� ���������� ����ϰ���

class UI : public ComponentBase, public AlignedAllocationPolicy<16>
{
public:
	void Init() override;
	void Update() override;

	//virtual void Render() abstract;
	virtual void Render(DirectX::XMFLOAT2 pos) abstract;

protected:
	UI();
	virtual ~UI();

};

