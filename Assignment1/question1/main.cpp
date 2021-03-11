#include<bits/stdc++.h>

using namespace std;

#include "inputOutput.cpp"

// Run Djikstra for a source to destination (optional: Can be -1 if looking for all shortest paths)
void djikstra(int src, int dest){
	set<pair<double, int>> rem;
	unordered_map <int, pair<int, double>> dist;
	// Initialise distances to INF
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
			}
		}
	}


	// Run Djikstra, finding unreached nodes with smallest distance
	while (!rem.empty()){
		double curDist = rem.begin()->first;
		int curNode = rem.begin()->second;
		rem.erase(rem.begin());
		
		// Store paths depending on smallest or second shortest paths
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

		// Update distances to neighbouring nodes
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


// Helper function to calculate shortest paths
void calculateShortestPaths(){
	for (int i=0; i<top.nodes; i++){
		djikstra (i, -1);
		for (int j=0; j<top.nodes; j++){
			if (i == j || top.shortestPath[i][j].empty())
				continue;
			int prev = -1;
			vector<double> costs;
			// Remove shortest path and rerun Djikstra to find link-disjoint second shortest path
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

// Check if connection constraints can be accommodated
int tryAdmit(Connection &req){

	// Check id shortest path admissible
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

	// Check id second shortest path admissible
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

// Helper function to admit connections request
void admitConnections(){
	for (auto &req: conn.connList){	
		int result = tryAdmit(req);
		if (!result)
			continue;
		req.allowed = 1;		
		// Calculate cost based on optimistic or pessimistic approach
		req.totCost = (result == 1 ? top.pathCost[req.src][req.dest] : top.secondPathCost[req.src][req.dest]);

		int prev = -1, index = 0;
		double reqVal = (opt.approach ? req.bMax : req.bEqv);
		vector <int> &chosenPath = (result == 1 ? top.shortestPath[req.src][req.dest] : top.secondShortestPath[req.src][req.dest]);
		// If admitted, update forwarding and connection tables
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

// Main function that takes input arguments 
int main(int argc, char *argv[]){
	// Check for number of arguments
	if (argc != 8){
		cout<<"Incorrect number arguments specified\n";
		exit(0);
	}
	opt.topFile = argv[1];
	opt.connFile = argv[2];
	opt.routeFile = argv[3];
	opt.fwdFile = argv[4];
	opt.pathsFile = argv[5];
	opt.flag = argv[6];
	opt.approach = stoi(argv[7]);

	if (opt.flag != "hop" && opt.flag != "dist"){
		cout<<"Error: hopjdist needs to be \"hop\" or \"dist\"\n";
		exit(0);
	}
	if (stoi(argv[7]) != 0 && stoi(argv[7]) != 1){
		cout<<"Error: Approach needs to be 0 (Optimistic) or 1 (Pessimistic)\n";
		exit(0);
	}

	// Parse inputs, admit connections and generate outputs
	parseTopology();
	parseConnections();
	calculateShortestPaths();
	admitConnections();
	printRoutingTable();
	printForwardingTable();
	printConnections();

	cout<<"\nRouting table stored in \""<<opt.routeFile<<"\" file.\n";
	cout<<"Forwarding table stored in \""<<opt.fwdFile<<"\" file.\n";
	cout<<"Admitted connections and count stored in \""<<opt.pathsFile<<"\" file.\n";

	return 0;
}
