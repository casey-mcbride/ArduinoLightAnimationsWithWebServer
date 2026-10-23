#include "ColorPalette.h"

ColorPalette::ColorPalette(const Color* colors, int numColors)
{
	updateColors(colors, numColors);
}

int ColorPalette::getNumColors()
{
	return numColors;
}

Color ColorPalette::getColor(int colorIndex) const
{
	assertPrint(colorIndex >= 0 && colorIndex < numColors, "Color index out of bounds");
	return colors[colorIndex];
}

void ColorPalette::updateColors(const Color* colors, int numColors)
{
	this->colors = colors;
	this->numColors = numColors;
}