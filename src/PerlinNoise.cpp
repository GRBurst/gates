/*
 * PerlinNoise.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: adrian
 */

#include "PerlinNoise.h"
PerlinNoise::PerlinNoise()
{

}

PerlinNoise::~PerlinNoise()
{
	// TODO Auto-generated destructor stub
}

void PerlinNoise::printParams()
{
    std::cout << "mXDim = " << mXDim << std::endl;
    std::cout << "mYDim = " << mYDim << std::endl;
    std::cout << "mZDim = " << mZDim << std::endl;
    std::cout << "mSeed = " << mSeed << std::endl;
    std::cout << "mOctaves = " << mOctaves << std::endl;
    std::cout << "mStart = " << mStart << std::endl;
    std::cout << "mFrequency = " << mFrequency << std::endl;
    std::cout << "mAmplitude = " << mAmplitude << std::endl;
    std::cout << "mMin = " << mMin << std::endl;
    std::cout << "mMax = " << mMax << std::endl;
	std::cout << "mSampleSize = " << mSampleSize << std::endl;// = 256;

    /* std::cout << "first gradients = " */ 

    std::cout << "mNoiseValues values:" << std::endl;
	for(int y = 0; y < mYDim; y++){
	    for(int x = 0; x < mXDim; x++){
            if(x == mXDim-1) std::cout << mNoiseValues[y * mXDim + x] << std::endl;
            else std::cout << mNoiseValues[y * mXDim + x] << ", ";
		}
	}
	
    /* std::cout << "mGradientTable2d values:" << std::endl; */
    /* for (std::vector<glm::dvec2>::iterator it = mGradientTable2d.begin() ; it != mGradientTable2d.end(); ++it) */
    /*         std::cout << "x = " << it->x << ", y =  " <<  it->y << std::endl; */
		
	/* for (int i = 0; i < mXDim; i++){ */
	/* 	mGradientNoise[i] = new double[mYDim](); */
	/* } */
    std::cout << "mPermutationTable values:" << std::endl;
    for (std::vector<int>::iterator it = mPermutationTable.begin() ; it != mPermutationTable.end(); ++it)
        std::cout << ' ' << *it;
}

void PerlinNoise::generateNoiseImage(){
    if(mStart){
        mStart = false;
        initPermutationTable();
        if (mZDim == 1)
        	init2DGradientTable();
        else
        	init3DGradientTable();
    }

//    printParams();
    if (mZDim == 1){
    	for(int y = 0; y < mYDim; y++){
			for(int x = 0; x < mXDim; x++){
				float value = static_cast<float>(noise(static_cast<double>(x), static_cast<double>(y)));
				//value = value -floor(value);
				mNoiseValues[y * mXDim + x] = value;
				if (value < mMin)
					mMin = value;
				if (value > mMax)
					mMax = value;
			}
		}
    	mMax = mMax - mMin;
		for(int y = 0; y < mYDim; y++){
			for(int x = 0; x < mXDim; x++){
				mNoiseValues[y * mXDim + x] = (mNoiseValues[y * mXDim + x] - mMin) / mMax;
			}
		}
    }
    else
    {
    	for(int z = 0; z < mZDim; z++){
    		for(int y = 0; y < mYDim; y++){
				for(int x = 0; x < mXDim; x++){
					float value = static_cast<float>(noise(static_cast<double>(x), static_cast<double>(y), static_cast<double>(z)));
					mNoiseValues[z * mYDim * mXDim + y * mXDim + x] = value;
//					if (value < mMin)
//						mMin = value;
//					if (value > mMax)
//						mMax = value;
				}
			}
    	}
    }



    std::cout << "perlinNoise: min = " << mMin << ", max = " << mMax << std::endl;
}

