#pragma once

#include "Basic.h"
#include "FastLED.h"
#include "ColorUtils.h"
#include "BasicAnimations.h"

enum AnimationMode
{
	PlainColor,
	HoldAnimation,
	RandomAnimation,
};

// A class for controlling animations that the user can interact with via a webserver.
class AnimationWebServer
{
public:
	static void startServer();
	static void startAnimationLoop();

	// Delays the animation, while also waiting for interupts. Returns true if the current animation should be cancelled and returned from
	static bool delayUnlessInterrupted(int delayMillieseconds);
	static bool shouldCurrentAnimationContinue();

private:

	AnimationWebServer() = delete;

	static void chooseNextRandomAnimation();
	static void setStrandColor(Color color);
	static void setStrandBrightness(int brightness);
};