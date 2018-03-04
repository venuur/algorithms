// algorithms.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
namespace fs = std::experimental::filesystem::v1;

void printUsage(int argc, char** argv);
double evalPoissonProcessIntervalPMF(double arrivalRate, double intervalDuration, unsigned long numberArrivals);
vector<double> samplePoissonProcessArrivalTimes(double arrivalRate, unsigned long numberArrivals, unsigned long seed);
unsigned long samplePoissonProcessNumberArrivals(double arrivalRate, double intervalDuration, unsigned long seed);
double sampleExponential(double arrivalRate, unsigned long seed);
unsigned long samplePoisson(double mean, unsigned long seed);

template<class elementType>
class Matrix {
public:
	Matrix(unsigned long dataRows, unsigned long dataCols, const vector<elementType>& data) :
		dim(dataRows, dataCols),
		elements(data) {}

	elementType operator()(unsigned long row, unsigned long col) const {
		return elements[dim.first*row + col];
	}

	static Matrix<elementType> load(const string& filename);

	unsigned long nRows() {
		return dim.first;
	}
	
	unsigned long nCols() {
		return dim.second;
	}

private:
	vector<elementType> elements;
	pair<unsigned long, unsigned long> dim;
};

template<class elementType>
Matrix<elementType> Matrix<elementType>::load(const string& filename) {
	unsigned long nRows, nCols;
	ifstream matrixFile(filename);

	matrixFile >> nRows >> nCols;

	vector<elementType> data(nRows*nCols);

	for (unsigned long i = 0; i < data.size(); i++) {
		matrixFile >> data[i];
	}

	return Matrix<elementType>(nRows, nCols, data);
}

template<class elementType>
ostream& operator<<(ostream& os, Matrix<elementType> matrix) {
	for (unsigned long row = 0; row < matrix.nRows(); ++row) {
		os << "[";
		for (unsigned long col = 0; col < matrix.nCols(); ++col) {
			os << matrix(row, col);
			os << (col == matrix.nCols() - 1 ? "]" : ", ");
		}
		os << "\n";
	}

	return os;
}


class Command {
public:
	virtual void run(int argc, char** argv) = 0;
	string name() {
		return m_name;
	}

protected:
	string m_name;
};



class PoissonProcessPMFSubCommand : public Command {
public:
	PoissonProcessPMFSubCommand() {
		m_name = "pmf";
	}

	virtual void run(int argc, char** argv) {
		LOG_DEBUG(
			cout << "DEBUG: Running PoissonProcessPMFSubCommand" << endl;
		);

		if (argc != 6) {
			cout << "ERROR: Wrong number of arguments." << endl;
			printUsage(argc, argv);
			return;
		}

		stringstream argStream;
		double rate, duration;
		unsigned long number;
		argStream << argv[3] << " " << argv[4] << " " << argv[5];
		argStream >> rate >> duration >> number;

		double poissonProbability = evalPoissonProcessIntervalPMF(rate, duration, number);
		cout << poissonProbability << endl;
	}
};

class PoissonProcessCDFSubCommand : public Command {
public:
	PoissonProcessCDFSubCommand() {
		m_name = "cdf";
	}

	virtual void run(int argc, char** argv) {
		LOG_DEBUG(
			cout << "DEBUG: Running PoissonProcessCDFSubCommand" << endl;
		);

		if (argc != 6) {
			cout << "ERROR: Wrong number of arguments." << endl;
			printUsage(argc, argv);
			return;
		}

		stringstream argStream;
		double rate, duration;
		unsigned long number;
		argStream << argv[3] << " " << argv[4] << " " << argv[5];
		argStream >> rate >> duration >> number;

		double poissonProbability = 0;
		for (unsigned long i = 0; i <= number; i++) {
			poissonProbability += evalPoissonProcessIntervalPMF(rate, duration, i);
		}
		cout << poissonProbability << endl;
	}
};

class PoissonProcessSampleArrivalTimesSubCommand : public Command {
public:
	PoissonProcessSampleArrivalTimesSubCommand() {
		m_name = "sample-arrival-times";
	}

