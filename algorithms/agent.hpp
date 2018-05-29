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
using std::cerr;

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

		virtual Move match(const Coordinate &not_used) {
			return direction;
		}

	private:
		Move direction;
	};
	class SpaceInvader : public Rules<Move, Coordinate> {
	public:
		SpaceInvader(bool even_right_, int left_edge_, int right_edge_) :
			even_right(even_right_), left_edge(left_edge_), right_edge(right_edge_) {
			// Check that the bounds (left_edge, right_edge) are valid.
			if (left_edge > right_edge) {
				cerr << "Warning: left_edge of space invader boundary must be to the left of the right_edge.\n"
					<< "         Replacing right_edge with left_edge + 1." << std::endl;
			}
		}

		virtual Move match(const Coordinate &loc) {
			// If out of bounds, move back to bounds.
			if (loc.first < left_edge) {
				return Move::right;
			}
			else if (loc.first > right_edge) {
				return Move::left;
			}

			// If at the edge, the even_right==true means we move right when on an even row and on the left edge,
			// and we move left when on an odd row and on the right edge. See the following grid as an example:
			//  > = Move::right, < = Move::left, V = Move::down
			// even_right==true     even_right==false
			//   0 1 2 3               0 1 2 3
			// 0 > > > V             0 V < < <
			// 1 V < < <             1 > > > V
			// 2 > > > V             2 V < < <
			// 3 V < < <             3 > > > V
			//

			// From prior checks, loc.first \in [left_edge, right_edge].
			if (even_right) {
				if (loc.first == left_edge) {
					if (loc.second % 2 == 0) {
						return Move::right;
					}
					else {
						return Move::down;
					}
				}
				else if (loc.first == right_edge) {
					if (loc.second % 2 == 1) {
						return Move::left;
					}
					else {
						return Move::down;
					}
				}
				// loc.first \in (left_edge, right_edge).
				else {
					if (loc.second % 2 == 0) {
						return Move::right;
					}
					else {
						return Move::left;
					}
				}
			}
			// even_right == false
			else {
				if (loc.first == left_edge) {
					if (loc.second % 2 == 0) {
						return Move::down;
					}
					else {
						return Move::right;
					}
				}
				else if (loc.first == right_edge) {
					if (loc.second % 2 == 1) {
						return Move::down;
					}
					else {
						return Move::left;
					}
				}
				// loc.first \in (left_edge, right_edge).
				else {
					if (loc.second % 2 == 0) {
						return Move::left;
					}
					else {
						return Move::right;
					}
				}
			}

			// Some input must be malformed if we reached here, so print an error and move upward, as that move is 
			// unexpected of a space invader.
			cerr << "ERROR: All rules for space invader passed without reaching a valid move."
				<< "       Defaulting to moving up to signal error." << std::endl;
			return Move::up;
		}

	private:
		bool even_right;
		int left_edge;
		int right_edge;
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
