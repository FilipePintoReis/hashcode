#include <bits/stdc++.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

using namespace std;
using fmt::print, fmt::format;

#define ALL(v) begin(v), end(v)

string replace_file_folder(string filename, string newfolder) {
	auto i = filename.find('/');
	assert(i != string::npos);
	return newfolder + filename.substr(i);
}

string repeat(int len, string c) {
	string s;
	while (len--)
		s += c;
	return s;
}

#define INTLEN(x) to_string(x).length()

// For integers (short/int/long); might need to be adjusted for floats
auto histogram(const vector<int> &v, int bar_width = 80, int num_boxes = -1) {
	int N = v.size();
	assert(N > 0);

	auto hist_min = *min_element(ALL(v));             // minimum value
	auto hist_max = *max_element(ALL(v));             // maximum value
	auto hist_avg = 1.0 * accumulate(ALL(v), 0L) / N; // average value
	auto hist_var = 1.0 / N * accumulate(ALL(v), 0L, [&](long acc, int val) {
						return acc + 1L * (hist_avg - val) * (hist_avg - val);
					});
	auto hist_dev = sqrt(hist_var);

	string s;
	s += format("  min {}\n", hist_min);
	s += format("  avg {:.3f}\n", hist_avg);
	s += format("  max {}\n", hist_max);
	s += format("  dev {:.3f}\n", hist_dev);

	if (hist_min == hist_max) {
		s += format(" {} | {} | {}\n", hist_min, N, repeat(bar_width, u8"█"));
		return s;
	}

	num_boxes = num_boxes < 0 ? 5 * ceil(log10(N + 1)) : num_boxes;
	num_boxes = min(num_boxes, N);
	assert(num_boxes > 1);

	vector<vector<int>> box(num_boxes);
	vector<int> box_cnt(num_boxes, 0);

	auto box_width = 1.0 * (hist_max - hist_min) / num_boxes;
	for (int i = 0; i < N; i++) {
		int x = int(floor((v[i] - hist_min) / box_width));
		x = clamp(x, 0, num_boxes - 1);
		box[x].push_back(v[i]);
		box_cnt[x]++;
	}
	int top = *max_element(ALL(box_cnt));
	int label_width = INTLEN(hist_max);
	int count_width = INTLEN(top);

	for (int x = 0; x < num_boxes; x++) {
		int L = int(ceil(hist_min + x * box_width));
		int R = max(L, int(ceil(hist_min + (x + 1) * box_width - 1.0)));

		int len = ceil(1.0 * bar_width * box_cnt[x] / top);
		string label = format("{:>{}} {:>{}}", L, label_width, R, label_width);
		string count = format("{:>{}}", box_cnt[x], count_width);
		string bar = repeat(len, u8"█");
		s += format("{} | {} | {}\n", label, count, bar);
	}
	return s;
}

auto collapse2D(int R, int C, int maxR = 70, int maxC = 150) {
	assert(maxR > 2 && maxC > 2);
	int ratioR = 1, ratioC = 1;
	while (R > maxR) {
		R = (R + 1) / 2, ratioR *= 2;
	}
	while (C > maxC) {
		C = (C + 1) / 2, ratioC *= 2;
	}
	return tuple<int, int, int, int>{R, C, ratioR, ratioC};
}

auto scatter(const vector<int> &vx, const vector<int> &vy, int X = 120,
			 int Y = 50, bool sw = true) {
	int N = vx.size();
	assert(vx.size() == vy.size() && N > 0);

	int x_min = *min_element(ALL(vx));
	int x_max = *max_element(ALL(vx));
	int y_min = *min_element(ALL(vy));
	int y_max = *max_element(ALL(vy));

	int x_mid = (x_min + x_max) / 2;
	int y_mid = (y_min + y_max) / 2;
	int x_label_space = X - (INTLEN(x_min) + INTLEN(x_mid) + INTLEN(x_max));
	int y_label_width = max(INTLEN(y_min), INTLEN(y_max));
	string xs0(x_label_space / 2, ' '), xs1((x_label_space + 1) / 2, ' ');

	vector<vector<int>> cnt(X, vector<int>(Y, 0));
	vector<string> graph(Y);

	auto x_width = 1.0 * (x_max - x_min) / X;
	auto y_width = 1.0 * (y_max - y_min) / Y;

	for (int i = 0; i < N; i++) {
		int x = floor((vx[i] - x_min) / x_width);
		int y = floor((vy[i] - y_min) / y_width);
		x = clamp(x, 0, X - 1), y = clamp(y, 0, Y - 1);
		cnt[x][y]++;
	}

	int max_cnt = 0;
	for (int i = 0; i < X; i++) {
		max_cnt = max(max_cnt, *max_element(ALL(cnt[i])));
	}

	static string braille[] = {" ",   u8"⠂", u8"⠆", u8"⠖", u8"⠶",
							   u8"⠾", u8"⡾", u8"⡿", u8"⣿"};

	for (int j = 0; j < Y; j++) {
		for (int i = 0; i < X; i++) {
			int c = int(ceil(8.0 * cnt[i][j] / max_cnt));
			graph[j] += braille[c];
		}
	}

	vector<string> srows;

	// x steps
	string xlabel = format("{}{}{}{}{}", x_min, xs0, x_mid, xs1, x_max);
	string xlabelrow = format("{}   {}\n", string(y_label_width, ' '), xlabel);
	srows.push_back(xlabelrow);
	// separator
	string sep = string(X, '-');
	string seprow = format("{} +{}\n", string(y_label_width, ' '), sep);
	srows.push_back(seprow);

	for (int j = 0; j < Y; j++) {
		string ylabel;
		if (j == Y - 1)
			ylabel = format("{:>{}}", y_max, y_label_width);
		else if (j == Y / 2)
			ylabel = format("{:>{}}", y_mid, y_label_width);
		else if (j == 0)
			ylabel = format("{:>{}}", y_min, y_label_width);
		else
			ylabel = string(y_label_width, ' ');

		string graphrow = format("{} | {}\n", ylabel, graph[j]);
		srows.push_back(graphrow);
	}

	string s;
	if (sw) {
		reverse(ALL(srows));
	}
	for (int j = 0, S = srows.size(); j < S; j++) {
		s += srows[j];
	}
	return s;
}

auto make_box(const vector<string> &matrix) {
	int R = matrix.size(), C = matrix[0].size();
	string s;
	s += format("┌{}┐\n", repeat(C, u8"─"));
	for (int i = 0; i < R; i++) {
		s += format("│{}│\n", matrix[i]);
	}
	s += format("└{}┘\n", repeat(C, u8"─"));
	return s;
}
