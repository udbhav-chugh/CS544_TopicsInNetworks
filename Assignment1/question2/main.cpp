#include<bits/stdc++.h>
using namespace std;

#define ld long double
#define ll long long

//include all cases and server files
#include "Server.cpp"
#include "CaseA.cpp"
#include "CaseB.cpp"
#include "CaseC.cpp"



int main(int argc, char *argv[]){
	char option;		
	ll caseNum = 1;		//default case to execute is 1	
	ld arrivalRate = 0.25, serviceRate = 0.3; // set default values of arrival and service rate if none given
	ll totalWorkers = 1000000;
	//update case number from input
	if(argc >= 2){
		caseNum = atoll(argv[1]);
	}
	//update arrival rate from input
	if (argc >= 3){
		arrivalRate = atof(argv[2]);
	}
	//update service rate from input
	if (argc >= 4){
		serviceRate = atof(argv[3]);
	}
	//if no service rate give, for case 3 update service rate s.t. blocking probablity = 0.02
	else if(caseNum == 3){
		serviceRate = arrivalRate / 0.60524;
	}
	//check if arrival rate and service rate are positive
	if(arrivalRate <= 0){
		cout<<"Arrival Rate must be a positive number"<<endl;
		exit(0);
	}
	if(serviceRate <= 0){
		cout<<"Service Rate must be a positive number"<<endl;
		exit(0);
	}
	
	switch(caseNum){
		//call caseA 
		case 1: {
					CaseA objectA(arrivalRate, serviceRate, totalWorkers); 
					objectA.simulate();
					break;
				}
		//call CaseB
		case 2: {
					CaseB objectB(arrivalRate, serviceRate, totalWorkers);
					objectB.simulate();
					break;
				}
		//call CaseC
		case 3: {
					CaseC objectC(arrivalRate, serviceRate, totalWorkers);
					objectC.simulate();
					break;
				}
		//invalid case number as input
		default: cout<<"Second argument must be 1,2 or 3"<<endl;break;
	}
}