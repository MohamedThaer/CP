3.1) 2SAT
USED FOR

Solve boolean constraints where each clause has 2 literals: (A OR B)
Typical: “choose one”, “if X then Y”, “not both”
INPUTS

n = number of boolean variables (0..n-1)
Clauses / implications
OUTPUTS

satisfiable? (bool)
assignment (vector) if satisfiable
ASSUMPTIONS

Each variable x has two nodes: x (true) and ~x (false)
IMPORTANT FUNCTIONS (API)

addClause(int u, bool fu, int v, bool fv)

Meaning: (u is fu) OR (v is fv)
Input:
u, v : variable indices
fu : desired value for u inside the literal (true means u, false means ~u)
fv : same for v
Output: none (adds edges in implication graph)
Why: standard way to add a 2-SAT clause
implies(int u, bool fu, int v, bool fv)

Meaning: (u is fu) -> (v is fv)
Input: u,fu,v,fv
Output: none
Why: easier to model “if X then Y”
assertVar(int u, bool fu)

Meaning: force variable u to be fu
Input: u, fu
Output: none
Implementation idea: add implication (~(u=fu) -> (u=fu))
satisfiable()

Input: none (uses built graph)
Output: bool
Why: runs SCC; checks if x and ~x are in same SCC
get_assignment()

Input: none (after satisfiable)
Output: vector assignment of size n
Why: returns one valid assignment
SMALL EXAMPLE

Variables: x0, x1
Clause: (x0 OR x1): addClause(0,true, 1,true)
Force x0=false: assertVar(0,false)
Then satisfiable() and get_assignment()
WHEN TO USE

Any problem reducible to (literal OR literal)
3.2) 3SAT (practical / limited)
USED FOR

3 literals per clause (hard in general); template is usually for small/heuristic cases.
INPUTS

n variables
clauses with 3 literals
OUTPUTS

possibly an assignment / satisfiable flag (not guaranteed for big worst-cases)
IMPORTANT FUNCTIONS (typical)

addClause(lit a, lit b, lit c)
Input: 3 literals (var + sign)
Output: none
solve()/satisfiable()
Output: bool (maybe), assignment (maybe)
get_assignment()
Output: vector
NOTE

Use only if constraints small or problem guarantees it works.
3.3) CHT (Convex Hull Trick) — dynamic lines
USED FOR

DP: dp[i] = min_j (m[j]*x[i] + c[j]) (or max)
INPUTS

Lines y = m*x + c
Query x
OUTPUTS

best y at x (min or max)
ASSUMPTIONS

Your version looks like “dynamic CHT with multiset” (arbitrary insert, arbitrary query)
IMPORTANT FUNCTIONS (API)

CHT(Mode mode)

Input: mode = Min or Max
Output: object
add(int64 m, int64 c)

Input: slope m, intercept c
Output: none
Why: insert a new line
query(int64 x)

Input: x
Output: int64 bestValue (min/max)
Why: get optimum line value at x
EXAMPLE

cht = CHT(Min)
cht.add(2,1) // y=2x+1
cht.add(-1,10) // y=-x+10
ans = cht.query(3) -> 7
WHEN NOT TO USE

If formula isn’t linear (m*x+c)
3.4) DSU (Union-Find)
USED FOR

Connected components, Kruskal MST, grouping
INPUTS/OUTPUTS

merge(u,v) connects sets
same_set(u,v) checks connectivity
IMPORTANT FUNCTIONS (API)

DSU(int n)

Input: n nodes (0..n-1)
Output: DSU initialized with n components
find_par(int x)

Input: node x
Output: representative root of its set
same_set(int a, int b)

Output: bool
merge(int a, int b)

Output: none (or bool if merged)
Effect: unions sets
get_size(int x) (if exists)

Output: size of component containing x
comp_cnt() (if exists)

Output: number of current components
3.5) Monostack
USED FOR

