#include "EnginePCH.h"
#include "Controller.h"
#include "Transform.h"
#include "Input.h"
#include "Time.h"

void Controller::Init()
{
	m_Transform = GetComponent<Transform>();
}

void Controller::Update()
{
	auto transform = m_Transform.lock();

	Vector3 Up = transform->GetUp();
	Vector3 Right = transform->GetRight();
	Vector3 LookAt = transform->GetForward();


	if (Input::KeyCheck(Key::CTRL, eKeyState::STAY))
	{
		//m_GameObject->GetComponent<Transform>()->SetRotation({3.1f, 0.0f, 0.0f });

		m_GameObject->GetComponent<Transform>()->Translate({ Up.x * (float)Time::m_deltaTime * m_speed, Up.y * (float)Time::m_deltaTime * m_speed, Up.z * (float)Time::m_deltaTime * m_speed });
	}

	if (Input::KeyCheck(Key::SHIFT, eKeyState::STAY))
	{
		//m_GameObject->GetComponent<Transform>()->SetRotation({ 3.4f, 0.0f, 0.0f });

		m_GameObject->GetComponent<Transform>()->Translate({ Up.x * -(float)Time::m_deltaTime * m_speed, Up.y * -(float)Time::m_deltaTime * m_speed, Up.z * -(float)Time::m_deltaTime * m_speed });
	}

	if (Input::KeyCheck(Key::UPARROW, eKeyState::STAY))
	{
		m_GameObject->GetComponent<Transform>()->Translate({ LookAt.x * (float)Time::m_deltaTime * m_speed, LookAt.y * (float)Time::m_deltaTime * m_speed, LookAt.z * (float)Time::m_deltaTime * m_speed });
	}

	if (Input::KeyCheck(Key::LEFTARROW, eKeyState::STAY))
	{
		m_GameObject->GetComponent<Transform>()->Translate({ Right.x * -(float)Time::m_deltaTime * m_speed, Right.y * -(float)Time::m_deltaTime * m_speed, Right.z * -(float)Time::m_deltaTime * m_speed });
	}

	if (Input::KeyCheck(Key::DOWNARROW, eKeyState::STAY))
	{
		m_GameObject->GetComponent<Transform>()->Translate({ LookAt.x * -(float)Time::m_deltaTime * m_speed, LookAt.y * -(float)Time::m_deltaTime * m_speed, LookAt.z * -(float)Time::m_deltaTime * m_speed });
	}

	if (Input::KeyCheck(Key::RIGHTARROW, eKeyState::STAY))
	{
		m_GameObject->GetComponent<Transform>()->Translate({ Right.x * (float)Time::m_deltaTime * m_speed, Right.y * (float)Time::m_deltaTime * m_speed, Right.z * (float)Time::m_deltaTime * m_speed });
	}
}
