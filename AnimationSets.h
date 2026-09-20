#pragma once

#include "Configuration.h"
#include "FastLED.h"
#include "ColorUtils.h"
#include "BasicAnimations.h"
#include "LedClusterAnimations.h"
#include "Basic.h"
#include "AnimationWebServer.h"
#include "ColorSets.h"

void rainbowColorBeamCollisionAnimation();

void rainbowColorThrob();

void randomMixedRainbowWaveAnimation();

void linnaeusFavoriteBrightSpotsAnimation();

void greenBlueMarchAnimation();

void greenBlueThrobAnimation();

void greenBlueWavesAnimation();

void rainbowLineSwap();

void rainbowColorHillAnimation();

void rainbowColorBeamAnimation();

void halloweenBlinkAnimation();

void halloweenLineSwap();

void halloweenBeamCollision();

void halloweenRedBeamCollision();

void halloweenMixedWave();

void candyCornMarch();

void thanksgivingBlinkAnimation();

void thanksgivingLineSwap();

void thanksgivingBeamCollision();

void thanksgivingMixedWave();

void christmasWhiteAndBlueMixedWave();

void candyCaneMixedWave();

void christmasWhiteAndBlueMarch();

void christmasBlinkAnimation();

void christmasLineSwap();

void christmasBeamCollision();

void christmasBrightSpots();

using AnimationFunction = void(void);
const static AnimationFunction* ANIMATIONS[] =
{
	&rainbowColorHillAnimation,
	&rainbowColorBeamCollisionAnimation,
	&greenBlueWavesAnimation,
	&rainbowColorBeamAnimation,
	&greenBlueThrobAnimation,
	&linnaeusFavoriteBrightSpotsAnimation,
	&randomMixedRainbowWaveAnimation,
	&rainbowLineSwap,

	&firelightAnimation,
	&halloweenMixedWave,
	&halloweenLineSwap,
	&halloweenBeamCollision,
	&halloweenBlinkAnimation,
	&candyCornMarch,
	&halloweenRedBeamCollision,

	&candyCornMarch,
	&firelightAnimation,
	&thanksgivingMixedWave,
	&thanksgivingLineSwap,
	&thanksgivingBeamCollision,
	&thanksgivingBlinkAnimation,

	&christmasWhiteAndBlueMixedWave,
	&christmasWhiteAndBlueMarch,
	&christmasBeamCollision,
	&christmasBlinkAnimation,
	&christmasLineSwap,
	&christmasBrightSpots,
	&candyCaneMixedWave,
};

static const int NUM_ANIMATIONS = getStaticArraySize(ANIMATIONS);