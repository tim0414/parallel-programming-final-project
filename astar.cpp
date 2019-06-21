#include "astar.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <chrono>

#define OBSTACLE -10

using namespace std;
using namespace std::chrono;

pthread_mutex_t mutex;

typedef void * (*THREADFUNCPTR)(void *);
typedef struct{
	vector<int> start;
	vector<int> goal;
	bool reverse;
	std::vector<std::vector<int>> expand;
	std::vector<std::vector<float>> heuristics;
	std::vector<std::vector<int>> grid;
	std::vector<std::vector<unsigned int>> closed;
	int print_result;
}path_info;

std::vector<std::vector<int>> astar::delta_ = { {-1, 0 },    //go up
			   { 0,-1 },    //go left
			   { 1, 0 },    //go down
			   { 0, 1 } };  //go right
std::vector<std::vector<int>> astar::grid_;
int astar::cost_ = 1;

std::vector<std::vector<unsigned int>> astar::walk_cost_;
std::vector<std::vector<float>> astar::heuristics_;
std::vector<std::vector<float>> astar::reverse_heuristics_;

std::vector<std::vector<unsigned int>> astar::closed_;
std::vector<std::vector<int>> astar::expand_;


astar::astar(){
	read_map();
	init_ = {0,0};
	int x = grid_.size();
	int y = grid_[0].size();
	goal_ = {x-1, y-1};
	for (unsigned int i = 0; i < grid_.size(); i++) {
		vector<unsigned int> temp1;
		vector<int> temp2;
		vector<float> temp3;
		vector<float> temp4;
		vector<unsigned int> temp5;
		for (unsigned int j = 0; j < grid_[0].size(); j++) {
			temp1.push_back(0);
			temp2.push_back(OBSTACLE);
			float distance = pow(goal_[0]-i, 2) + pow(goal_[1]-j, 2);
			float reverse_distance = pow(i-init_[0], 2) + pow(j-init_[1], 2);
			temp3.push_back(i+j);
			temp4.push_back(goal_[0]+goal_[1]-i-j);
			temp5.push_back(i+j);
		}

		closed_.push_back(temp1);
		// define the expand vector
		expand_.push_back(temp2);
		reverse_heuristics_.push_back(temp3);
		heuristics_.push_back(temp4);
		walk_cost_.push_back(temp5);
	}
	//expand_[4][3] = 10;
/*
	for(int i=0; i<heuristics_.size(); i++){
		for(int j=0; j<heuristics_[0].size(); j++){
			cout << heuristics_[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	for(int i=0; i<heuristics_.size(); i++){
		for(int j=0; j<heuristics_[0].size(); j++){
			cout << reverse_heuristics_[i][j] << " ";
		}
		cout << endl;
	}
	*/
}

astar::~astar() {}

std::vector<std::vector<int>> astar::Search() {
	cout << "create thread" << endl;
	pthread_t *thread_handles;
	thread_handles = (pthread_t*) malloc(2*sizeof(pthread_t));
	path_info *info1 = new path_info();
	path_info *info2 = new path_info();

	info1->start = init_;
	info1->goal = goal_;
	info1->reverse = 0;
	info1->expand = expand_;
	info1->heuristics = heuristics_;
	info1->print_result = 0;
	info1->grid = grid_;
	info1->closed = closed_;

	info2->start = goal_;
	info2->goal = init_;
	info2->reverse = 0;
	info2->expand = expand_;
	info2->heuristics = reverse_heuristics_;
	info2->print_result = 1;
	info2->grid = grid_;
	info2->closed = closed_;

	cout << "before create thread" << endl;
	

	pthread_mutex_init(&mutex, NULL);
	high_resolution_clock::time_point t1 = high_resolution_clock::now(); 
	pthread_create(&thread_handles[0], NULL,
      (THREADFUNCPTR)&astar::thread_search, (void*)info1);
	high_resolution_clock::time_point t2 = high_resolution_clock::now(); 
	pthread_create(&thread_handles[1], NULL,
      (THREADFUNCPTR)&astar::thread_search, (void*)info2);
	high_resolution_clock::time_point t3 = high_resolution_clock::now(); 
	cout << "after create thread" << endl;


	void *thread_result;
	pthread_join(thread_handles[0], NULL);
	high_resolution_clock::time_point t4 = high_resolution_clock::now(); 
	pthread_join(thread_handles[1], NULL);
	high_resolution_clock::time_point t5 = high_resolution_clock::now();
	duration<double,std::ratio<1,1>> duration_s1(t2-t1);
	duration<double,std::ratio<1,1>> duration_s2(t3-t2);
	duration<double,std::ratio<1,1>> duration_s3(t4-t3);
	duration<double,std::ratio<1,1>> duration_s4(t5-t4);
	duration<double,std::ratio<1,1>> duration_s5(t5-t1);
	cout << "thread create1 Time taken: " << duration_s1.count() << " ms" << endl;
	cout << "thread create2 Time taken: " << duration_s2.count() << " ms" << endl;
	cout << "thread join1 Time taken: " << duration_s3.count() << " ms" << endl;
	cout << "thread join2 Time taken: " << duration_s4.count() << " ms" << endl;
	cout << "total Time taken: " << duration_s5.count() << " ms" << endl;
	//vector<vector<int>> result = reinterpret_cast<vector<vector<int>>>(thread_result);

	pthread_mutex_destroy(&mutex);
	return expand_;
}

