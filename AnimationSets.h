#pragma once

#include "Configuration.h"
#include "FastLED.h"
#include "ColorUtils.h"
#include "BasicAnimations.h"
#include "LedClusterAnimations.h"
#include "Basic.h"
#include "AnimationWebServer.h"
#include "ColorSets.h"

#ifdef INCLUDE_RAINBOW_ANIMATIONS

void rainbowColorBeamCollisionAnimation()
{
	colorBeamAnimation(COMPLEX_RAINBOW_COLOR_PALETTE);
}

void rainbowColorThrob()
{
	colorThrob(COMPLEX_RAINBOW_COLOR_PALETTE);
}

void randomMixedRainbowWaveAnimation()
{
	Color color1, color2;
	COMPLEX_RAINBOW_COLOR_PAIR_PALETTE.getNextRandomColorPair(color1, color2);

	mixedWaveAnimation(color1, color2);
}

void linnaeusFavoriteBrightSpotsAnimation()
{
	Color linnaeusColors[] = {Color::Green, Color::Blue};
	ColorPalette palette(linnaeusColors, getStaticArraySize(linnaeusColors));
	randomBrightSpots(palette, 10);
}

void greenBlueMarchAnimation()
{
	Color colors[] = {Color::Green, Color::Blue};
	ColorPalette palette(colors, getStaticArraySize(colors));

	colorMarch(palette, 2);
}

void greenBlueThrobAnimation()
{
	Color colors[] = {Color::Green, Color::Blue};
	ColorPalette palette = ColorPalette(colors, getStaticArraySize(colors));

	colorThrob(palette);
}

void greenBlueWavesAnimation()
{
	mixedWaveAnimation(LIME_GREEN, ROYAL_BLUE);
}

void rainbowLineSwap()
{
	lineSwap(COMPLEX_RAINBOW_COLOR_PALETTE);
}

void rainbowColorHillAnimation()
{
	colorHillAnimation(COMPLEX_RAINBOW_COLOR_PALETTE);
}

void rainbowColorBeamAnimation()
{
	colorBeamAnimation(COMPLEX_RAINBOW_COLOR_PALETTE);
}
#endif

#ifdef INCLUDE_HALLOWEEN_ANIMATIONS

void halloweenBlinkAnimation()
{
	Color colors[] = {Color::Purple, Color::Orange, Color::Green};
	ColorPalette palette(colors, getStaticArraySize(colors));
	randomBrightSpots(palette, 10);
}

void halloweenLineSwap()
{
	lineSwap(HALLOWEEN_COLOR_PALETTE);
}

void halloweenBeamCollision()
{
	colorBeamCollisionAnimation(HALLOWEEN_COLOR_PAIR_PALETTE);
}

void halloweenRedBeamCollision()
{
	Color colors[] = {Color::Red, Color::Red};
	ColorPairPalette palette(colors, getStaticArraySize(colors) / 2);
	colorBeamCollisionAnimation(palette);
}

void halloweenMixedWave()
{
	Color one, two;
	HALLOWEEN_COLOR_PAIR_PALETTE.getNextRandomColorPair(one, two);
	mixedWaveAnimation(one, two);
}

#endif

#ifdef INCLUDE_THANKSGIVING_ANIMATIONS

void candyCornMarch()
{
	Color colors[] = {Color::White, Color::OrangeRed, GOLDEN_YELLOW};
	ColorPalette palette(colors, getStaticArraySize(colors));

	colorMarch(palette, 3);
}

void thanksgivingBlinkAnimation()
{
	randomBrightSpots(THANKSGIVING_COLOR_PALETTE, 10);
}

void thanksgivingLineSwap()
{
	lineSwap(THANKSGIVING_COLOR_PALETTE);
}

void thanksgivingBeamCollision()
{
	colorBeamCollisionAnimation(THANKSGIVING_COLOR_PAIR_PALETTE);
}

void thanksgivingMixedWave()
{
	Color one, two;
	THANKSGIVING_COLOR_PAIR_PALETTE.getNextRandomColorPair(one, two);
	mixedWaveAnimation(one, two);
}

#endif

#ifdef INCLUDE_CHRISTMAS_ANIMATIONS

void candyCaneMixedWave()
{
	mixedWaveAnimation(Color::Red, DIM_WHITE);
}

void christmasWhiteAndBlueMarch()
{
	Color colors[] = {Color::White, Color::Blue};
	ColorPalette palette(colors, getStaticArraySize(colors));

	colorMarch(palette, 2);
}

void christmasWhiteAndBlueMixedWave()
{
	Color colors[] = {Color::White, Color::Blue};
	ColorPalette palette(colors, getStaticArraySize(colors));

	mixedWaveAnimation(Color::White, Color::Blue);
}

void christmasBlinkAnimation()
{
	randomBrightSpots(CHRISTMAS_COLOR_PALETTE, 10);
}

void christmasLineSwap()
{
	lineSwap(CHRISTMAS_COLOR_PALETTE);
}

void christmasBeamCollision()
{
	colorBeamCollisionAnimation(CHRISTMAS_COLOR_PAIR_PALETTE);
}

void christmasBrightSpots()
{
	randomBrightSpots(CHRISTMAS_COLOR_PALETTE, 10);
}

#endif

using AnimationFunction = void(void);
AnimationFunction* ANIMATIONS[] =
{
#ifdef INCLUDE_RAINBOW_ANIMATIONS
	&rainbowColorHillAnimation,
	&rainbowColorBeamCollisionAnimation,
	&greenBlueWavesAnimation,
	&rainbowColorBeamAnimation,
	&greenBlueThrobAnimation,
	&linnaeusFavoriteBrightSpotsAnimation,
	&randomMixedRainbowWaveAnimation,
	&rainbowLineSwap,
#endif

#ifdef INCLUDE_HALLOWEEN_ANIMATIONS
	&firelightAnimation,
	&halloweenMixedWave,
	&halloweenLineSwap,
	&halloweenBeamCollision,
	&halloweenBlinkAnimation,
	&candyCornMarch,
	&halloweenRedBeamCollision,
#endif

#ifdef INCLUDE_THANKSGIVING_ANIMATIONS
	&candyCornMarch,
	&firelightAnimation,
	&thanksgivingMixedWave,
	&thanksgivingLineSwap,
	&thanksgivingBeamCollision,
	&thanksgivingBlinkAnimation,
#endif

#ifdef INCLUDE_CHRISTMAS_ANIMATIONS
	&christmasWhiteAndBlueMixedWave,
	&christmasWhiteAndBlueMarch,
	&christmasBeamCollision,
	&christmasBlinkAnimation,
	&christmasLineSwap,
	&christmasBrightSpots,
	&candyCaneMixedWave,
#endif
};

int NUM_ANIMATIONS = getStaticArraySize(ANIMATIONS);