	virtual void run(int argc, char** argv) {
		LOG_DEBUG(
			cout << "DEBUG: Running PoissonProcessSampleArrivalTimesSubCommand" << endl;
		);

		if (argc != 6) {
			cout << "ERROR: Wrong number of arguments." << endl;
			printUsage(argc, argv);
			return;
		}

		stringstream argStream;
		double rate;
		unsigned long number, seed;
		argStream << argv[3] << " " << argv[4] << " " << argv[5];
		argStream >> rate >> number >> seed;
		LOG_DEBUG(
			cout << "DEBUG: argStream " << argStream.str() << endl;
		);

		vector<double> arrivalTimes = samplePoissonProcessArrivalTimes(rate, number, seed);

		for (unsigned long i = 0; i < number; i++) {
			cout << arrivalTimes[i] << '\n';
		}
	}
};

class PoissonProcessSampleNumberArrivalsSubCommand : public Command {
public:
	PoissonProcessSampleNumberArrivalsSubCommand() {
		m_name = "sample-number-arrivals";
	}

	virtual void run(int argc, char** argv) {
		LOG_DEBUG(
			cout << "DEBUG: Running PoissonProcessSampleNumberArrivalsSubCommand" << endl;
		);

		if (argc != 6) {
			cout << "ERROR: Wrong number of arguments." << endl;
			printUsage(argc, argv);
			return;
		}

		stringstream argStream;
		double rate, duration;
		unsigned long seed;
		argStream << argv[3] << " " << argv[4] << " " << argv[5];
		argStream >> rate >> duration >> seed;
		LOG_DEBUG(
			cout << "DEBUG: argStream " << argStream.str() << endl;
		);

		unsigned long numberArrivals = samplePoissonProcessNumberArrivals(rate, duration, seed);

		cout << numberArrivals << endl;
	}
};


class PoissonProcessCommand : public Command {
public:
	PoissonProcessCommand() {
		m_name = "poisson-process";
		subCommands[pmfSubCommand.name()] = &pmfSubCommand;
		subCommands[cdfSubCommand.name()] = &cdfSubCommand;
		subCommands[sampleArrivalTimesSubCommand.name()] = &sampleArrivalTimesSubCommand;
		subCommands[sampleNumberArrivalsSubCommand.name()] = &sampleNumberArrivalsSubCommand;
	}

	virtual void run(int argc, char** argv) {
		if (argc < 3) {
			cout << "ERROR: Subcommand required." << endl;
			printUsage(argc, argv);
			return;
		}
		
		string subCommandArg(argv[2]);
		LOG_DEBUG(
			cout << "DEBUG: subCommandArg " << subCommandArg << endl;
		);

		auto subCommand = subCommands.find(subCommandArg);
		if (subCommand == subCommands.end()) {
			printUsage(argc, argv);
			return;
		}

		subCommand->second->run(argc, argv);
	}

private:
	unordered_map<string, Command*> subCommands;
	PoissonProcessPMFSubCommand pmfSubCommand;
	PoissonProcessCDFSubCommand cdfSubCommand;
	PoissonProcessSampleArrivalTimesSubCommand sampleArrivalTimesSubCommand;
	PoissonProcessSampleNumberArrivalsSubCommand sampleNumberArrivalsSubCommand;
};


class MatrixTestSubCommand : public Command {
public:
	MatrixTestSubCommand() {
		m_name = "test";
	}

	virtual void run(int argc, char** argv) {
		LOG_DEBUG(
			cout << "DEBUG: Running MatrixTestSubCommand" << endl;
		);

		if (argc != 4) {
			cout << "ERROR: Wrong number of arguments." << endl;
			printUsage(argc, argv);
			return;
		}

		stringstream argStream;
		string matrixFilename;
		argStream << argv[3];
		argStream >> matrixFilename;
		LOG_DEBUG(
			cout << "DEBUG: argStream " << argStream.str() << endl;
		);

		Matrix<double> matrix = Matrix<double>::load(matrixFilename);

		cout << matrix;
	}
};


