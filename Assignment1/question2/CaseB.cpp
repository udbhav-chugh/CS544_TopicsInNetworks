//class for Case B -> One M/M/2 Queue systems
class CaseB{

	ld arrivalRate, serviceRate; //input arrival and service rate
	ll totalWorkers; // total workers to simulate on

	ld totalResponseTime; // total response time
	ld totalTime; // total time of simulation

	ld totalServerTime, totalWaitingTime; // total checking time in server and waiting time in queue

	vector<ld> arrivalTime, serviceEnterTime, leavingTime; // for each worker, store its arrival, service entering and leaving time

	Server *servers[2]; //two servers

public:
	//intialise variables
	CaseB(ld arrivalRate, ld serviceRate, ll totalWorkers){

		this -> arrivalRate = arrivalRate;
		this -> serviceRate = serviceRate;
		this -> totalWorkers = totalWorkers;

		for(int i=0;i<2;i++){
			servers[i] = new Server(this -> serviceRate);
		}

		totalResponseTime = 0;
		totalWaitingTime = 0;
		totalTime = 0;
		totalServerTime = 0;
	}

	void simulate(){
		if(arrivalRate >= 2*serviceRate){
			cout<<"The system is unstable. Exiting program"<<endl;
			exit(0);
		}

		//exponential input generator
		default_random_engine generator (time(0));
		exponential_distribution<double> distributionArrival (this -> arrivalRate);
		exponential_distribution<double> distributionServer (this -> serviceRate);



		ld curArrivalTime = 0;
		ld sum=0;
		//find out arrivals for workers
		for(int j=0;j<totalWorkers;j++){
			curArrivalTime += distributionArrival(generator);
			arrivalTime.push_back(curArrivalTime);
		}
		int arrivalIndex = 0;
		queue<int> waitingQueue;

		//start simulation
		while(arrivalIndex < arrivalTime.size() || waitingQueue.size() > 0){
			ld curArrival = LDBL_MAX;
			//check if any arrival is still pending
			if(arrivalIndex < arrivalTime.size()){
				curArrival = arrivalTime[arrivalIndex];
			}

			int freeServer = 0;
			if(servers[1] -> getNextFreeTime() < servers[0] -> getNextFreeTime()){
				freeServer = 1;
			}

			//Arrival Event for a worker
			if(curArrival < servers[freeServer] -> getNextFreeTime() || waitingQueue.size() == 0){
				waitingQueue.push(arrivalIndex);
				arrivalIndex++;
			}

			//Servicing and Leaving Event for a worker
			if( servers[freeServer] -> getNextFreeTime() <= curArrival){
				if(waitingQueue.size() == 0){
					continue;
				}
				ld curQueueSize = waitingQueue.size();

				//get element at head of queue
				int headIndex = waitingQueue.front();

				waitingQueue.pop();
				ld serviceStartTime = max(arrivalTime[headIndex], servers[freeServer] -> getNextFreeTime());	

				ld serviceTime = distributionServer(generator); // find service time using exponential distribution
				
				ld serviceEndTime = serviceStartTime + serviceTime;
				// update service start and end time for the worker
				serviceEnterTime.push_back(serviceStartTime);
				leavingTime.push_back(serviceEndTime);
				servers[freeServer] -> setNextFreeTime(serviceEndTime);
				totalServerTime += serviceTime;
			}
		}
		//simulation completed
		//calculate required values to poutput
		totalTime = max(servers[0] -> getNextFreeTime(), servers[1] -> getNextFreeTime());
		for(int j=0;j<arrivalTime.size();j++){
			totalResponseTime += leavingTime[j] - arrivalTime[j];
			totalWaitingTime += serviceEnterTime[j] - arrivalTime[j];
		}
		ld averageWorkersGettingChecked = totalServerTime / totalTime;
		ld averageResponseTime = totalResponseTime / totalWorkers;
		ld averageWaitingTime = totalWaitingTime / totalWorkers;
		ld averageWorkersInQueue = totalWaitingTime / totalTime;
		ld averageWorkersInSystem = totalResponseTime / totalTime;

		//output on console as well as file
		ofstream fil("CaseB.txt");
		cout<<"Given:"<<endl;
		fil<<"Given:"<<endl;
		cout<<"For the common queue, mean arrival rate is "<<arrivalRate<<" workers per second with exponential distribution"<<endl;
		fil<<"For the common queue, mean arrival rate is "<<arrivalRate<<" workers per second with exponential distribution"<<endl;
		cout<<"For each officer, mean service rate is "<<serviceRate<<" workers per second with exponential distribution"<<endl;
		fil<<"For each officer, mean service rate is "<<serviceRate<<" workers per second with exponential distribution"<<endl;
		cout<<"This is a case of M/M/2 queue system"<<endl<<endl;
		fil<<"This is a case of M/M/2 queue system"<<endl<<endl;

		cout<<"a) Average number of workers getting checked: "<<averageWorkersGettingChecked<<endl;
		fil<<"a) Average number of workers getting checked: "<<averageWorkersGettingChecked<<endl;
		cout<<"b) Average response time for workers in getting checked: "<<averageResponseTime<<endl;
		fil<<"b) Average response time for workers in getting checked: "<<averageResponseTime<<endl;
		cout<<"c) Average time for which a worker has to wait until getting checked: "<<averageWaitingTime<<endl;
		fil<<"c) Average time for which a worker has to wait until getting checked: "<<averageWaitingTime<<endl;
		cout<<"d) Average number of workers waiting in the queue before each officer: "<<averageWorkersInQueue<<endl;
		fil<<"d) Average number of workers waiting in the queue before each officer: "<<averageWorkersInQueue<<endl;
		cout<<"Additionally, Average number of workers in the system (getting checked + in queue): "<<averageWorkersInSystem<<endl<<endl;
		fil<<"Additionally, Average number of workers in the system (getting checked + in queue): "<<averageWorkersInSystem<<endl<<endl;
		fil.close();
	}
};