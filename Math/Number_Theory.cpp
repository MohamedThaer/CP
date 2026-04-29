const int N = 1e6 + 5;
/******************** sieve_all ********************/
// init: sieve_all() 
int phi[N], spf[N], mob[N];
bitset<N> is_prime;
void sieve_all() {
    is_prime.set();
    is_prime[0] = is_prime[1] = false;
    for (int i = 1; i < N; i++) {
        phi[i] = spf[i] = i;
        mob[i] = 1;
    }
    mob[0] = 0;
    for (int i = 2; i < N; i++) {
        if (is_prime[i]) {
            for (int j = i; j < N; j += i) {
                if (i != j) is_prime[j] = false;
                if (spf[j] == j) spf[j] = i;
                phi[j] -= phi[j] / i;
                mob[j] *= -1;
            }
            int64_t sq = 1LL * i * i;
            if (sq < N) {
                for (int64_t j = sq; j < N; j += sq)
                    mob[j] = 0;
            }
        }
    }
}

/******************** nextPrime ********************/
// init: sieve_all() :: is_prime
int nextPrime[N];
void init_next_prime() {
    int last = 1e9 + 7;
    for (int i = N - 1; i >= 0; i--) {
        nextPrime[i] = last;
        if (is_prime[i]) last = i;
    }
}

/******************** prime factor ********************/
// init: sieve_all() :: spf
vector<pair<int, int>> prime_factor_spf(int n) {
    vector<pair<int, int>> f;
    while (n > 1) {
        int p = spf[n], cnt = 0;
        while (n % p == 0) {
            n /= p;
            cnt++;
        }
        f.push_back({p, cnt});
    }
    return f;
}
vector<int> factor_spf(int n) {
    vector<int> f;
    while (n > 1) {
        f.push_back(spf[n]);
        n /= spf[n];
    }
    return f;
}

// O(sqrt(n))
vector<pair<int, int>> prime_factor(int n) {
    vector<pair<int, int>> f;
    for (int p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            int cnt = 0;
            while (n % p == 0) {
                n /= p;
                cnt++;
            }
            f.push_back({p, cnt});
        }
    }
    if (n > 1) f.push_back({n, 1});
    return f;
}
/******************** number of divisors ********************/
// init: sieve_all() :: spf
int64_t number_of_divisors_spf(int n) {
    int64_t ans = 1;
    while (n > 1) {
        int p = spf[n], cnt = 0;
        while (n % p == 0) {
            n /= p;
            cnt++;
        }
        ans *= (cnt + 1);
    }
    return ans;
}

