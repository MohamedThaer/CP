4.1) DP SOS (Subset DP / Sum Over Subsets)
USED FOR

Given A[mask], compute F[mask] = sum of A[sub] for all sub ⊆ mask
Also used for counting submasks, subset convolution helpers, bitmask DP speedups
CORE IDEA (SIMPLE)

For each bit i:
if mask has bit i, then subsets of mask are either:
subsets not using bit i (mask without bit i)
subsets using bit i
Classic transition:
F[mask] = F[mask] + F[mask ^ (1<<i)] when (mask has bit i)
INPUTS

A: array/vector of size 2^N (N bits), A[mask] is initial value
OUTPUTS

F: array/vector size 2^N where:
F[mask] = Σ A[submask] for all submask ⊆ mask
CONSTRAINTS / ASSUMPTIONS

N usually <= 20..22 for 2^N memory/time
Time: O(N * 2^N)
IMPORTANT COMPONENTS (as shown in template comments)
A) Iterative SOS DP (most common)

Input: A[mask]
Output: F[mask] (often reuse same array)
Pseudocode:
F = A
for i in [0..N-1]:
for mask in [0..2^N-1]:
if (mask & (1<<i)):
F[mask] += F[mask ^ (1<<i)]
B) Recursive memo (less used in contests)

solve(mask, i) returns SOS value using bits up to i
SMALL EXAMPLE (N=2)
A[00]=1, A[01]=2, A[10]=3, A[11]=4
F[00]=1
F[01]=A00+A01=3
F[10]=A00+A10=4
F[11]=A00+A01+A10+A11=10

WHEN TO USE

Any time you see: “sum over all submasks” / “count submasks”
WHEN NOT TO USE
N too large (2^N not possible)
4.2) SOS (Zeta / Möbius transforms for masks)
USED FOR

Fast transformations on mask arrays:
sum over submasks
sum over supermasks
invert those sums (recover original) using Möbius
Common in:
inclusion–exclusion on subsets
counting problems on bitmasks
subset convolution / OR-convolution helpers
CORE IDEA (SIMPLE)

Zeta transform = “prefix sums” on subset lattice
Möbius transform = inverse of zeta (like undoing prefix sums)
HELPER: power_two_size(n)

Purpose: find B such that size becomes 2^B
Input : n (array size)
Output: B = log2(next_power_of_two(n))
Why : transforms expect size is power of two
A) zeta_transform (submasks)
FUNCTION
template
vector zeta_transform(vector f)

MEANING

After: f[mask] = Σ original_f[sub] for all sub ⊆ mask
INPUT

f: vector size M (will be resized to power-of-two)
OUTPUT

transformed vector f (same vector returned)
ASSUMPTIONS

Operation is “+” in template (can change to min/max/gcd if valid)
WHEN USED

“sum over submasks” queries in O(1) after transform
B) zeta_transform_for_supermasks
MEANING

After: f[mask] = Σ original_f[super] for all super ⊇ mask
INPUT/OUTPUT

Same style as zeta_transform
C) mobius_transform (inverse of submask zeta)
MEANING

If f currently equals submask-zeta(original),
then mobius_transform(f) returns original.
INPUT

f: vector that is already zeta-transformed
OUTPUT

original values
D) mobius_transform_for_supermasks (inverse of supermask zeta)
Same idea but for supermasks.

E) inverse_zeta_transform / inverse_mobius_transform
Convenience wrappers:
inverse_zeta_transform(f) typically = mobius_transform(f)
inverse_mobius_transform(f) typically = zeta_transform(f)
(Depending on naming in your file: they are “undo” functions.)
F) No-empty-subset helpers (appears in your code)
zeta_and_subsets / or_subsets style functions
Used when you want to exclude empty subset or apply constraints like:
“subset must be non-empty”, “subset must intersect”, etc.
They usually:
transform
modify each f[mask]
inverse transform
SMALL EXAMPLE (submask zeta)
f size 4 (N=2):
f[0]=1,f[1]=2,f[2]=3,f[3]=4
zeta -> [1, 3, 4, 10] (same as DP SOS example)

WORKFLOW TIP

