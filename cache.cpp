#include "cache.h"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <cmath>
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
	long long DMcache [sizes][2]; //one entry to store valid bit , one entry to store tag
	for(int i = 0; i<sizes;i++){
		for(int j=0;j<2;j++){
			DMcache[i][j] = 0;
		}
	}
        for(int i =0;i<address.size();i++){
		//figure out if its a hit or not
		//find address and indeex
		int block =floor( address[i]/32);
		int index = block / sizes;	
		//cache line size 32 = 5 bits
		//log2(sizes)
        	int tag = address[i]>>((int)log2(sizes)+5);
		if(DMcache[index][0]==1 && DMcache[index][1] == tag){
			hits++;

		}else{
	
			DMcache[index][0] =1;
			DMcache[index][1] = tag;
		}
	}


	return hits;
}