next greater/smaller element, prev greater/smaller
histogram, subarray contribution problems
INPUT

array v (size n)
OUTPUT

arrays:
prev[i] = index of previous element satisfying condition
next[i] = index of next element satisfying condition
IMPORTANT COMPONENTS (API)

Monostack(vector& v, Compare cmp)

Input:
v : array
cmp : decides order (greater/less, etc.)
Output:
fills/creates prev[], next[]
prev / next vectors

Output: indices (or -1 if none)
EXAMPLE

For next smaller:
Compare = greater so we pop while v[top] > v[i]
3.6) Wavelet Tree
USED FOR (static array queries)

kth smallest in subarray
count of <= k in subarray
sometimes range sum by value
INPUTS

array v (static)
queries (l,r,k)
OUTPUTS

kth element / count / sums
ASSUMPTIONS

Usually static (no updates)
values in some range [lo..hi] or compress
IMPORTANT FUNCTIONS (API)

WaveletTree(vector v, lowValue, highValue)

Input: array + value range
Output: built tree
kth_order(int l, int r, int k)

Input:
l,r : subarray range (often 1-indexed in wavelet implementations; check your usage)
k : 1-based order (k=1 -> smallest)
Output: kth smallest value in v[l..r]
LTE(int l, int r, int64 k)

Output: count of elements <= k in v[l..r]
count(int l, int r, int64 k)

Output: count of elements == k in v[l..r] (some versions)
sum_k / sum (if present)

Output: sum of smallest k elements, or sum of <=k, depending on implementation
3.7) bigint
USED FOR

Exact integer arithmetic beyond 64-bit
INPUT/OUTPUT

construct from int/string
output to string
operators return bigint
IMPORTANT FUNCTIONS / OPERATORS (API)

bigint(int64 x=0)

Input: x
Output: bigint
to_bigint(string s, int base_in=10)

Input: string number, base
Output: bigint
to_string(int new_base=10)

Output: string
Operators:

a + b, a - b, a * b -> bigint
a / b, a % b -> bigint (if implemented)
comparisons: <, <=, ==, ...
3.8) disjoint_sparse_table
USED FOR

Static range query O(1) with associative merge (not necessarily idempotent)
INPUT

array a
merge function
OUTPUT

query(l,r)
ASSUMPTIONS

no updates
merge is associative
IMPORTANT FUNCTIONS (API)

disjoint_sparse_table(vector a, Merge merge)

Input: array, merge functor
Output: built table
query(int l, int r)

Input: l,r (inclusive)
Output: merged value over [l,r]
3.9) dynamic connectivity (offline)
USED FOR

Add/remove edges + connectivity queries (offline)
INPUT

operations sequence:
add edge(u,v)
remove edge(u,v)
query connected(u,v)
query component count (optional)
OUTPUT

list of answers in query order
ASSUMPTIONS

offline only (need full list first)
uses rollback_dsu inside
IMPORTANT FUNCTIONS (API)

DynamicConnectivity(int num_nodes)

Output: object
add_edge(int u, int v)

Input: endpoints
Output: none
Meaning: edge becomes active from now
remove_edge(int u, int v)

Meaning: edge becomes inactive from now
query_is_connected(int u, int v)

Output: none immediately; stored as query
query_component_count()

Output: stored query
run()

Output: vector / vector answers (depends on query types)
3.10) dynamic connectivity (online) (Euler Tour Tree / Splay)
USED FOR

Online dynamic forest:
link, cut, connected, size (sometimes)
INPUT/OUTPUT

link(u,v): connect two trees
cut(u,v): remove edge
same(u,v): are in same tree?
ASSUMPTIONS

usually forest only (no general graph cycles unless extra checks)
IMPORTANT FUNCTIONS (API)

dynamic_connectivity(int n)
bool same(int u, int v)
bool link(int u, int v)
Output: true if linked (false if already connected)
bool cut(int u, int v)
Output: true if edge existed and removed
int size(int u) (if present)
Output: size of tree containing u
3.11) fraction
USED FOR

