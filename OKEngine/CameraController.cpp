#include "EnginePCH.h"
#include "Input.h"
#include "Time.h"
#include "CameraController.h"
#include "Camera.h"
#include "Transform.h"

void CameraController::Init()
{

}

void CameraController::Update()
{
	static POINTS preclicked{ 0,0 };
	int m_speed = 3.0f;
	std::shared_ptr<Camera> pCamera = GetComponent<Camera>();

	if (pCamera->GetUsingNOW())
	{

		Vector3 Up = pCamera->GetUp();
		Vector3 Right = pCamera->GetRight();
		Vector3 LookAt = pCamera->GetLookAt();


		if (Input::KeyCheck(Key::RIGHTMOUSE, eKeyState::DOWN))
		{
			preclicked = Input::GetMousePos();
		}

		if (Input::KeyCheck(Key::RIGHTMOUSE, eKeyState::STAY))
		{
			POINTS nowclicked = Input::GetMousePos();
			if (preclicked.x != 0) {
				float deltax = (nowclicked.x - preclicked.x) * 0.1f;
				float deltay = (nowclicked.y - preclicked.y) * 0.1f;
				//m_GameObject->GetComponent<Transform>()->SetRotation({DegToRad(90), DegToRad(23), 0 });
				m_GameObject->GetComponent<Transform>()->Rotate({ DegToRad(deltay), DegToRad(deltax), 0 });
			}
			preclicked = nowclicked;
		}

		if (Input::KeyCheck(Key::E, eKeyState::STAY))
		{
			//m_GameObject->GetComponent<Transform>()->SetRotation({3.1f, 0.0f, 0.0f });

			m_GameObject->GetComponent<Transform>()->Translate({ Up.x * (float)Time::m_deltaTime * m_speed, Up.y * (float)Time::m_deltaTime * m_speed, Up.z * (float)Time::m_deltaTime * m_speed });
		}

		if (Input::KeyCheck(Key::Q, eKeyState::STAY))
		{
			//m_GameObject->GetComponent<Transform>()->SetRotation({ 3.4f, 0.0f, 0.0f });

			m_GameObject->GetComponent<Transform>()->Translate({ Up.x * -(float)Time::m_deltaTime * m_speed, Up.y * -(float)Time::m_deltaTime * m_speed, Up.z * -(float)Time::m_deltaTime * m_speed });
		}

		if (Input::KeyCheck(Key::W, eKeyState::STAY))
		{
			m_GameObject->GetComponent<Transform>()->Translate({ LookAt.x * (float)Time::m_deltaTime * m_speed, LookAt.y * (float)Time::m_deltaTime * m_speed, LookAt.z * (float)Time::m_deltaTime * m_speed });
		}

		if (Input::KeyCheck(Key::A, eKeyState::STAY))
		{
			m_GameObject->GetComponent<Transform>()->Translate({ Right.x * -(float)Time::m_deltaTime * m_speed, Right.y * -(float)Time::m_deltaTime * m_speed, Right.z * -(float)Time::m_deltaTime * m_speed });
		}

		if (Input::KeyCheck(Key::S, eKeyState::STAY))
		{
			m_GameObject->GetComponent<Transform>()->Translate({ LookAt.x * -(float)Time::m_deltaTime * m_speed, LookAt.y * -(float)Time::m_deltaTime * m_speed, LookAt.z * -(float)Time::m_deltaTime * m_speed });
		}

		if (Input::KeyCheck(Key::D, eKeyState::STAY))
		{
			m_GameObject->GetComponent<Transform>()->Translate({ Right.x * (float)Time::m_deltaTime * m_speed, Right.y * (float)Time::m_deltaTime * m_speed, Right.z * (float)Time::m_deltaTime * m_speed });
		}
	}
}
