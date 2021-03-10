//class for server
class Server{
	ld nextFreeTime; // stores the time instance when server is free
	ld serviceRate; // stores the service rate of the server
public:
	Server(ld serviceRate){
		this -> serviceRate = serviceRate;
		nextFreeTime = 0;
	}
	//getter and setter for next free time
	ld getNextFreeTime(){
		return nextFreeTime;
	}
	void setNextFreeTime(ld nextFreeTime){
		this-> nextFreeTime = nextFreeTime;
	}
};