// algorithms.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include "Command.hpp"

int main(int argc, char** argv) {
	CommandDispatcher poissonProcessCommand(
		"poisson-process",
		2,
		"SubCommand",
		{
			&PoissonProcessPMFSubCommand(),
			&PoissonProcessCDFSubCommand(),
			&PoissonProcessSampleArrivalTimesSubCommand(),
			&PoissonProcessSampleNumberArrivalsSubCommand()
		});
	CommandDispatcher matrixCommand(

		"matrix",
		2,
		"SubCommand",
		{
			&MatrixTestSubCommand()
		});
	CommandDispatcher application(
		"algorithms-cli",
		1,
		"Command",
		{
			&poissonProcessCommand,
			&matrixCommand
		});

	//Application application;
	
	application.run(argc, argv);

    return 0;
}