If you need many “sum over submasks(mask)” queries:
do zeta_transform once, then answer is f[mask] in O(1).
4.3) dp_dac (Divide & Conquer DP Optimization)
USED FOR
DP of form:
dp[i][j] = min over k<=j ( dp[i-1][k-1] + cost(k, j) )
(Or k< = j depending on indexing)
Typical: partition array into K groups minimizing cost.

COMMON PROBLEMS

“Divide array into k segments minimize cost”
DP with quadrangle inequality / monotone opt condition
Examples: IOI Aliens trick family, CSES “Array Division”-like (with right conditions), many classic “partition DP”
CORE IDEA (SIMPLE)

If the best split point opt[i][j] is monotone:
opt[i][j] <= opt[i][j+1]
Then we can compute each DP row in O(n log n) using divide&conquer.
CLASS INTERFACE (from your template)
class DP_DAC

CONSTRUCTOR
DP_DAC(int n, int k, const auto& cost)

INPUTS

n: length of array (0-indexed positions 0..n-1)
k: number of divisions/groups
cost(l, r): function returning cost of segment [l..r]
(in your code it is called as cost(k, j) etc)
OUTPUT

builds dp table dp[k][n] internally
IMPORTANT FUNCTIONS

compute(int i, int l, int r, int optl, int optr)
Purpose: compute dp[i][mid] for mid in [l..r]
Inputs:
i : which DP layer (number of groups used)
l,r : range of j indices to compute
optl,optr: allowed range of optimal k
Output:
none (fills dp[i][mid])
Why needed:
divide & conquer recursion that reduces search range using monotonicity.
int64_t solve(int i, int k)
In your snippet: solve(i,k) returns dp[k-1][i]
Meaning: minimal cost to partition prefix [0..i] into k groups
Input:
i: last index of prefix
k: number of groups
Output:
dp value
ASSUMPTIONS (IMPORTANT)

The DP must satisfy monotone opt:
opt[i][j] <= opt[i][j+1]
cost must be computable fast (often O(1) or O(log n))
Typical constraints: n up to 2e5 with fast cost
TINY EXAMPLE IDEA

Suppose cost(l,r) = (sum of a[l..r])^2
dp[g][j] = min split
If monotonicity holds, dp_dac speeds it up.
WHEN TO USE / NOT

Use when you KNOW monotone opt holds.
Don’t use if unsure: wrong assumption => wrong answers.
4.4) dp_knuth (Knuth Optimization)
USED FOR
Interval DP of form:
dp[i][j] = min over k in [i..j-1]:
dp[i][k] + dp[k+1][j] + cost(i, j)

COMMON PROBLEMS

Optimal BST
Optimal merge / file merging
Some “divide interval” problems (when Knuth conditions hold)
CORE IDEA (SIMPLE)

If quadrangle inequality / Knuth condition holds, then:
opt[i][j-1] <= opt[i][j] <= opt[i+1][j]
So we only search k in a SMALL range, making O(n^2).
CLASS INTERFACE (from your template)
class DP_Knuth

CONSTRUCTOR
DP_Knuth(int n, const auto& cost)

INPUTS

n: number of items (0..n-1)
cost(i,j): cost to merge interval [i..j] (depends on problem)
OUTPUT

dp[i][j] minimal cost for interval
opt[i][j] stores best k
IMPORTANT FUNCTIONS / COMPONENTS

dp (2D)
dp[i][j] = best cost for interval [i..j]
type: int64
opt (2D)
opt[i][j] = argmin k for dp[i][j]
solve(int i, int j)
Input : interval endpoints i, j
Output: dp[i][j]
ASSUMPTIONS (VERY IMPORTANT)

Knuth optimization conditions must hold.
Typically cost must satisfy quadrangle inequality and monotone property.
WHEN TO USE / NOT

Use only for classic Knuth-suitable problems.
If conditions don’t hold => incorrect.
4.5) dp_sos_class (SOS precomputation class)
USED FOR

Precompute quickly:
sum over submasks
sum over supermasks
some “zero_and” style queries
This is basically SOS transforms wrapped in a class.
CLASS (from your screenshot)
template
class DP_SOS

INPUT

vector v (list of masks/values) OR array counts (depends on usage)
Your shown code increments:
F1[v[i]]++
F2[inv(v[i])]++
So it’s building frequency tables.
OUTPUT QUERIES

submasks_ans(mask)
zero_and(mask)
supermasks_ans(mask)
ASSUMPTIONS

