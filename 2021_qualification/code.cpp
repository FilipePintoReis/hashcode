#include "lib.hpp"

struct Street {
	int id;
	int B, E; // begin and end intersections
	int L;    // length of the street, <=D
	int length() const { return L; }
};

struct Path {
	int P;           // number of streets, <=1000
	vector<int> ids; // street ids, starts at end of first street
};

int D; // duration of the simulation, <=10000
int V; // number of intersections, <=100000
int E; // number of streets, <= 100000
int C; // number of cars, <=1000
int F; // points for each car that reaches destination, <=1000
vector<Street> streets;
vector<Path> paths;
vector<string> street_names;

void read_and_stats(ifstream &in, ofstream &out) {
	in >> D >> V >> E >> C >> F;
	streets.resize(E);
	paths.resize(C);
	street_names.resize(E);

	for (int i = 0; i < E; i++) {
		auto &[id, B, E, L] = streets[i];
		id = i, in >> B >> E >> street_names[i] >> L;
	}
	for (int i = 0; i < C; i++) {
		auto &[P, ids] = paths[i];
		in >> P;
		ids.resize(P);
		for (int j = 0; j < P; j++)
			in >> ids[j];
	}

	vector<int> cnt_outgoing(V, 0);
	vector<int> cnt_incoming(V, 0);
	vector<int> street_L(E, 0);

	for (int i = 0; i < E; i++) {
		const auto &[id, B, E, L] = streets[i];
		cnt_outgoing[B]++;
		cnt_incoming[E]++;
		street_L[i] = L;
	}

	print(out, "{} nodes / {} edges\n", V, E);
	print(out, "{} cars\n", C);
	print(out, "{} bonus points\n", F);
	print(out, "{} duration\n", D);

	print(out, "Outgoing distribution\n{}\n", histogram(cnt_outgoing));
	print(out, "Incoming distribution\n{}\n", histogram(cnt_incoming));
	print(out, "Individual street lengths\n{}\n", histogram(street_L));
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
