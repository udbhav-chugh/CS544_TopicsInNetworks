Name: Udbhav Chugh, Mayank Baranwal
Roll No: 170101081, 170101084

CS 544 Assignment 1 Q1:

Steps to Run:
1) Compile: g++ main.cpp -o routing.o
2) Run program: 
	- Order of arguments same as assignment: "topologyFile" "connectionsFile" "routingTableFile" "forwardingTableFile" "pathsFile" "hopjdist" "0|1"

	- ./routing.o top24.txt ARPANET_100.txt routingTable.txt forwardingTable.txt paths.txt dist 1	
		--> Run with distance metric with pessimistic approach for 24 nodes
		--> Output:
				Total Connections: 100
				Admitted Connections: 36
				Blocking Probability: 0.64


	- ./routing.o top14.txt NSFNET_100.txt routingTable.txt forwardingTable.txt paths.txt hop 0
		--> Run with hop metric with optimistic approach for 14 nodes 
		--> Output: 
				Total Connections: 100
				Admitted Connections: 47
				Blocking Probability: 0.53

3) File outputs stored in: "routingTableFile" "forwardingTableFile" "pathsFile"