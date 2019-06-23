#include <fstream>
#include <iostream>
#include <string>
#include <ios>
#include <vector>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int max_w, max_h, tot_size, tot_num, obj, x_dis, y_dis;

#if 0
Image *read(const char *filename)
{
    std::ifstream bmp(filename, std::ios::binary);
    char header[54];
    bmp.read(header, 54);
    uint32_t size = *(int *)&header[2];
    uint32_t offset = *(int *)&header[10];
    uint32_t w = *(int *)&header[18];
    uint32_t h = *(int *)&header[22];
    uint16_t depth = *(uint16_t *)&header[28];
    if (depth != 24 && depth != 32)
    {
        printf("we don't suppot depth with %d\n", depth);
        exit(0);
    }
    bmp.seekg(offset, bmp.beg);

    Image *ret = new Image();
    ret->type = 1;
    ret->height = h;
    ret->weight = w;
    ret->size = w * h;
    ret->data = new RGB[w * h];
    for (int i = 0; i < ret->size; i++)
    {
        bmp.read((char *)&ret->data[i], depth / 8);
    }
    return ret;
}
#endif

int write(ofstream &fout, vector<vector<int> > vec)
{    
	char str[2];
	int tmp = 0;
    //std::ofstream fout;
    //fout.open(filename, std::ios::binary);
	for (int j =0; j < max_h; ++j) {
		for (int i =0; i < max_w; ++i) {
			tmp = vec[j][i];			
			//itoa(tmp, str, 10);
			snprintf(str, sizeof(str), "%d", tmp);
			str[1] = ' ';
			fout.write((char*)str, 2);
		}
		str[0] = '\n';
		fout.write((char*)str, 1);
	}
	str[0] = '\n';
	fout.write((char*)str, 1);
    //fout.close();
}

