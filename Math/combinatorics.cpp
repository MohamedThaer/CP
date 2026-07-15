const int MOD = 1e9 + 7;

int64_t add(int64_t a, int64_t b) {
    a %= MOD, b %= MOD;
    return (a + b + MOD) % MOD;
}

int64_t sub(int64_t a, int64_t b) {
    a %= MOD, b %= MOD;
    return (a - b + MOD) % MOD;
}

int64_t mul(int64_t a, int64_t b) {
    a %= MOD, b %= MOD;
    return (a * b) % MOD;
}

int64_t pw(int64_t n, int64_t e) {
    int64_t ans = 1;
    n %= MOD;
    for (; e; n = n * n % MOD, e /= 2)
        if (e & 1) ans = ans * n % MOD;
    return ans;
}

int64_t div(int64_t a, int64_t b) {
    a %= MOD, b %= MOD;
    int64_t inv_b = pw(b, MOD - 2);
    return mul(a, inv_b);
}
vector<int64_t> fact,invFact;
void pre(int n) {
    fact.assign(n + 1, 1);
    invFact.assign(n + 1, 1);
    for (int i = 1; i <= n; i++) {
        fact[i] = mul(fact[i - 1], i);
    }
    invFact[n] = pw(fact[n], MOD - 2);
    for (int i = n - 1; i >= 0; i--) {
        invFact[i] = mul(invFact[i + 1], i + 1);
    }
}

int64_t npr(int64_t n, int64_t r) {
    if (n < r || r < 0) return 0;
    return mul(fact[n], invFact[n - r]);
}

int64_t ncr(int64_t n, int64_t r) {
    if (n < r || r < 0) return 0;
    return mul(npr(n, r), invFact[r]);
}

int64_t stars_and_bars(int64_t star, int64_t bar) {
    return ncr(star + bar, star);
}
// Standard Catalan Number
int64_t catalan(int64_t n, int64_t k = 0) {
    if (2 * n - k < n) return 0;
    // catalan = ncr(2n - k, n) * (k + 1) / (n + 1)
    int64_t ways = mul(ncr(2 * n - k, n), k + 1);
    return div(ways, n + 1);
}

// Generalized Catalan Number
// Number of bracket sequences of length 2*n starting with 'a' '(' and ending with 'b' ')'
int64_t catalan_gen(int64_t n, int64_t a = 0, int64_t b = 0) {
    if (n < 0 || a < 0 || b < 0 || a > n || b > n) return 0;
    int64_t len = 2 * n - a - b;
    return sub(ncr(len, n - a), ncr(len, n + 1));
}

// Permutations of a multiset (Permutation with repetition)
int64_t permutation_with_repetition(vector<int>& frq) {
    int64_t sum = 0;
    for (int x : frq) sum += x;

    int64_t ret = fact[sum];
    for (int x : frq) {
        ret = mul(ret, invFact[x]);
    }
    return ret;
}

// Lucas Theorem for large n, r
// O(log(n) / log(MOD))
int64_t ncr_lucas(int64_t n, int64_t r) {
    if (r > n || r < 0) return 0;
    if (n < MOD) return ncr(n, r);
    return mul(ncr_lucas(n / MOD, r / MOD), ncr_lucas(n % MOD, r % MOD));
}

// Derangements: Count of permutations where no element appears in its original position
int64_t derangement(int n) {
    if (n < 0) return 0;
    std::vector<int64_t> d(n + 1, 0);
    if (n >= 0) d[0] = 1;
    if (n >= 1) d[1] = 0;

    for (int i = 2; i <= n; i++) {
        d[i] = mul(i - 1, add(d[i - 1], d[i - 2]));
    }
    return d[n];
}

// Burnside's Lemma / Pólya Enumeration for necklaces
int64_t count_necklaces(int64_t beads, int64_t colors) {
    if (beads == 0) return 0;
    int64_t total = 0;
    for (int64_t rot = 0; rot < beads; rot++) {
        total = add(total, pw(colors, std::gcd(rot, beads)));
    }
    return div(total, beads);
}

