# Solving Monte Carlo simulation using union-find algorithms
The simulation aims to find a `P` value that is a practical threshold for a connectivity problem.
Given `N*N` square each cell is "enabled" with a probability `P`. The connectivity is then determined by
the fact that we can get from top to bottom using "enabled" cells.

To determine if the path is available we use union-find alrorythm. There are three versions implemented and tested.
The fastest one is used for the actual Monte Carlo simulation.
The task is taken from [here](https://coursera.cs.princeton.edu/algs4/assignments/percolation/specification.php)

