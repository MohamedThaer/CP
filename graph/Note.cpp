7.1) EulerianGraph
USED FOR

Find Eulerian CYCLE (uses every edge exactly once and returns to start)
Find Eulerian PATH (uses every edge exactly once, start != end possible)
Works for directed or undirected graphs.
CORE IDEA (SIMPLE)

Euler path/cycle exists if:
Undirected:
all vertices with degree>0 are in one connected component
number of odd-degree vertices is 0 (cycle) or 2 (path)
Directed:
all vertices with degree>0 connected in underlying sense
in-degree == out-degree for all (cycle)
for path: exactly one node has out-in=1 (start) and one has in-out=1 (end)
Construct path using Hierholzer algorithm (stack, walk edges, backtrack).
INPUTS

n vertices
edges (u,v)
directed flag
OUTPUTS

vector circuit/path (sequence of vertices)
size = edges_count + 1 if success
empty if no Eulerian traversal possible
IMPORTANT COMPONENTS / FUNCTIONS (from your class)

EulerianGraph(int num_vertices, bool is_directed=false)

Input : n, directed?
Output: object with empty graph
void addEdge(int u, int v)

Input : endpoints
Output: none
Effect:
adds edge in adjacency
updates degrees
stores reverse-edge index for undirected
bool isConnected()

Output: true if all non-isolated vertices are connected (in required sense)
bool hasEulerianCycle()

Output: true/false
bool hasEulerianPath()

Output: true/false
vector findEulerianCycle()

Output: vertex list of Eulerian cycle, or {} if impossible
vector findEulerianPath()

Output: vertex list of Eulerian path, or {} if impossible
WHEN TO USE

Problems: “print euler path”, “arrange edges/strings/dominoes”, “de Bruijn-like”.
7.2) MST (Minimum Spanning Tree)
USED FOR

Build Minimum Spanning Tree of an undirected weighted graph.
CORE IDEA

Your snippet builds MST using a Prim-like approach with a priority queue:
pick smallest edge reaching a new node.
INPUTS

Weighted graph g where g[u] holds (v, cost)
Typically 1-indexed in snippet (n = g.size()-1)
OUTPUTS

vector<vector<pair<int,int64>>> mst adjacency list
(MST edges stored as adjacency with weights)
IMPORTANT FUNCTION

auto MST(auto &g)
Input : adjacency list of original graph
Output: adjacency list of MST (size n+1)
ASSUMPTIONS

Graph is connected to get full MST; otherwise returns a forest for reachable part.
WHEN TO USE

Minimum cost to connect nodes, “roads” problems, clustering, etc.
7.3) SCC (Strongly Connected Components)
USED FOR

Decompose directed graph into SCCs.
Useful for: 2SAT, condensation DAG, reachability on SCC DAG.
CORE IDEA

Tarjan algorithm:
dfs order id, low-link
nodes popped when root of SCC found
INPUTS

directed graph g (vector<vector>)
template has base=1 (often nodes from base..n)
OUTPUTS

component count
component id for each node
compressed DAG (optional)
list of components (optional)
IMPORTANT FUNCTIONS (from shown API)

SCC(const vector<vector>& g)

Input : graph
Output: builds SCC structure
int components_cnt()

Output: number of SCCs
int component(int node)

Output: SCC id of node
bool same_component(int u, int v)

Output: true if in same SCC
vector<vector>& compress()

Output:
returns DAG of SCCs (condensation graph)
Notes:
builds dag[] and in_deg[] for SCC graph
vector<vector>& get_components()

Output: list of nodes in each SCC
bool is_head(int comp)

Output: true if SCC has indegree 0 in SCC DAG (after compress)
WHEN TO USE

“condense graph”, “count SCCs”, “detect cycles in directed graph”, “2SAT”.
7.4) ZeroOneBFS
USED FOR

Shortest paths when edge weights are only 0 or 1.
CORE IDEA

Use deque:
weight 0 -> push_front
weight 1 -> push_back
Equivalent to Dijkstra but O(V+E).
INPUTS

graph g: vector<vector<pair<int,int>>> where pair = (to, w) and w in {0,1}
sources list (multi-source supported)
OUTPUTS

dist[] shortest distance
get_path(node) returns one shortest path
IMPORTANT FUNCTIONS (from class)

ZeroOneBFS(graph, vector src={base})

Input : graph, sources
Output: computes dist[] and parent[]
vector<int64_t> dis

Output: distances (INF if unreachable)
vector get_path(int node)

Input : target node
Output: path from a source to node (empty if unreachable)
WHEN TO USE

Grid with 0/1 costs, “min reversals”, “min edges to flip”, etc.
7.5) bellman_ford
USED FOR

