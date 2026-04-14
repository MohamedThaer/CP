8.16) factorizer (fast factorization + divisors + phi + mobius)
USED FOR

Factorize 64-bit integers (up to ~1e18)
Fast primality test
Enumerate divisors from factorization
Compute φ(n), μ(n) for large n using factorization
WHERE IT SHOWS UP

problems with big numbers: gcd/lcm, divisor sums, multiplicative functions
“factorize many queries of up to 1e18”
“count divisors / sum divisors” when n is large
CORE IDEA (high level)

For small values: use sieve / SPF.
For large values:
primality: Miller–Rabin
factorization: Pollard Rho finds a non-trivial divisor
recursively factor until primes
IMPORTANT API (as in your pages)

A) constructor / sieve part

factorizer(int N = 1)
INPUT : N sieve limit (for SPF arrays)
OUTPUT: object ready

vector spf(int N) (if exposed) / internal spf storage
OUTPUT: smallest prime factor array up to N

B) primality
3) template bool IsPrime(T n)
INPUT : n
OUTPUT: true if n is prime, else false
NOTES :

for n <= sieveLimit: quick lookup
for larger: Miller–Rabin
C) factorization (main)
4) template vector Factorize(T n)
INPUT : n
OUTPUT: list of prime factors (with multiplicity), not necessarily sorted
EXAMPLE:
Factorize(12) -> [2,2,3]

template vector<pair<T,int>> get_freq(vector facs)
INPUT : list of prime factors (maybe unsorted)
OUTPUT: grouped factorization [(p, exponent)]
EXAMPLE:
[2,2,3] -> [(2,2),(3,1)]
D) divisors from factorization
6) template vector divisors(T n)
INPUT : n
OUTPUT: all divisors of n (sorted in your code)
EXAMPLE:
divisors(12) -> [1,2,3,4,6,12]

E) multiplicative functions for big n
7) template T phi(T n)
INPUT : n
OUTPUT: Euler phi(n)
METHOD: factorize then phi = n * Π (1 - 1/p)

template T mobius(T n)
INPUT : n
OUTPUT: μ(n)
METHOD:
if any prime exponent >=2 => 0
else (-1)^(#distinct primes)
ASSUMPTIONS / CONSTRAINTS

Randomized algorithms: extremely low failure probability.
mulmod / Montgomery is used internally to avoid overflow.
CONTEST WORKFLOW (how to use quickly)

Create one object globally: factorizer F(2000000); (or small N)
For each query:
if need factorization: auto fac = F.get_freq(F.Factorize(x));
if need divisors: auto divs = F.divisors(x);
WHEN NOT TO USE

If x <= 1e6 and many queries: SPF sieve is faster/simpler.
If you only need gcd: factorization is unnecessary.
8.42) primality_test (simple randomized)
USED FOR

Quick “probably prime” test for smaller constraints (not the strongest 64-bit method).
WHAT IT DOES

Uses modular exponent checks like Fermat with random bases.
IMPORTANT FUNCTIONS

int64_t power(int64_t n, int64_t p, int64_t mod)
int64_t rand(int64_t l, int64_t r)
bool is_prime(int64_t n)
OUTPUT: true/false
NOTE

For strict 64-bit correctness, prefer factorizer::IsPrime().
8.43) primitive_root (primitive_generator)
USED FOR

Check if a number g is a primitive root mod p
Find smallest primitive root (usually for prime p)
List all primitive roots
COMMON USES

NTT primitive roots
discrete log / primitive root problems
multiplicative group generator tasks
CLASS
class primitive_generator

STATE

int64 p, _phi
vector fact // prime factors of phi(p)
IMPORTANT FUNCTIONS

static int64_t phi(int64_t n)
INPUT : n
OUTPUT: Euler phi(n)

static int64_t power(int64_t n, int64_t p, int64_t mod)
OUTPUT: n^p % mod

primitive_generator(int64_t p)
INPUT : modulus p
EFFECT:

computes _phi = phi(p)
factorizes _phi into fact list (distinct primes)
bool is_primitive_root(int64_t x)
INPUT : x
OUTPUT: true if x is primitive root mod p
WHY:

