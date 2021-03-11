//class for Case A -> Two M/M/1 Queue systems
class CaseA{

	ld arrivalRate, serviceRate; //input arrival and service rate
	ll totalWorkers; // total workers to simulate on

	ld totalResponseTime[2]; // total response time for each queue system
	ld totalTime[2]; // total time for each queue system

	ld totalServerTime[2], totalWaitingTime[2]; // total checking time in server and waiting time in queue

	vector<ld> arrivalTime[2], serviceEnterTime[2], leavingTime[2]; // for each worker, store its arrival, service entering and leaving time

	Server *servers[2]; //two servers, 1 for each queue

public:
	//intialise variables
	CaseA(ld arrivalRate, ld serviceRate, ll totalWorkers){

		this -> arrivalRate = arrivalRate;
		this -> serviceRate = serviceRate;
		this -> totalWorkers = totalWorkers;

		for(int i=0;i<2;i++){
			servers[i] = new Server(this -> serviceRate);
			totalResponseTime[i] = 0;
			totalWaitingTime[i] = 0;
			totalTime[i] = 0;
			totalServerTime[i] = 0;
		}
	}

	void simulate(){
		if(arrivalRate >= serviceRate){
			cout<<"The system is unstable as arrival rate  >= service rate, so queue size will tend to go to inifinte. Exiting program"<<endl;
			exit(0);
		}

		//exponential input generator
		default_random_engine generator (time(0));
		exponential_distribution<double> distributionArrival (this -> arrivalRate);
		exponential_distribution<double> distributionServer (this -> serviceRate);


		//find out arrivals for workers in each queue
		for(int i=0;i<2;i++){
			ld curArrivalTime = 0;
			for(int j=0;j<totalWorkers;j++){
				curArrivalTime += distributionArrival(generator);
				arrivalTime[i].push_back(curArrivalTime);
				serviceEnterTime[i].push_back(0);
				leavingTime[i].push_back(0);
			}
		}
		
		// merge arrivals into a single vector sorted by time of arrival
		vector<pair<int,int> > overallArrival;
		int index0=0,index1=0;
		while(index0<totalWorkers && index1<totalWorkers){
			if(arrivalTime[0][index0]<=arrivalTime[1][index1]){
				pair<int,int> arrivalValue(0, index0++);
				overallArrival.push_back(arrivalValue);
			}
			else{
				pair<int,int> arrivalValue(1, index1++);
				overallArrival.push_back(arrivalValue);	
			}
		}
		while(index0<totalWorkers){
			pair<int,int> arrivalValue(0, index0++);			
			overallArrival.push_back(arrivalValue);
		}
		while(index1<totalWorkers){
			pair<int,int> arrivalValue(1, index1++);			
			overallArrival.push_back(arrivalValue);	
		}
		int arrivalIndex = 0;
		queue<int> waitingQueue[2];
		//start simulation
		while(arrivalIndex < overallArrival.size() || waitingQueue[0].size() > 0 || waitingQueue[1].size() > 0){
			ld curArrival = LDBL_MAX;
			int type=0;
			int index=0;
			//check if any arrival is still pending
			if(arrivalIndex < overallArrival.size()){
				type = overallArrival[arrivalIndex].first;
				index = overallArrival[arrivalIndex].second;
				curArrival = arrivalTime[type][index];
			}

			//Arrival Event for a worker
			if(arrivalIndex < overallArrival.size() && (curArrival < servers[type] -> getNextFreeTime() || waitingQueue[type].size() == 0)){
				waitingQueue[type].push(index);
				arrivalIndex++;
			}
			int traverse[2] = {0,1};
			if(servers[0] -> getNextFreeTime() > servers[1] -> getNextFreeTime()){
				traverse[0]=1;traverse[1]=0;
			}
			//Servicing and Leaving Event for a worker
			for(int i=0;i<2;i++){
				int curtype = traverse[i];
				if( servers[curtype] -> getNextFreeTime() <= curArrival){
					if(waitingQueue[curtype].size() == 0){
						continue;
					}
					ld curQueueSize = waitingQueue[curtype].size();

					//get element at head of queue
					int headIndex = waitingQueue[curtype].front();

					waitingQueue[curtype].pop();
					ld serviceStartTime = max(arrivalTime[curtype][headIndex], servers[curtype] -> getNextFreeTime());	

					ld serviceTime = distributionServer(generator); // find service time using exponential distribution
					
					ld serviceEndTime = serviceStartTime + serviceTime;
					// update service start and end time for the worker
					serviceEnterTime[curtype][headIndex] = serviceStartTime; 
					leavingTime[curtype][headIndex] = serviceEndTime;
					servers[curtype] -> setNextFreeTime(serviceEndTime);
					totalServerTime[curtype] += serviceTime;
				}
			}
		}
		//simulation completed
		//output on console as well as file
		ofstream fil("OutputCaseA.txt");
		cout<<"Given:"<<endl;
		fil<<"Given:"<<endl;
		cout<<"For each queue, mean arrival rate is "<<arrivalRate<<" workers per second with exponential distribution"<<endl;
		fil<<"For each queue, mean arrival rate is "<<arrivalRate<<" workers per second with exponential distribution"<<endl;
		cout<<"For each officer, mean service rate is "<<serviceRate<<" workers per second with exponential distribution"<<endl;
		fil<<"For each officer, mean service rate is "<<serviceRate<<" workers per second with exponential distribution"<<endl;
		cout<<"This is a case of two M/M/1 queue systems"<<endl<<endl;
		fil<<"This is a case of two M/M/1 queue systems"<<endl<<endl;

		for(int i=0;i<2;i++){
			totalTime[i] = servers[i] -> getNextFreeTime();
			for(int j=0;j<arrivalTime[i].size();j++){
				totalResponseTime[i] += leavingTime[i][j] - arrivalTime[i][j];
				totalWaitingTime[i] += serviceEnterTime[i][j] - arrivalTime[i][j];
			}
			//calculate required parameters after simulation
			ld averageWorkersGettingChecked = totalServerTime[i] / totalTime[i];
			ld averageResponseTime = totalResponseTime[i] / totalWorkers;
			ld averageWaitingTime = totalWaitingTime[i] / totalWorkers;
			ld averageWorkersInQueue = totalWaitingTime[i] / totalTime[i];
			ld averageWorkersInSystem = totalResponseTime[i] / totalTime[i];
			cout<<"For Queue "<<i+1<<": "<<endl;
			fil<<"For Queue "<<i+1<<": "<<endl;
			cout<<"a) Average number of workers getting checked: "<<averageWorkersGettingChecked<<" workers"<<endl;
			fil<<"a) Average number of workers getting checked: "<<averageWorkersGettingChecked<<" workers"<<endl;
			cout<<"b) Average response time for workers in getting checked: "<<averageResponseTime<<" seconds"<<endl;
			fil<<"b) Average response time for workers in getting checked: "<<averageResponseTime<<" seconds"<<endl;
			cout<<"c) Average time for which a worker has to wait until getting checked: "<<averageWaitingTime<<" seconds"<<endl;
			fil<<"c) Average time for which a worker has to wait until getting checked: "<<averageWaitingTime<<" seconds"<<endl;
			cout<<"d) Average number of workers waiting in the queue before each officer: "<<averageWorkersInQueue<<" workers"<<endl;
			fil<<"d) Average number of workers waiting in the queue before each officer: "<<averageWorkersInQueue<<" workers"<<endl;
			cout<<"Additonally, Average number of workers in the system (getting checked + in queue): "<<averageWorkersInSystem<<" workers"<<endl;
			fil<<"Additonally, Average number of workers in the system (getting checked + in queue): "<<averageWorkersInSystem<<" workers"<<endl;
			cout<<endl;
			fil<<endl;
		}
		fil.close();
	
	}
};