#include <bits/stdc++.h>
#include <fmt/format.h>

using namespace std;
using fmt::print, fmt::format;

string replace_file_folder(string filename, string newfolder) {
	auto i = filename.find('/');
	assert(i != string::npos);
	return newfolder + filename.substr(i);
}