Shortest paths with negative edges
Detect nodes affected by negative cycles (or mark as -INF/INF depending mode)
CORE IDEA

Relax edges n-1 times.
If can relax on nth iteration -> negative cycle reachable.
Your class also propagates “infinite improvement” via BFS/DFS after detection.
INPUTS

graph g: vector<vector<pair<int,int>>> or (to,cost) list
src list (multi-source)
template parameters:
base (1 usually)
mode = 1 for min, mode = -1 for max (by flipping sign)
OUTPUTS

dis[] distances
best(node) = mode * dis[node]
get_path(node) (if reachable and not affected by neg cycles)
IMPORTANT FUNCTIONS (from class)

bellman_ford(graph g, vector src={base})

Input : graph, sources
Output: computes dis + parent arrays
int64_t best(int node)

Output: shortest (or longest if mode=-1) distance value
tuple<vector, int, int> get_path(int node) (some versions return extra)

Input : target
Output: path (or empty if unreachable/invalid)
NOTES

If dis[node] becomes -INF/INF (cycle affected), path is not meaningful.
WHEN TO USE

Negative edges, detect arbitrage/profit cycles, constraints graphs.
7.6) bfs (unweighted shortest path)
USED FOR

Shortest path in unweighted graphs
Multi-source BFS
INPUTS

graph g: vector<vector>
src list
OUTPUTS

dis[] (levels)
get_path(node)
IMPORTANT FUNCTIONS

BFS(graph g, vector src={base})

computes dis and parent
vector<int64_t> dis

vector get_path(int node)

7.7) bipartite_graph (check + coloring)
USED FOR

Check if undirected graph is bipartite
Return side/color of each node
CORE IDEA

DFS/BFS coloring with 2 colors.
If edge connects same color -> not bipartite.
INPUTS

undirected graph g: vector<vector>
OUTPUTS

pair<bool, vector>:
ok: bipartite or not
side[i]: 0/1 color, or -1 for unvisited (depending)
IMPORTANT FUNCTION

bipartite_graph(const vector<vector>& g)
Input : graph
Output: (ok, side)
WHEN TO USE

Bipartite check, odd cycle detection (indirect), 2-coloring.
7.8) blockcut_tree (BCC + articulation points -> block-cut tree)
USED FOR

Find biconnected components (BCCs) in undirected graph
Find articulation points
Build Block-Cut Tree:
One node per BCC
One node per articulation point
Edges connect articulation to BCCs it belongs to
CORE IDEA

DFS with tin/low (Tarjan).
Use edge stack to form BCCs.
Then build a tree structure connecting BCC nodes and articulation nodes.
INPUTS

undirected graph edges added by add_edge(u,v)
OUTPUTS

get_tree(): adjacency of block-cut tree
query mapping: tree_idx(u) gives node index in block-cut tree representing original vertex u
get_bccs(): list of BCC components (original vertices)
IMPORTANT FUNCTIONS (from class)

BlockCutTree(int n=0)

Input : n nodes
Output: initialized (call init if needed)
void add_edge(int u, int v)

Input : undirected edge
Output: none
void build_tree()

Input : none
Output: computes:
bcc list
articulation points
tree adjacency
mappings id[] / node_to_block[] (names per code)
vector<vector> get_tree()

Output: block-cut tree adjacency
bool is_articulation(int u)

Output: true if u is articulation point
int tree_idx(int u)

Output:
if u articulation -> its own tree node id
else -> the BCC tree node containing it
vector<vector> get_bccs()

Output: list of BCCs as vectors of original vertices
WHEN TO USE

“Remove vertex disconnects graph” queries, cactus-ish transforms, articulation/BCC tasks.
7.9) bridge_tree
USED FOR

Build bridge tree (2-edge-connected components compressed)
Helpful for queries after removing bridges, counting bridges on path, etc.
CORE IDEA

Find bridges using tin/low.
Compress graph by contracting non-bridge edges into components.
Bridges become edges between components => bridge tree/forest.
INPUTS

undirected graph g (vector<vector>) in constructor
OUTPUTS

component id for each node
list of bridges
bridge tree adjacency
get_all_comp(): nodes in each component
IMPORTANT FUNCTIONS (from class)

Bridge_tree(const vector<vector>& g)

Input : original graph
Output: computes bridges + components
int component(int node)

Output: component id of original node
int component_cnt()

Output: number of components
vector<pair<int,int>> get_bridges()

Output: list of bridges (u,v) in original graph
vector<vector> get_all_comp()

Output: nodes per component
vector<vector> bridge_tree()

Output: adjacency of compressed tree (components as nodes)
WHEN TO USE

