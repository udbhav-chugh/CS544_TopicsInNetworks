class Server{
	ld nextFreeTime;
	ld serviceRate;
public:
	Server(ld serviceRate){
		this -> serviceRate = serviceRate;
		nextFreeTime = 0;
	}
	ld getNextFreeTime(){
		return nextFreeTime;
	}
	void setNextFreeTime(ld nextFreeTime){
		this-> nextFreeTime = nextFreeTime;
	}
};