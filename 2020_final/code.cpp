#include <bits/stdc++.h>
#include <fmt/format.h>

using namespace std;

// *****

struct Point {
	int x, y;
};

struct Task {
	int S, P; // score; number of assembly points
	int D;    // assembly manhattan distance
	vector<Point> assembly;
};

struct Arm {
	int M;                     // chosen mountpoint
	vector<int> tasks;         // chosen tasks
	vector<char> instructions; // instructions
};

int W, H; // width (columns) and height (rows)
int R;    // number of robots
int M;    // mount points
int T;    // number of tasks
int L;    // time steps

vector<Point> mounts;
vector<Task> tasks;
vector<Arm> arms;

int dist(Point a, Point b) {
	return abs(a.x - b.x) + abs(a.y - b.y);
}

void read() {
	cin >> W >> H >> R >> M >> T >> L;
	mounts.resize(M);
	for (auto &mount : mounts) {
		cin >> mount.x >> mount.y;
	}
	tasks.resize(T);
	for (auto &task : tasks) {
		cin >> task.S >> task.P;
		task.assembly.resize(task.P);
		for (auto &ass : task.assembly) {
			cin >> ass.x >> ass.y;
		}
		task.D = 0;
		for (int i = 1; i < task.P; i++) {
			task.D += dist(task.assembly[i - 1], task.assembly[i]);
		}
	}
}

void write() {
	fmt::print("{}\n", arms.size());
	for (const auto &arm : arms) {
		fmt::print("{} {} {} {}\n", mounts[arm.M].x, mounts[arm.M].y,
				   arm.tasks.size(), arm.instructions.size());
		fmt::print("{}\n", fmt::join(arm.tasks, " "));
		fmt::print("{}\n", fmt::join(arm.instructions, " "));
	}
}

// *****

void stats() {
	fmt::print("{}x{}\n", W, H);
	fmt::print("Robots: {} @ {} mountpoints\n", R, M);
	fmt::print("{} tasks\n", T);
	fmt::print("{} timesteps\n", L);
	fmt::print("\n");

	// Task statistics
	int A = 0; // total assembly points
	int S = 0;
	int D = 0;
	for (auto &task : tasks) {
		S += task.S;
		A += task.P;
		D += task.D;
	}
	fmt::print("{} assembly points ({:.2f}/task)\n", A, 1.0 * A / T);
	fmt::print("{} total score ({:.2f}/task)\n", S, 1.0 * S / T);
	fmt::print("{} assembly travel ({:.2f}/task)\n", D, 1.0 * D / T);

	vector<string> mat(W, string(H, ' '));
	for (auto mount : mounts) {
		auto [x, y] = mount;
		mat[x][y] = 'M';
	}
	for (const auto &task : tasks) {
		for (auto ass : task.assembly) {
			auto [x, y] = ass;
			mat[x][y] = '.';
		}
	}

	fmt::print("+{}+\n", string(H, '-'));
	for (const auto &row : mat) {
		fmt::print("|{}|\n", row);
	}
	fmt::print("+{}+\n", string(H, '-'));
}

// *****

void solve() {}

// *****

int main(int argc, char **argv) {
	if (argc <= 1) {
		fmt::print("Missing input filename\n");
		return 1;
	}
	string filename = argv[1];
	ifstream input(filename);
	if (!input.is_open()) {
		fmt::print("Failed to open input '{}'\n", filename);
		return 1;
	}
	cin.rdbuf(input.rdbuf());
	read();
	stats();
	return 0;
}
