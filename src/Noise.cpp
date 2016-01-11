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
	this->xMax = x;
	this->yMax = y;
	gradientNoise = new double* [x];
	for (int i = 0; i < x; i++){
		gradientNoise[i] = new double[y];
	}
	smoothNoise = new double* [x];
	for (int i = 0; i < x; i++){
		smoothNoise[i] = new double[y];
	}
}

Noise::~Noise()
{
	// TODO Auto-generated destructor stub
}

void Noise::generate(int seed){
	int octave = 2;
	generateNoise(seed);
	int wavelength = 1 << octave;
	double frequency = 1.0 / wavelength;

	for (int x = 0; x < this-> xMax; x++){
		//horizontales sampling
		int sample_x0 = (x / wavelength) * wavelength;
		int sample_x1 = (sample_x0 + wavelength) % xMax;
		double horBlend = (x- sample_x0) * wavelength;

		for (int y = 0; y < this-> yMax; y++){
			int sample_y0 = (y / wavelength) * wavelength;
			int sample_y1 = (sample_y0 + wavelength) % yMax;
			double verBlend = (y - sample_y0) * wavelength;
		}
	}

}

void Noise::generateNoise(int seed, int samplesPerLattice){
	std::default_random_engine generator(seed);
	std::uniform_real_distribution<double> distribution (0.0, 1.0);

	for (int y = 0; y < this-> yMax; y++){
		for (int x = 0; x < this-> xMax; x++){
			gradientNoise[x][y] = distribution(generator);
			std::cout<< (y + x) + y * xMax << std::endl;
		}
	}

}

bool Noise:: saveToFile(const char* filename){
	char* bmp = new char[xMax*yMax];
	char* it = bmp;
	for (int y = 0; y < this-> yMax; y++){
		for (int x = 0; x < this-> xMax; x++){
			*it = (char)(gradientNoise[x][y] * 255);
			it++;
			std::cout<< (y + x) + y * xMax << std::endl;
		}
	}

	uint16_t header[9] = {0, 3, 0, 0 ,0 ,0, (uint16_t)xMax, (uint16_t)yMax, 8};
	std::ofstream file(filename, std::ios::binary);
	if(file.is_open()){
		file.write((char*) (&header), sizeof(header));
		file.write(bmp, sizeof(char)*xMax*yMax);
		file.close();
	}
	else{
		std::cout << "Error TGA Save: File not found" << std::endl;
		return false;
	}

}
