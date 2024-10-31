# Latencybench-FastDDS

To build:
  cd build
  make clean; cmake .; make

NOTE: To run on a single machine, there is code for initial peers that must be commented out.

To run:
  Set up static ip on both machines
  ./HelloWorld -rate 4 -size 64 -driver
  ./HelloWorld -rate 4 -size 64 -server

8MB:
  ./HelloWorld -rate 4 -size 8388608 -driver
  ./HelloWorld -rate 4 -size 8388608 -server
