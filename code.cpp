#include <bits/stdc++.h>
#include <fmt/format.h>

using namespace std;

// *****

struct Point {
	int x, y;
};

struct Task {
	int score, P;
	vector<Point> assembly;
};

int W, H; // width (columns) and height (rows)
int R;    // number of robots
int M;    // mount points
int T;    // number of tasks
int L;    // time steps

vector<Point> mounts;
vector<Task> tasks;

void read() {
	cin >> W >> H >> R >> M >> T >> L;
	mounts.resize(M);
	for (int i = 0; i < M; i++) {
		cin >> mounts[i].x >> mounts[i].y;
	}
	tasks.resize(T);
	for (int i = 0; i < T; i++) {
		cin >> tasks[i].score >> tasks[i].P;
		tasks[i].assembly.resize(tasks[i].P);
		for (int j = 0; j < tasks[i].P; j++) {
			cin >> tasks[i].assembly[j].x >> tasks[i].assembly[j].y;
		}
	}
}

// *****

void stats() {}

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
	return 0;
}
