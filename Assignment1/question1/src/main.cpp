#include<bits/stdc++.h>

using namespace std;

class Options {
 public:
	string topFile, connFile, routeFile, fwdFile, pathsFile, flag;
	bool approach;
};

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

class Connection{
public:
	bool allowed;
	int src, dest;
	double bMin, bAvg, bMax, bEqv, totCost;
	vector<int> lblList;
};

class Connections{
public:
	int count, lblCount;
	vector<Connection> connList;
};

Topology top;
Options opt;
Connections conn;

void parseTopology()
{
	ifstream in(opt.topFile);
	streambuf *cinbuf = std::cin.rdbuf();
	cin.rdbuf(in.rdbuf());

	cin >> top.nodes >> top.edges;
	top.linkCost.resize(top.nodes, vector < double >(top.nodes, -1));
	top.linkCapacity.resize(top.nodes, vector < double >(top.nodes, -1));
	top.shortestPath.resize(top.nodes, vector < vector<int> >(top.nodes));
	top.secondShortestPath.resize(top.nodes, vector < vector<int> >(top.nodes));
	top.pathCost.resize(top.nodes, vector < double >(top.nodes, -1));
	top.secondPathCost.resize(top.nodes, vector < double >(top.nodes, -1));
	top.forwardTable.resize(top.nodes);


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

	// cout << top.nodes << " " << top.edges << "\n";
	// for (int i = 0; i < top.nodes; i++) {
	// 	for (int j = 0; j < top.nodes; j++){
	// 		if (top.linkCost[i][j] != -1){
	// 			cout<<i<<" "<<j<<" "<<top.linkCost[i][j]<<" "<<top.linkCapacity[i][j]<<"\n";
	// 		}
	// 	}
	// }
}

void parseConnections(){
	ifstream in(opt.connFile);
	streambuf *cinbuf = std::cin.rdbuf();
	cin.rdbuf(in.rdbuf());

	cin >> conn.count;
	for (int i=0; i<conn.count; i++){
		Connection req;
		cin>>req.src>>req.dest>>req.bMin>>req.bAvg>>req.bMax;
		req.bEqv = min(req.bMax, req.bAvg + 0.25*(req.bMax - req.bMin));
		req.allowed = 0;
		conn.connList.push_back(req);
	}

	cin.rdbuf(cinbuf);

	// cout << conn.count << "\n";
	// for (auto c: conn.connList){
	// 	cout<<c.src<<" "<<c.dest<<" "<<c.bMin<<" "<<c.bAvg<<" "<<c.bMax<<" "<<c.bEqv<<"\n";
	// }
}

void djikstra(int src, int dest){
	set<pair<double, int>> rem;
	unordered_map <int, pair<int, double>> dist;
	for (int i=0; i<top.nodes; i++){
		if (i != src){
			rem.insert({INT64_MAX, i});
			dist[i] = {-1, INT64_MAX};
		}
		else{
			rem.insert({0, i});
			dist[i] = {-1, 0};
			if (dest == -1){
				top.shortestPath[src][i].push_back(src);
				// pathCost[src][i] = 0;
				// secondPathCost[src][i] = 0;
			}
		}
	}


	while (!rem.empty()){
		double curDist = rem.begin()->first;
		int curNode = rem.begin()->second;
		rem.erase(rem.begin());
		
		if (dist[curNode].first != -1){
			if (dest == -1){
				top.shortestPath[src][curNode] = top.shortestPath[src][dist[curNode].first];
				top.shortestPath[src][curNode].push_back(curNode);
				top.pathCost[src][curNode] = curDist;
			}
			else if (curNode == dest){
				int node = curNode;
				while (node != -1){
					top.secondShortestPath[src][curNode].push_back(node);
					node = dist[node].first;
				}
				reverse(top.secondShortestPath[src][curNode].begin(), top.secondShortestPath[src][curNode].end());
				top.secondPathCost[src][curNode] = curDist;
			}
		}

		for (int i=0; i<top.nodes; i++){
			if (top.linkCost[curNode][i] != -1 && rem.find({dist[i].second, i}) != rem.end()){
				if (dist[curNode].second != INT64_MAX && dist[curNode].second + top.linkCost[curNode][i] < dist[i].second){
					rem.erase(rem.find({dist[i].second, i}));
					dist[i] = {curNode, dist[curNode].second + top.linkCost[curNode][i]};
					rem.insert({dist[i].second, i});

				}
			}
		}
	}

}

