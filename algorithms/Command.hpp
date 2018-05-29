#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

class Command {
public:
	virtual void run(int argc, char** argv) = 0;
	std::string name() const;

protected:
	std::string m_name;
};

std::ostream& operator<<(std::ostream& os, const Command& command);

class CommandDispatcher : public Command {
public:
	CommandDispatcher(
		std::string dispatchName, 
		int dispatchLevel, 
		std::string dispatchLevelName, 
		std::vector<Command*> dispatchCommands = {});

	virtual void run(int argc, char** argv);
	virtual void add_subcommand(Command* command);

private:
	int level;
	std::string levelName;
	std::unordered_map<std::string, Command*> commands;
};


class PoissonProcessPMFSubCommand : public Command {
public:
	PoissonProcessPMFSubCommand();

	virtual void run(int argc, char** argv);
};


class PoissonProcessCDFSubCommand : public Command {
public:
	PoissonProcessCDFSubCommand();

	virtual void run(int argc, char** argv);
};


class PoissonProcessSampleArrivalTimesSubCommand : public Command {
public:
	PoissonProcessSampleArrivalTimesSubCommand();

	virtual void run(int argc, char** argv);
};


class PoissonProcessSampleNumberArrivalsSubCommand : public Command {
public:
	PoissonProcessSampleNumberArrivalsSubCommand();

	virtual void run(int argc, char** argv);
};


class MatrixTestSubCommand : public Command {
public:
	MatrixTestSubCommand();

	virtual void run(int argc, char** argv);
};

class GridWorldTestSubCommand : public Command {
public:
	GridWorldTestSubCommand();
	virtual void run(int argc, char** argv);
};