void* astar::thread_search(void* param){

	path_info *info = (path_info *) param;
	vector<int> start = info->start;
	vector<int> goal = info->goal;
	std::vector<std::vector<int>> expand = info->expand;
	std::vector<std::vector<float>> heuristics = info->heuristics;
	int print_result = info->print_result;
	std::vector<std::vector<int>> grid = info->grid;
	std::vector<std::vector<unsigned int>> closed = info->closed;
	

	int x = start[0];
	int y = start[1];
	int g = astar::walk_cost_[0][0];
	int h = heuristics[x][y];
	int	f = h + g;

	// Set the closed corresponding starting position
	closed[start[0]][start[1]] = 1;
	
	//cout << "x: "<< x << " y " << y << endl;
	//cout << "goal " << goal[0] << " " << goal[1] << endl;

	// Declare and define an open 2-D matrix called open
	// one row, five elements in the row initialized to 0
	vector<vector<int>> open(1, vector<int>(5, 0));
	// f is first for sorting
	open = { { f, g, h, x, y } };
	int count = 0;
	bool found = false; // set when search is complete
	bool resign = false; // set if we cannot expand

	while (found == false && resign == false) {
		if (open.size() == 0) {
			resign = true;
			//if(print_result==1)
			//	std::cout << "resign" << std::endl;
		}
		else {
/*
			std::cout << "count: " << count << std::endl;
			for(int i=0; i<open.size(); i++){
				for(int j=0; j<open[0].size();j++){
					std::cout << open[i][j] << " ";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
*/
			sort(open.begin(), open.end());
			reverse(open.begin(), open.end());
/*
			if(print_result==1){
				std::cout << " sorted: " << count << std::endl;
				for(int i=0; i<open.size(); i++){
					for(int j=0; j<open[0].size();j++){
						std::cout << open[i][j] << " ";
					}
					std::cout << std::endl;
				}
				std::cout << std::endl;
			}
*/
			std::vector<int> next = open.back();
			open.pop_back();
			
			int f = next[0];
			int g = next[1];
			int h = next[2];
			int x = next[3];
			int y = next[4];
			//cout << "cout " << count << endl;
			//cout << "x2: " << x << ", y2: " << y << endl << endl;;

			pthread_mutex_lock(&mutex);
			expand_[x][y] = count;
			pthread_mutex_unlock(&mutex);
			count += 1;
			//cout << "goal_[0] is " << goal_[0] << "goal_[1] is " << goal_[1];
			if ((x == goal[0] && y == goal[1])) {
				found = true;
			}
			else {
				for (unsigned int i = 0; i < delta_.size(); i++) {
					//cout << "print result " << print_result << endl;
					int x2 = x + delta_[i][0];
					int y2 = y + delta_[i][1];
					if (x2 >= 0 && x2 < grid.size() && y2 >= 0 && y2 < grid[0].size()) {
						if (closed[x2][y2] == 0 && grid[x2][y2] == 0) {
							int g2 = g + cost_;
							int h2 = heuristics[x2][y2];
							int	f2 = g2 + h2;
							open.push_back({ f2, h2, g2, x2, y2 });
							//pthread_mutex_lock(&mutex);
							
							//pthread_mutex_unlock(&mutex);
							//cout << print_result << ": " << expand_[x2][y2]  << " count: " << count << endl;
							if(expand_[x2][y2]!=OBSTACLE &&
								closed[x2][y2] != 1) {
									//cout << "over x2: " << x2 << " y2: " << y2 << endl;
									//cout << "expand: " << expand_[x2][y2] << endl;
									found=true;
							}
							closed[x2][y2] = 1;
							
							
						}
						else
						{
							if (grid[x2][y2] == 1)
							{
								pthread_mutex_lock(&mutex);
								expand_[x2][y2] = -1;
								pthread_mutex_unlock(&mutex);
							}
						}
					}
				}
			}

		}

		if(found==true) cout << count << " found goal!" << endl;
	}
/*
	if(print_result==1){
		for(int i=0; i<closed_.size(); i++){
			for(int j=0; j<closed_[0].size(); j++){
				cout << setw(4) << expand[i][j] << " ";
			}
			cout << endl;
		}
	}
*/

	//pthread_exit((void*)&expand);


}

void astar::read_map(){
	ifstream fin( "map.txt" );

  vector< vector< int > > datas; //2 維陣列

  int tmp; //暫存
  istringstream iline; //一行的資料流
  string line; //一行的資料
  while( getline( fin, line ) ){

    grid_.push_back( vector<int>() );
    iline.str( line );

    while( iline >> tmp ){
      grid_.rbegin()->push_back( tmp );
    }

    iline.clear();

  }
  //資料存進 datas 二維陣列中了

  //印出資料
	/*
  int i, j;
  for( i=0 ; i<grid_.size() ; ++i ){
    for( j=0 ; j<grid_[i].size() ; ++j ){
      cout << grid_[i][j] << " ";
    }
    cout << endl;
  }
*/
  fin.close();

}
