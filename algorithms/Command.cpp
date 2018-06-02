#include "Command.hpp"
#include "Logging.hpp"
#include "Usage.hpp"
#include "PoissonProcess.hpp"
#include "Matrix.hpp"
#include "PrettyPrint.hpp"
#include "Agent.hpp"

#include <string>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>


using namespace std;

ostream& operator<<(ostream& os, const Command& command) {
	cout << "Command(" << command.name() << ")";
	return os;
}


string Command::name() const {
	return m_name;
}


PoissonProcessPMFSubCommand::PoissonProcessPMFSubCommand() {
	m_name = "pmf";
}


void PoissonProcessPMFSubCommand::run(int argc, char** argv) {
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

PoissonProcessCDFSubCommand::PoissonProcessCDFSubCommand() {
	m_name = "cdf";
}

void PoissonProcessCDFSubCommand::run(int argc, char** argv) {
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



PoissonProcessSampleArrivalTimesSubCommand::PoissonProcessSampleArrivalTimesSubCommand() {
	m_name = "sample-arrival-times";
}

void PoissonProcessSampleArrivalTimesSubCommand::run(int argc, char** argv) {
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


PoissonProcessSampleNumberArrivalsSubCommand::PoissonProcessSampleNumberArrivalsSubCommand() {
	m_name = "sample-number-arrivals";
}


void PoissonProcessSampleNumberArrivalsSubCommand::run(int argc, char** argv) {
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


MatrixTestSubCommand::MatrixTestSubCommand() {
	m_name = "test";
}

void MatrixTestSubCommand::run(int argc, char** argv) {
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



CommandDispatcher::CommandDispatcher(
	string dispatchName,
	int dispatchLevel,
	string dispatchLevelName,
	vector<Command*> dispatchCommands) :
	level(dispatchLevel),
	levelName(dispatchLevelName) {
	m_name = dispatchName;
	for (auto command : dispatchCommands) {
		LOG_DEBUG(
			cout << "DEBUG: Adding command " << *command << " to " << this->name() << "." << endl;
		);
		commands[command->name()] = command;
	}
}

void CommandDispatcher::run(int argc, char** argv) {
	if (argc <= level) {
		cout << "ERROR: " << levelName << " required." << endl;
		printUsage(argc, argv);
		return;
	}

	string commandArg(argv[level]);
	LOG_DEBUG(
		cout << "DEBUG: commandArg " << commandArg << endl;
	);

	auto commandIt = commands.find(commandArg);
	if (commandIt == commands.end()) {
		cout << "ERROR: " << levelName << ": " << commandArg << " not found." << endl;
		LOG_DEBUG(
			cout << commands;
		);
		printUsage(argc, argv);
		return;
	}

	commandIt->second->run(argc, argv);
}

void CommandDispatcher::add_subcommand(Command* command)
{
	LOG_DEBUG(
		cout << "CommandDispatcher::add_subcommand DEBUG: Adding command " << *command << " to " << this->name() << "." << endl;
	);
	commands[command->name()] = command;
}

GridWorldTestSubCommand::GridWorldTestSubCommand()
{
	m_name = "test";
}

void grid_world_test_line_walkers();
void grid_world_test_space_invaders();

void GridWorldTestSubCommand::run(int argc, char ** argv)
{
	LOG_DEBUG(
		cout << "DEBUG: Running GridWorldTestSubCommand" << endl;
	);

	if (argc != 3) {
		cout << "ERROR: Wrong number of arguments." << endl;
		printUsage(argc, argv);
		return;
	}

	//stringstream argStream;
	//string matrixFilename;
	//argStream << argv[3];
	//argStream >> matrixFilename;
	//LOG_DEBUG(
	//	cout << "DEBUG: argStream " << argStream.str() << endl;
	//);

	grid_world_test_line_walkers();
	grid_world_test_space_invaders();
	
}

void grid_world_test_line_walkers() {
	cout << "---- Testing line walkers ----\n\n";

	GridWorld::Move directions[] = {
		GridWorld::Move::up,
		GridWorld::Move::down,
		GridWorld::Move::right,
		GridWorld::Move::left
	};

	vector<GW_SR_Agent> walkers;
	for (GridWorld::Move direction : directions) {
		walkers.push_back(
			GW_SR_Agent(
				new GridWorld::LocalView,
				new GridWorld::WalkLine(direction)));
	}

	for (GW_SR_Agent &agent : walkers) {
		GridWorld env(agent, GridWorld::Coordinate(2, 2), GridWorld::Coordinate(4, 4));
		cout << "Iteration " << 0 << "\n";
		cout << env;
		for (int i = 0; i < 5; ++i) {
			env.run();
			cout << "Iteration " << i << "\n";
			cout << env;
		}
	}
}

void grid_world_test_space_invaders() {
	cout << "---- Testing space invaders ----\n\n"; 

	struct alien_args {
		bool even_right;
		int  left_edge;
		int  right_edge;
	};
	alien_args args[] = {
	{true, 1, 3},
	{false, 1, 3}
	};
	GridWorld::Coordinate starts[] = {
	{2, 2},
	{0, 0},
	{4, 0}
	};

	//TODO finish creating loops for space invaders.
	vector<GW_SR_Agent> walkers;
	for (alien_args arg : args) {
		walkers.push_back(
			GW_SR_Agent(
				new GridWorld::LocalView,
				new GridWorld::SpaceInvader(
					arg.even_right,
					arg.left_edge,
					arg.right_edge)));
	}

	for (GW_SR_Agent &agent : walkers) {
		for (GridWorld::Coordinate &start : starts) {
			GridWorld env(agent, start, GridWorld::Coordinate(4, 4));
			cout << "Iteration " << 0 << "\n";
			cout << env;
			for (int i = 0; i < 10; ++i) {
				env.run();
				cout << "Iteration " << i << "\n";
				cout << env;
			}
		}
	}
}
