#pragma once
#include "Animation.h"

namespace KREngine::Graphics
{
	struct AnimationClip
	{
		std::string name;
		float ticksDuration = 0.0f;
		float ticksPerSecond = 0.0f;
		std::vector<std::unique_ptr<Animation>>boneAnimation;

	};
}