double PerlinNoise::noise(double x, double y){
	double result = 0.0;
	double amp = mAmplitude;
	x = x / (double)mXDim;
	y = y / (double)mYDim;
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

double PerlinNoise::noise(double x, double y, double z){
	double result = 0.0;
	double amp = mAmplitude;
	x = x / (double)mXDim;
	y = y / (double)mYDim;
	z = z / (double)mZDim;
	x *= mFrequency;
	y *= mFrequency;
	z *= mFrequency;
	for(int i = 0; i < mOctaves; i++){
		result += calculateNoiseValue(x, y, z) * amp;
		x *= 2.0;
		y *= 2.0;
		z *= 2.0;
		amp *= 0.2;
	}
	return result;
}

//double clamp(double x, double min, double max){
//	if (x > max)
//		x = max;
//	if (x < min)
//		x = min;
//	return x;
//}

void PerlinNoise::setGridPoints(double inputVector, int &b0, int &b1, double &r0, double &r1){
	double t = inputVector + N;
	b0 = ((int)t) & (mSampleSize - 1); //faster modulo operation
	b1 = (b0 + 1) & (mSampleSize - 1);
	r0 = t - ((int)t);
	r1 = r0 - 1;

}

double PerlinNoise::calculateNoiseValue(double x, double y){

	glm::dvec2 inputVector(x, y);
	int b00, b10, b01, b11;
	//double a, b, u, v;
	int i, j;
	glm::ivec2 b0, b1;
	glm::dvec2 t, r0, r1, s, q, u, v;
	for(int i = 0; i < inputVector.length(); i++)
	{
		setGridPoints(inputVector[i], b0[i], b1[i], r0[i], r1[i]);
	}
	i = mPermutationTable[b0.x];
	j = mPermutationTable[b1.x];

	b00 = mPermutationTable[i + b0.y];
	b10 = mPermutationTable[j + b0.y];
	b01 = mPermutationTable[i + b1.y];
	b11 = mPermutationTable[j + b1.y];

	s = interpolationPolynomial(r0);

	q = glm::dvec2(mGradientTable2d.at(b00));
	u[0] = glm::dot(q, glm::dvec2(r0.x, r0.y));
	q = glm::dvec2(mGradientTable2d.at(b10));
	v[0] = glm::dot(q, glm::dvec2(r1.x, r0.y));
	//a = lerp(s.x, u, v);


	q = glm::dvec2(mGradientTable2d.at(b01));
	u[1] = glm::dot(q, glm::dvec2(r0.x, r1.y));
	q = glm::dvec2(mGradientTable2d.at(b11));
	v[1] = glm::dot(q, glm::dvec2(r1.x, r1.y));
	//b = lerp(s.x, u, v);

	return clamp(lerp(s.y, lerp(s.x, u[0], v[0]), lerp(s.x, u[1], v[1])), 0.0, 1.0);
//	return lerp(s.y, a, b);
}

double PerlinNoise::calculateNoiseValue(double x, double y, double z){

	glm::dvec3 inputVector(x, y, z);
	int b00, b10, b01, b11,  b000, b100, b010, b110, b001, b101, b011, b111;
	double a, b, c, d, e, f;
	int i, j;

	glm::ivec3 b0, b1, b2;
	glm::dvec3 t, r0, r1, r2, s, q;
	std::vector<double> u(4);
	std::vector<double> v(4);
	for(int i = 0; i < inputVector.length(); i++)
	{
		setGridPoints(inputVector[i], b0[i], b1[i], r0[i], r1[i]);
	}
	i = mPermutationTable[b0.x];
	j = mPermutationTable[b1.x];

	b00 = mPermutationTable[i + b0.y];
	b10 = mPermutationTable[j + b0.y];
	b01 = mPermutationTable[i + b1.y];
	b11 = mPermutationTable[j + b1.y];

	b000 = mPermutationTable[b00 + b0.z];
	b100 = mPermutationTable[b10 + b0.z];
	b010 = mPermutationTable[b01 + b0.z];
	b110 = mPermutationTable[b11 + b0.z];
	b001 = mPermutationTable[b00 + b1.z];
	b101 = mPermutationTable[b10 + b1.z];
	b011 = mPermutationTable[b01 + b1.z];
	b111 = mPermutationTable[b11 + b1.z];

	s = interpolationPolynomial(r0);

	q = glm::dvec3(mGradientTable3d.at(b000));
	u[0] = glm::dot(q, glm::dvec3(r0.x, r0.y, r0.z));
	q = glm::dvec3(mGradientTable3d.at(b100));
	v[0] = glm::dot(q, glm::dvec3(r1.x, r0.y, r0.z));



	q = glm::dvec3(mGradientTable3d.at(b010));
	u[1] = glm::dot(q, glm::dvec3(r0.x, r1.y, r0.z));
	q = glm::dvec3(mGradientTable3d.at(b110));
	v[1] = glm::dot(q, glm::dvec3(r1.x, r1.y, r0.z));

	q = glm::dvec3(mGradientTable3d.at(b001));
	u[2] = glm::dot(q, glm::dvec3(r0.x, r0.y, r1.z));
	q = glm::dvec3(mGradientTable3d.at(b101));
	v[2] = glm::dot(q, glm::dvec3(r1.y, r0.y, r1.z));

	q = glm::dvec3(mGradientTable3d.at(b011));
	u[3] = glm::dot(q, glm::dvec3(r0.x, r1.y, r1.z));
	q = glm::dvec3(mGradientTable3d.at(b111));
	v[3] = glm::dot(q, glm::dvec3(r1.y, r1.y, r1.z));

	a = lerp(s.x, u[0], v[0]);
	b = lerp(s.x, u[1], v[1]);
	c = lerp(s.x, u[2], v[2]);
	d = lerp(s.x, u[3], v[3]);
	e = lerp(s.y, a, b);
	f = lerp(s.y, c, d);

	return lerp(s.z, e, f);
//	return lerp(s.y, a, b);
}
glm::dvec4 PerlinNoise::interpolationPolynomial(glm::dvec4 vec){
	double x = interpolationPolynomial(vec.x);
	double y = interpolationPolynomial(vec.y);
	double z = interpolationPolynomial(vec.z);
	double w = interpolationPolynomial(vec.w);
	return glm::dvec4(x, y, z, w);
}

glm::dvec3 PerlinNoise::interpolationPolynomial(glm::dvec3 vec){
	double x = interpolationPolynomial(vec.x);
	double y = interpolationPolynomial(vec.y);
	double z = interpolationPolynomial(vec.z);
	return glm::dvec3(x, y, z);
}

glm::dvec2 PerlinNoise::interpolationPolynomial(glm::dvec2 vec){
	double x = interpolationPolynomial(vec.x);
	double y = interpolationPolynomial(vec.y);
	return glm::dvec2(x, y);
}

double PerlinNoise::interpolationPolynomial(double t){
	return t * t *(3 - 2 * t);
	//return t * t * t * (t * (t * 6 - 15) + 10); //simplex
}

void PerlinNoise::initPermutationTable(){
	std::default_random_engine generator(mSeed);
	mPermutationTable = std::vector<int>(mSampleSize);
	iota(mPermutationTable.begin(), mPermutationTable.end(), 0);
	std::shuffle(mPermutationTable.begin(), mPermutationTable.end(), generator);
	mPermutationTable.insert(std::end(mPermutationTable), std::begin(mPermutationTable), std::end(mPermutationTable));

}

void PerlinNoise::init2DGradientTable(){
	std::default_random_engine generator(mSeed);
	std::uniform_real_distribution<double> distribution (-1.0, 1.0);
	mGradientTable2d = std::vector<glm::dvec2>(mSampleSize);
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

void PerlinNoise::init3DGradientTable(){
	std::default_random_engine generator(mSeed);
	std::uniform_real_distribution<double> distribution (-1.0, 1.0);
	mGradientTable3d = std::vector<glm::dvec3>(mSampleSize);
	for(int i = 0; i < mSampleSize; i++){
	//mGradientTable2333d.at(i) = glm::dvec2();
		for (int j = 0; j < 3; j++)
			mGradientTable3d.at(i)[j] = distribution(generator);
		mGradientTable3d.at(i) = glm::normalize(mGradientTable3d.at(i));
		//std::cout << mGradientTable2d.at(i).x << ", b0  " <<  mGradientTable2d.at(i).y << std::endl;
	}
	//std::cin.get();

	mGradientTable3d.insert(std::end(mGradientTable3d), std::begin(mGradientTable3d), std::end(mGradientTable3d));
}

void PerlinNoise::init4DGradientTable(){
	std::default_random_engine generator(mSeed);
	std::uniform_real_distribution<double> distribution (-1.0, 1.0);
	mGradientTable4d = std::vector<glm::dvec4>(mSampleSize);
	for(int i = 0; i < mSampleSize; i++){
	//mGradientTable2333d.at(i) = glm::dvec2();
		for (int j = 0; j < 3; j++)
			mGradientTable4d.at(i)[j] = distribution(generator);
		mGradientTable4d.at(i) = glm::normalize(mGradientTable4d.at(i));
		//std::cout << mGradientTable2d.at(i).x << ", b0  " <<  mGradientTable2d.at(i).y << std::endl;
	}
	//std::cin.get();

	mGradientTable4d.insert(std::end(mGradientTable4d), std::begin(mGradientTable4d), std::end(mGradientTable4d));
}
double PerlinNoise::lerp(double alpha, double x0, double x1){
	return x0 + alpha * (x1 - x0);
}



