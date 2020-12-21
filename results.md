## __RESULTS__
The project successfully demonstrates shortest-path BFS, shortest-path A* search, and Prim's MST while indirectly addressing the Landmark problem. The implemented graph is directed and weighted with airports represented as nodes and routes represented as edges. A fully initialized graph consists of 10484 routes by 72 airlines covering 539 airports in 519 cities, with 533 unique source airports and 532 unique destination airports. Top airlines and airports are summarized as follows:

| Airport                                          | Routes |
| ------------------------------------------------ | ------ |
| Hartsfield Jackson Atlanta International Airport | 755    |
| Chicago O'Hare International Airport             | 380    |
| Dallas Fort Worth International Airport          | 330    |
| Denver International Airport                     | 320    |
| Los Angeles International Airport                | 297    |

| Airline            | Routes |
| ------------------ | ------ |
| American Airlines  | 1414   |
| Delta Air Lines    | 1327   |
| US Airways         | 1327   |
| United Airlines    | 1303   |
| Southwest Airlines | 1126   |

Nearly 20% of the routes go through the top 5 airports while nearly 62% of the routes are flown by the top 5 airlines. The median airport only has 4 routes, with large airports skewing the mean to nearly 20 routes per airport. Similarly, the median airline only has 11.5 routes, with large airlines skewing the mean to nearly 146 routes per airline. However, variance in route cost is less due to normalization by the mean, with a median edge weight of 0.46 and mean of 2.0.

Due to their high connectivity within the graph, routes containing the top 5 airports and airlines are prevalent in BFS shortest-paths.  Similarly, since route costs are inversely proportional to airline and destination airport frequency, these routes are also prevalent in A* Search shortest-paths and Prim MSTs due to their extremely low cost.

Furthermore, since the graph is directed and weakly connected, some airports are not necessarily guaranteed to be reachable by a given starting airport, defining limitations to the MST. However, most starting airports produce similar MSTs, reaching 524 airports for over 97% coverage and excluding the same 15 airports. Therefore, while the entire graph is connected, it contains multiple small subgraphs in which it is possible to exit but not enter. Starting airports not selected within these isolated components consistently produce MSTs with 5000-5200 cost, or under 25% of the total graph cost of approximately 20976.

Another phenomenon was the route similarity between BFS and A* search. All tested shortest-paths traversed the same number of airports, with most paths visiting the same airports. The primary difference occurred in the airline selected — whereas BFS selected an arbitrary airline, A* search selected the airline with lowest cost. As such, a simple optimization to BFS in which it selected the lowest cost airline would produce a close approximation to A* search at significantly faster runtimes for the particular graph. Even without the optimization, BFS produced paths with similar costs to A* search for many routes, with diverging paths often converging within the next 1-2 airports. However, there were also exceptions in which A* search produced noticeable cost differences, increasing as the route length increased. This is likely due to density of the graph and the difference in the number of routes to certain destinations. Several low cost paths exist to common destinations and are dictated by the few top airlines and airports, resulting in similar routes by BFS and A* search. On the other hand, very few routes exist to uncommon destinations, forcing both BFS and A* search to traverse similar routes. Thus, the shortest-path that minimizes the number of airports visited also minimizes the cost, with minor cost differences largely attributed to the cost of an airline rather than the cost of the destination airport.