Bridge problems, compress to tree then do LCA/DP on tree.
7.10) bridges
USED FOR

Find all bridges in undirected graph.
CORE IDEA

DFS tin/low:
edge (u,v) is bridge if low[v] > tin[u]
INPUTS

graph g (vector<vector>), typically 1-indexed with n = g.size()-1
OUTPUTS

vector<pair<int,int>> list of bridges
IMPORTANT FUNCTION

vector<pair<int,int>> bridges(vector<vector>& g)
Input : graph
Output: list of bridges
7.11) cut_points (articulation points)
USED FOR

Find all articulation points in undirected graph.
CORE IDEA

DFS tin/low.
Root is articulation if it has >1 DFS child.
Non-root u is articulation if exists child v with low[v] >= tin[u].
INPUTS

graph g (vector<vector>), usually 1-indexed
OUTPUTS

vector articulation points (sorted unique in your code)
IMPORTANT FUNCTION

vector cut_points(vector<vector>& g)
Input : graph
Output: list of cut vertices
7.12) dfs (simple)
USED FOR

Basic traversal, connected components, visited marking.
INPUTS

graph g (vector<vector>)
start node(s)
OUTPUTS

visited array
IMPORTANT PART

dfs lambda/function:
Input : node
Output: none, marks visited and recurses.
7.13) dijkstra (set-based)
USED FOR

Shortest paths with non-negative weights.
CORE IDEA

Maintain a set of (dist,node) and extract min each time.
INPUTS

weighted graph g: vector<vector<pair<int,T>>> where (to, weight)
src nodes list (multi-source)
template parameters:
T cost type (default int64)
base indexing
OUTPUTS

dist[] distances from sources
INF if unreachable
IMPORTANT FUNCTION

vector dijkstra(g, src={base})
Input : graph, sources
Output: distance array
WHEN TO USE

Standard shortest path when weights >= 0.
7.14) dijkstra on segment (graph_segtree)
USED FOR

Problems with edges like:
from a node u to ALL nodes in range [l,r]
or from ALL nodes in range [l,r] to a node u
Then run Dijkstra on expanded graph.
CORE IDEA

Build a segment tree as graph nodes:
internal seg nodes connect to children with 0 cost
Range edge becomes O(log n) edges to/from segtree nodes.
Dijkstra runs on this augmented graph.
INPUTS

n original nodes (often 1..n)
operations:
add_edges(l1,r1, l2,r2, cost) depending on implementation
(your code shows add_edges(int l1,int r1,int l2,int r2,int64 c))
OUTPUTS

RunDijkstra(src) returns dist for original nodes
IMPORTANT FUNCTIONS (from class graph_segtree)

graph_segtree(int n)

Input : n original nodes
Output: builds segtree structure graph with extra nodes
void add_edges(int l1,int r1,int l2,int r2,int64 c)

Meaning (based on code):
for each a in range1 and each b in range2 add directed edge a -> b with cost c
implemented by connecting range nodes to seg nodes
Input : ranges + cost
Output: none
vector<int64_t> RunDijkstra(vector src={1})

Input : sources (original node indices)
Output: dist array for original nodes (size n+1 or n)
WHEN TO USE

Range edges shortest path problems (common on Codeforces).
7.15) dijkstra2 (class version with parent + path)
USED FOR

Same as Dijkstra but also stores parent to reconstruct path.
INPUTS

weighted graph g: vector<vector<pair<int,int>>> (to, cost)
src list
OUTPUTS

dis[] distances
get_path(node) returns path from source to node
IMPORTANT FUNCTIONS

dijkstra(g, src={base}) (constructor in your code)

Input : graph + sources
Output: fills:
dis[]
par[] parent
vector get_path(int node)

Output: node list from source to node (empty if unreachable)
7.16) euler_tour (tree flattening)
USED FOR

Flatten a tree to an array so subtree queries become range queries.
Common with segment tree / fenwick: subtree sum, subtree updates.
CORE IDEA

DFS order:
st[u] = entry time index in flat array
en[u] = exit index
subtree of u corresponds to flat[ st[u] .. en[u] ]
INPUTS

tree adjacency g (vector<vector>), typically 1-indexed
root is fixed in code: euler(1,-1)
OUTPUTS

tuple(flat, st, en)
flat: DFS order list of vertices
st[u], en[u]: subtree range boundaries in flat
IMPORTANT FUNCTION

auto euler_tour(vector<vector>& g)
Output: (flat, st, en)
7.17) floyd (Floyd–Warshall)
USED FOR

All-pairs shortest paths.
Also can reconstruct paths.
CORE IDEA

