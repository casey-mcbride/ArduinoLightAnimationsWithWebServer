#include "ColorPairPalette.h"

ColorPairPalette::ColorPairPalette(const Color* colorPairs, int numColorPairs)
{
	this->colorPairs = colorPairs;
	this->numColorPairs = numColorPairs;
	randomColorPairIndex = random(numColorPairs);
}

void ColorPairPalette::getNextRandomColorPair(Color& color1, Color& color2)
{
	randomColorPairIndex = getNextRandomExclusive(randomColorPairIndex, numColorPairs);

	// Also randomize which one is 1, and which is 2
	int offsetIndex = randomColorPairIndex * 2;
	if(random(2) == 0)
	{
		color1 = colorPairs[offsetIndex];
		color2 = colorPairs[offsetIndex + 1];
	}
	else
	{
		color1 = colorPairs[offsetIndex + 1];
		color2 = colorPairs[offsetIndex];
	}
}
