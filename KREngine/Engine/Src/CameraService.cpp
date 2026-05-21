#include "Precompiled.h"
#include "CameraService.h"
#include <SFML/OpenGL.hpp>
#include <algorithm>
#include <cmath>

namespace KREngine
{
	CameraService& CameraService::GetInstance()
	{
		static CameraService instance;
		return instance;
	}

	void CameraService::Initialize()
	{
		Reset();
	}

	void CameraService::Reset()
	{
		mTarget = { 0.0f, 0.0f, 0.0f };
		mDistance = 8.0f;
		mPitch = 18.0f;
		mYaw = 0.0f;
	}

	void CameraService::Apply3DView(float aspectRatio) const
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-aspectRatio, aspectRatio, -1.0, 1.0, 1.0, 100.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.0f, -0.2f, -mDistance);
		glRotatef(mPitch, 1.0f, 0.0f, 0.0f);
		glRotatef(mYaw, 0.0f, 1.0f, 0.0f);
		glTranslatef(-mTarget.x, -mTarget.y, -mTarget.z);
	}

	void CameraService::UpdateKeyboardControls(sf::Time deltaTime)
	{
		const float seconds = deltaTime.asSeconds();
		const float panSpeed = 3.0f * seconds;
		const float orbitSpeed = 90.0f * seconds;
		const float zoomSpeed = 6.0f * seconds;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) mTarget.x -= panSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) mTarget.x += panSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) mTarget.z -= panSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) mTarget.z += panSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) mTarget.y -= panSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) mTarget.y += panSpeed;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) mYaw -= orbitSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) mYaw += orbitSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) mPitch -= orbitSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) mPitch += orbitSpeed;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) mDistance -= zoomSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) mDistance += zoomSpeed;

		mPitch = std::clamp(mPitch, -80.0f, 80.0f);
		mDistance = std::clamp(mDistance, 3.0f, 20.0f);
	}

	void CameraService::UpdateMouseControls(const sf::RenderWindow& window, bool enabled, float mouseWheel)
	{
		if (!enabled)
		{
			mWasOrbiting = false;
			mWasPanning = false;
			return;
		}

		if (mouseWheel != 0.0f)
		{
			mDistance = std::clamp(mDistance - mouseWheel * 0.8f, 3.0f, 20.0f);
		}

		const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
		const bool orbiting = sf::Mouse::isButtonPressed(sf::Mouse::Right);
		const bool panning = sf::Mouse::isButtonPressed(sf::Mouse::Middle);

		if ((orbiting && !mWasOrbiting) || (panning && !mWasPanning))
		{
			mLastMousePosition = mousePosition;
		}

		const sf::Vector2i mouseDelta = mousePosition - mLastMousePosition;

		if (orbiting)
		{
			mYaw += static_cast<float>(mouseDelta.x) * 0.25f;
			mPitch += static_cast<float>(mouseDelta.y) * 0.25f;
			mPitch = std::clamp(mPitch, -80.0f, 80.0f);
		}
		else if (panning)
		{
			const float yawRadians = mYaw * 3.14159265f / 180.0f;
			const float rightX = std::cos(yawRadians);
			const float rightZ = -std::sin(yawRadians);
			const float panScale = std::max(0.01f, mDistance * 0.0025f);

			mTarget.x -= rightX * static_cast<float>(mouseDelta.x) * panScale;
			mTarget.z -= rightZ * static_cast<float>(mouseDelta.x) * panScale;
			mTarget.y += static_cast<float>(mouseDelta.y) * panScale;
		}

		mLastMousePosition = mousePosition;
		mWasOrbiting = orbiting;
		mWasPanning = panning;
	}

	sf::Vector3f& CameraService::Target()
	{
		return mTarget;
	}

	float& CameraService::Distance()
	{
		return mDistance;
	}

	float& CameraService::Pitch()
	{
		return mPitch;
	}

	float& CameraService::Yaw()
	{
		return mYaw;
	}
}
