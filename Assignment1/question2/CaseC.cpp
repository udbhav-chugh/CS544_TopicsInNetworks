class CaseC{

	ld arrivalRate, serviceRate; 
	ll totalWorkers;

	ld totalResponseTime[2];
	ld totalTime[2];

	ld totalServerTime[2], totalWaitingTime[2];

	vector<ld> arrivalTime[2], serviceEnterTime[2], leavingTime[2];

	Server *servers[2];
	vector<bool> droppedPackets[2];

public:
	CaseC(ld arrivalRate, ld serviceRate, ll totalWorkers){

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
		
		default_random_engine generator (time(0));
		exponential_distribution<double> distributionArrival (this -> arrivalRate);
		exponential_distribution<double> distributionServer (this -> serviceRate);

		for(int i=0;i<2;i++){
			ld curArrivalTime = 0;
			for(int j=0;j<totalWorkers;j++){
				curArrivalTime += distributionArrival(generator);
				arrivalTime[i].push_back(curArrivalTime);
				leavingTime[i].push_back(0);
				serviceEnterTime[i].push_back(0);
				droppedPackets[i].push_back(0);
			}
		}
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
		while(arrivalIndex < overallArrival.size() || waitingQueue[0].size() > 0 || waitingQueue[1].size() > 0){
			ld curArrival = LDBL_MAX;
			int type=0;
			int index=0;
			if(arrivalIndex < overallArrival.size()){
				type = overallArrival[arrivalIndex].first;
				index = overallArrival[arrivalIndex].second;
				curArrival = arrivalTime[type][index];
			}

			//Arrival Event
			if(arrivalIndex < overallArrival.size() && (curArrival < servers[type] -> getNextFreeTime() || waitingQueue[type].size() == 0)){
				if(waitingQueue[type].size() < 5){
					waitingQueue[type].push(index);
				}
				//packet dropped
				else{
					droppedPackets[type][index] = 1;
				}
				arrivalIndex++;	
			}
			int traverse[2] = {0,1};
			if(servers[0] -> getNextFreeTime() > servers[1] -> getNextFreeTime()){
				traverse[0]=1;traverse[1]=0;
			}
			//Servicing and Leaving Event
			for(int i=0;i<2;i++){
				int curtype = traverse[i];
				if( servers[curtype] -> getNextFreeTime() <= curArrival){
					if(waitingQueue[curtype].size() == 0){
						continue;
					}
					ld curQueueSize = waitingQueue[curtype].size();

					int headIndex = waitingQueue[curtype].front();

					waitingQueue[curtype].pop();
					ld serviceStartTime = max(arrivalTime[curtype][headIndex], servers[curtype] -> getNextFreeTime());	

					ld serviceTime = distributionServer(generator);
					
					ld serviceEndTime = serviceStartTime + serviceTime;
					serviceEnterTime[curtype][headIndex] = serviceStartTime;
					leavingTime[curtype][headIndex] = serviceEndTime;
					servers[curtype] -> setNextFreeTime(serviceEndTime);
					totalServerTime[curtype] += serviceTime;
				}
			}
		}
		for(int i=0;i<2;i++){
			totalTime[i] = servers[i] -> getNextFreeTime();
			ll totalWorkersEntering = 0;
			for(int j=0;j<arrivalTime[i].size();j++){
				if(droppedPackets[i][j] == 1){
					continue;
				}
				totalWorkersEntering++;
				totalResponseTime[i] += leavingTime[i][j] - arrivalTime[i][j];
				totalWaitingTime[i] += serviceEnterTime[i][j] - arrivalTime[i][j];
			}
			ld averageWorkersGettingChecked = totalServerTime[i] / totalTime[i];
			ld averageResponseTime = totalResponseTime[i] / totalWorkersEntering;
			ld averageWaitingTime = totalWaitingTime[i] / totalWorkersEntering;
			ld averageWorkersInQueue = totalWaitingTime[i] / totalTime[i];
			ld averageWorkersInSystem = totalResponseTime[i] / totalTime[i];
			cout<<"For Counter "<<i+1<<": "<<endl;
			cout<<"Average number of workers getting checked: "<<averageWorkersGettingChecked<<endl;
			cout<<"Average response time for workers in getting checked: "<<averageResponseTime<<endl;
			cout<<"Average time for which a worker has to wait until getting checked: "<<averageWaitingTime<<endl;
			cout<<"Average number of workers waiting in the queue before each officer: "<<averageWorkersInQueue<<endl;
			cout<<"Average number of workers in the system (getting checked + in queue): "<<averageWorkersInSystem<<endl;
			cout<<endl;
		}

	
	}
};