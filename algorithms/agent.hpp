#pragma once

#include <memory>
#include <utility>
#include <vector>
#include <algorithm>

using std::unique_ptr;
using std::pair;
using std::vector;
using std::min;
using std::max;

template<class State, class Percept>
class Sensor {
public:
	virtual State interpret_input(const Percept& percept) = 0;
};

template <class Action, class State>
class Rules {
public:
	virtual Action match(const State& state) = 0;
};


template <class Action, class Percept>
class Agent {
public:
	virtual Action operator(const Percept &percept) = 0;
};

template <class State, class Action, class Percept>
class SimpleReflexAgent : public Agent<Action, Percept> {
public:
	SimpleReflexAgent(Sensor<State, Percept> *sensor_, Rules<Action, State> *rules_) :
		sensor(sensor_), rules(rules_) {}

	virtual Action operator()(const Percept &percept) {
		const State &state = sensor->interpret_input(percept);
		return rules->match(state);
	}

private:
	unique_ptr<Sensor<State, Percept>> sensor;
	unique_ptr<Rules<Action, State>> rules;
};


class GridWorld {
public:

	typedef pair<int, int> Coordinate;
	typedef vector<Coordinate> History;
	enum class Move {up, down, left, right};

	GridWorld(Agent<Move, Coordinate> &agent_, Coordinate start, Coordinate max_coord_) :
		max_coord(max_coord_),
		agent_loc(start), 
		agent(agent_) {
		history.push_back(agent_loc);
	}

	void run(int n_steps = 1) {
		for (int i = 0; i < n_steps; ++i) {
			Move move = agent(agent_loc);
			update(move);
			history.push_back(agent_loc);
		}
	}

private:
	Coordinate max_coord;
	Coordinate agent_loc;
	History history;

	Agent<Move, Coordinate> &agent;

	void update(Move move) {
		switch (move) {
		case Move::up:
			agent_loc.second = min(max_coord.second, agent_loc.second + 1);
			break;
		case Move::down:
			agent_loc.second = max(0, agent_loc.second - 1);
			break;
		case Move::right:
			agent_loc.first = min(max_coord.first, agent_loc.first + 1);
			break;
		case Move::left:
			agent_loc.first = max(0, agent_loc.first - 1);
			break;
		}
	}
};
