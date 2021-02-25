#include "lib.hpp"

struct Edge {
	int id;
	int u, v;  // begin and end nodes
	int L = 0; // length of the street, <=D
	int length() const { return L; }
};

struct Car {
	int P;           // number of edges, <=1000
	vector<int> ids; // street ids, starts at end of first street
	int L = 0;       // total length of the path
};

struct Light {
	int id; // street id
	int duration;
};

using Schedule = vector<Light>;

int D; // duration of the simulation, <=10000
int V; // number of nodes, <=100000
int E; // number of edges, <=100000
int C; // number of cars, <=1000
int F; // points for each car that reaches destination, <=1000
unordered_map<string, int> edge_names_rev;
unordered_map<pair<int, int>, int, pair_hasher> edgemap;
vector<string> edge_names;

vector<Edge> edges;
vector<Car> cars;
vector<vector<int>> in_edges, out_edges; // list of edges through each node
vector<vector<int>> in_cars, out_cars;   // list of cars through each node
vector<vector<int>> cars_through;        // list of cars through each edge

vector<int> cnt_out_edges(V, 0);
vector<int> cnt_in_edges(V, 0);
vector<int> cnt_out_cars(V, 0);
vector<int> cnt_in_cars(V, 0);
vector<int> cnt_cars_through(E, 0);
vector<int> cnt_starting_edge(V, 0);
vector<int> cnt_starting_node(V, 0);
vector<int> edge_L(E, 0);
vector<int> cars_L(C, 0);
vector<int> cars_P(C, 0);

vector<Schedule> schedules; // the solution

void read_and_stats(ifstream &in, ofstream &out) {
	in >> D >> V >> E >> C >> F;
	edges.resize(E);
	cars.resize(C);
	edge_names.resize(E);

	in_edges.resize(V);
	out_edges.resize(V);
	in_cars.resize(V);
	out_cars.resize(V);
	cars_through.resize(E);

	schedules.resize(V);

	cnt_out_edges.assign(V, 0);
	cnt_in_edges.assign(V, 0);
	cnt_out_cars.assign(V, 0);
	cnt_in_cars.assign(V, 0);
	cnt_cars_through.assign(E, 0);
	cnt_starting_edge.assign(E, 0);
	cnt_starting_node.assign(V, 0);
	edge_L.assign(E, 0);
	cars_L.assign(C, 0);
	cars_P.assign(C, 0);

	for (int e = 0; e < E; e++) {
		auto &[id, u, v, L] = edges[e];
		id = e, in >> u >> v >> edge_names[e] >> L;
		edge_names_rev[edge_names[e]] = e;
		edgemap[{u, v}] = e;

		out_edges[u].push_back(e);
		in_edges[v].push_back(e);
		cnt_out_edges[u]++;
		cnt_in_edges[v]++;
		edge_L[e] = L;
	}

	for (int c = 0; c < C; c++) {
		auto &[P, ids, L] = cars[c];
		in >> P, ids.resize(P);
		for (int j = 0; j < P; j++) {
			string name;
			in >> name;
			int id = edge_names_rev[name];
			ids[j] = id, L += j > 0 ? edges[id].L : 0;

			cars_through[id].push_back(c);
		}
		int e = ids[0];
		int start = edges[e].v;

		cnt_starting_edge[e]++;
		cnt_starting_node[start]++;
		cars_L[c] = L;
		cars_P[c] = P;
	}

	for (int e = 0; e < E; e++) {
		for (int car : cars_through[e]) {
			out_cars[edges[e].u].push_back(car);
			in_cars[edges[e].v].push_back(car);
		}
		cnt_cars_through[e] = cars_through[e].size();
	}

	for (int i = 0; i < V; i++) {
		cnt_out_cars[i] = out_cars[i].size();
		cnt_in_cars[i] = in_cars[i].size();
	}

	long perfect_score = 0;
	for (int i = 0; i < C; i++) {
		if (cars[i].L <= D) {
			perfect_score += (D - cars[i].L) + F;
		}
	}

	print(out, "{} nodes / {} edges\n", V, E);
	print(out, "{} cars\n", C);
	print(out, "{} bonus points\n", F);
	print(out, "{} duration\n", D);
	print(out, "Theoretical perfect score: {}\n", perfect_score);

	print(out, "Outgoing edges\n{}\n", histogram(cnt_out_edges));
	print(out, "Incoming edges\n{}\n", histogram(cnt_in_edges));
	print(out, "Outgoing cars\n{}\n", histogram(cnt_out_cars));
	print(out, "Incoming cars\n{}\n", histogram(cnt_in_cars));
	print(out, "Individual street lengths\n{}\n", histogram(edge_L));
	print(out, "Car path lengths\n{}\n", histogram(cars_L));
	print(out, "Car path street counts\n{}\n", histogram(cars_P));

	print(out, "Incoming cars VS incoming edges\n{}\n",
		  scatter(cnt_in_cars, cnt_in_edges));
}

void write(ofstream &out) {
	int A = 0;
	for (int u = 0; u < V; u++)
		A += !schedules[u].empty();

	print(out, "{}\n", A);
	for (int u = 0; u < V; u++) {
		if (!schedules[u].empty()) {
			print(out, "{}\n{}\n", u, schedules[u].size());
			for (auto [e, duration] : schedules[u]) {
				print(out, "{} {}\n", edge_names[e], duration);
			}
		}
	}
}

