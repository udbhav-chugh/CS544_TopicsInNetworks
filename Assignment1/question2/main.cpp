#include<bits/stdc++.h>
using namespace std;

#define ld long double
#define ll long long

#include "Server.cpp"
#include "CaseA.cpp"
#include "CaseB.cpp"
#include "CaseC.cpp"



int main(int argc, char *argv[]){
	char option;		
	ll caseNum = 1;			
	ld arrivalRate = 15, serviceRate = 25;
	ll totalWorkers = 1000000;
	if(argc >= 2){
		caseNum = atoll(argv[1]);
	}
	if (argc >= 3){
		arrivalRate = atof(argv[2]);
	}
	if (argc >= 4){
		serviceRate = atof(argv[3]);
	}
	else if(caseNum == 3){
		serviceRate = arrivalRate / 0.52934;
	}
	if (argc >= 5){
		totalWorkers = atoll(argv[4]);
	}
	// cout<<caseNum<<" "<<arrivalRate<<" "<<serviceRate<<" "<<totalWorkers<<" "<<simulationTime<<endl;
	
	switch(caseNum){
		case 1: {
					CaseA objectA(arrivalRate, serviceRate, totalWorkers); 
					objectA.simulate();
					break;
				}
		case 2: {
					CaseB objectB(arrivalRate, serviceRate, totalWorkers);
					objectB.simulate();
					break;
				}
		case 3: {
					CaseC objectC(arrivalRate, serviceRate, totalWorkers);
					objectC.simulate();
					break;
				}
		default: cout<<"Second argument must be 1,2 or 3"<<endl;break;
	}
}