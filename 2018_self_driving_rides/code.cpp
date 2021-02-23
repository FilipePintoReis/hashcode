#include "lib.hpp"

void read(ifstream &in) { (void)in; }

void write(ofstream &out) { (void)out; }

// *****

void stats(ofstream &out) {
	vector<int> vec(740);
	for (int &u : vec)
		u = rand() % 1337;
	print(out, histogram(vec));
}

// *****

void solve() {}

// *****

int main(int argc, char **argv) {
	if (argc <= 1) {
		print("Missing input file? Run with 'make run'\n");
		return 1;
	}
	string filename = argv[1];
	ifstream inputfile(filename);
	if (!inputfile.is_open()) {
		print("Failed to open input '{}'\n", filename);
		return 1;
	}

	print("Running... {}\n", filename);
	read(inputfile);

	ofstream statsfile(replace_file_folder(filename, "stats"));
	stats(statsfile);

	// solve();

	ofstream outfile(replace_file_folder(filename, "output"));
	write(outfile);
	return 0;
}
