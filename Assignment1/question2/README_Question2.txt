This is the README for question 2 of Assignemnt 1.

===================================================

To compile the code:
make compile

===================================================

To run case A with custom input of lambda and mu:
./a.out 1 0.25 0.3

To run caseA with values specified in makefile:
make casea

Here, we have simulated a case with 2 M/M/1 queueing systems. Each queue has an arrival rate of lambda and each officer has a service rate of mu given as input.

===================================================

To run case B with custom input of lambda and mu:
./a.out 2 0.25 0.15

To run case B with values specified in makefile:
make caseb

Here, we have simulated a case with M/M/2 queueing system. The queue has an arrival rate of lambda and each officer has a service rate mu.

===================================================

To run case C with custom input of lambda and mu:
./a.out 3 0.25 0.413

To run case C with values specified in makefile:
make casec

Here we have simulated a case with 2 M/M/1/6 queueing systems (as for each system, there are 5 chairs and 1 serving officer). Each queue has an arrival rate of lambda and each officer has a service rate of mu. 
Here, if you specify only lambda as input, mu will be taken such that blocking probability will be 0.02. If both lambda and mu are given, blocking will be in accordance to the rates provided as input.
For blocking probability to be 0.02, serviceRate(mu) = arrivalRate(lambda) / 0.60524

===================================================

Note: 
First arguemnt after ./a.out is the case number (1->CaseA, 2->CaseB, 3->CaseC)
Second argument is value of lambda (in workers per second)
Third arguemnt is value of mu (in workers per second)

Note: If lambda or mu values are not specified, default values in the code will be considered.

Note: The simulation is run for 1000000 workers per queue to get the behaviour close to infinite population.

Note: For CaseC, if you specify only the lambda input, the mu input will be taken such that the blocking probability of the system will be equal to 0.02 (as specified),i.e., on an average 2 workers out of 100 will be dropped due to queue being full.

=========================================

To run case C only with lambda input:
./a.out 1 0.25

=========================================