x is primitive root iff for every prime q | phi:
x^(phi/q) != 1 (mod p)
int64_t smallest_primitive_root()
OUTPUT: smallest g >= 1 that is primitive root, or -1 if none

vector<int64_t> all_primitive_roots()
OUTPUT: all primitive roots modulo p (for prime p there are φ(φ(p)) many)

ASSUMPTIONS

Primitive roots exist for:
p = 2, 4, p^k, 2*p^k where p odd prime.
In contests, usually p is prime.
EXAMPLE
p=7, primitive roots are 3 and 5. smallest_primitive_root()=3.

8.38) PCF (Prime Counting Function π(n), Lehmer)
USED FOR

Compute π(n) for large n quickly (n up to 1e11..1e12)
Also get π(r)-π(l-1)
WHY IT APPEARS

counting primes in range
problems needing #primes up to n
CORE IDEA (high level)

Lehmer prime counting:
π(n) computed using:
φ(n, a): count integers <=n not divisible by first a primes
recursion + memo
Precompute primes up to MAXN
Precompute small DP table for φ
CLASS
class PCF

IMPORTANT FIELDS (from snippet comments)

factorizer F (sieve primes)
pref[i] = number of primes <= i
dp[n][k] precomputed φ for small n,k
IMPORTANT FUNCTIONS

PCF()
OUTPUT: object precomputes:

primes up to MAXN
prefix counts
dp table
int64_t prime_cnt(int64_t n)
INPUT : n
OUTPUT: π(n) = #primes <= n

int64_t prime_cnt(int64_t l, int64_t r)
INPUT : l,r
OUTPUT: π(r) - π(l-1)

ASSUMPTIONS

Heavy precomputation, but fast queries after.
WHEN TO USE

Only when constraints demand huge n prime counting.
8.39) permutation class
USED FOR

Work with permutations in many standard ways:
inverse permutation
apply permutation to an array
power permutation (compose k times)
decompose into cycles
restore permutation from cycles
k-th permutation (factoradic)
permutation index/rank
CLASS
class permutation
{
public: vector p; // 0-index mapping
}

ASSUMPTION ABOUT REPRESENTATION

p[i] = where i goes (image of i).
Applying to array v usually means: ret[i] = v[p[i]] or ret[p[i]] = v[i]
Your code shows:
ret[i] = v[p[i]];
so it applies as: new[i] = old[p[i]].
IMPORTANT FUNCTIONS

permutation(const vector& p)
INPUT : p mapping size n
OUTPUT: permutation object

vector inverse() const
OUTPUT: inv such that inv[p[i]] = i

vector apply_on(const vector& v) const
INPUT : array v size n
OUTPUT: ret[i] = v[p[i]]

permutation power(int64_t k) const
INPUT : k
OUTPUT: permutation equal to applying this permutation k times
METHOD:

binary exponentiation using composition via apply_on
vector<vector> get_cycles() const
OUTPUT: list of cycles (each cycle is list of nodes)

static permutation restore(int n, const vector<vector>& cycles)
INPUT : n and cycles
OUTPUT: permutation that has exactly those cycles

static vector kth_permutation(int n, int64_t k)
INPUT : n, k (0-index)
OUTPUT: k-th permutation in lexicographic order
METHOD: factoradic using factorials

static int64_t permutation_index(const vector& perm)
INPUT : permutation perm
OUTPUT: rank/index (0-index) in lexicographic order
METHOD: count smaller unused elements * factorial

EXAMPLE (cycles / power)
p = [1,2,0] is cycle (0 1 2)
power(2) => [2,0,1]

WHEN TO USE

any “apply k times”, “cycle decomposition”, “k-th permutation” tasks.
8.41) power_tower
USED FOR
Compute:
a[l]^(a[l+1]^(...a[r])) mod m
(“tower exponentiation”)

CORE DIFFICULTY

exponent is huge; reduce using Euler φ(m), but only valid carefully:
when exponent >= φ(m), you add φ(m) to keep correctness if gcd(base,m)!=1.
IMPORTANT FUNCTIONS (from your class)