void func(ofstream &fout) 
{
	vector <vector<int> > array(max_h);
	int obstacle = ((int)sqrt(tot_size)) / 2;
    int centor_w, centor_h, long_w, long_h, type, now_w, now_h, x, y, x_x, y_y; 
	if (x_dis == 0)
		x_x = 2 + obstacle / 10;
	else
		x_x = x_dis;
	if (y_dis == 0)
		y_y = obstacle / 5;
	else
		y_y = y_dis;
	
	for (int i = 0; i < max_h; ++i) {
		array[i].resize(max_w);
		array[i].assign(max_w, 0);
	}		
		
	/*type :
	  0單點 
	  1正T 2右長T 3倒T 4左長T 
	  5上短下長十字架 6上長下短十字架
	  7小橫條 8中橫條 9大橫條
	  10小直條 11中直條 12大直條 			  
	  13二乘二 14三乘三 15四乘四
	  */	
	now_w = 0; 
	now_h = 0;  
	
	//ofstream fout;
	//fout.open("map.txt", ios::binary);
#if 0	
    for(int i = 0; i < obstacle * 2; ++i) {
#else  /*19-6-16*/
	for(int i = 0; i < obj; ++i) {
#endif	
        //res_w = rand() % (up_w - low + 1) + low; 
		//res_h = rand() % (up_h - low + 1) + low;
		type = rand() % 16;
		centor_w = rand() % (max_w / obstacle) + now_w;
		centor_h = rand() % (max_h / obstacle) + now_h;
		if (centor_w > max_w - 1) 
			centor_w = max_w - 1;
		if (centor_h > max_h - 1) 
			centor_h = max_h - 1;
		x = 0;
		y = 0;
		switch (type) {
			case 0:
				array[centor_h][centor_w] = 1;
			break;
			case 1:
				array[centor_h][centor_w] = 1;
				
				for (int i = 1; i <= 2; ++i) {
					if (centor_w - i >= 0)
						array[centor_h][centor_w - i] = 1;	
					
					if (centor_w + i <= max_w - 1) {
						array[centor_h][centor_w + i] = 1;
						x = i;
					}
				}
				
				for (int i = 1; i <= 6; ++i) {
					if (centor_h + i <= max_h - 1) {
						array[centor_h + i][centor_w] = 1;
						if (i > y)
						y = i;
					}
				}
			break;
			case 2:
				if (centor_h + 2 <= max_h - 1)
					centor_h += 2; 
				
				array[centor_h][centor_w] = 1;
				
				for (int i = 1; i <= 2; ++i) {
					if (centor_h - i >= 0)
						array[centor_h - i][centor_w] = 1;	
					
					if (centor_h + i <= max_h - 1) {
						array[centor_h + i][centor_w] = 1;
						if (i > y)
						y = i;
					}
				}
				
				for (int i = 1; i <= 6; ++i) {
					if (centor_w + i <= max_w - 1) {
						array[centor_h][centor_w + i] = 1;
						x = i;
					}
				}
			break;
			case 3:
				if (centor_h + 6 <= max_h - 1)
					centor_h += 6; 
			
				array[centor_h][centor_w] = 1;
	
				for (int i = 1; i <= 2; ++i) {
					if (centor_w - i >= 0)
						array[centor_h][centor_w - i] = 1;	
					
					if (centor_w + i <= max_w - 1) {
						array[centor_h][centor_w + i] = 1;
						x = i;
					}
				}
				
				for (int i = 1; i <= 6; ++i) {
					if (centor_h - i >= 0) 
						array[centor_h - i][centor_w] = 1;
				}
			break;
			case 4:
				if (centor_h + 2 <= max_h - 1)
					centor_h += 2; 
			
				array[centor_h][centor_w] = 1;
				
				for (int i = 1; i <= 2; ++i) {
					if (centor_h - i >= 0)
						array[centor_h - i][centor_w] = 1;	
					
					if (centor_h + i <= max_h - 1) {
						array[centor_h + i][centor_w] = 1;
						if (i > y)
						y = i;
					}
				}
				
				for (int i = 1; i <= 6; ++i) {
					if (centor_w - i >= 0) 
						array[centor_h][centor_w - i] = 1;
				}
			break;
			case 5:
				if (centor_h + 2 <= max_h - 1)
					centor_h += 2; 
			
				array[centor_h][centor_w] = 1;
				
				for (int i = 1; i <= 2; ++i) {
					if (centor_h - i >= 0)
						array[centor_h - i][centor_w] = 1;	
					
					if (centor_w - i >= 0) 
						array[centor_h][centor_w - i] = 1;
					
					if (centor_w + i <= max_w - 1) {
						array[centor_h][centor_w + i] = 1;
						x = i;
					}
				}
				
				for (int i = 1; i <= 4; ++i) {
					if (centor_h + i <= max_h - 1) {
						array[centor_h + i][centor_w] = 1;
						if (i > y)
						y = i;
					}
				}
			break;
			case 6:
				if (centor_h + 4 <= max_h - 1)
					centor_h += 4; 
				
				array[centor_h][centor_w] = 1;
				
				for (int i = 1; i <= 2; ++i) {
					if (centor_h + i <= max_h - 1) {
						array[centor_h + i][centor_w] = 1;
						if (i > y)
						y = i;
					}
					
					if (centor_w - i >= 0) 
						array[centor_h][centor_w - i] = 1;
					
					if (centor_w + i <= max_w - 1) {
						array[centor_h][centor_w + i] = 1;
						x = i;
					}
				}
				
				for (int i = 1; i <= 4; ++i) {					
					if (centor_h - i >= 0)
						array[centor_h - i][centor_w] = 1;	
				}
			break;
			/*type :
			  0單點 
			  1正T 2右長T 3倒T 4左長T 
			  5上短下長十字架 6上長下短十字架
			  7小橫條 8中橫條 9大橫條
			  10小直條 11中直條 12大直條 			  
			  13二乘二 14三乘三 15四乘四
			  */	
			case 7:					
				array[centor_h][centor_w] = 1;
				
				for (int i = 1; i <= 1; ++i) {
					if (centor_w - i >= 0)
						array[centor_h][centor_w - i] = 1;	
					
					if (centor_w + i <= max_w - 1) {
						array[centor_h][centor_w + i] = 1;
						x = i;
					}
				}
			break;
			case 8:
				array[centor_h][centor_w] = 1;
				
				for (int i = 1; i <= 2; ++i) {
					if (centor_w - i >= 0)
						array[centor_h][centor_w - i] = 1;	
					
					if (centor_w + i <= max_w - 1) {
						array[centor_h][centor_w + i] = 1;
						x = i;
					}
				}
			break;
			case 9:
				array[centor_h][centor_w] = 1;
				
				for (int i = 1; i <= 4; ++i) {
					if (centor_w - i >= 0)
						array[centor_h][centor_w - i] = 1;	
					
					if (centor_w + i <= max_w - 1) {
						array[centor_h][centor_w + i] = 1;
						x = i;
					}
				}
			break;
			case 10:
				if (centor_h + 1 <= max_h - 1)
					centor_h += 1; 
				
				array[centor_h][centor_w] = 1;

				for (int i = 1; i <= 1; ++i) {
					if (centor_h - i >= 0)
						array[centor_h - i][centor_w] = 1;	
					
					if (centor_h + i <= max_h - 1) {
						array[centor_h + i][centor_w] = 1;
						if (i > y)
						y = i;
					}
				}
			break;
			case 11:
				if (centor_h + 2 <= max_h - 1)
					centor_h += 2; 
				
				array[centor_h][centor_w] = 1;
				
				for (int i = 1; i <= 2; ++i) {
					if (centor_h - i >= 0)
						array[centor_h - i][centor_w] = 1;	
					
					if (centor_h + i <= max_h - 1) {
						array[centor_h + i][centor_w] = 1;
						if (i > y)
						y = i;
					}
				}
			break;
			case 12:
				if (centor_h + 4 <= max_h - 1)
					centor_h += 4; 
				
			    array[centor_h][centor_w] = 1;
				
				for (int i = 1; i <= 4; ++i) {
					if (centor_h - i >= 0)
						array[centor_h - i][centor_w] = 1;	
					
					if (centor_h + i <= max_h - 1) {
						array[centor_h + i][centor_w] = 1;
						if (i > y)
						y = i;
					}
				}
			break;
			/*type :
			  0單點 
			  1正T 2右長T 3倒T 4左長T 
			  5上短下長十字架 6上長下短十字架
			  7小橫條 8中橫條 9大橫條
			  10小直條 11中直條 12大直條 			  
			  13二乘二 14三乘三 15四乘四
			  */	
			case 13:
				array[centor_h][centor_w] = 1;
								
				if (centor_w + 1 <= max_w - 1) {
					array[centor_h][centor_w + 1] = 1;	
					x = 1;
				}
				if (centor_h + 1 <= max_h - 1) {
					array[centor_h + 1][centor_w] = 1;
					if (i > y)
					y = 1;
					if (x > 0)
						array[centor_h + 1][centor_w + 1] = 1;
				}				
			break;
			case 14:
				array[centor_h][centor_w] = 1;
								
				for (int i = 1; i <= 2; ++i)				
					if (centor_w + i <= max_w - 1) {
						array[centor_h][centor_w + i] = 1;	
						x = i;
					}
				for (int i = 1; i <= 2; ++i)	
					if (centor_h + i <= max_h - 1) {
						array[centor_h + i][centor_w] = 1;
						if (i > y)
						y = i;
						if (x > 0)
							array[centor_h + i][centor_w + 1] = 1;
						if (x > 1)
							array[centor_h + i][centor_w + 2] = 1;
					}				
			break;
			case 15:
				array[centor_h][centor_w] = 1;
								
				for (int i = 1; i <= 3; ++i)				
					if (centor_w + i <= max_w - 1) {
						array[centor_h][centor_w + i] = 1;	
						x = i;
					}
				for (int i = 1; i <= 3; ++i)	
					if (centor_h + i <= max_h - 1) {
						array[centor_h + i][centor_w] = 1;
						if (i > y)
						y = i;
						if (x > 0)
							array[centor_h + i][centor_w + 1] = 1;
						if (x > 1)
							array[centor_h + i][centor_w + 2] = 1;
						if (x > 2)
							array[centor_h + i][centor_w + 3] = 1;
					}				
			break;
		}
		if (centor_w < max_w - 4)
			now_w = centor_w + x_x + x; 
		else {
			now_w = 0; 
			now_h = centor_h + y_y + y;
		}	
	}
	
	array[0][max_w - 1] = 0;
	array[0][max_w - 1] = 0;
	array[1][max_w - 1] = 0;
	array[max_h - 1][0] = 0;
	array[max_h - 1][0] = 0;
	array[max_h - 1][1] = 0;
	
	write(fout, array);		

	//fout.close();
}

int main(int argc, char *argv[])
{
    if (argc == 7)
    {
		max_w = atoi(argv[1]); 
		max_h = atoi(argv[2]); 
		tot_size = max_w * max_h;
		tot_num = atoi(argv[3]);
		obj = atoi(argv[4]);
		x_dis = atoi(argv[5]);
		y_dis = atoi(argv[6]);
        		
		if (max_w < 5 || max_h < 5 || obj < 1 || x_dis < 0 || y_dis < 0) {
			printf("Usage: ./gen <width> <height> <number> <object> <x-x> <y-y> \
			(w & h must >= 5, obj  must >= 1, x-x & y-y must >= 0.)\n");
			return 0;
		}
		
		unsigned seed;
		seed = (unsigned)time(NULL);
		srand(seed);
		
		ofstream fout;
		fout.open("map.txt", ios::binary);
		
		for (int t = 0; t < tot_num; ++t) 
			func(fout);
		
		fout.close();

		/*
		vector <vector<int> > array(max_h, vector<int>(max_x, 0));
		
		vector<vector<vector<float>  >  > grdarray(NZ,vector<vector<float> >(NY,vector<float>(NX,0)));	
		*/
	
    }else{
        printf("Usage: ./gen <width> <height> <number> <object> <x-x> <y-y> \
			(w & h must >= 5, obj  must >= 1, x-x & y-y must >= 0.)\n");
    }
    return 0;
}
