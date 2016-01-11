/*
 * Noise.cpp
 *
 *  Created on: Jan 11, 2016
 *      Author: adrian
 */

#include "Noise.h"


Noise::Noise()
{
	// TODO Auto-generated constructor stub

}

Noise::Noise(int x, int y, NoiseType noise)
{
	this->x = x;
	this->y = y;
	textureBuffer = new double[x][y];

}

Noise::~Noise()
{
	// TODO Auto-generated destructor stub
}

Noise::generate(int seed){

}