dp[i][j] = min(dp[i][j], dp[i][k] + dp[k][j]) for all k
INPUTS

weighted graph adjacency list
n usually small (<= 400 typical, depending on time)
OUTPUTS

dp matrix distances
get_path(a,b) reconstructs one shortest path (if stored)
IMPORTANT FUNCTIONS (from class floyd)

floyd(const auto& g)

Input : weighted graph
Output: computes dp and next arrays
int64 dis(int a, int b)

Output: shortest distance (INF if unreachable, -INF if negative cycle affects if handled)
vector get_path(int a, int b)

Output: path vertices list (empty if no path)
WHEN TO USE

Small n, many queries, or need all pairs.
7.18) graph_cycle (find a cycle)
USED FOR

Detect and extract one cycle in a graph.
Supports directed/undirected modes (template params).
CORE IDEA

DFS with colors:
0 unvisited, 1 in recursion stack, 2 finished
Back-edge to a node with color=1 => cycle found.
INPUTS

n
add edges with ids
OUTPUTS

get_cycle() returns cycle edges/vertices (depends):
your code shows returning pair<vector, vector>
(edge ids, vertices) or similar.
IMPORTANT FUNCTIONS (from class GraphCycle)

GraphCycle(int n)
void add_edge(int u, int v, int id)
Input : endpoints + edge id
pair<vector, vector> get_cycle()
Output: one found cycle, or empty if none
WHEN TO USE

“find any cycle”, “check DAG”, “extract cycle for answer”.
7.19) graph_segtree (same concept as 7.14, another file section)
This is essentially the segment-tree graph builder described above.
Use the same API notes as 7.14.

7.20) online_bridges (dynamic add edges, maintain #bridges)
USED FOR

Online graph where edges are added one by one.
Maintain:
whether an edge is a bridge right now
number of bridges
2-edge-connected components structure
CORE IDEA (HIGH LEVEL, simple)

Maintain DSU for connected components and DSU for 2-edge-connected components.
When adding edge(u,v):
if u and v in different connected components: new bridge added
else: adding edge creates a cycle -> some bridges on the path stop being bridges
This is classic “online bridges” algorithm (CP-algorithms).
INPUTS

n nodes
add_edge(u,v) operations
OUTPUTS

count_bridges()
is_bridge(u,v) (if you track specific edge; your code shows is_bridge(u,v) by checking 2ECC roots)
IMPORTANT FUNCTIONS (from shown class OnlineBridge)

OnlineBridge(int n)

Input : n
Output: initializes DSU arrays
void add_edge(int u, int v)

Input : edge endpoints
Output: none
Effect: updates bridge count and DSU structures
bool is_bridge(int u, int v)

Output: true if u-v is currently a bridge in maintained structure
Note: meaning depends on how you query; in your snippet:
return find_2ecc(u) != find_2ecc(v)
int count_bridges()

Output: current number of bridges
WHEN TO USE

Problems: incremental connectivity + bridge count queries online.
7.21) reachability (SCC + bitset reach on DAG)
USED FOR

Fast reachability queries in directed graph:
can_reach(a,b)?
Also count how many nodes reachable from a.
CORE IDEA (SIMPLE)

Compute SCC -> compress to DAG of components.
DP on DAG in reverse topological order using bitsets:
canComp[c] bitset of components reachable from c.
For node queries, use component ids.
INPUTS

directed graph g
template parameter Nodes (max nodes / bitset size)
OUTPUTS

can_reach(a,b)
count_reachable(a)
IMPORTANT FUNCTIONS (from your class)

reachability(vector<vector> g)

Input : graph
Output: builds SCC, DAG, and bitset DP
bool can_reach(int a, int b)

Input : original nodes a,b
Output: true if path a -> b exists
int count_reachable(int a)

Output: number of nodes/components reachable from a (based on bitset count)
ASSUMPTIONS

Needs bitset size fixed at compile-time (Nodes template).
Works when Nodes is not too large (e.g., <= 5000..20000 depending memory).
7.22) topo_sort
USED FOR

Topological sorting of a directed graph.
Also detect cycles (if not all nodes included).
CORE IDEA

DFS ordering:
push node after exploring children
reverse at end
Detect back-edge using recursion stack “check[]” -> cycle.
INPUTS

directed graph g (vector<vector>), usually 1-indexed
OUTPUTS

vector order (topological order)
if cycle exists: in your function it sets ok=0; may still return something but invalid.
IMPORTANT FUNCTION

vector topo_sort(vector<vector>& g)
Input : graph
Output: topological order (reverse finish time)
Cycle detection: uses check[]; if cycle, marks ok=false.
WHEN TO USE

DAG DP, scheduling, prerequisite problems.