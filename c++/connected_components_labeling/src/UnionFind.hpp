#ifndef _UNION_FIND_HPP_
#define _UNION_FIND_HPP_

class UnionFind {
	private:
		std::vector<int> ids;

	public:
		UnionFind (int size);
		bool find (int p, int q);
		void unite (int p, int q); 
	private:
		int root (int i);

}; 

#endif
