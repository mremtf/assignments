#include <vector>

#include "UnionFind.hpp"

UnionFind::UnionFind (int size) {
	ids.resize(size);
	for (int i = 0; i < size; i++) {
		ids[i] = i;
	}
}

void UnionFind::unite (int x, int y, std::vector<int>& parent) {
	while (parent[x] > 0) {
		x = parent[x];
	}

	while (parent[y > 0]) {
		y = parent[y];
	}
	if (x != y) {
		if (x < y)
			parent[x] = y;
		else parent[y] = x;
	}
}

int UnionFind::find (int x, const std::vector<int>& parent, std::vector<int>& labels) {
	while (parent[x]  > 0){
		x = parent[x];
	}
	if (labels[x] == 0) {
		labels[x] = next_label++;
	}
	return label[x];
}

