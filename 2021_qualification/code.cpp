#include "lib.hpp"

void read_and_stats(ifstream &in, ofstream &out) {}

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
