#include "lib.hpp"

void read_and_stats(ifstream &in, ofstream &out) {
	vector<int> x(40000);
	vector<int> y(40000);
	for (int i = 0; i < 40000; i++)
		x[i] = rand() % 700, y[i] = rand() % 700;
	print(out, "{}", scatter(x, y));
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
