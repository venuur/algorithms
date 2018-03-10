#include <string>
#include <iostream>

// <filesystem> was not in the standard at the time of relase for Visual Studio 2017 .
#include <experimental/filesystem> // C++-standard header file name  
#include <filesystem> // Microsoft-specific implementation header file name  

namespace fs = std::experimental::filesystem::v1;
using std::string;
using std::cout;
using std::endl;

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
