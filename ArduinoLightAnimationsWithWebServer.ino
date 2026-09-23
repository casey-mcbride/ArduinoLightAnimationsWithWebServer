// #include "LedClusterAnimations.h"
#include "AnimationWebServer.h"
#include "Configuration.h"
#include "Basic.h"

#include "BasicAnimations.h"
#include "AnimationSets.h"
// #include "ColorUtils.h"
// #include "BasicAnimations.h"
// #include "ColorUtils.h"

// typedef void* (int) AnimationFunction;
// typedef void* (int) AnimationFunction;

void setup()
{
	initDebugIO();
	initFastLeds();

	// If nothing in 0, noise produces a semi-random number
	randomSeed(analogRead(0));

	AnimationWebServer::startServer();
}

void loop()
{
	AnimationWebServer::startAnimationLoop();
}