LG fixed, masks in [0 .. (1<<LG)-1]
IMPORTANT FUNCTIONS (API)

DP_SOS(vector& v)
Input : list of masks
Output: builds internal arrays
inv(int x)
Input : x mask
Output: bitwise complement within LG bits:
inv(x) = x xor ((1<<LG)-1)
Why : used to convert supermask queries into submask queries or vice versa.
int64_t submasks_ans(int mask)
Output: number of elements u in v such that u is a submask of mask:
u ⊆ mask (i.e., (u & mask) == u)
int64_t supermasks_ans(int mask)
Output: number of elements u in v such that u is a supermask of mask:
u ⊇ mask (i.e., (u & mask) == mask)
int64_t zero_and(int mask)
Typical meaning in such templates:
count of u such that (u & mask) == 0
This is common query; achieved by using inv and submask sums.
SMALL EXAMPLE
v = [1 (001), 3 (011), 4 (100)], LG=3

submasks_ans(3) counts masks ⊆ 3: {1,3} => 2
supermasks_ans(1) counts masks containing bit0: {1,3} => 2
zero_and(3) count masks with (u & 3)==0: only 4 => 1
WHEN TO USE

Many offline queries on masks: “count submasks / supermasks / AND=0”.
4.6) dynamic_sos (Dynamic “add mask” + query submask sums)
USED FOR

You have online operations:
add(mask, value)
query(mask): sum over submasks (or related)
But you can’t rebuild full SOS each time.
CORE IDEA (SIMPLE)

Split bits into two halves (left and right).
Maintain partial contributions for left-part buckets.
When you query, you “finalize” needed buckets lazily.
INPUTS

add(x, val=1) adds value to mask x
OUTPUTS

submask_sum(x): returns Σ value[y] for all y ⊆ x (typical meaning)
ASSUMPTIONS

Mask size k is fixed, k not too big (like <= 20..24 typically)
This structure is a tradeoff: faster than rebuilding SOS every update.
IMPORTANT FUNCTIONS (from your snippet)

dynamic_sos(int k)
Input : k = number of bits
Output: initializes split sizes and internal buffers
void add(int x, int64 val=1)
Input :
x = mask
val = added weight
Output: none
Effect:
stores update into a “pending” vector for the bucket of left-part,
instead of updating all submasks immediately.
int64_t submask_sum(int x)
Input : x mask
Output: sum of values of all masks y ⊆ x
Internal:
may “push” pending updates for needed left bucket before answering.
WHEN TO USE / NOT

Use when you need many updates + many submask queries online.
Don’t use if you can do offline zeta once (offline is simpler and faster).
4.7) subset_basis (Max AND / Max OR with existing masks)
USED FOR
Given a set S of masks, answer:

max_and(x) = max over y in S of (x & y)
max_or(x) = max over y in S of (x | y)
CORE IDEA (SIMPLE)

Precompute which masks are “possible as submasks” of some inserted mask.
If a mask m is a submask of some y in S, then:
m can appear as (x & y) for some x that contains m
and bits in m can be “turned on together” inside some y (useful for OR)
INPUTS

add(x): insert mask x into the set
OUTPUTS

max_and(x)
max_or(x)
ASSUMPTIONS

LG bits (template parameter)
Memory is big: dp[LG][1<<LG], so LG should be small (<= 20)
IMPORTANT FUNCTIONS (from your code)

void add(int x)
Input : x mask
Output: none
Effect:
marks x and all its submasks in dp (via recursion).
After this, dp[LG-1][m]=1 means “m is submask of some inserted mask”.
int max_and(int x)
Input : x mask
Output: maximum value of (x & y) over inserted y
How:
greedily build largest mask ans ⊆ x such that dp says ans is achievable.
int max_or(int x)
Input : x mask
Output: maximum value of (x | y) over inserted y
How:
tries to add bits where x has 0, but only if those bits can exist together in some y.
SMALL EXAMPLE
Inserted masks: S = {6 (110), 3 (011)}
Query x = 5 (101)

max_and(5):
5&6 = 4 (100)
5&3 = 1 (001)
answer = 4
max_or(5):
5|6 = 7 (111)
5|3 = 7 (111)
answer = 7
WHEN TO USE

Problems asking max AND/OR with a set of masks quickly.