/*
 * Noise.cpp
 *
 *  Created on: Jan 11, 2016
 *      Author: adrian
 */

#include "Noise.h"

Noise::Noise() : mWidth(256), mHeight(256), mSeed(1024), mOctaves(2), mFrequency(2), mAmplitude(1),
		mPermutationTable(std::vector<int>(0)),	mGradientTable2d(std::vector<glm::dvec2>())
{
	// TODO Auto-generated constructor stub

}

Noise::Noise(int x, int y, NoiseType noise, int seed, int octaves, double frequency, double amplitude)
{
	this->mWidth = x;
	this->mHeight = y;
	this->mSeed = seed;
	this->mOctaves = octaves;
	this->mFrequency = frequency;
	this->mAmplitude = amplitude;
	this->mPermutationTable = std::vector<int>(mSampleSize);
	this->mGradientTable2d = std::vector<glm::dvec2>(mSampleSize);
	mGradientNoise = new double* [x];
	for (int i = 0; i < x; i++){
		mGradientNoise[i] = new double[y]();
	}
	mNoise = new double[mWidth * mHeight];
	mNoiseF = new float[mWidth * mHeight];


}

Noise::~Noise()
{
	// TODO Auto-generated destructor stub
}

void Noise::setParams(int x, int y, NoiseType noiseType, int seed, int octaves, double frequency, double amplitude)
{
    this->mWidth = x;
    this->mHeight = y;
    this->mSeed = seed;
    this->mOctaves = octaves;
    this->mFrequency = frequency;
    this->mAmplitude = amplitude;
    this->mPermutationTable = std::vector<int>(mSampleSize);
    this->mGradientTable2d = std::vector<glm::dvec2>(mSampleSize);
    mGradientNoise = new double* [x];
    for (int i = 0; i < x; i++)
    {
        mGradientNoise[i] = new double[y]();
    }
    mNoise = new double[mWidth * mHeight]();
    mNoiseF = new float[mWidth * mHeight]();
}
/*
void Noise::generate(int seed){
	int octave = 2;
	//generateNoise(seed);
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
*/
void Noise::generateNoiseImage(){

	if(mStart){
		mStart = false;
		initPermutationTable();
		initGradientTable();
	}

	for(int x = 0; x < mWidth; x++){
		for(int y = 0; y < mHeight; y++){
			double value = perlinNoise2D(x, y);
			mNoise[y * mWidth + x] = value;
			if (value < mMin)
				mMin = value;
			if (value > mMax)
				mMax = value;
		}
	}
	mMax = mMax - mMin;
	for(int x = 0; x < mWidth; x++){
		for(int y = 0; y < mHeight; y++){
			mNoise[y * mWidth + x] = (mNoise[y * mWidth + x] - mMin) / mMax;
			mNoiseF[y * mWidth + x]    = mNoise[y * mWidth + x];
		}
	}


}

double Noise::perlinNoise2D(double x, double y){
	double result = 0.0;
	double amp = mAmplitude;
	x = x / (double)mWidth;
	y = y / (double)mHeight;
	x *= mFrequency;
	y *= mFrequency;
	for(int i = 0; i < mOctaves; i++){
		result += calculateNoiseValue(x, y) * amp;
		x *= 2.0;
		y *= 2.0;
		amp *= 0.5;
	}
	return result;
}

//double Noise::clamp(double x, double min, double max){
//	if (x > max)
//		x = max;
//	if (x < min)
//		x = min;
//	return x;
//}

