#include "EnginePCH.h"
#include "UI.h"
#include "MeshRenderer.h"

void UI::Init()
{

}

void UI::Update()
{
	MeshRenderer::PushUI(shared_from_this());
}

UI::UI()
{

}

UI::~UI()
{

}
