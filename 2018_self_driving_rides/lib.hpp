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

// For integers (short/int/long); might need to be adjusted for floats
template <typename T>
string histogram(const vector<T> &vec, int boxes = -1, int barlen = 80) {
	int N = vec.size();
	assert(N > 0);
	auto lo = *min_element(ALL(vec));
	auto hi = *max_element(ALL(vec));
	boxes = boxes < 0 ? 5 * ceil(log10(N + 1)) : boxes;
	boxes = min(boxes, N);
	assert(boxes > 1);

	vector<int> cnt(boxes, 0);
	auto size = 1.0 * (hi - lo) / boxes;
	for (auto u : vec) {
		int x = int(floor((u - lo) / size));
		cnt[clamp(x, 0, boxes - 1)]++;
	}
	auto mode = *max_element(ALL(cnt));

	int range_width = max(to_string(lo).length(), to_string(hi).length());
	int cnt_width = to_string(mode).length();

	string str;
	for (int i = 0; i < boxes; i++) {
		int len = 1.0 * barlen * cnt[i] / mode;
		int L = int(ceil(lo + i * size));
		int R = max(L, int(ceil(lo + (i + 1) * size - 1.0)));
		string bar;
		while (len--)
			bar += u8"█";
		str += format("{:>{}} {:>{}} | {:>{}} | {}\n", L, range_width, R,
					  range_width, cnt[i], cnt_width, bar);
	}
	return str;
}
