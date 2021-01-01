This project was compiled with clang version 6.0.1 on a Linux system.

### Overview
This project implements a directed, weighted graph representing the network of airport routes within the U.S. Airports are represented as nodes while flights are represented as edges. The complete graph contains 10484 routes by 72 airlines covering 539 airports in 519 cities.

### Data
Datasets were obtained from [OpenFlights](https://openflights.org/data.html) and cleaned to only include relevant data for domestic flights. The airport dataset contains the name and city for each airport while the route dataset contains the source airport ID, destination airport ID, airline name, and cost for each route.

Route weights were artificially generated as costs inversely proportional to the frequency of the destination airport and the airline (i.e. routes with popular airlines/destinations cost less). Both components of the weight were normalized by their respective averages before being summed to generate the overall weight.

### Algorithms
The three algorithms implemented are:
1. __Dijkstra's Algorithm__: Implements Dijkstra's Algorithm for finding the Single-Source Shortest-Path (SSSP). Finds the shortest-path between the source and destination airport, defined as the minimum route cost between them, which is not necessarily guaranteed to exist. Automatically finds the landmark path for multiple destinations by performing the algorithm for each airport sequentially.
    * __Landmark Path__: Finds the shortest-path from airport A to airport B through airport C, which is simply the shortest-path from A to C combined with the shortest-path from C to B. Separate searches are performed for each destination airport by taking the last destination as the new source and combining each subpath to derive the full path.
2. __Prim's Algorithm__: Implements Prim's Algorithm for finding the minimum spanning tree (MST). Since not all airports are guaranteed to be reachable from a given starting airport, the algorithm generates the largest possible MST. Additionally implements a priority queue as a min heap that returns the lowest cost airport that has not been visited yet.
3. __Floyd-Warshall's Algorithm__: Implements Floyd-Warshall's algorithm for finding the All-Pairs Shortest-Path (APSP). Specifically calculates the betweenness centrality of every airport by finding the shortest-path between every pair of airports in the graph. Betweenness centrality is defined as the number of shortest-paths through a given airport divided by the number of total shortest-paths not including the airport as a source or destination. 

### Running
The project can be compiled with `make` and ran with `./main` for a user-friendly interface that provides a guided entry of city and airport inputs. Floyd-Warshall's algorithm requires no arguments and Prim's algorithm only requires a starting airport, while Dijkstra's Algorithm requires at least one destination in addition to a starting airport. Command-line arguments can also be provided for quick queries if airport IDs are known. 

For instance, `./main 200` finds Prim's MST starting at Chicago O'Hare International Airport, while `./main 200 124` finds the shortest-path from Chicago O'Hare International Airport to Hartsfield Jackson Atlanta International Airport. Any number of destinations can be entered, and the ID for each airport can be found in the airport dataset.

All algorithms generate a text file containing their results in the __data__ directory, which is also where the airport and route datasets along with the python script used to clean the data can be found.
