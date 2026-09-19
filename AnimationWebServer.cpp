#include "AnimationSets.h"

#define MIN_BRIGHTNESS 5
#define MAX_BRIGHTNESS 100

#ifdef DEBUG
	static const ulong animationMilliSeconds = (ulong)12 * (ulong)1000;// need to cast, otherwise you get int max, idky
#else
	static const ulong animationMilliSeconds = (ulong)90 * (ulong)1000;
#endif

static ulong startOfCurrentAnimation = millis();
static int lightBrightness = 75;
static Color plainStrandColor = Color::White;
static AnimationMode currentMode = AnimationMode::RandomAnimation;
static int currentAnimationIndex = 0;

void AnimationWebServer::startServer()
{
	// TODO: Add web server logic

	initFastLeds();
	// setStrandColor(plainStrandColor);
	FastLED.setBrightness(lightBrightness);
	FastLED.show();

	currentMode = AnimationMode::RandomAnimation;
}

void AnimationWebServer::startAnimationLoop()
{
	if(NUM_ANIMATIONS <= 0)
	{
		debug("No animations, so exiting animation loop");
		return;
	}

	debug("Starting animation loop");

	while(true)
	{
		switch(currentMode)
		{
			case AnimationMode::PlainColor:
				delayUnlessInterrupted(100000);
				debug("Doing plain color animation");
			break;
			case AnimationMode::HoldAnimation:
				debug("Doing hold animation");
				startOfCurrentAnimation = millis();
				ANIMATIONS[currentAnimationIndex]();
			break;

			case AnimationMode::RandomAnimation:
				debug("Doing RandomAnimation ");
				startOfCurrentAnimation = millis();
				ANIMATIONS[currentAnimationIndex]();
				// If we're still in the rotation mode, increment
				if(currentMode == AnimationMode::RandomAnimation)
					chooseNextRandomAnimation();
			break;
		}
	}
}

void AnimationWebServer::chooseNextRandomAnimation()
{
	currentAnimationIndex = getNextRandomExclusive(currentAnimationIndex, NUM_ANIMATIONS);
	toggleLedDirection();
	debugValue("Newly picked animation index", currentAnimationIndex);
}

bool AnimationWebServer::delayUnlessInterrupted(int delayMillieseconds)
{
	ulong startTime = millis();
	while(millis() - startTime < delayMillieseconds)
	{
		// TODO: Add server code that detects an interrupt
	}

	return false;
}

bool AnimationWebServer::shouldCurrentAnimationContinue()
{
	return currentMode == AnimationMode::HoldAnimation || millis() - startOfCurrentAnimation < animationMilliSeconds;
}

void AnimationWebServer::setStrandColor(Color color)
{
	plainStrandColor = color;
	for(int i = 0; i < NUM_LEDS; i++)
	{
		setLed(i, plainStrandColor);
	}
	currentMode = AnimationMode::PlainColor;
	FastLED.show();
}

void AnimationWebServer::setStrandBrightness(int brightness)
{
	lightBrightness = clamp(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
	FastLED.setBrightness(lightBrightness);
	FastLED.show();
}