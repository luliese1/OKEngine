#pragma once
#include "ComponentBase.h"
#include "AlignedAllocationPolicy.h"

//UI의 BASECOMPONENT
//버튼, 텍스트.. 뭐 여러가지를 상속하겠지

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

