#include "lib.hpp"

int M;                      // number of pizzas
int C;                      // number of different ingredients (ids are [0,C[)
int T2, T3, T4;             // number of teams with 2,3,4 elements
vector<vector<int>> pizzas; // pizzas[i]: ingredients in pizza i
vector<int> counts;         // count[j]: number of instances of ingredients j

void read(ifstream &in) {
	in >> M >> T2 >> T3 >> T4;
	pizzas.resize(M);
	unordered_map<string, int> mapping;

	for (auto &pizza : pizzas) {
		int n;
		in >> n;
		pizza.resize(n);
		for (auto &ingredient : pizza) {
			string text;
			in >> text;
			ingredient = mapping.emplace(text, mapping.size()).first->second;
		}
		sort(begin(pizza), end(pizza));
	}

	C = mapping.size();
	counts.resize(C);
	for (const auto &pizza : pizzas) {
		for (auto ingredient : pizza) {
			counts[ingredient]++;
		}
	}
}

void write(ofstream &out) {
	(void)out;
}

// *****

void stats(ofstream &out) {
	vector<int> lengths(M);
	for (int i = 0; i < M; i++)
		lengths[i] = pizzas[i].size();

	int min_ingredients = *min_element(begin(lengths), end(lengths));
	int max_ingredients = *max_element(begin(lengths), end(lengths));
	int min_count = *min_element(begin(counts), end(counts));
	int max_count = *max_element(begin(counts), end(counts));
	int T = accumulate(begin(counts), end(counts), 0);
	int people = 2 * T2 + 3 * T3 + 4 * T4;

	print(out, "{}/{}/{} teams ({} total people)\n", T2, T3, T4, people);
	print(out, "{} pizzas\n", M);
	print(out, "  {:4} min ingredients\n", min_ingredients);
	print(out, "  {:4} max ingredients\n", max_ingredients);
	print(out, "  {:7.3f} avg. per pizza\n", 1.0 * T / M);
	print(out, "{} unique ingredients\n", C);
	print(out, "{} total ingredients\n", T);
	print(out, "  {:4} min ingredient count\n", min_count);
	print(out, "  {:4} max ingredient count\n", max_count);
	print(out, "  {:7.3f} avg. per unique ingredient\n", 1.0 * T / C);
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

	// ofstream outfile(replace_file_folder(filename, "output"));
	// write(outfile);
	return 0;
}
