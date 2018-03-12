#include "Logging.hpp"
#include "PrettyPrint.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

// <filesystem> was not in the standard at the time of relase for Visual Studio 2017 .
#include <experimental/filesystem> // C++-standard header file name  
#include <filesystem> // Microsoft-specific implementation header file name  

namespace fs = std::experimental::filesystem::v1;
using namespace std;

class  ColumnarText {
public:
	ColumnarText(const vector<vector<string>>& columnEntries, const string&, int inIdent);

	void print(ostream&) const;

private:
	int indent;
	string columnSeparator;
	vector<vector<string>> textMatrix;
	vector<unsigned int> columnWidths;
};

ColumnarText::ColumnarText(
	const vector<vector<string>>& columnEntries, 
	const string& inColumnSeparator="  ", 
	int inIdent=2) :
		indent{inIdent},
		columnSeparator{inColumnSeparator}
	{
	//TODO: Handle empty inputs.

	//Require same number of columns each row.
	auto nColumns = columnEntries[0].size();
	columnWidths = vector<unsigned int>(nColumns, 0);

	for (const auto& row : columnEntries) {
		LOG_DEBUG(
			cout << "DEBUG row of columnEntries\n" << row;
		);
		auto columnWidth = columnWidths.begin();
		auto entries = vector<string>(nColumns);
		auto entriesIt = entries.begin();
		for (const auto& entry : row) {
			//TODO: Strip extra white space and handle new lines gracefully.
			*columnWidth = max(*columnWidth, static_cast<unsigned int>(entry.length()));
			*entriesIt = entry;
			++columnWidth;
			++entriesIt;
		}
		textMatrix.push_back(entries);
	}
}


void ColumnarText::print(ostream& os) const {
	auto initialIndent = string(indent, ' ');
	for (const auto& row : textMatrix) {
		os << initialIndent;
		auto columnWidth = columnWidths.begin();
		for (auto& entry = begin(row); entry != end(row); ++entry) {
			os << setw(*columnWidth) << left
				<< *entry
				<< ((entry == end(row) - 1) ? "\n" : columnSeparator);
			++columnWidth;
		}
	}
}


ostream& operator<<(ostream& os, const ColumnarText& text) {
	text.print(os);
	return os;
}


void printUsage(int argc, char** argv) {
	string programNameArg(argv[0]);
	fs::path programPath(programNameArg);
	auto programFilename = programPath.filename().string();

	auto pdfCdfParameterText = ColumnarText({
			{ "rate", "Rate of arrivals in Poisson process." },
			{ "duration", "Duration of interval over while to calculate probability." },
			{ "number", "Number of arrivals to calculate probability" }
		});
	auto sampleArrivalTimesParameterText = ColumnarText({
			{ "rate", "Rate of arrivals in Poisson process."},
			{ "number", "Number of arrival times to sample."},
			{ "seed", "Seed for random number generator."}
		});
	auto sampleNumberArrivalsParameterText = ColumnarText({
			{"rate", "Rate of arrivals in Poisson process."},
			{"duration", "Duration of interval in which to count arrivals."},
			{"seed", "Seed for random number generator."}
		});
	auto matrixTestParameterTextMatrix = vector<vector<string>>();
	matrixTestParameterTextMatrix.push_back(vector<string>{ "test-matrix", "Filename of matrix to use for tests." });
	auto matrixTestParameterText = ColumnarText(matrixTestParameterTextMatrix);

	cout
		<< "Usage 1: " << programFilename << " poisson-process {pmf|cdf|sample-arrival-times} args\n\n"
		<< "pmf|cdf\n"
		<< "Choose whether to evaluate pmf or cdf.\n"
		<< pdfCdfParameterText
		///////////////////////////////////////////////////////////////////////////////
		<< endl
		<< "Calculates the probability P(N(t+s) - N(t) {=|<=} k) = exp(-L*s) * (L*s)**k / k!\n"
		"for a Poisson process N with rate of arrivals L equal to the argument \"rate\",\n"
		"over an interval of length s equal to the argument \"duration\", for a number\n"
		"of arrivals k equal to the argument \"number\". The sign \"=\" or \"<=\" is chosen\n"
		"by selecting pmf or cdf respectively.\n"
		<< endl
		<< "sample-arrival-times\n"
		<< "Choose to sample a sequence of arrival time variates.\n"
		<< sampleArrivalTimesParameterText
		<< endl
		<< "sample-number-arrivals\n"
		<< "Choose to sample the number of arrivals in and interval.\n"
		<< sampleNumberArrivalsParameterText
		<< endl
		////////////////////////////////////////////////////////////////////////////////
		<< "Usage 2: " << programFilename << " matrix test args\n\n"
		<< "test\n"
		<< "Choose whether to test matrix functions.\n"
		<< matrixTestParameterText
		///////////////////////////////////////////////////////////////////////////////
		<< "\n"
		"Calculates various matrix operations.\n"
		<< endl;
}
