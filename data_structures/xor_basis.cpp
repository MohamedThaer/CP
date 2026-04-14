template <int LOG = 62>
class xor_basis {
private:
    int elements = 0, sz = 0;
    int64_t basis[LOG] = {};

    int64_t power(int64_t n, int64_t p, int64_t mod) {
        n %= mod;
        int64_t ret = 1;
        while (p) {
            if (p & 1) {
                ret = ret * n % mod;
            }
            n = n * n % mod;
            p >>= 1;
        }
        return ret;
    }

public:
    void insert(int64_t mask) {
        elements++;
        for (int i = LOG - 1; ~i; --i) {
            if (mask & (1LL << i)) {
                if (!basis[i]) {
                    basis[i] = mask;
                    ++sz;

                    for (int j = i - 1; j >= 0; --j) {
                        if (basis[j] && (basis[i] & (1LL << j))) {
                            basis[i] ^= basis[j];
                        }
                    }
                    for (int j = i + 1; j < LOG; ++j) {
                        if (basis[j] & (1LL << i)) {
                            basis[j] ^= basis[i];
                        }
                    }

                    return;
                }
                mask ^= basis[i];
            }
        }
    }

    int64_t distinct_xors() {
        return 1LL << sz;
    }

    int64_t mx_xor() {
        int64_t ret = 0;
        for (int i = LOG - 1; ~i; --i) {
            ret = max(ret, ret ^ basis[i]);
        }
        return ret;
    }

    int64_t count(int64_t x, int64_t mod) {
        for (int i = LOG - 1; ~i; --i) {
            if (x & (1LL << i)) {
                x ^= basis[i];
            }
        }
        return !x * power(2, elements - sz, mod);
    }

    int64_t kth_distinct_xor(int64_t k) {
        if (k >= distinct_xors()) return -1;
        int64_t ans = 0;
        for (int i = 0, pos = 0; i < LOG; ++i) {
            if (basis[i]) {
                if (k & (1LL << pos++)) {
                    ans ^= basis[i];
                }
            }
        }
        return ans;
    }

    int64_t kth_xor(int64_t k) {
        if (elements - sz > __lg(k)) {
            return 0;
        }
        k /= (1LL << (elements - sz));
        return kth_distinct_xor(k);
    }

    // Count how many distinct subset XORs < x
    int64_t count_lt(int64_t x) {
        int64_t ans = 0, cnt = 1LL << sz, mask = 0;
        for (int i = LOG - 1; i >= 0; --i) {
            if (basis[i]) {
                if ((x >> i) & 1) {
                    ans += (cnt >> 1);
                    if (!((mask >> i) & 1))
                        mask ^= basis[i];
                }
                else {
                    if ((mask >> i) & 1)
                        mask ^= basis[i];
                }
                cnt >>= 1;
            }
            else {
                if (((x >> i) & 1) != ((mask >> i) & 1)) {
                    return ((x >> i) & 1) ? ans + cnt : ans;
                }
            }
        }
        return ans;
    }
};


NAME

xor_basis: linear basis for XOR (binary vector space) using int64.
USED FOR (What problems it solves)

Maximum XOR from any subset of numbers
Count how many distinct subset XOR values exist
Check if a value x can be formed as XOR of some subset
Count number of subsets producing a specific XOR x (mod MOD)
k-th smallest distinct XOR value (and k-th XOR considering duplicates)
Count how many distinct XOR values are < x
COMMON CP PATTERNS

“Maximum subset XOR”
“How many different XORs can we make?”
“How many subsets have XOR = X?”
“k-th smallest subset XOR”
Queries on XOR-space / GF(2) linear algebra
CORE IDEA (Simple)
Treat every number as a binary vector (bits).
Maintain a set of independent vectors (basis) such that:
any inserted number can be represented by XOR of basis vectors if it’s dependent.
This is like Gaussian elimination but with XOR instead of +/−.
sz = rank of the space (number of independent vectors).
elements = how many numbers you inserted total.
Number of distinct subset XOR values = 2^sz.
If rank < elements, then some numbers are dependent, meaning:
each distinct XOR can be formed in 2^(elements - sz) different subsets.
TEMPLATE PARAMETERS / STATE
template

LOG is number of bits to consider.
For int64, 62 is used to be safe (avoid sign bit issues).
basis[i] stores a basis vector whose highest set bit is i (pivot at bit i).
Internal variables:

int elements: total inserted numbers
int sz : number of basis vectors (rank)
int64_t basis[LOG]: basis vectors
Complexity:

insert: O(LOG^2) because it also “cleans” other basis vectors
other queries: O(LOG)
memory: O(LOG)
IMPORTANT FUNCTIONS (Inputs / Outputs / Why)
void insert(int64_t mask)
PURPOSE

Add a number to the basis.
INPUT

mask: the value you want to insert.
OUTPUT

none (updates internal basis).
Side effects:
elements++
if mask is independent: sz++
HOW IT WORKS (intuitive)

Try to remove the highest bit of mask using existing basis vectors.
If you reach a bit i where mask has bit i = 1 and basis[i] is empty:
basis[i] = mask becomes a new independent vector (new pivot).
Then the code “cleans”:
Remove lower bits from basis[i] using smaller pivots
Remove bit i from all larger pivot vectors
This keeps the basis in a reduced form, which makes k-th and counting queries easier.
WHEN IT FAILS TO INCREASE RANK

