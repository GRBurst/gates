/*
 * Noise.h
 *
 *  Created on: Jan 11, 2016
 *      Author: adrian
 */

#ifndef SRC_NOISE_H_
#define SRC_NOISE_H_

#include <random>
#include <iostream>
#include <fstream>
class Noise
{
	public:
		enum NoiseType
			        {
			            SIMPLEX = 0,
			            PERLIN,
						WHITE,
			            WORLEY
			        };
		Noise();
		Noise(int x, int y, NoiseType noiseType);
		virtual ~Noise();

		void generate(int seed);
		void generateNoise(int seed, int samplesPerLattice);
		float getTexture();
		bool saveToFile(const char* filename);
	private:
		int xMax, yMax;
		double** gradientNoise;
		double** smoothNoise;
		void perlin(double x, double y);
		void blend();

		double gradient();
		double lerp(double x0, double x1, double alpha);

};

#endif /* SRC_NOISE_H_ */
