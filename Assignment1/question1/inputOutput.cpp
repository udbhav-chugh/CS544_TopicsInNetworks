#include<bits/stdc++.h>

// Class to store input and output options
class Options {
 public:
	string topFile, connFile, routeFile, fwdFile, pathsFile, flag;
	bool approach;
};

// Store network graph with shortest and second shortest paths
class Topology {
 public:
	int nodes;
	int edges;
	vector < vector < double >>linkCost;
	vector < vector < double >>linkCapacity;
	vector<vector<vector<int>>> shortestPath;
	vector<vector<vector<int>>> secondShortestPath;
	vector<vector<double>> pathCost;
	vector<vector<double>> secondPathCost;
	vector<vector<vector<int>>> forwardTable;

};

// Store data of a single connection request
class Connection{
public:
	bool allowed;
	int src, dest;
	double bMin, bAvg, bMax, bEqv, totCost;
	vector<int> lblList;
};

// Class to store all connection requests
class Connections{
public:
	int count, lblCount;
	vector<Connection> connList;
};

Topology top;
Options opt;
Connections conn;

// Parse input Topology file and generate network graph
void parseTopology()
{
	ifstream in(opt.topFile);
	if (!in.good()){
		cout<<"Input topology file does not exist\n";
		exit(0);
	}
	streambuf *cinbuf = std::cin.rdbuf();
	cin.rdbuf(in.rdbuf());

	cin >> top.nodes >> top.edges;
	
	// Initialise variables
	top.linkCost.resize(top.nodes, vector < double >(top.nodes, -1));
	top.linkCapacity.resize(top.nodes, vector < double >(top.nodes, -1));
	top.shortestPath.resize(top.nodes, vector < vector<int> >(top.nodes));
	top.secondShortestPath.resize(top.nodes, vector < vector<int> >(top.nodes));
	top.pathCost.resize(top.nodes, vector < double >(top.nodes, -1));
	top.secondPathCost.resize(top.nodes, vector < double >(top.nodes, -1));
	top.forwardTable.resize(top.nodes);


	// Construct topology graph
	for (int i = 0; i < top.edges; i++) {
		int src, dest, cost, cap;
		cin >> src >> dest >> cost >> cap;
		if (opt.flag == "hop"){
			top.linkCost[src][dest] = 1;
			top.linkCost[dest][src] = 1;
		}
		else{
			top.linkCost[src][dest] = cost;
			top.linkCost[dest][src] = cost;
		}
		top.linkCapacity[src][dest] = cap;
		top.linkCapacity[dest][src] = cap;
	}

	cin.rdbuf(cinbuf);
}

// Parse input connections request and calculate bEqv
void parseConnections(){
	ifstream in(opt.connFile);
	if (!in.good()){
		cout<<"Input connections file does not exist\n";
		exit(0);
	}streambuf *cinbuf = std::cin.rdbuf();
	cin.rdbuf(in.rdbuf());

	cin >> conn.count;
	// Input and store connection requests
	for (int i=0; i<conn.count; i++){
		Connection req;
		cin>>req.src>>req.dest>>req.bMin>>req.bAvg>>req.bMax;
		req.bEqv = min(req.bMax, req.bAvg + 0.25*(req.bMax - req.bMin));
		req.allowed = 0;
		conn.connList.push_back(req);
	}

	cin.rdbuf(cinbuf);
}

// Print routing table of each node to file
void printRoutingTable(){
	ofstream out(opt.routeFile);
	streambuf *coutbuf = std::cout.rdbuf();
	cout.rdbuf(out.rdbuf());
	
	cout << "Routing Table for All Nodes\n";
	// Print shortest routing paths
	for (int i=0; i<top.nodes; i++){
		cout << "Shotest Path Routing for Node "<<i<<":\n";
		cout <<setw(16)<<"Destination Node"<<setw(29)<<"Path"<<setw(17)<<"Path Cost\n";
		for (int j=0; j<top.nodes; j++){
			if (i == j) 
				continue;
			string path;
			for (auto it: top.shortestPath[i][j])
				path += (to_string(it) + " ");
			cout<<setw(16)<<j<<setw(30)<<path<<setw(15)<<top.pathCost[i][j]<<"\n";
		}
		cout<<"\n";
	}
	cout<<"\n\n";

	// Print second shortest routing paths
	for (int i=0; i<top.nodes; i++){
		cout << "Second Shotest Path Routing for Node "<<i<<":\n";
		cout <<setw(16)<<"Destination Node"<<setw(29)<<"Path"<<setw(17)<<"Path Cost\n";
		for (int j=0; j<top.nodes; j++){
			if (i == j) 
				continue;
			string path;
			for (auto it: top.secondShortestPath[i][j])
				path += (to_string(it) + " ");
			cout<<setw(16)<<j<<setw(30)<<path<<setw(15)<<top.secondPathCost[i][j]<<"\n";

		}
		cout<<"\n";
	}

	cout.rdbuf(coutbuf);

}

// Print forwarding table of each node to file
void printForwardingTable(){
	ofstream out(opt.fwdFile);
	streambuf *coutbuf = std::cout.rdbuf();
	cout.rdbuf(out.rdbuf());

	// Interface represents incomingoutoing node number
	for (int i=0; i<top.nodes; i++){
		cout << "Forwarding table for Node "<<i<<":\n";
		cout <<setw(12)<<"Interface In"<<setw(15)<<"Label In"<<setw(20)<<"Interface Out"<<setw(16)<<"Label Out\n";
		for (auto it: top.forwardTable[i]){
			cout <<setw(12)<<it[0]<<setw(15)<<it[1]<<setw(20)<<it[2]<<setw(15)<<it[3]<<"\n";
		}
		cout<<"\n";
	}

	cout.rdbuf(coutbuf);
}

// Print admitted connections to file
void printConnections(){

	ofstream out(opt.pathsFile);
	streambuf *coutbuf = std::cout.rdbuf();
	cout.rdbuf(out.rdbuf());

	int admitConn = 0;
	cout << "Admitted Connections:\n";
	cout <<setw(13)<<"Connection Id"<<setw(15)<<"Source"<<setw(15)<<"Destination"<<setw(34)<<"Label List"<<setw(17)<<"Path Cost\n";
	int index = 0;
	// Convert label list to string and print to file
	for (auto it: conn.connList){
		if (it.allowed){
			admitConn++;
			string lblStr;
			for (auto node: it.lblList){
				lblStr += (to_string(node) + " ");
			}
			cout <<setw(13)<<index<<setw(15)<<it.src<<setw(15)<<it.dest<<setw(35)<<lblStr<<setw(15)<<it.totCost<<"\n";
		}
		index++;		
	}

	cout.rdbuf(coutbuf);

	// Final number of successful connections
	cout<<"Total Connections: "<<conn.connList.size()<<"\n";;
	cout<<"Admitted Connections: "<<admitConn<<"\n";
	cout<<"Blocking Probability: "<<1-double(admitConn)/conn.connList.size()<<"\n";
}