6.1) BipartiteMatching (Hopcroft–Karp)
USED FOR

Maximum matching in bipartite graph (unweighted).
Typical: assign tasks to people, pair left-right nodes, grid matching.
CORE IDEA (SIMPLE)

Alternating BFS + DFS on “layered graph”.
BFS finds shortest augmenting paths length.
DFS sends augmenting paths respecting layers.
Complexity: O(E * sqrt(V)).
INPUTS

Left side size n, Right side size m.
Edges from left -> right.
OUTPUTS

max matching size
matching pairs (optional)
IMPORTANT FUNCTIONS (from your template style)

HopcroftKarp(int _n, int _m)

Input : n (left count), m (right count)
Output: object, internal graph sized n+m
void add_edge(int u, int v)

Input :
u in [0..n-1] (left)
v in [0..m-1] (right)
Output: none
Why : adds bipartite edge u -> v
int max_matching()

Input : none
Output: int = maximum matching size
vector<pair<int,int>> get_matching()

Input : none (call after max_matching)
Output: list of matched pairs (u,v) using 0-indexed left/right
WHEN TO USE / NOT

Use: big bipartite graph, need fast maximum matching.
Avoid: weighted costs (use Hungarian or MinCost).
6.2) Hungarian (Assignment Problem)
USED FOR

Minimum/Maximum cost perfect matching in bipartite graph with costs matrix.
Standard assignment: n workers, n jobs, cost[i][j].
CORE IDEA (SIMPLE)

Maintains potentials (u, v) + alternating tree.
Finds augmenting path with reduced costs.
Complexity: O(n^3).
INPUTS

n1, n2 (usually n1==n2 for perfect assignment)
Costs c[i][j]
OUTPUTS

best total cost
matching (which left matched to which right)
ASSUMPTIONS

Costs defined for every pair (or set big cost for missing edges).
Your template uses Mode Min/Max (by multiplying cost with mode sign).
IMPORTANT FUNCTIONS (as in your code)

Hungarian(int n1, int n2, Mode mode)

Input :
n1 left size, n2 right size
mode = Min or Max
Output: object
void add_edge(int u, int v, cost)

Input :
u in [0..n1-1], v in [0..n2-1]
cost value (int64)
Output: none
Why: sets cost matrix entry (keeps best if multiple)
cost_type BestMatching()

Input : none
Output: best total cost (min or max based on mode)
vector<pair<int,int>> get_matching()

Output: pairs (u,v) matched
WHEN TO USE / NOT

Use: assignment with costs and n <= ~400 (depends).
Avoid: huge sparse graphs (use min-cost flow if sparse, but slower constants).
6.3) MCMF (Min-Cost Max-Flow)
USED FOR

Send flow from source to sink with minimum cost.
Problems: minimum cost to send K units, min-cost matching, transportation, etc.
CORE IDEA (SIMPLE)

Residual graph edges with (cap, cost).
Repeatedly find cheapest augmenting path (shortest path by cost) while capacity exists.
Uses potentials (Johnson) + Dijkstra for non-negative reduced costs.
If negative edges exist: initialize potentials using Bellman-Ford.
INPUTS

Directed graph with capacities and costs.
Source s, sink t.
Optional: required maxFlow.
OUTPUTS

pair(cost, flow)
can also return used flow edges / paths
ASSUMPTIONS

Costs can be negative, but no negative cycles reachable in residual (otherwise unbounded).
Capacities typically int.
IMPORTANT FUNCTIONS (matching your template)
STRUCT Edge fields

to: destination
cost: edge cost per unit
cap: capacity
flow: current flow
rev: index of reverse edge in adjacency
MCMF(int n)

Input : n nodes
Output: object with empty graph
void addEdge(int u, int v, int64 cost, int cap)

Input :
u, v endpoints
cost per unit
cap capacity
Output: none
Effect:
adds forward edge (u->v) and reverse edge (v->u) with -cost
pair<int64,int> MinCostMaxFlow(int s, int t, int maxFlow=1e9, bool has_negative_costs=false)

