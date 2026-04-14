8.1) AlgebraicSolutionCount
GOAL
Count integer solutions to:
x1 + x2 + ... + xn = s
with bounds: l <= xi <= r

WHY IT APPEARS IN PROBLEMS

“number of arrays length n with sum s and each element in [l,r]”
“count distributions with limits”
classic combinatorics counting
CORE IDEA (VERY CLEAR)

Shift variables to remove lower bound:
yi = xi - l => 0 <= yi <= (r-l)
sum yi = s - n*l = S
Without upper bound, #solutions for yi >= 0 is:
C(S + n - 1, n - 1)
Use inclusion-exclusion for upper bound yi <= U where U = r-l:
Count solutions where some variables exceed U.
TEMPLATE FUNCTION (as shown)
Z AlgebraicSolutionCount(int n, int s, int l, int r)

INPUTS

n: number of variables
s: target sum
l, r: inclusive bounds for each variable
OUTPUT

number of solutions (type Z, usually mint)
ASSUMPTIONS

Needs combinations nCr available (from combinatorics template)
If S < 0 → return 0
COMMON CONSTRAINTS

n up to 1e6 sometimes (needs factorial precompute)
s up to 1e6..2e6 typically in mod problems
EXAMPLE
n=2, s=5, l=1, r=4
Valid pairs: (1,4),(2,3),(3,2),(4,1) => answer = 4

WHEN TO USE / WHEN NOT

Use when you need exact count under bounds.
If n and s huge without mod/combinatorics precompute, may not fit.
8.2) CRT (Chinese Remainder Theorem, non-coprime)
GOAL
Solve system:
x ≡ r1 (mod m1)
x ≡ r2 (mod m2)
...
even if mi are NOT coprime.

WHY IT APPEARS

merging modular constraints
reconstructing number from different mod results
NTT CRT combine, calendar problems, modular equations
IMPORTANT BUILDING BLOCK
int64_t extended_euclid(a,b,x,y)

extended_euclid INPUT

a, b
extended_euclid OUTPUT

returns g = gcd(a,b)
sets x,y such that: ax + by = g
CRT FUNCTION (from your code)
template
__int128_t CRT(vector rems, vector mods)

CRT INPUT

rems[i] : remainder
mods[i] : modulus (positive)
CRT OUTPUT

smallest non-negative x satisfying all equations
returns -1 if no solution
internally builds modulus = lcm of all mods
ASSUMPTIONS / WARNINGS

Uses __int128 to reduce overflow.
If lcm grows beyond safe range, your code may return -1.
MINI EXAMPLE
x ≡ 2 (mod 3)
x ≡ 3 (mod 5)
=> x = 8

CONTEST WORKFLOW

If you only have coprime mods, CRT is simpler.
If not coprime, you MUST check consistency using gcd.
8.3) EquationSystem (Linear System Solver)
GOAL
Solve A*x = b and also:

check consistency
compute rank
return one solution
return basis of solution space (nullspace)
WHERE USED

linear algebra in CP:
systems over mod
probabilities / expected values
constraints solving
XOR/bit equations (sometimes with mod 2)
CLASS
template
class EquationSystem

CONSTRUCTORS IN YOUR PAGES

EquationSystem(vector<vector> Equations, vector res)

Input:
Equations = A (n x m)
res = b (size n)
Effect: builds augmented matrix [A|b] and Gaussian eliminates
EquationSystem(vector<vector> v)

Input: matrix (often already augmented)
Effect: eliminates
MAIN OUTPUT METHODS

bool is_consistent()

Output: true if system has at least one solution
int get_rank()

Output: rank(A)
vector get_solution()

Output: one solution vector x (size m)
Convention: free variables set to 0
vector<vector> get_basis()

Output:
basis vectors for the null space (solutions to A*x=0)
Meaning:
all solutions = particular_solution + sum(ci * basis[i])
ASSUMPTIONS

Z must support:
+, -, *, and division (or modular inverse)
Over mod prime: use mint
Over doubles: use long double with EPS (not shown here)
TINY EXAMPLE
x + y = 3
x - y = 1
Solution: x=2, y=1
Rank = 2, basis empty (unique solution)

CONTEST WORKFLOW

Build A and b
Run EquationSystem if mod
Check consistent → get_solution()
8.4) LD_equation + modular_equation (Diophantine)
A) Linear Diophantine: ax + by = c
USED FOR

“find integer x,y”
“count solutions in range”
modular inverses when gcd != 1
KEY FUNCTION
auto LD_equation(int64 a, int64 b, int64 c, int k=1)

INPUT

a, b, c
OUTPUT

