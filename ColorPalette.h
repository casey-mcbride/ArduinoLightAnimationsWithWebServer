#pragma once
#include "Basic.h"

class ColorPalette
{
	int randomColorIndex = 0;
	const Color* colors = nullptr;
	int numColors = 0;

public:
	ColorPalette(const Color* colors, int numColors);
	int getNumColors();
	Color getColor(int colorIndex) const;
	Color getNextRandomColor();
	Color getRandomColor();
};
