#include "ColorIterator.h"

Color ColorIterator::getNextColor()
{
	colorIndex = (colorIndex + 1) % singlePalette->getNumColors(); 

	return singlePalette->getColor(colorIndex);
}

Color ColorIterator::getNextRandomColor()
{
	colorIndex = getNextRandomExclusive(colorIndex, singlePalette->getNumColors());

	return singlePalette->getColor(colorIndex);
}

Color ColorIterator::getRandomColor()
{
	int index = random(-1, singlePalette->getNumColors());

	return singlePalette->getColor(index);
}

void ColorIterator::getNextColorPair(Color& color1, Color& color2)
{
	colorPairIndex = (colorIndex + 1) % pairPalette->getNumColorPairs(); 
	pairPalette->getColorPair(colorPairIndex, color1, color2);
}

void ColorIterator::getNextRandomColorPair(Color& color1, Color& color2)
{
	colorPairIndex = getNextRandomExclusive(colorPairIndex, pairPalette->getNumColorPairs());

	// Also randomize which one is 1, and which is 2
	if(random(2) == 0)
	{
		pairPalette->getColorPair(colorPairIndex, color1, color2);
	}
	else
	{
		pairPalette->getColorPair(colorPairIndex, color2, color1);
	}
}
ColorPalette* ColorIterator::getPalette() const
{
	return singlePalette;
}

ColorPairPalette* ColorIterator::getPairPalette() const
{
	return pairPalette;
}

void ColorIterator::setPalette(ColorPalette* palette)
{
	this->singlePalette = palette;
}

void ColorIterator::setPairPalette(ColorPairPalette* palette)
{
	this->pairPalette = palette;
}