
/*

POSET:

in a DAG with n vertices
we want to find the largest antichain (largest set of vertices with no path between them)

for every [u-->v] add it as edge for max bipartite matching

then answer to the problem is: n - max_matching()

---------------------------------------------------------------------------------

Konig theorem: minimum vertex cover size = max_matching()
vertex cover: a set of vertices that touches every edge.

---------------------------------------------------------------------------------

independent set: set where no two vertices are adjacent to each other

max independent set is the complement to min vertex cover

max independent set = n - min vertex cover

---------------------------------------------------------------------------------
min cost vertex cover (for bipartite graphs, cost on vertices):
- from source to right nodes make the edge capcity = cost(node)
- from left nodes to sink make the edge capcity = cost(node)
- from left nodes to right nodes make the edge capacity = infinity

*/