If mask becomes 0 after xoring with basis vectors -> it’s dependent, sz doesn’t change.
int64_t distinct_xors()
PURPOSE

Number of DISTINCT XOR values you can form from all subsets.
INPUT

none
OUTPUT

2^sz
ASSUMPTIONS / WARNING

Uses (1LL << sz). This is safe only if sz <= 62 (fits in signed 64-bit).
If sz can be 63, this would overflow. With LOG=62 you avoid that.
int64_t mx_xor()
PURPOSE

Maximum XOR value obtainable from any subset.
INPUT

none
OUTPUT

maximum possible XOR
HOW IT WORKS

Greedy: start ret=0.
For i from high to low:
if (ret ^ basis[i]) is bigger than ret, take it.
Works because basis vectors represent independent “bit improvements”.
int64_t count(int64_t x, int64_t mod)
PURPOSE

Count number of subsets whose XOR equals exactly x, modulo mod.
INPUT

x : target XOR
mod : modulus for the count
OUTPUT

If x is representable: 2^(elements - sz) mod mod
Else: 0
WHY 2^(elements - sz)?

There are elements numbers, but only sz are independent.
The remaining (elements - sz) are dependent and can be toggled in subsets
without changing the XOR result, giving multiplicity factor.
HOW IT WORKS

Reduce x by the basis (same as checking representability):
for i from high to low:
if x has bit i: x ^= basis[i]
If result x becomes 0 => representable.
Return power(2, elements - sz, mod), else return 0.
int64_t kth_distinct_xor(int64_t k)
PURPOSE

Return the k-th DISTINCT subset XOR value (0-indexed) in increasing order
(assuming the reduced basis ordering used here).
INPUT

k : 0 <= k < 2^sz
OUTPUT

the k-th distinct XOR value
returns -1 if k is out of range
HOW IT WORKS (simple)

Each basis vector corresponds to one “choice bit” in k.
The code iterates bits from low to high:
if basis[i] exists and k’s next bit is 1 -> ans ^= basis[i]
NOTE

This works correctly because insert() keeps the basis reduced/cleaned.
int64_t kth_xor(int64_t k)
PURPOSE

k-th XOR among ALL subsets including duplicates (0-indexed),
where each distinct XOR appears 2^(elements - sz) times.
INPUT

k : index in the multiset of all subset XOR results
OUTPUT

the XOR value at position k
HOW IT WORKS

multiplicity = 2^(elements - sz)
if k < multiplicity -> answer is 0 (smallest XOR repeated)
else k /= multiplicity and return kth_distinct_xor(k)
ASSUMPTION

This logic assumes ordering starts from smallest XOR and duplicates are grouped.
Common use: when you only need mapping by multiplicity, not strict sorted-by-subset listing.
int64_t count_lt(int64_t x)
PURPOSE

Count how many DISTINCT subset XOR values are strictly < x.
INPUT

x : threshold
OUTPUT

count of distinct XOR results < x
HOW IT WORKS (high level)

Walk bits from high to low.
Maintain:
mask: current “best possible prefix” built from basis choices
cnt : number of remaining combinations at current stage (starts 2^sz)
ans : number of XORs confirmed < x so far
At each pivot bit i:
if x has bit i = 1, then half combinations with bit i = 0 are definitely < x
so add cnt/2 to ans, then adjust mask to follow the “equal prefix” branch.
if x has bit i = 0, you must stay in bit i = 0 branch; adjust mask if needed.
If there is no basis at bit i, the bit is fixed; may cause early finish.
WHEN USED

Problems like: “how many subset XORs are in range [L, R)”
using: count_lt(R) - count_lt(L)
SMALL CONCRETE EXAMPLES
Example 1: rank, distinct xors

Insert: 1 (001), 2 (010), 3 (011)
3 is dependent because 3 = 1 xor 2
elements = 3
sz = 2
distinct_xors() = 2^2 = 4 -> {0,1,2,3}
multiplicity = 2^(3-2) = 2
so each distinct XOR appears in exactly 2 subsets.
Example 2: maximum XOR

Insert: [5 (101), 1 (001), 2 (010)]
mx_xor() returns 5 xor 2 = 7 (111) if possible in basis.
Example 3: count subsets giving XOR = x

Using example 1:
count(3, 1e9+7) = 2^(elements-sz) = 2
because XOR=3 can be formed by:
subset {3}
subset {1,2}
Example 4: k-th distinct XOR

With example 1 (distinct are 0,1,2,3):
kth_distinct_xor(0)=0
kth_distinct_xor(1)=1
kth_distinct_xor(2)=2
kth_distinct_xor(3)=3
Example 5: count_lt

With example 1:
count_lt(2) = number of distinct XORs < 2 = {0,1} => 2
CONTEST WORKFLOW (When to use / when not)
USE IT WHEN

You see “subset XOR” and constraints are big (n up to 2e5), so brute subset is impossible.
Tasks ask for max XOR, count solutions, k-th XOR, etc.
DON’T USE IT WHEN

You need to delete numbers (this template has insert only; deletions need more complex DS).
The operation is not XOR / not over GF(2).
HOW TO INTEGRATE QUICKLY

Create basis object:
xor_basis<62> xb;
Insert all numbers:
for (auto v : a) xb.insert(v);
Answer queries:
xb.mx_xor()
xb.distinct_xors()
xb.count(x, MOD)
xb.kth_distinct_xor(k)
xb.count_lt(x)