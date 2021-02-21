#include "lib.hpp"

using pizza_t = vector<short>;     // ordered list of ingredients in a pizza
using selection_t = vector<short>; // selection of pizzas for delivery

int M;                  // number of pizzas
int C;                  // number of different ingredients (ids are [0,C[)
int T2, T3, T4;         // number of teams with 2,3,4 elements
vector<pizza_t> pizzas; // pizzas[i]: ingredients in pizza i
vector<int> counts;     // count[j]: number of instances of ingredients j
vector<selection_t> D2, D3, D4; // deliveries

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
	int D = D2.size() + D3.size() + D4.size();

	print(out, "{}\n", D);
	for (const auto &team : D2) {
		print(out, "{} {} {}\n", 2, team[0], team[1]);
	}
	for (const auto &team : D2) {
		print(out, "{} {} {} {}\n", 3, team[0], team[1], team[2]);
	}
	for (const auto &team : D2) {
		print(out, "{} {} {} {} {}\n", 4, team[0], team[1], team[2], team[3]);
	}
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
// ***** SOLUTION
// *****

// Sorting method number 1:

struct LargestPizzasFirst {};

set<Phantom> universe;
int t2 = 0, t3 = 0, t4 = 0; // number of selections currently prepared
long total_score = 0;

// How many ingredients in common do two pizzas have?
struct Evaluation {
	int A, B, C, D; // in a, int b, int both, in only one. A + B == C + D.
};

// A phantom pizza that is the result of aggregating two or more pizzas
struct Phantom {
	pizza_t pizza;
	selection_t selection;
	int ingredients() const { return pizza.size(); }
	int people() const { return selection.size(); }
};

Evaluation evaluate(const pizza_t &a, const pizza_t &b) {
	int i = 0, j = 0, A = a.size(), B = b.size();
	int C = 0;
	while (i < A && j < B) {
		if (a[i] < b[j])
			i++;
		else if (a[i] > b[j])
			j++;
		else
			i++, j++, C++;
	}
	int D = A + B - C;
	return {A, B, C, D};
}

pizza_t merge_pizzas(const pizza_t &a, const pizza_t &b) {
	int i = 0, j = 0, A = a.size(), B = b.size();
	pizza_t result;
	result.reserve(A + B);
	while (i < A && j < B) {
		if (a[i] < b[j])
			result.push_back(a[i]), i++;
		else if (a[i] > b[j])
			result.push_back(b[j]), j++;
		else
			result.push_back(a[i]), i++, j++;
	}
	return result;
}

Phantom merge_phantoms(const Phantom &a, const Phantom &b) {
	auto pizza = merge_pizzas(a.pizza, b.pizza);
	auto selection = a.selection;
	selection.insert(selection.end(), begin(b.selection), end(b.selection));
	return {pizza, selection};
}

bool available(int p) {
	return (p == 2 && t2 < T2) || (p == 3 && t3 < T3) || (p == 4 && t4 < T4);
}

void deliver(const Phantom &a) {
	if (a.people() == 2) {
		D2.push_back(a.selection);
	} else if (a.people() == 3) {
		D3.push_back(a.selection);
	} else if (a.people() == 4) {
		D4.push_back(a.selection);
	} else {
		assert(false);
	}
	total_score += long(a.ingredients()) * a.ingredients();
}

void adjust(int ap, int bp) {
	t2 -= (ap == 2) + (bp == 2) - (ap + bp == 2);
	t3 -= (ap == 3) + (bp == 3) - (ap + bp == 3);
	t4 -= (ap == 4) + (bp == 4) - (ap + bp == 4);
	assert(0 <= t2 && t2 <= T2 && 0 <= t3 && t3 <= T3 && 0 <= t4 && t4 <= T4);
}

void solve() {
	while (!universe.empty() && (t2 || t3 || t4)) {
		Phantom a = *universe.begin();
		universe.erase(a);

		double best_ratio = 1.01;
		Phantom best;

		for (const auto &b : universe) {
			if (available(a.people() + b.people())) {
				auto [A, B, C, D] = evaluate(a.pizza, b.pizza);
				if (best_ratio > C / (C + D)) {
					best_ratio = C / (C + D);
					best = b;
				}
			}
		}

		if (best_ratio > 1.0) {
			if (available(a.people())) {
				deliver(a);
			} else {
				print("Warning: Ignoring phantom pizza with unavailable team "
					  "({}/{}/{}) #ingredients={} size={}\n",
					  T2 - t2, T3 - t3, T4 - t4, a.ingredients(), a.people());
			}
		} else {
			universe.erase(best);
			Phantom ab = merge_phantoms(a, best);
			if (ab.people() == 4) {
				deliver(ab);
			} else {
				universe.insert(ab);
			}
		}
	}
}

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
