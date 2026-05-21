#pragma once

#include <SFML/Graphics.hpp>

namespace KREngine
{
	class CameraService
	{
	public:
		static CameraService& GetInstance();

		void Initialize();
		void Reset();
		void Apply3DView(float aspectRatio) const;
		void UpdateKeyboardControls(sf::Time deltaTime);
		void UpdateMouseControls(const sf::RenderWindow& window, bool enabled, float mouseWheel);

		sf::Vector3f& Target();
		float& Distance();
		float& Pitch();
		float& Yaw();

	private:
		CameraService() = default;

		sf::Vector3f mTarget = { 0.0f, 0.0f, 0.0f };
		float mDistance = 8.0f;
		float mPitch = 18.0f;
		float mYaw = 0.0f;
		sf::Vector2i mLastMousePosition = { 0, 0 };
		bool mWasOrbiting = false;
		bool mWasPanning = false;
	};
}