vector<pair<int64,int64>> solutions (size depends on k / template)
empty if no solutions
ASSUMPTION

Solutions exist iff gcd(a,b) divides c
CORE FORMULA (what your code is based on)
Let g=gcd(a,b). If c%g!=0 → none.
Find (x0,y0) solving ax0 + by0 = g.
Scale: x0 *= c/g, y0 *= c/g.
General:
x = x0 + (b/g)*t
y = y0 - (a/g)*t

B) Modular equation: a*x ≡ b (mod M)
USED FOR

solve congruence when a and M not coprime
FUNCTION
auto modular_equation(int64 a, int64 b, int64 M)

INPUT

a, b, M
OUTPUT

vector all solutions x in [0..M-1]
empty if none
EXAMPLE
4x ≡ 2 (mod 6)
Solutions: x=2,5

CONTEST WORKFLOW

Use modular_equation when gcd(a,M) != 1.
If gcd(a,M)=1, solution is x = b * inv(a) mod M.
8.5) Legendre (v_p(n!))
USED FOR

exponent of prime p in factorial
trailing zeros
combinatorics divisibility
FUNCTION
int64_t Legendre(int64_t n, int64_t prime)

INPUT

n, prime p
OUTPUT

v_p(n!) = Σ floor(n/p^k)
EXAMPLE
Legendre(10,2)=8

8.6) Legendre_composite (max x with k^x | n!)
USED FOR

maximum power of composite k dividing n!
FUNCTION
int64_t Legendre_composite(int64_t n, int64_t k)

INPUT

n, k
OUTPUT

max x such that k^x divides n!
HOW

factorize k = Π p_i^{a_i}
answer = min_i floor( Legendre(n,p_i) / a_i )
8.10) combinatorics namespace (fac, inv, nCr, nPr, catalan…)
USED FOR

fast nCr / nPr under mod
stars and bars
catalan numbers
derangements / Stirling (some are elsewhere)
TYPICAL TYPES

Z = mint
IMPORTANT STORAGE

_fac[i] = i!
_inv[i] = inverse of i
_facinv[i] = (i!)^-1
IMPORTANT FUNCTIONS (as in your code)

void init(int N)
INPUT : maximum N needed
OUTPUT: none (fills arrays)
WHY : must be called before fac/ncr if not already large enough

Z fac(int n)
OUTPUT: n!

Z inv(int n)
OUTPUT: modular inverse of n

Z facinv(int n)
OUTPUT: (n!)^-1

Z ncr(int n, int r)
INPUT : n,r
OUTPUT: C(n,r) mod
NOTE : returns 0 if r<0 or r>n

Z npr(int n, int r)
OUTPUT: P(n,r) = n!/(n-r)!

Z ncr_lucas(int64 n, int64 r)
USED WHEN: n huge, mod prime small
OUTPUT : C(n,r) mod prime (Lucas theorem)

Z catalan(int n, int k=0)
OUTPUT:

standard: C(2n,n)/(n+1)
generalized variant if k used in your code
Z stars_and_bars(int star, int bar)
OUTPUT: C(star+bar, star)

Z permutation_with_repetition(freq)
OUTPUT: multinomial = n! / Π (freq[i]!)

CONTEST WORKFLOW

Call combinatorics::init(maxN) once per file/test.
Then ncr/npr/catalan are O(1).
8.14) divisor_analysis (count/sum/product of divisors)
USED FOR
Given factorization n = Π p^k, compute:

d(n) = number of divisors
σ(n) = sum of divisors
Π divisors (product of divisors) mod MOD
FUNCTION
auto divisor_analysis(vector<pair<int,int>> factors)

INPUT

vector of (prime p, exponent k)
OUTPUT

tuple(count, sum, product) as mod values (Z / mint types)
IMPORTANT DETAILS

count = Π (k+1)
sum = Π (p^(k+1)-1)/(p-1)
product:
if count is even: product = n^(count/2)
if count is odd: n is perfect square; product = (sqrt(n))^count
(template handles via exponent arithmetic mod (MOD-1) often)
WHEN TO USE

“given prime factorization, output divisor stats mod”.
8.15) extended_euclid (again, core primitive)
FUNCTION
int64_t extended_euclid(int64 a, int64 b, int64 &x, int64 &y)

INPUT

a,b
OUTPUT

g=gcd(a,b)
x,y such that ax + by = g
WHY IT’S NEEDED

inverse modulo when gcd=1
CRT merging
Diophantine equations
8.31) mint (modular integer)
USED FOR

clean mod arithmetic with operator overloading
STRUCT
template
struct mint

STATE

int64 value (always normalized to [0, mod))
KEY METHODS / OPERATORS

