#include "lib.hpp"

struct Point {
	int x, y;
};

int dist(Point a, Point b) { return abs(a.x - b.x) + abs(a.y - b.y); }

struct Ride {
	int id;
	Point start, finish;
	int s, t; // earliest start, latest finish
	int earliest_start() const { return s; }
	int earliest_finish() const { return s + length(); }
	int latest_start() const { return t - length(); }
	int latest_finish() const { return s; }
	int length() const { return dist(start, finish); }
	bool possible() const { return s + length() <= t; }

	bool arrives_early(int t, Point where) const {
		return t + dist(where, start) <= earliest_start();
	}
	bool arrives_on_time(int t, Point where) const {
		return t + dist(where, start) <= latest_start();
	}
	int leaves(int t, Point where) const {
		return max(t + dist(where, start), earliest_start()) + length();
	}
};

struct TaxiPlan {
	vector<int> ids;
	Point start, finish;
	int s, t; // start time, finish time
	void add(Ride ride) {
		ids.push_back(ride.id);
		int arrives = max(ride.earliest_start(), t + dist(finish, ride.start));
		assert(arrives <= ride.latest_start());
		finish = ride.finish;
		t = arrives + ride.length();
	}
};

int R, C; // rows and columns
int F;    // number of taxi
int N;    // number of rides
int B;    // on-time bonus
int T;    // number of steps
vector<Ride> rides;
vector<int> dists;
vector<int> time_start;
vector<int> time_finish;

vector<TaxiPlan> taxis; // solution assignment

void read(ifstream &in) {
	in >> R >> C >> F >> N >> B >> T;
	rides.resize(N);
	dists.resize(N);
	time_start.resize(N);
	time_finish.resize(N);
	for (int i = 0; i < N; i++) {
		auto &[id, start, finish, s, t] = rides[i];
		id = i;
		in >> start.x >> start.y >> finish.x >> finish.y >> s >> t;
		s = max(s, dist({0, 0}, start));
		dists[i] = dist(start, finish);
		time_start[i] = s;
		time_finish[i] = t;
	}
}

void write(ofstream &out) {
	for (const auto &taxi : taxis) {
		print(out, "{} {}\n", taxi.ids.size(), fmt::join(taxi.ids, " "));
	}
}

// *****

void stats(ofstream &out) {
	int min_dist = *min_element(ALL(dists));
	double avg_dist = 1.0 * accumulate(ALL(dists), 0L) / N;
	int max_dist = *max_element(ALL(dists));

	print(out, "{}x{} board\n", R, C);
	print(out, "{} timesteps\n", T);
	print(out, "{} taxis | {} rides\n", F, N);
	print(out, "  {:.3f} avg. rides per taxi\n", 1.0 * N / F);
	print(out, "  {:.3f} avg. ridetime per taxi\n", 1.0 * N * avg_dist / F);
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
// ***** SOLUTION
// *****

struct ByEarliestStart {
	template <typename T>
	bool operator()(const T &a, const T &b) {
		return a.earliest_start() < b.earliest_start();
	}
};
struct ByEarliestFinish {
	template <typename T>
	bool operator()(const T &a, const T &b) {
		return a.earliest_finish() < b.earliest_finish();
	}
};
struct ByLatestStart {
	template <typename T>
	bool operator()(const T &a, const T &b) {
		return a.latest_start() < b.latest_start();
	}
};
struct ByLatestFinish {
	template <typename T>
	bool operator()(const T &a, const T &b) {
		return a.latest_finish() < b.latest_finish();
	}
};

int PENALTY_SKIP_THRESHOLD = 300000;

int compute_penalty(const TaxiPlan &taxi, Ride ride) {
	int commute = dist(taxi.finish, ride.start);
	int arrives = taxi.t + commute;
	if (arrives > ride.latest_start())
		return INT_MAX;

	int waits = max(ride.earliest_start() - arrives, 0);
	int is_late = arrives > ride.earliest_start() ? B : 0;

	// heuristics...
	// quanto mais perto do fim 

	return 2 * commute + 3 * waits + 9 * is_late;
}

void solve_simulate_ride_by_ride() {
	taxis.resize(F, {});

	vector<Ride> sorted_rides = rides;

	// 1: Sort rides
	if (3L * B * N > accumulate(ALL(dists), 0L)) {
		sort(ALL(sorted_rides), ByEarliestStart{}); // e
	} else {
		sort(ALL(sorted_rides), ByLatestStart{}); // a, b, c, d
	}

	// 2: Evaluate rides

	// 3: Go through the rides one by one
	int rides_early = 0, rides_ok = 0, rides_missed = 0, rides_skipped = 0;

	for (int i = 0; i < N; i++) {
		auto ride = sorted_rides[i];
		int best_f = -1;
		int best_penalty = INT_MAX;

		for (int f = 0; f < F; f++) {
			int penalty = compute_penalty(taxis[f], ride);
			if (best_penalty > penalty) {
				best_f = f;
				best_penalty = penalty;
			}
		}

		if (best_penalty == INT_MAX) {
			rides_missed++;
		} else if (best_penalty >= PENALTY_SKIP_THRESHOLD) {
			rides_skipped++;
		} else {
			auto &taxi = taxis[best_f];
			ride.arrives_early(taxi.t, taxi.finish) ? rides_early++
													: rides_ok++;
			taxis[best_f].add(ride);
		}

		double progress = 100.0 * (i + 1) / N;
		print("\r{:6.2f}% | {:5}/{:5} | {:5} early, {:5} ok, {:5} missed, {:5} "
			  "skipped\r",
			  progress, i + 1, N, rides_early, rides_ok, rides_missed,
			  rides_skipped);
		fflush(stdout);
	}

	print("\nDone\n");
}

void solve() { solve_simulate_ride_by_ride(); }

// *****

void judge_and_score() {
	long score = 0;
	vector<bool> vis(N, false);

	for (const auto &taxi : taxis) {
		Point where = {0, 0};
		int t = 0;
		for (int id : taxi.ids) {
			if (vis[id]) {
				print("BUG: Ride {} has already been taken\n", id);
				return;
			}
			vis[id] = true;

			if (!rides[id].arrives_on_time(t, where)) {
				print("BUG: Taxi arrives too late to ride {}\n", id);
				return;
			}

			int early = rides[id].arrives_early(t, where);
			score += rides[id].length() + (early ? B : 0);

			t = rides[id].leaves(t, where);
			where = rides[id].finish;
		}
	}

	print("OK. Score: {}\n", score);
}

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

	solve();
	judge_and_score();

	ofstream outfile(replace_file_folder(filename, "output"));
	write(outfile);
	return 0;
}
