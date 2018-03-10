#pragma once

#include <vector>

using std::vector;

double evalPoissonProcessIntervalPMF(double arrivalRate, double intervalDuration, unsigned long numberArrivals);
vector<double> samplePoissonProcessArrivalTimes(double arrivalRate, unsigned long numberArrivals, unsigned long seed);
unsigned long samplePoissonProcessNumberArrivals(double arrivalRate, double intervalDuration, unsigned long seed);
double sampleExponential(double arrivalRate, unsigned long seed);
unsigned long samplePoisson(double mean, unsigned long seed);