long simulate(bool show_details = false) {
	vector<uint> light(V, 0);    // index of the current light that is green
	vector<int> countdown(V, 0); // remaining green time
	vector<int> road(C, 0);      // index of the edge the car is in
	vector<int> dist(C, 0);      // remaining distance for the car to travel

	vector<queue<int>> waiting(E); // list of cars waiting
	vector<bool> done(C, false);
	vector<int> reach(C, -1);

	for (int c = 0; c < C; c++) {
		int e = cars[c].ids[0];
		waiting[e].push(c);
	}

	for (int t = 0; t < D; t++) {
		for (int u = 0; u < V; u++) {
			if (schedules[u].empty())
				continue;

			int e = schedules[u][light[u]].id;

			if (!waiting[e].empty()) {
				int c = waiting[e].front();
				waiting[e].pop();

				road[c]++;
				int f = cars[c].ids[road[c]];
				if (road[c] == cars[c].P - 1) {
					done[c] = true;
					reach[c] = t + edges[f].L;
				} else {
					dist[c] = edges[f].L;
				}
			}

			if (++countdown[u] == schedules[u][light[u]].duration) {
				if (++light[u] == schedules[u].size()) {
					light[u] = 0;
				}
				countdown[u] = 0;
			}
		}

		for (int c = 0; c < C; c++) {
			if (!done[c] && dist[c] > 0 && --dist[c] == 0) {
				int f = cars[c].ids[road[c]];
				waiting[f].push(c);
			}
		}
	}

	int got_there = 0;
	int score_on_time = 0;
	int score_on_bonus = 0;
	int total_time = 0;

	for (int c = 0; c < C; c++) {
		if (reach[c] >= 0 && reach[c] <= D) {
			got_there++;
			score_on_time += D - reach[c];
			score_on_bonus += F;
			total_time += reach[c];
		}
	}

	if (show_details) {
		print("Score: {}\n", score_on_time + score_on_bonus);
		print("  {}  time\n  {}  bonus\n", score_on_time, score_on_bonus);
		print("  Cars reached: {}/{}\n", got_there, C);
		print("  Average time: {:.3f}\n", 1.0 * total_time / got_there);
	}
	return score_on_time + score_on_bonus;
}

// *****

auto hill_climbing(int iterations = 50) {
	int score = simulate();

	while (iterations--) {
		auto save = schedules;
		int improvements = 0;

		default_random_engine mt(random_device{}());
		bernoulli_distribution dist(150.0 / V);

		for (int u = 0; u < V; u++) {
			if (dist(mt)) {
				shuffle(ALL(schedules[u]), mt);
			}
		}

		int new_score = simulate();
		if (new_score > score) {
			improvements++;
			iterations++;
			score = new_score;
		} else {
			schedules = save;
		}
		print("Score: {} ({})  \r", score, iterations);
		cout << flush;
	}
	print("\n");

	return score;
}

void solve_one_second() {
	for (int u = 0; u < V; u++) {
		for (int e : in_edges[u]) {
			schedules[u].push_back({e, 1});
		}
	}
}

auto solve_dynamic_ratio() {
	schedules.assign(V, {});
	for (int u = 0; u < V; u++) {
		int total_cars = cnt_in_cars[u];
		double weight = 1.0 * total_cars / D;
		double div = 0.2 / weight;
		for (int e : in_edges[u]) {
			int cars = cars_through[e].size();
			if (cars > 0) {
				schedules[u].push_back({e, int(ceil(cars / div))});
			}
		}
		sort(ALL(schedules[u]), [&](Light a, Light b) {
			if (cnt_starting_edge[a.id] != cnt_starting_edge[b.id])
				return cnt_starting_edge[a.id] > cnt_starting_edge[b.id];
			else if (cars_through[a.id] != cars_through[b.id])
				return cars_through[a.id] > cars_through[b.id];
			else
				return a.id < b.id;
		});
	}
	int score = simulate();
	print("Score: {}\n", score);
}

auto solve_ratio(double div) {
	schedules.assign(V, {});
	for (int u = 0; u < V; u++) {
		for (int e : in_edges[u]) {
			int cars = cars_through[e].size();
			if (cars > 0) {
				schedules[u].push_back({e, int(ceil(cars / div))});
			}
		}
		sort(ALL(schedules[u]), [&](Light a, Light b) {
			if (cnt_starting_edge[a.id] != cnt_starting_edge[b.id])
				return cnt_starting_edge[a.id] > cnt_starting_edge[b.id];
			else if (cars_through[a.id] != cars_through[b.id])
				return cars_through[a.id] > cars_through[b.id];
			else
				return a.id < b.id;
		});
	}
	return hill_climbing(400);
}

auto solve_in_count(double lo, double hi, double increment) {
	double best_div = 1.0;
	int best_score = 0;
	for (double div = lo; div <= hi; div += increment) {
		int score = solve_ratio(div);
		if (score > best_score) {
			best_div = div;
			best_score = score;
			print("\rBest: {} (div {:.2f})   \r", best_score, best_div);
			cout << flush;
		}
	}
	solve_ratio(best_div);
	print("Score: {} (div {:.2f})\n", best_score, best_div);
}

auto solve(char which) {
	if (which == 'b')
		return solve_ratio(5.55);
	if (which == 'c')
		return solve_ratio(14.50);
	if (which == 'd')
		return solve_ratio(7.09);
	if (which == 'e')
		return solve_ratio(3.10);
	if (which == 'f')
		return solve_ratio(37.40);

	return solve_ratio(1);
}

// *****

int main(int argc, char **argv) {
	if (argc <= 1) {
		print("Missing input file? Run with 'make run'\n");
		return 1;
	}
	string filename = argv[1];
	print("Running... {}\n", filename);

	ifstream inputfile(filename);
	ofstream statsfile(replace_file_folder(filename, "stats"));
	ofstream outfile(replace_file_folder(filename, "output"));
	assert(inputfile.is_open());
	assert(statsfile.is_open());
	assert(outfile.is_open());

	read_and_stats(inputfile, statsfile);
	// solve(deduce_input_file(filename));
	// write(outfile);
	return 0;
}
