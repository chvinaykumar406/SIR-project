# SIR-project
An epidemic like Covid-19 is an SIR epidemic. An individual is initially Sus- ceptible (S). Then s/he might get Infected (I) and finally Recover (R). However,  as one can expect, each transition is probabilistic.this simulation is  for a maximum of 300 days, use τ(transmission probability) as 0.5 and γ (recovery probability)as 0.2 with a a graph of 10000 nodes and max 3000 connections to each node

An efficient algorithm simulating continuous-time SIR epidemics in static
networks with rates τ and γ. Events are
stored in a priority queue Q, and the first is executed. If the event is a transmission
and the recipient is susceptible, it becomes infected, its recovery is added to Q and
transmissions to its neighbours may be added. If the event is a recovery, the node
recovers.

this code prints the respective susceptile,trasmitted,recovered counts on each day.
while giving intial infected nodes please consider giving more than 1 because giving only one may have the case that getting recovery time 1 and recover before tansmitting the disease other.

The output is stored in the text files which has the data of infected count ,recovered count with time.you can use gnu plot for the graph if you would like to.
also i am maintaning the separate list of infected,recovery,susceptible.you can check the lists at some infectedcount or recovered by placing a simple if condition in my code.
thankyou.
