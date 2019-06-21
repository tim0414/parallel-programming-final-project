/*
https://www.ptt.cc/bbs/C_and_CPP/M.1326016730.A.0D8.html

剛寫了一個讀檔程式

input.txt 如下

1 2 3
2 3 4 5
3 4
5 -6 7 8

想要存成一個 vector< vector< int > >

我想到的寫法是這樣

*/

#include<iostream>
#include<fstream>
#include<sstream>

#include<string>

#include<vector>

using namespace std;

int main(){

  ifstream fin( "map.txt" );

  vector< vector< int > > datas; //2 維陣列

  int tmp; //暫存
  istringstream iline; //一行的資料流
  string line; //一行的資料
  while( getline( fin, line ) ){

    datas.push_back( vector<int>() );
    iline.str( line );

    while( iline >> tmp ){
      datas.rbegin()->push_back( tmp );
    }

    iline.clear();

  }
  //資料存進 datas 二維陣列中了

  //印出資料
  int i, j;
  for( i=0 ; i<datas.size() ; ++i ){
    for( j=0 ; j<datas[i].size() ; ++j ){
      cout << datas[i][j] << " ";
    }
    cout << endl;
  }

  fin.close();

  return 0;

}