Input :
s source, t sink
maxFlow = limit flow to send
has_negative_costs: if true, run Bellman-Ford for initial potentials
Output:
(total_cost, total_flow_sent)
vector<tuple<int,int,int,int64>> getEdgesWithFlow() const

Output: list of edges with positive flow:
(u, v, flow, cost)
vector<vector> get_paths()

Output: decomposes positive flow into s->t paths (useful for printing)
Note: works after flow computed.
INTERNAL IMPORTANT HELPERS (why they exist)

dijkstra(s, t, d, p, pe)
finds shortest path by reduced costs
calculatePotentials(s)
Bellman-Ford to init potentials if negative costs
updatePotentials(d)
add distances to potentials
WHEN TO USE / NOT

Use: min-cost flow, min-cost matching with extra constraints.
Avoid: if just max flow (use Dinic/PushRelabel).
Avoid: huge graphs with big costs if time tight.
6.4) MaxCostAntiChain (DAG)
USED FOR

Maximum cost antichain in a DAG (a set of vertices with no path between them).
Classic reduction to min-cut/max-flow on bipartite split graph.
CORE IDEA (SIMPLE)

Split each DAG node v into left(v) and right(v).
Add edge source->left(v) with cost/weight (capacity=cost)
Add edge right(v)->sink with cost (capacity=cost)
For each DAG edge u->v, add infinite capacity edge left(u)->right(v)
Then:
max antichain cost = total_cost - min_cut = total_cost - max_flow
INPUTS

n nodes
cost[v] for each node
DAG edges u->v
OUTPUTS

MaxCost() returns maximum antichain total weight
IMPORTANT FUNCTIONS (from your class)

MaxCostAntiChain(int n, vector cost)

Input : n, node costs
Output: builds base flow network and stores total cost
void add_edge(int u, int v)

Input : directed edge u->v in DAG
Output: none
Effect: adds INF capacity edge in flow network
int64 MaxCost()

Output: maximum antichain total cost
ASSUMPTIONS

Graph must be DAG (template comment says “make sure no cycles occur”).
6.5) MinCostVertexCover (Bipartite, costs on vertices)
USED FOR

Minimum cost vertex cover in bipartite graph with costs on vertices.
Equivalent to min s-t cut in a constructed network.
CORE IDEA (SIMPLE)

Make flow network:
source -> left node u capacity = costL[u]
right node v -> sink capacity = costR[v]
for each bipartite edge (u,v): left(u) -> right(v) capacity = INF
Min vertex cover cost = min cut value.
The actual cover set can be obtained from min-cut partition.
INPUTS

n left size, m right size
costs arrays costL, costR
edges (u,v)
OUTPUTS

MinCost() = minimal cover cost
GetVertexCoverSet() returns (LeftCoverNodes, RightCoverNodes)
GetIndependentSet() returns complement (max weight independent set in bipartite)
IMPORTANT FUNCTIONS (from your class)

MinCostVertexCover(int n, int m, vector costL, vector costR)

builds MCMF / maxflow network (your code uses max_flow mf)
stores total = sum(costL)+sum(costR)
void add_edge(int u, int v)

add bipartite edge u->v (internally INF capacity)
int64 MinCost()

Output: min cut value (runs maxflow once)
int64 MaxCostIndependentSet()

Output: total - MinCost()
pair<vector, vector> GetVertexCoverSet()

Output:
left list, right list of nodes in minimum vertex cover
pair<vector, vector> GetIndependentSet()

Output:
nodes NOT in cover (left,right)
6.6) bipartite_matching (using maxflow)
USED FOR

Maximum matching by building unit-capacity flow network.
CORE IDEA

source->left cap=1
left->right cap=1 for edges
right->sink cap=1
maxflow = max matching
INPUTS

n left, m right
edges
OUTPUTS

max matching size
list of matches
min vertex cover (optional using reachability in alternating graph)
IMPORTANT FUNCTIONS (from your class)

bipartite_matching(int n, int m)
void add_edge(int u, int v)
int max_matching()
vector<array<int,2>> get_matching()
pair<vector, vector> min_vertex_cover()
returns sets (Lcover, Rcover)
6.7) bipartite_matching (Dinic)
USED FOR