class MatrixCommand : public Command {
public:
	MatrixCommand() {
		m_name = "matrix";
		subCommands[testSubCommand.name()] = &testSubCommand;
	}

	virtual void run(int argc, char** argv) {
		if (argc < 3) {
			cout << "ERROR: Subcommand required." << endl;
			printUsage(argc, argv);
			return;
		}
		
		string subCommandArg(argv[2]);
		LOG_DEBUG(
			cout << "DEBUG: subCommandArg " << subCommandArg << endl;
		);

		auto subCommand = subCommands.find(subCommandArg);
		if (subCommand == subCommands.end()) {
			printUsage(argc, argv);
			return;
		}

		subCommand->second->run(argc, argv);
	}

private:
	unordered_map<string, Command*> subCommands;
	MatrixTestSubCommand testSubCommand;
};


class Application : public Command {
public:
	Application() {
		m_name = "algorithms-cli";
		subCommands[poissonProcessCommand.name()] = &poissonProcessCommand;
		subCommands[matrixCommand.name()] = &matrixCommand;
	}

	virtual void run(int argc, char** argv) {
		string subCommandArg(argv[1]);
		LOG_DEBUG(
			cout << "DEBUG: subCommandArg " << subCommandArg << endl;
		);

		auto subCommand = subCommands.find(subCommandArg);
		if (subCommand == subCommands.end()) {
			printUsage(argc, argv);
			return;
		}

		subCommand->second->run(argc, argv);
	}

private:
	unordered_map<string, Command*> subCommands;
	PoissonProcessCommand poissonProcessCommand;
	MatrixCommand matrixCommand;
};


//class CommandDispatcher : public Command {
//	CommandDispatcher(int argc, char**) {}
//};



int main(int argc, char** argv) {
	Application application;
	
	application.run(argc, argv);

    return 0;
}


void printUsage(int argc, char** argv) {
	string programNameArg(argv[0]);
	fs::path programPath(programNameArg);
	string programFilename(programPath.filename().string());

	cout
		<< "Usage 1: " << programFilename << " poisson-process {pmf|cdf|sample-arrival-times} args\n\n"
		<< "pmf|cdf\n"
		<< "Choose whether to evaluate pmf or cdf.\n"
		<< "  rate      Rate of arrivals in Poisson process.\n"
		<< "  duration  Duration of interval over which to calculate probability.\n"
		<< "  number    Number of arrivals to calculate probability\n"
		///////////////////////////////////////////////////////////////////////////////
		<< "\n"
		"Calculates the probability P(N(t+s) - N(t) {=|<=} k) = exp(-L*s) * (L*s)**k / k!\n"
		"for a Poisson process N with rate of arrivals L equal to the argument \"rate\",\n"
		"over an interval of length s equal to the argument \"duration\", for a number\n"
		"of arrivals k equal to the argument \"number\". The sign \"=\" or \"<=\" is chosen\n"
		"by selecting pmf or cdf respectively.\n"
		<< "\n"
		<< "sample-arrival-times\n"
		<< "Choose to sample a sequence of arrival time variates.\n"
		<< "  rate    Rate of arrivals in Poisson process.\n"
		<< "  number  Number of arrival times to sample.\n"
		<< "  seed    Seed for random number generator.\n"
		<< "\n"
		<< "sample-number-arrivals\n"
		<< "Choose to sample the number of arrivals in and interval.\n"
		<< "  rate      Rate of arrivals in Poisson process.\n"
		<< "  duration  Duration of interval in which to count arrivals.\n"
		<< "  seed      Seed for random number generator.\n"
		<< "\n"
		////////////////////////////////////////////////////////////////////////////////
		<< "Usage 2: " << programFilename << " matrix test args\n\n"
		<< "test\n"
		<< "Choose whether to test matrix functions.\n"
		<< "  test-matrix  Filename of matrix to use for tests.\n"
		///////////////////////////////////////////////////////////////////////////////
		<< "\n"
		"Calculates various matrix operations.\n"
		<< endl;
}


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