Exact rational arithmetic without floating errors
INPUT

numerator, denominator
OUTPUT

reduced fraction, exact comparisons
IMPORTANT FUNCTIONS (API)

fraction(M n=0, M d=1)

Input: n,d
Output: normalized fraction
Operators:

+,-,*,/ return fraction
comparisons return bool
Printing:

outputs n/d
3.12) hash_map (gp_hash_table + splitmix64)
USED FOR

Faster + safer hashmap than unordered_map
INPUT/OUTPUT

Same as unordered_map operations
IMPORTANT COMPONENTS (API)

custom_hash(uint64 x) -> uint64
HashMap<K,V> type alias
HashSet type alias
USAGE EXAMPLE

HashMap<int,int> mp;
mp[x]++;
3.13) implicit_treap (sequence treap)
USED FOR

Dynamic array operations in O(log n):
insert/erase/reverse/range add/range sum (depending on node fields)
INPUT

positions (indices), ranges
OUTPUT

query values (sum/min/etc), updated sequence
IMPORTANT FUNCTIONS (API)

split(node* t, int k, node*& a, node*& b)

Input:
t = root
k = number of elements to put in 'a'
Output:
a = first k elements
b = remaining
merge(node* a, node* b)

Input: two treaps where all elements of a come before b
Output: merged root
insert(pos, val)

Input: position, value
Output: none (updates root)
erase(l, r)

Input: range
Output: none
reverse(l, r)

Input: range
Output: none
query(l, r)

Output: aggregate on range (sum in your shown code)
push()/pull()

push(): apply lazy tags to children
pull(): recompute size/sum from children
3.14) indexed_set (PBDS order statistics tree)
USED FOR

ordered set with:
k-th element
count < x
IMPORTANT FUNCTIONS (API)

insert(x)
erase(x)
order_of_key(x) -> int (#elements < x)
find_by_order(k) -> iterator to k-th (0-index)
OUTPUTS

order_of_key returns int
*find_by_order returns value
NOTE

For duplicates: store (value, unique_id)
3.15) li_chao_tree
USED FOR

Min/max of lines at x, very robust
INPUT

insert line (m,b)
query x
OUTPUT

best value at x
ASSUMPTIONS

x domain known (e.g., [0..1e6]) or compressed
IMPORTANT FUNCTIONS (API)

insertLine(m, b)

Output: none
addLine(m,b, l,r) (if range insertion exists)

Output: none
Query(x)

Output: min/max y
3.16) linkcut_tree (LCT)
USED FOR

Dynamic trees online + path queries/updates
INPUT/OUTPUT (common API)

make_root(u) -> none
connected(u,v) -> bool
link(u,v) -> none/bool
cut(u,v) -> none/bool
lca(u,v) -> int (if provided)
path query/update (depends on stored values)
ASSUMPTIONS

Complex; use only if required.
3.17) mex (minimum excluded) structure
USED FOR

Maintain multiset counts and get mex quickly
INPUT

add(x)
remove(x)
OUTPUT

get_mex() -> smallest non-present integer
ASSUMPTIONS

x in [0..N] where N fixed in template
IMPORTANT FUNCTIONS (API)

add(int x) -> none
remove(int x) -> none
get_mex() -> int
3.18) monotonic queue
USED FOR

Sliding window min/max, DP window optimization
INPUT

push(x)
pop() (when oldest element leaves)
get()
OUTPUT

get() -> current min/max
IMPORTANT FUNCTIONS (API)

Push(T x) -> none
Pop() -> none
Get() -> T
Empty() -> bool
Size() -> int
3.19) multiset hashing
USED FOR

Compare multisets fast (with duplicates)
INPUT

add(x), remove(x)
OUTPUT

get_hash() / current_hash (uint64)
ASSUMPTIONS

randomized; tiny collision probability
IMPORTANT FUNCTIONS (API)

