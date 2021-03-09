class CaseA{

	ld arrivalRate, serviceRate; 
	ll totalWorkers;

	ld totalResponseTime;
	ld totalTime;

	ld totalServerTime, totalWaitingTime;

	vector<ld> arrivalTime, serviceEnterTime, leavingTime;

	Server *server;

public:
	CaseA(ld arrivalRate, ld serviceRate, ll totalWorkers){

		this -> arrivalRate = arrivalRate;
		this -> serviceRate = serviceRate;
		this -> totalWorkers = totalWorkers;

		server = new Server(this -> serviceRate);

		totalResponseTime = 0;
		totalWaitingTime = 0;
		totalTime = 0;
		totalServerTime = 0;
	}

	void simulate(){
		if(arrivalRate >= serviceRate){
			cout<<"The system is unstable. Exiting program"<<endl;
			exit(0);
		}

		default_random_engine generator (time(0));
		exponential_distribution<double> distributionArrival (this -> arrivalRate);
		exponential_distribution<double> distributionServer (this -> serviceRate);



		ld curArrivalTime = 0;
		ld sum=0;
		for(int j=0;j<totalWorkers;j++){
			curArrivalTime += distributionArrival(generator);
			arrivalTime.push_back(curArrivalTime);
		}
		int arrivalIndex = 0;
		queue<int> waitingQueue;

		while(arrivalIndex < arrivalTime.size() || waitingQueue.size() > 0){
			ld curArrival = LDBL_MAX;
			if(arrivalIndex < arrivalTime.size()){
				curArrival = arrivalTime[arrivalIndex];
			}

			//Arrival Event
			if(curArrival < server -> getNextFreeTime() || waitingQueue.size() == 0){
				waitingQueue.push(arrivalIndex);
				arrivalIndex++;
			}

			//Servicing and Leaving Event
			if( server -> getNextFreeTime() <= curArrival || arrivalIndex == arrivalTime.size()){
				if(waitingQueue.size() == 0){
					continue;
				}
				ld curQueueSize = waitingQueue.size();

				int headIndex = waitingQueue.front();

				waitingQueue.pop();
				ld serviceStartTime = max(arrivalTime[headIndex], server -> getNextFreeTime());	

				ld serviceTime = distributionServer(generator);
				// ld serviceTime = distributionServer(generatorArrival);
				
				ld serviceEndTime = serviceStartTime + serviceTime;
				serviceEnterTime.push_back(serviceStartTime);
				leavingTime.push_back(serviceEndTime);
				server -> setNextFreeTime(serviceEndTime);
				totalServerTime += serviceTime;
			}
		}
		totalTime = server -> getNextFreeTime();
		for(int j=0;j<arrivalTime.size();j++){
			totalResponseTime += leavingTime[j] - arrivalTime[j];
			totalWaitingTime += serviceEnterTime[j] - arrivalTime[j];
		}

		ld averageWorkersGettingChecked = totalServerTime / totalTime;
		ld averageResponseTime = totalResponseTime / totalWorkers;
		ld averageWaitingTime = totalWaitingTime / totalWorkers;
		ld averageWorkersInQueue = totalWaitingTime / totalTime;

		cout<<"Average number of workers getting checked: "<<averageWorkersGettingChecked<<endl;
		cout<<"Average response time for workers in getting checked: "<<averageResponseTime<<endl;
		cout<<"Average time for which a worker has to wait until getting checked: "<<averageWaitingTime<<endl;
		cout<<"Average number of workers waiting in the queue before each officer: "<<averageWorkersInQueue<<endl;

	}
};