void printRoutingTable(){
	ofstream out(opt.routeFile);
	streambuf *coutbuf = std::cout.rdbuf();
	cout.rdbuf(out.rdbuf());
	
	cout << "Routing Table for All Nodes\n";
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

void calculateShortestPaths(){
	for (int i=0; i<top.nodes; i++){
		djikstra (i, -1);
		for (int j=0; j<top.nodes; j++){
			if (i == j || top.shortestPath[i][j].empty())
				continue;
			int prev = -1;
			vector<double> costs;
			for (auto it: top.shortestPath[i][j]){
				if (prev != -1){
					costs.push_back(top.linkCost[prev][it]);
					top.linkCost[prev][it] = -1;
					top.linkCost[it][prev] = -1;
				}
				prev = it;
			}
			djikstra (i, j);
			prev = -1;
			int index = 0;
			for (auto it: top.shortestPath[i][j]){
				if (prev != -1){
					top.linkCost[prev][it] = costs[index];
					top.linkCost[it][prev] = costs[index++];
				}
				prev = it;
			}
		}
	}
}

int tryAdmit(Connection &req){

	if (top.shortestPath[req.src][req.dest].empty())
		return 0;

	int prev = -1;
	bool possible = 1;
	double reqVal = (opt.approach ? req.bMax : req.bEqv);
	for (auto it: top.shortestPath[req.src][req.dest]){
		if (prev != -1){
			if (reqVal > top.linkCapacity[it][prev]){
				possible = 0;
				break;
			}
		}
		prev = it;
	}

	if (possible)
		return 1;

	if (top.secondShortestPath[req.src][req.dest].empty())
		return 0;

	prev = -1;
	possible = 1;
	for (auto it: top.secondShortestPath[req.src][req.dest]){
		if (prev != -1){
			if (reqVal > top.linkCapacity[it][prev]){
				possible = 0;
				break;
			}
		}
		prev = it;
	}

	if (possible)
		return 2;
	else
		return 0;

}


void admitConnections(){
	for (auto &req: conn.connList){	
		int result = tryAdmit(req);
		if (!result)
			continue;
		req.allowed = 1;		
		req.totCost = (result == 1 ? top.pathCost[req.src][req.dest] : top.secondPathCost[req.src][req.dest]);

		int prev = -1, index = 0;
		double reqVal = (opt.approach ? req.bMax : req.bEqv);
		vector <int> &chosenPath = (result == 1 ? top.shortestPath[req.src][req.dest] : top.secondShortestPath[req.src][req.dest]);
		if (chosenPath.size() >= 2)
			top.forwardTable[chosenPath[0]].push_back({-1, -1, chosenPath[1], conn.lblCount});
		for (auto it: chosenPath){
			if (prev != -1){
				top.linkCapacity[it][prev] -= reqVal;
				top.linkCapacity[prev][it] -= reqVal;
				int lbl = conn.lblCount++;
				if (!req.lblList.empty()){
					top.forwardTable[chosenPath[index-1]].push_back({chosenPath[index-2], *(req.lblList.rbegin()), it, lbl});
				}
				req.lblList.push_back(lbl);
			}
			prev = it;
			index++;
		}
		if (chosenPath.size() >= 2)
			top.forwardTable[chosenPath[chosenPath.size() - 1]].push_back({chosenPath[chosenPath.size() - 2], conn.lblCount-1, -1, -1});
	}
}

void printForwardingTable(){
	ofstream out(opt.fwdFile);
	streambuf *coutbuf = std::cout.rdbuf();
	cout.rdbuf(out.rdbuf());

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

void printConnections(){

	ofstream out(opt.pathsFile);
	streambuf *coutbuf = std::cout.rdbuf();
	cout.rdbuf(out.rdbuf());

	int admitConn = 0;
	cout << "Admitted Connections:\n";
	cout <<setw(13)<<"Connection Id"<<setw(15)<<"Source"<<setw(15)<<"Destination"<<setw(34)<<"Label List"<<setw(17)<<"Path Cost\n";
	int index = 0;
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

	cout<<"Total Connections: "<<conn.connList.size()<<"\n";;
	cout<<"Admitted Connections: "<<admitConn<<"\n";
	cout<<"Blocking Probability: "<<1-double(admitConn)/conn.connList.size()<<"\n";
}

int main(int argc, char *argv[]){
	// Check for number of arguments
	if (argc < 8){
		cout<<"Not enough arguments specified\n";
		exit(0);
	}
	opt.topFile = argv[1];
	opt.connFile = argv[2];
	opt.routeFile = argv[3];
	opt.fwdFile = argv[4];
	opt.pathsFile = argv[5];
	opt.flag = argv[6];
	opt.approach = stoi(argv[7]);

	parseTopology();
	parseConnections();
	calculateShortestPaths();
	admitConnections();
	printRoutingTable();
	printForwardingTable();
	printConnections();

	// cout << top.nodes << " " << top.edges << "\n";
	// for (int i = 0; i < top.nodes; i++) {
	// 	for (int j = 0; j < top.nodes; j++){
	// 		if (top.linkCost[i][j] != -1){
	// 			cout<<i<<" "<<j<<" "<<top.linkCost[i][j]<<" "<<top.linkCapacity[i][j]<<"\n";
	// 		}
	// 	}
	// }

	return 0;
}
