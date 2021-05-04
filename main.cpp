#include  <iostream>
#include <vector>
#include<fstream>
#include<string>
#include "cache.h"
using namespace std;

int main(int argc, char*argv[]){

	vector<long long>address;
	vector<char>instruction;
         unsigned long long addr;
 	 char instr;
	 ifstream infile(argv[1]);
	while(infile >>instr >> std::hex>> addr) {
		instruction.push_back(instr);
		address.push_back(addr);
        }
	int  sizes[] = {32,128,512,1024};
	int assoc [] = {2,4,8,16};
	Cache cache(instruction, address);
	ofstream myfile;
	myfile.open(argv[2]);
	
	//direct mapped
	for(auto &i: sizes){
		myfile<<cache.directMapped(i)<<","<<address.size()<<"; ";
	}
	myfile<<"\n";	
	for(auto &i: assoc){
                myfile<<cache.setAssociative(i)<<","<<address.size()<<"; ";
        }
	myfile<<"\n";	
	myfile<<cache.setAssociative(512)<<","<<address.size()<<"; ";
	myfile<<"\n";
	myfile<<cache.hotColdLRU()<<","<<address.size()<<"; ";
	myfile<<"\n";
	for(auto &i: assoc){
                myfile<<cache.noAlloc(i)<<","<<address.size()<<"; ";
        }
	myfile<<"\n";
	for(auto &i: assoc){
                myfile<<cache.nextLine(i)<<","<<address.size()<<"; ";
        }
	myfile<<"\n";
	for(auto &i: assoc){
                myfile<<cache.preFetchMiss(i)<<","<<address.size()<<"; ";
        }


}