Same as 6.6 but uses Dinic maxflow as backend (faster general maxflow).
IMPORTANT FUNCTIONS

constructor(n,m)
add_edge(u,v)
max_matching() -> int
get_matching() -> vector<array<int,2>>
6.8) kuhn (DFS matching)
USED FOR

Maximum bipartite matching (simpler, slower).
Good when constraints smaller (e.g., n<=2e3, E<=2e5 sometimes ok).
CORE IDEA

Try to match each left node with DFS augmenting path.
Complexity: O(VE) worst-case.
IMPORTANT FUNCTIONS (from your class)

Kuhn(int n, int m)
void add_edge(int u, int v)
int max_matching()
vector<pair<int,int>> get_matching()
pair<vector, vector> min_vertex_cover()
Uses alternating graph reachability after matching.
vector get_reachable_nodes()
Output: vis array on combined graph for min vertex cover / antichain.
6.9) max_flow (Dinic-style in your template)
USED FOR

Max flow in directed graph.
CORE IDEA

BFS builds levels
DFS sends blocking flows
IMPORTANT FUNCTIONS (typical from your class)

max_flow(int n)
void add_edge(int u, int v, int64 cap)
int64 flow(int s, int t)
Output: max flow value
vector min_cut()
Output: nodes reachable from s in residual after maxflow
vector edges()
Output: all edges (for extracting used flow)
vector mincut_edges()
Output: edges crossing min cut
vector<vector> get_paths(s,t)
Output: decomposition of flow into paths (after flow)
6.10) max_flow (MPM)
USED FOR

Another maxflow algorithm (Malhotra–Pramodh–Maheshwari).
Good on dense graphs sometimes.
IMPORTANT FUNCTIONS (from your shown class)

MPM(int n, int s, int t) or resize(n)
void add_edge(u,v,cap)
int64 flow()
Output: max flow
NOTES

More complex than Dinic; usually Dinic is enough.
6.11) maxflow (Dinic) (another implementation)
Same purpose as 6.9: max flow.

IMPORTANT FUNCTIONS

Dinic(n, s, t)
add_edge(from, to, cap)
flow() -> int64
6.12) maxflow (Edmonds–Karp)
USED FOR

Simple max flow using BFS augmenting paths.
Complexity: O(V * E^2) (slow), but very easy & safe for small graphs.
IMPORTANT FUNCTIONS

EdmondsKarp(int n)
add_edge(u,v,cap)
maxflow(s,t) -> int64
USE WHEN

V <= ~500 and E small/moderate.
6.13) maxflow (Push-Relabel)
USED FOR

Very fast max flow in practice on many graphs (especially dense).
Complexity good in practice.
IMPORTANT FUNCTIONS (from your class)

PushRelabel(int n)
addEdge(s, t, cap, rcap=0)
maxflow(s,t) -> int64
leftOfMinCut(a) -> bool (if present)
tells if node is on source side of min cut.
6.14) maxflow theorems (notes)
Contains common reductions:

In DAG: maximum antichain via max bipartite matching (Dilworth/Kőnig)
In bipartite graphs:
min vertex cover size = max matching (Kőnig)
max independent set = |V| - min vertex cover
(These are theory notes used to choose correct reduction.)

6.15) minCostBipartiteMatching
USED FOR

Minimum cost matching in bipartite graph (often perfect or size k)
Built using MCMF:
source->left cap1 cost0
left->right cap1 cost=given
right->sink cap1 cost0
IMPORTANT FUNCTIONS (from your class)

minCostBipartiteMatching(int n, int m)
void add_edge(int u, int v, int c)
cost c for matching u with v
auto max_matching(int k=1e9)
Output: pair(cost,flow) from MCMF (depends on wrapper)
vector<array<int,2>> get_matching()
Output: matched edges (u,v) after running flow
resetState()
resets “used” flags if your Euler path/other graph shares state (in your file it appears near EulerianGraph, ignore if unrelated)
WHEN TO USE

When you need min-cost matching and constraints allow flow.
If n is square and dense, Hungarian may be simpler/faster.