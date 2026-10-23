#pragma once
// Contains a set of animations for animating a string of led lights

#include "ColorUtils.h"
#include "Basic.h"
#include "ColorPalette.h"
#include "ColorPairPalette.h"
#include "ColorSets.h"



// Marches throught the given colors, with bandSize number of colors repeated in a row
void colorMarch(const int bandSize);

/*
// Animation that has two colors moving together in one direction, with one color then the next with them lerped together
void mixedWaveAnimation(Color color1, Color color2);

void colorThrob();

// One color beam comes from one direction and fades out, then a different color beam comes from the other, repeat
// The color starts dim, gets bright, gets dim again
void colorHillAnimation();

// One color beam comes from one direction and fades out, then a different color beam comes from the other, repeat
// The color starts bright, then gets dim near the tailend
void colorBeamAnimation();

// Two colors come from both sides and pass through eachother
void colorBeamCollisionAnimation();

// Randomly dim bulbs light up to a color, then fade back to black
void randomBrightSpots(int fadeTicks);

// One color goes up, making it solid then another color merges down merging and erasing the covered one, rinse repeat
void lineSwap();
*/