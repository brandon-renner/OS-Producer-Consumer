# OS-Producer-Consumer
#
# This project and its programs aims to solve the Producer-Consumer Problem 
# with the specifications set by the assignment for Operating Systems class
#
# these programs are written in the C++ language, and were 
# developed on a Linux-POSIX environment to be run with the 
# use of threads. To run this code, use the following 
# commands in sequence:
#
# $ g++ producer.cpp -pthread -lrt -o producer
# $ g++ consumer.cpp -pthread -lrt -o consumer
# $ ./producer & ./consumer &
#
# this project does not require any user inputs, and 
# should run a finite amount of time/iterations once being executed,
# successfully acting as parallel and communicating producer 
# and consumer processes
