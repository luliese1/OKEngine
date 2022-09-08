#include "EnginePCH.h"
#include "CameraObject.h"
#include "Camera.h"
#include "Transform.h"
#include "CameraController.h"

CameraObject::CameraObject() : GameObject()
{
	AddComponent<Transform>();
	AddComponent<Camera>();
	AddComponent<CameraController>();
}

CameraObject::~CameraObject()
{

}