double Noise::calculateNoiseValue(double x, double y){
	int N = 4096;
	glm::dvec2 inputVector(x, y);
	int b00, b10, b01, b11;
	double a, b, u, v;
	int i, j;
	glm::ivec2 bx, by;
	glm::dvec2 t, rx, ry, s, q;
	t = inputVector + glm::dvec2(N, N);
	//std::cout << t.x << ", " << t.y << std::endl;
	bx.x = ((int)t.x) & (mSampleSize - 1);
	bx.y = (bx.x + 1) & (mSampleSize - 1);

	rx.x = t.x - (int)t.x;
	rx.y = rx.x - 1.0;
//	std::cout << bx.x << ", " << bx.y << std::endl;
//	std::cout << rx.x << ", " << rx.y << std::endl;

	by.x = ((int)t.y) & (mSampleSize - 1);
	by.y = (by.x + 1) & (mSampleSize - 1);

	ry.x = t.y - (int)t.y;
	ry.y = ry.x - 1.0;
//	std::cout << by.x << ", " << by.y << std::endl;
//	std::cout << ry.x << ", " << ry.y << std::endl;
//	std::cin.get();
	i = mPermutationTable[bx.x];
	j = mPermutationTable[bx.y];

	b00 = mPermutationTable[i + by.x];
	b10 = mPermutationTable[j + by.x];
	b01 = mPermutationTable[i + by.y];
	b11 = mPermutationTable[j + by.y];

	s = interpolationPolynomial2D(glm::dvec2(rx.x, ry.x));

	q = glm::dvec2(mGradientTable2d.at(b00));
	u = glm::dot(q, glm::dvec2(rx.x, ry.x));
	q = glm::dvec2(mGradientTable2d.at(b10));
	v = glm::dot(q, glm::dvec2(rx.y, ry.x));
	a = lerp(s.x, u, v);


	q = glm::dvec2(mGradientTable2d.at(b01));
	u = glm::dot(q, glm::dvec2(rx.x, ry.y));
	q = glm::dvec2(mGradientTable2d.at(b11));
	v = glm::dot(q, glm::dvec2(rx.y, ry.y));
	b = lerp(s.x, u, v);

//	return clamp(lerp(s.y, a, b), 0.0, 1.0);
	return lerp(s.y, a, b);
}


double Noise::scaleToZeroToOne(double value){

}

glm::dvec2 Noise::interpolationPolynomial2D(glm::dvec2 vec){
	double x = interpolationPolynomial2D(vec.x);
	double y = interpolationPolynomial2D(vec.y);
	return glm::dvec2(x, y);
}

double Noise::interpolationPolynomial2D(double t){
	return t * t * t * (t * (t * 6 - 15) + 10);
}

void Noise::initPermutationTable(){
	std::default_random_engine generator(mSeed);
	iota(mPermutationTable.begin(), mPermutationTable.end(), 0);
	std::cout << mPermutationTable.size() << std::endl;
//	for(std::vector<int>::iterator it = mPermutationTable.begin(); it != mPermutationTable.end(); it++){
//		std::cout << *it << std::endl;
//	}
	std::shuffle(mPermutationTable.begin(), mPermutationTable.end(), generator);
//	for(std::vector<int>::iterator it = mPermutationTable.begin(); it != mPermutationTable.end(); it++){
//		std::cout << *it << std::endl;
//	}
	mPermutationTable.insert(std::end(mPermutationTable), std::begin(mPermutationTable), std::end(mPermutationTable));
//	for(std::vector<int>::iterator it = mPermutationTable.begin(); it != mPermutationTable.end(); it++){
//		std::cout << *it << std::endl;
//	}
	std::cout << "XXXXXXXXXXXXXXX" << std::endl;

}

void Noise::initGradientTable(){
	std::default_random_engine generator(mSeed);
	std::uniform_real_distribution<double> distribution (-1.0, 1.0);
	for(int i = 0; i < mSampleSize; i++){
	//mGradientTable2d.at(i) = glm::dvec2();
		for (int j = 0; j < 2; j++)
			mGradientTable2d.at(i)[j] = distribution(generator);
		mGradientTable2d.at(i) = glm::normalize(mGradientTable2d.at(i));
		//std::cout << mGradientTable2d.at(i).x << ", b0  " <<  mGradientTable2d.at(i).y << std::endl;
	}
	//std::cin.get();
	mGradientTable2d.insert(std::end(mGradientTable2d), std::begin(mGradientTable2d), std::end(mGradientTable2d));
}


double Noise::lerp(double alpha, double x0, double x1){
	return x0 + alpha * (x1 - x0);
}
bool Noise:: saveToFile(const char* filename){
	char* bmp = new char[mWidth*mHeight];
	char* it = bmp;
	for (int x = 0; x < this-> mWidth * this->mHeight; x++){
		*it = (char)(mNoise[x] * 255);
		it++;
			//std::cout<< (y + x) + y * mWidth << std::endl;
			//std::cout << mNoise[x][y] << std::endl;
	}


	uint16_t header[9] = {0, 3, 0, 0 ,0 ,0, (uint16_t)mWidth, (uint16_t)mHeight, 8};
	std::ofstream file(filename, std::ios::binary);
	if(file.is_open()){
		file.write((char*) (&header), sizeof(header));
		file.write(bmp, sizeof(char)*mWidth*mHeight);
		file.close();
	}
	else{
		std::cout << "Error TGA Save: File not found" << std::endl;
		return false;
	}

}

double *Noise::getTextureData(){
	return mNoise;
}

float *Noise::getTextureDataF(){
	return mNoiseF;
}
