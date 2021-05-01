#include "cache.h"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <cmath>
using namespace std;

Cache::Cache(){
}

struct entry{
	long long tag;
	long long valid; 
	long long access;
};

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
        int offset = (int)(log2(sizes)+5);
        for(int i =0;i<address.size();i++){
		//figure out if its a hit or not
		//find address and indeex
		int block =floor( address[i]/32);
		int index = block%sizes;	
		//cache line size 32 = 5 bits
		//log2(sizes)
               // int offset = (int)log2(sizes)+5;
        	int tag = address[i]>>offset;
		if(DMcache[index][0]==1 && DMcache[index][1] == tag){
			hits++;

		}else{
	
			DMcache[index][0] =1;
			DMcache[index][1] = tag;
		}
	}


	return hits;
}
long Cache:: setAssociative(int ways){
	long hits = 0;
        //figure out number of sets
	int sets = 512/ways;	
	entry SAcache[sets][ways]; 
	long long lru [sets][ways]; //keep track of the count
	for(int i=0;i<sets;i++){
		for(int j=0;j<ways;j++){
			SAcache[i][j].tag = -1;
			SAcache[i][j].valid = 0;
			SAcache[i][j].access = 0;
			lru[i][j] = 0;
		}
	}
        int offset = (int) (log2(sets) + 5);
 	for(int i =0;i<address.size();i++){
		int block = floor(address[i]/32);
		int index = block%sizes;
		int tag = address[i]>>offset;
		bool found = false
		//traverse through each way in the in the set
		for(int j = 0;j<ways;j++){
			if(SAcache[index][j].tag == tag && SAcache[index][j].valid == 1){
				//found a hit
				hits++;
				SAcache[index][j].access++;
				found = true;
				
			}	
		}
		//bring data in cache	
	}   	
	

	

	return hits;
}
