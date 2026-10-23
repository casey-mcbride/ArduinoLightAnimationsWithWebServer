#include "ColorPairPalette.h"

ColorPairPalette::ColorPairPalette(const Color* colorPairs, int numColorPairs)
{
	updateColors(colorPairs, numColorPairs);
	randomColorPairIndex = random(numColorPairs);
}

int ColorPairPalette::getNumColorPairs()
{
	return numColorPairs;
}

void ColorPairPalette::getColorPair(int index, Color& color1, Color& color2)
{
	assertPrint(index >= 0 && index < numColorPairs, "Color index out of range");

	int offsetIndex = index * 2;
	color1 = colorPairs[offsetIndex];
	color2 = colorPairs[offsetIndex + 1];
}

void ColorPairPalette::updateColors(const Color* colorPairs, int numColorPairs)
{
	this->colorPairs = colorPairs;
	this->numColorPairs = numColorPairs;
}