// Stirling Numbers of the First Kind (Count permutations of n items with k cycles)
int64_t stirling_kind1(int n, int k) {
    if (n == 0 && k == 0) return 1;
    if (k == 0 || n < k) return 0;

    return add(mul(n - 1, stirling_kind1(n - 1, k)), stirling_kind1(n - 1, k - 1));
}

// Stirling Numbers of the Second Kind (Count ways to divide n distinct items into k non-empty groups)
int64_t stirling_kind2(int n, int k) {
    if (n == 0 && k == 0) return 1;
    if (k == 0 || n < k) return 0;

    return add(mul(k, stirling_kind2(n - 1, k)), stirling_kind2(n - 1, k - 1));
}
// Combinations with Repetition (Multichoose)
// Count the number of ways to choose r items from n distinct types (with replacement)
int64_t ncr_repetition(int64_t n, int64_t r) {
    if (n == 0 && r == 0) return 1;
    if (n == 0) return 0;
    return ncr(n + r - 1, r);
}

// Total subsets / Sum of all combinations
// Sum of nCr(n, i) for i = 0 to n. (Equivalent to 2^n)
int64_t total_combinations(int64_t n) {
    return pw(2, n);
}

// Eulerian Numbers (Recursive)
// Counts permutations of length n with exactly m ascents (elements greater than the previous)
// Note: For large n, use dynamic programming to avoid TLE.
int64_t eulerian_number(int n, int m) {
    if (m >= n || n == 0) return 0;
    if (m == 0) return 1;
    
    int64_t term1 = mul(n - m, eulerian_number(n - 1, m - 1));
    int64_t term2 = mul(m + 1, eulerian_number(n - 1, m));
    return add(term1, term2);
}

// Bell Numbers (O(N^2) using Bell Triangle)
// Counts the number of ways to partition a set of n elements into non-empty subsets.
int64_t bell_number(int n) {
    if (n == 0) return 1;
    std::vector<std::vector<int64_t>> b(n + 1, std::vector<int64_t>(n + 1, 0));
    b[0][0] = 1;
    
    for (int i = 1; i <= n; i++) {
        b[i][0] = b[i - 1][i - 1]; // Start row with the last element of the previous row
        for (int j = 1; j <= i; j++) {
            b[i][j] = add(b[i - 1][j - 1], b[i][j - 1]);
        }
    }
    return b[n][0];
}

// Integer Partitions (Recursive)
// Counts the number of ways to write integer n as a sum of exactly k positive integers.
// Note: Order does not matter (e.g., 2+1 and 1+2 are the same). For large n, use DP.
int64_t partition_integer(int n, int k) {
    if (n == k) return 1;
    if (n < k || k == 0) return 0;
    return add(partition_integer(n - 1, k - 1), partition_integer(n - k, k));
}

// Lah Numbers
// Counts the number of ways to partition a set of n elements into k non-empty linearly ordered subsets.
// Formula: L(n,k) = C(n-1, k-1) * (n! / k!)
int64_t lah_number(int64_t n, int64_t k) {
    if (k > n || k == 0 || n == 0) return 0;
    if (n == k) return 1;
    
    int64_t part1 = ncr(n - 1, k - 1);
    int64_t part2 = div(fact[n], fact[k]);
    return mul(part1, part2);
}

// Narayana Numbers
// Used to count expressions containing n pairs of parentheses, which are correctly matched and which contain k distinct nestings.
// Formula: N(n,k) = (1 / n) * C(n, k) * C(n, k-1)
int64_t narayana_number(int64_t n, int64_t k) {
    if (k > n || k < 1 || n < 1) return 0;
    int64_t part1 = ncr(n, k);
    int64_t part2 = ncr(n, k - 1);
    
    int64_t num = mul(part1, part2);
    return div(num, n);
}