add(uint64 val) -> none
remove(uint64 val) -> none
get_hash() -> uint64
operator== compares hashes
3.20) ordered_set (alias)
Same as indexed_set:

insert, erase
order_of_key(x) -> int
find_by_order(k) -> iterator
3.21) ordered_set_erase (helper)
USED FOR

erase by rank or erase by order_of_key
IMPORTANT ACTIONS

it = os.find_by_order(k) (Input k, Output iterator)
os.erase(it) (Input iterator, Output none)
3.22) rollback_dsu
USED FOR

DSU with undo (offline dynamic connectivity, divide&conquer on time)
INPUT/OUTPUT

merge(a,b)
snapshot()
rollback_to(snapshot)
IMPORTANT FUNCTIONS (API)

rollback_dsu(int n)

Output: object
find_par(x)

Output: representative (often without full path compression)
same_set(a,b)

Output: bool
merge(a,b)

Output: none/bool
Side effect: pushes changes onto history stack
snapshot()

Output: int token (history size)
rollback_to(int snap)

Input: snap token
Output: none
Effect: undo operations until history size == snap
comp_cnt()

Output: int components
3.23) rope
USED FOR

Many insert/erase in middle of a sequence/string
IMPORTANT OPERATIONS (API)

insert(pos, value or range)
erase(pos, len)
substr(pos, len) -> rope
operator[] (sometimes)
OUTPUT

substr returns piece
printing iterates all
3.24) sparse_table (1D)
USED FOR

Static RMQ: min/max/gcd
INPUT

array a
merge function (usually min/max/gcd)
OUTPUT

query(l,r)
ASSUMPTIONS

static
for O(1) query usually idempotent merge
IMPORTANT FUNCTIONS (API)

sparse_table(vector a, Merge merge)
query(int l, int r)
Output: merge over [l,r]
3.25) sparse_table_2d
USED FOR

Static 2D min/max queries in submatrix
INPUT

matrix a[n][m]
query(x1,y1,x2,y2)
OUTPUT

min/max on sub-rectangle
IMPORTANT FUNCTIONS (API)

constructor/build
query(x1,y1,x2,y2) -> value
3.26) sqrt_tree
USED FOR

Static range queries with associative merge, O(1) query (complex build)
INPUT

array a
merge op
OUTPUT

query(l,r)
IMPORTANT FUNCTIONS (API) (as appears)

build(...)
query(l,r) -> value
update(x, item) (if supported in your version) -> none
ASSUMPTIONS

merge must be associative
3.27) tc_arr (time-clearing array)
USED FOR

Fast reset of large arrays without O(n) fill each test
INPUT/OUTPUT

operator[] gives reference and marks index used
reset() clears only used indices
IMPORTANT FUNCTIONS (API)

tc_arr(int N, M reset_val = 0)

Input: N size, default value
Output: object with array[0..N-1] logically = reset_val
M& operator[](int idx)

Input: idx
Output: reference to arr[idx]
Side effect: marks idx as used (so reset() knows to clear it)
reset()

Output: none
Effect: sets all used indices back to reset_val and unmarks them
EXAMPLE

tc_arr seen(n, 0);
seen[5] = 1;
seen.reset(); // only index 5 is cleared
3.28) treap (explicit key treap)
USED FOR

Ordered set/map with split/merge, expected O(log n)
INPUT

keys (or nodes), operations insert/erase/split/merge
OUTPUT

updated root
queries like kth / sum (if augmented)
IMPORTANT FUNCTIONS (API)

merge(node* left, node* right) -> node*

Input: two treaps where all keys in left < keys in right
Output: merged treap root
split(node* t, key, node*& a, node*& b)

Output:
a: keys <= key (or < key depending implementation)
b: remaining
insert(key, value?) -> none/root

erase(key) -> none/root

pull() / push() (if lazy/augmented)

pull recomputes size/sum
push propagates lazy to children




