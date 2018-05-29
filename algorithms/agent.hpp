#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <algorithm>

using std::unique_ptr;
using std::pair;
using std::vector;
using std::min;
using std::max;
using std::ostream;

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
	virtual Action operator()(const Percept &percept) = 0;
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
	struct Percept {
		Coordinate loc;
		Coordinate max_coord;
	};
	enum class Move {up, down, left, right};
	class LocalView : public Sensor<Coordinate, Percept> {
	public:
		virtual Coordinate interpret_input(const Percept& percept) {
			return percept.loc;
		}
	};
	class WalkLine : public Rules<Move, Coordinate> {
	public:
		WalkLine(Move direction_) : direction(direction_) {}

		virtual Move match(const Coordinate& not_used) {
			return direction;
		}

	private:
		Move direction;
	};

	GridWorld(Agent<Move, Percept> &agent_, Coordinate start, Coordinate max_coord_) :
		max_coord(max_coord_),
		agent_loc(start), 
		agent(agent_) {
		history.push_back(agent_loc);
	}

	void run(int n_steps = 1) {
		for (int i = 0; i < n_steps; ++i) {
			Move move = agent(Percept{ agent_loc, max_coord });
			update(move);
			history.push_back(agent_loc);
		}
	}

	friend ostream& operator<<(ostream&, GridWorld&);

private:
	Coordinate max_coord;
	Coordinate agent_loc;
	History history;

	Agent<Move, Percept> &agent;


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

ostream& operator<<(ostream& os, GridWorld &env) {
	for (int row = env.max_coord.second; row >= 0; --row) {
		for (int col = 0; col <= env.max_coord.first; ++col) {
			if (row == env.agent_loc.second && col == env.agent_loc.first) {
				os << "A";
			}
			else {
				os << ".";
			}
		}
		os << "\n";
	}
	os << std::endl;

	return os;
}

typedef SimpleReflexAgent<GridWorld::Coordinate, GridWorld::Move, GridWorld::Percept> GW_SR_Agent;
