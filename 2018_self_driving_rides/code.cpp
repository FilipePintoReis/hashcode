#include "lib.hpp"

struct Point {
	int x, y;
};

struct Ride {
	Point start, finish;
	int s; // earliest start
	int t; // latest finish
};

int R, C; // rows and columns
int F;    // number of taxi
int N;    // number of rides
int B;    // on-time bonus
int T;    // number of steps
vector<Ride> rides;
long score = 0;

int dist(Point a, Point b) { return abs(a.x - b.x) + abs(a.y - b.y); }

void read(ifstream &in) {
	in >> R >> C >> F >> N >> B >> T;
	rides.resize(N);
	for (auto &[start, finish, s, t] : rides)
		in >> start.x >> start.y >> finish.x >> finish.y >> s >> t;
}

void write(ofstream &out) { (void)out; }

// *****

void stats(ofstream &out) {
	vector<int> dists(N);
	vector<int> time_start(N);
	vector<int> time_finish(N);

	for (int i = 0; i < N; i++) {
		dists[i] = dist(rides[i].start, rides[i].finish);
		time_start[i] = rides[i].s;
		time_finish[i] = rides[i].t;
	}

	int min_dist = *min_element(ALL(dists));
	double avg_dist = 1.0 * accumulate(ALL(dists), 0L) / N;
	int max_dist = *max_element(ALL(dists));

	print(out, "{}x{} board\n", R, C);
	print(out, "{} timesteps\n", T);
	print(out, "{} taxis | {} rides\n", F, N);
	print(out, "  {:.3f} avg. rides per taxi\n", 1.0 * N / F);
	print(out, "  {} bonus\n", B);
	print(out, "\n");

	print(out, "Distances...\n");
	print(out, "  min {}\n", min_dist);
	print(out, "  avg {}\n", avg_dist);
	print(out, "  max {}\n", max_dist);
	print(out, "{}\n", histogram(dists));

	print(out, "Earliest starts...\n");
	print(out, "{}\n", histogram(time_start));

	print(out, "Latest finishes...\n");
	print(out, "{}\n", histogram(time_finish));

	int compR = R, compC = C, ratio = 1;
	while (compR > 200 || compC > 200)
		compR = (compR + 1) / 2, compC = (compC + 1) / 2, ratio *= 2;

	vector<string> board(compR, string(compC, ' '));
	for (int i = 0; i < N; i++) {
		auto [a, b] = rides[i].start;
		auto [x, y] = rides[i].finish;
		a /= ratio, b /= ratio, x /= ratio, y /= ratio;
		board[a][b] = (board[a][b] == ' ' || board[a][b] == '@') ? '@' : '$';
		board[x][y] = (board[x][y] == ' ' || board[x][y] == '#') ? '#' : '$';
	}

	print(out, "   @ start points\n");
	print(out, "   # finish points\n");
	print(out, "   $ both\n");
	print(out, "+{}+\n", string(compC, '-'));
	for (const auto &row : board)
		print(out, "|{}|\n", row);
	print(out, "+{}+\n\n", string(compC, '-'));
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
