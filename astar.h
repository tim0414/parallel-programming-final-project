#pragma once
#pragma once
#ifndef astar_H
#define astar_H
#include <vector>
class astar {
public:
	/*
	* Vectors
	*/

	static std::vector<std::vector<int>> grid_;
	static std::vector<std::vector<float>> heuristics_;
	static std::vector<std::vector<float>> reverse_heuristics_;
	static std::vector<std::vector<unsigned int>> walk_cost_;
	std::vector<int> goal_;
	std::vector<int> init_;
	static int cost_;
	static std::vector<std::vector<int>> delta_;
	static std::vector<std::vector<unsigned int>> closed_;
	static std::vector<std::vector<int>> expand_;
	std::vector<std::vector<int>> action_;
	std::vector<std::vector<char>> policy_;
	std::vector<std::vector<char>> delta_name_;
	/*
	* Constructor
	*/
	astar();

	/*
	* Destructor.
	*/
	~astar();


	/*
	* Search  astar error variables given cross track error.
	*/
	std::vector<std::vector<int>> Search();
	static void read_map();
	static void* thread_search(void* param);

};



#endif /* astar_H */