mint(int64 x=0)

Input : any int64
Output: mint normalized mod
mint power(int64 p)

Output: this^p mod
mint inverse()

Output: modular inverse (uses extended euclid in your code)
operators:
+,-,,/, +=,-=,=,/=
==, !=
stream in/out

ASSUMPTIONS

inverse exists only if gcd(value,mod)=1 (mod prime -> OK if value != 0)
CONTEST WORKFLOW

Define: using Z = mint<1000000007>;
Use Z everywhere for safe mod.
8.32) mobius(n) (single number)
USED FOR

compute μ(n) for a single n
FUNCTION
int mobius(int64_t n)

OUTPUT

0 if n has squared prime factor
1 if even number of distinct prime factors
-1 if odd number
EXAMPLE
μ(1)=1
μ(6)=1 (primes 2*3)
μ(12)=0 (2^2 divides)

8.33) Mobius class (bulk mobius + transforms)
USED FOR

Precompute mobius up to N
Provide transforms:
zeta over multiples (for gcd problems)
zeta over divisors (for lcm problems)
and inverses (mobius transforms)
Count coprime pairs using μ and divisor frequency
CLASS
class Mobius

IMPORTANT FUNCTIONS YOU ACTUALLY CALL

static int64_t mob(int n)

Input : n
Output: μ(n) with caching (builds up to 2n in your code)
static vector<int64_t> divisors(int64_t n)

Output: list of divisors
MULTIPLES TRANSFORMS (arrays 1..N)
3) template static vector zeta_multiples(vector f)

Input : f[1..N]
Output: g where g[d] = Σ f[m] for all m multiple of d
template static vector mobius_multiples(vector f)
Input : zeta_multiples result
Output: original f (inverse)
DIVISORS TRANSFORMS
5) template static vector zeta_divisors(vector f)

Output: g[x] = Σ f[d] for all d | x
template static vector mobius_divisors(vector f)
inverse of zeta_divisors
Coprime helpers (from your file)
7) static int64_t coprime_pairs(a,b) / coprime_power(...) etc.

Used for counting pairs with gcd=1 using μ sums.
WHERE USED

gcd_convolution / lcm_convolution sections
coprimality DS
counting gcd=1 pairs quickly
8.40) power (fast exp mod)
FUNCTION
int64_t power(int64_t n, int64_t p, int64_t mod=1e9+7)

INPUT

n: base
p: exponent
mod
OUTPUT

n^p % mod
USED EVERYWHERE

modular exponentiation
8.36) mulmod (safe multiply mod)
PROBLEM IT SOLVES

If ab overflows int64, but you still need (ab)%mod.
FUNCTION
int64_t mulmod(int64_t a, int64_t b, int64_t mod)

INPUT

a,b,mod
OUTPUT

(a*b)%mod safely using repeated doubling
WHEN TO USE

Miller-Rabin / Pollard Rho
big mod ~ 1e18
8.44) sieve namespace (prime utilities)
USED FOR

is_prime up to N
list primes
spf (smallest prime factor)
phi array
mobius array
segmented sieve
divisors list for all numbers
IMPORTANT FUNCTIONS (from your page)

vector is_prime(int N)
OUTPUT: isPrime[i] for 0..N

vector primes(int N)
OUTPUT: list of primes <= N

vector spf(int N)
OUTPUT: spf[i] smallest prime factor for i

vector mobius(int N)
OUTPUT: μ[0..N]

vector phi(int N)
OUTPUT: phi[0..N]

vector segmented_sieve(int64 l, int64 r)
OUTPUT: primality for each integer in [l..r]
NOTE: good when r-l up to ~1e6

vector<vector> divs(int N)
OUTPUT: divs[x] list of divisors of x for all x<=N
WARNING: memory heavy for N=2e6+

vector next_prime(int N)
OUTPUT: nextPrime[i] = smallest prime >= i

CONTEST WORKFLOW

Use spf(N) when you need many factorizations up to N.
Use segmented_sieve for checking primes in a big interval.
8.13) discrete_log (BSGS with gcd handling)
GOAL
Find smallest x such that:
a^x ≡ b (mod m)

FUNCTION
int discrete_log(int a, int b, int m)

INPUT

a,b,m
OUTPUT

x if exists
-1 otherwise
ASSUMPTIONS

Your code handles gcd(a,m) != 1 by:
removing common gcd factors step-by-step
adjusting b and tracking “add” to exponent
COMPLEXITY

~ O(sqrt(m)) time and memory
EXAMPLE
2^x ≡ 8 (mod 13) => x=3

WHEN TO USE

modular exponent equations, primitive root tasks, crypto-like problems