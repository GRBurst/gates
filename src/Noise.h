/*
 * Noise.h
 *
 *  Created on: Jan 11, 2016
 *      Author: adrian
 */

#ifndef SRC_NOISE_H_
#define SRC_NOISE_H_

class Noise
{
	public:
		enum NoiseType
			        {
			            SIMPLEX = 0,
			            PERLIN,
			            WORLEY
			        };
		Noise();
		Noise(int x, int y, NoiseType noiseType);
		virtual ~Noise();

		void generate(int seed);
		float getTexture();
	private:
		int x, y;
		double textureBuffer[][];
		void perlin(double x, double y);
		double gradient();

};

#endif /* SRC_NOISE_H_ */