static int64_t phi(int64_t n)
static int64_t MOD(int64_t x, int64_t m)
Output: x mod m in [0..m-1]
static int64_t power(int64_t n, int64_t p, int64_t mod)
static int64_t solve(const vector<int64_t>& a, int64_t l, int64_t r, int64_t m)
INPUT :
a array, range l..r, modulus m
OUTPUT:
value of power tower mod m
ASSUMPTIONS

Works for moderate recursion depth (array length).
Uses phi recursion until modulus becomes 1.
WHEN TO USE

classical problems: “a^(b^(c^...)) mod m”.
8.37) partition (Pentagonal number theorem)
USED FOR

Compute partition numbers p(i): number of ways to write i as sum of positive integers
Usually modulo MOD.
FUNCTION
vector partition(int n)

INPUT

n maximum
OUTPUT

ans[0..n], ans[i]=p(i)
CORE IDEA

Euler pentagonal theorem recurrence:
p(n) = Σ (-1)^{k-1} p(n - g(k))
where g(k) are generalized pentagonal numbers:
k(3k-1)/2 for k=1,-1,2,-2,...
ASSUMPTIONS

O(n*sqrt(n)) roughly.
EXAMPLE
p(0)=1, p(1)=1, p(2)=2, p(3)=3, p(4)=5, ...

8.45) Stirling (1st kind, small mod, large n)
USED FOR

Compute Stirling numbers of the 1st kind s(n,k) mod MOD
Here MOD is small, n can be large (different technique).
CLASS
struct StirlingNumber1SqmodP

IMPORTANT API

StirlingNumber1SqmodP(int mod)
Input : mod
Modint comb(int64_t n, int64_t k) const
Output: C(n,k) mod mod (handles large n)
Modint operator()(int64_t n, int64_t k) const
Output: s(n,k) mod mod
ASSUMPTIONS

Specialized; use if the problem specifically matches “small mod, huge n”.
8.46) Stirling (2nd kind, small mod, large n)
USED FOR

Stirling numbers of 2nd kind S(n,k) mod MOD
CLASS
struct StirlingNumber2SqmodP

API same style:

constructor(mod)
operator()(n,k)
8.47) sum_n_mod_i
USED FOR
Compute:
Σ_{i=1..n} (n mod i)
in about O(sqrt(n))

FUNCTION
__int128_t sum_n_mod_i(int64_t n)

INPUT

n
OUTPUT

sum (128-bit)
CORE IDEA (grouping)
n mod i = n - i * floor(n/i)
So sum = nn - Σ ifloor(n/i)
Group i where floor(n/i) is constant:

for i from 1:
q = n/i
last = n/q
handle range [i..last] in O(1)
Then continue i=last+1
WHEN TO USE

number theory sums, floor-division patterns.
8.48) triplets (frequency counting trick)
USED FOR
Count #triplets under patterns like:

(x,x,x)
(x,x,y) with x!=y
(x,y,z) all distinct
depending on the target problem.
Your snippet shows idea using:
frq_1[x]++
frq_2[x] += something
Then formulas:
triplets = frq_1 * frq_1 * frq_1 - exclude...
TO DOCUMENT EXACTLY
I need the exact code block of 8.48 (not fully visible in image).
Paste it and I will write:

exact input
exact output meaning
formulas and example.
8.7 / 8.30) Min_25_sieve (advanced multiplicative sums)
USED FOR
Compute prefix sums of multiplicative functions for huge n:
S(n) = Σ_{i=1..n} f(i)
Examples:

Σ φ(i)
Σ μ(i)
Σ i^k for primes, etc.
API IN YOUR COMMENT
Min_25Sieve<sieve_size, max_degree> sieve;
mint result = sieve.solve(n, poly_coeffs, eval_func);

KEY INPUTS

n: up to very large (1e11+)
poly_coeffs: coefficients for polynomial in k or p
eval_func(prime p, exponent k, p^k): returns required term for prime powers
OUTPUT

sum f(i) up to n (mod or integer type)
WHEN TO USE

only in problems explicitly requiring it (rare + advanced).