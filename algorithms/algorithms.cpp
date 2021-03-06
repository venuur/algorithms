// algorithms.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include "Command.hpp"

#include <memory>
using std::make_shared;

int main(int argc, char** argv) {
	PoissonProcessCDFSubCommand pp_cdf_sc;
	PoissonProcessPMFSubCommand pp_pmf_sc;
	PoissonProcessSampleArrivalTimesSubCommand pp_sat_sc;
	PoissonProcessSampleNumberArrivalsSubCommand pp_sna_sc;
	CommandDispatcher pp_c("poisson-process", 2, "SubCommand",
		{ &pp_cdf_sc, &pp_pmf_sc, &pp_sat_sc, &pp_sna_sc });

	MatrixTestSubCommand m_t_sc;
	CommandDispatcher m_c("matrix", 2, "SubCommand", 
		{ &m_t_sc });

	GridWorldTestSubCommand gw_t_sc;
	CommandDispatcher gw_c("grid-world", 2, "SubCommand",
		{ &gw_t_sc });

	CommandDispatcher application("algorithms-cli", 1, "Command",
		{ &pp_c, &m_c, &gw_c });

	//CommandDispatcher poissonProcessCommand(
	//	"poisson-process",
	//	2,
	//	"SubCommand",
	//	{
	//		&PoissonProcessPMFSubCommand(),
	//		&PoissonProcessCDFSubCommand(),
	//		&PoissonProcessSampleArrivalTimesSubCommand(),
	//		&PoissonProcessSampleNumberArrivalsSubCommand()
	//	});
	//CommandDispatcher matrixCommand(

	//	"matrix",
	//	2,
	//	"SubCommand",
	//	{
	//		&MatrixTestSubCommand()
	//	});

	//CommandDispatcher application(
	//	"algorithms-cli",
	//	1,
	//	"Command",
	//	{
	//		&poissonProcessCommand,
	//		&matrixCommand
	//	});

	////Application application;
	//
	application.run(argc, argv);

    return 0;
}
