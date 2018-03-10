#include "PoissonProcess.hpp"

#include <iostream>
#include <vector>
#include <random>

#include "Logging.hpp"

using namespace std;

double evalPoissonProcessIntervalPMF(double arrivalRate, double intervalDuration, unsigned long numberArrivals) {
	// Compute Poisson PMF P(N(t+s)-N(t) = k) = exp(-L*s) * (L*s)**k / k! 
	// L := arrivalRate
	// s := intervalDuration
	// k := numberArrivals

	LOG_DEBUG(
		cout << "DEBUG:"
		<< " arrivalRate " << arrivalRate
		<< " intervalDuration " << intervalDuration
		<< " numberArrivals " << numberArrivals
		<< endl;
	)

		double poissonMean = arrivalRate * intervalDuration;
	double exponentialFactor = exp(-poissonMean);

	// Compute poissonMean**numberArrivals / numberArrivals! as a running product
	// as both numerator and denominator are possible huge.
	double runningProduct = 1;
	for (unsigned long i = 1; i <= numberArrivals; i++) {
		runningProduct *= poissonMean / i;
	}

	double probability = exponentialFactor * runningProduct;

	return probability;
}


vector<double> samplePoissonProcessArrivalTimes(double arrivalRate, unsigned long numberArrivals, unsigned long seed = 0) {
	// Sample n arrival times from Poisson process with arrival rate L. If seed is 0, sample new seed from random_device.
	// n := numberArrivals
	// L := arrivalRate

	static random_device seedGen;
	static unsigned long activeSeed = seed;
	if (seed == 0) {
		activeSeed = seedGen();
	}
	LOG_DEBUG(
		cout << "DEBUG: seed " << seed << " activeSeed " << activeSeed << endl;
	)

		vector<double> arrivalTimes(numberArrivals);
	double latestArrivalTime = 0;

	// Sample exponential inter-arrival times and accumulate to determine arrival times.
	for (unsigned long i = 0; i < numberArrivals; i++) {
		double interArrivalTime = sampleExponential(arrivalRate, activeSeed);
		latestArrivalTime += interArrivalTime;
		arrivalTimes[i] = latestArrivalTime;
	}

	return arrivalTimes;
}


unsigned long samplePoissonProcessNumberArrivals(double arrivalRate, double intervalDuration, unsigned long seed = 0) {
	// Return random variate of N(t+s) - N(t) from a Poisson process N with arrival rate L.
	// s := intervalDuration
	// L := arrivalRate

	static random_device seedGen;
	static unsigned long activeSeed = seed;
	if (seed == 0) {
		activeSeed = seedGen();
	}
	LOG_DEBUG(
		cout << "DEBUG: seed " << seed << " activeSeed " << activeSeed << endl;
	);

	double meanNumberArrivals = arrivalRate * intervalDuration;

	return samplePoisson(meanNumberArrivals, activeSeed);
}



double sampleExponential(double arrivalRate, unsigned long seed) {
	// Return exponential random variate with rate L.
	// L := arrivalRate
	// pdf: f(x) = L e**(-Lx)

	static unsigned long currentSeed = 0;
	static mt19937_64 gen(currentSeed);

	if (currentSeed != seed) {
		currentSeed = seed;
		gen.seed(currentSeed);
	}

	exponential_distribution<double> dist(arrivalRate);

	return dist(gen);
}


unsigned long samplePoisson(double mean, unsigned long seed) {
	// Return Poisson random variate with mean m.
	// m := mean
	// pmf: f(x) =  e**(-m) * m**x / x!

	static unsigned long currentSeed = 0;
	static mt19937_64 gen(currentSeed);

	if (currentSeed != seed) {
		currentSeed = seed;
		gen.seed(currentSeed);
	}

	poisson_distribution<unsigned long> dist(mean);

	return dist(gen);
}

