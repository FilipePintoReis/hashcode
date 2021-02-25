#include "lib.hpp"

struct Edge {
	int id;
	int B, E;  // begin and end nodes
	int L = 0; // length of the street, <=D
	int length() const { return L; }
};

struct Node {
	int id;
	vector<int> in, out;
	int incnt() const { return in.size(); }
	int outcnt() const { return out.size(); }
};

struct Car {
	int P;           // number of edges, <=1000
	vector<int> ids; // street ids, starts at end of first street
	int L = 0;       // total length
};

int D; // duration of the simulation, <=10000
int V; // number of nodes, <=100000
int E; // number of edges, <= 100000
int C; // number of cars, <=1000
int F; // points for each car that reaches destination, <=1000
vector<Node> nodes;
vector<Edge> edges;
vector<Car> cars;
vector<string> edge_names;

void read_and_stats(ifstream &in, ofstream &out) {
	in >> D >> V >> E >> C >> F;
	nodes.resize(V);
	edges.resize(E);
	cars.resize(C);
	edge_names.resize(E);

	vector<int> cnt_outgoing(V, 0);
	vector<int> cnt_incoming(V, 0);
	vector<int> edge_L(E, 0);
	vector<int> cars_L(C, 0);
	vector<int> cars_P(C, 0);

	for (int i = 0; i < E; i++) {
		auto &[id, B, E, L] = edges[i];
		id = i, in >> B >> E >> edge_names[i] >> L;

		cnt_outgoing[B]++;
		cnt_incoming[E]++;
		edge_L[i] = L;
	}
	for (int i = 0; i < C; i++) {
		auto &[P, ids, L] = cars[i];
		in >> P, ids.resize(P);
		for (int j = 0; j < P; j++) {
			in >> ids[j];
			L += j > 0 ? edges[ids[j]].L : 0;
		}

		cars_L[i] = L;
		cars_P[i] = P;
	}

	long perfect_score = 0;
	for (int i = 0; i < C; i++) {
		if (cars[i].L < D) {
			perfect_score += (D - cars[i].L) + F;
		}
	}

	print(out, "{} nodes / {} edges\n", V, E);
	print(out, "{} cars\n", C);
	print(out, "{} bonus points\n", F);
	print(out, "{} duration\n", D);

	print(out, "Theoretical perfect score: {}\n", perfect_score);

	print(out, "Outgoing distribution\n{}\n", histogram(cnt_outgoing));
	print(out, "Incoming distribution\n{}\n", histogram(cnt_incoming));
	print(out, "Individual street lengths\n{}\n", histogram(edge_L));
	print(out, "Car path lengths\n{}\n", histogram(cars_L));
	print(out, "Car path street counts\n{}\n", histogram(cars_P));
}

void write(ofstream &out) {}

// *****

void solve(char which) {}

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
	solve(deduce_input_file(filename));
	write(outfile);
	return 0;
}
