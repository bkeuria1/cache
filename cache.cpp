#include "cache.h"
#include <iostream>

using namespace std;

Cache::Cache(){
}

Cache::Cache(vector<char>_instruction,vector<long long>_address):instruction(_instruction), address(_address){}

long Cache:: directMapped(int sizes){
	long hits = 0;
	//2^5 = 32 
	//Block address mode number of block in cache
	//Since 32 bits, 3
	//Cache size = Number of sets * Number of ways * Block size 
	//need to store tag and data in table
        //calculate number of blocks
	long long DMcache [sizes][2]; //one entry to store tage, one entry to store tag
	for(int i = 0; i<sizes;i++){
		for(int j=0;j<2;j++){
			DMcache[i][j] = 0;
		}
	}



	return hits;
}

