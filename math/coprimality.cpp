
class coprimality {
private:
    vector<int64_t> divisors(int64_t n) {
        vector<int64_t> ret;
        for (int64_t i = 1; i <= n / i; i++) {
            if (n % i == 0) {
                ret.push_back(i);
                if (i * i != n) {
                    ret.push_back(n / i);
                }
            }
        }
        return ret;
    }

    static vector<int> mobius(int N) {
        vector<int> _primes, mob(N + 1, 1);
        vector<bool> _is_prime(N + 1, 1);
        _is_prime[0] = _is_prime[1] = 0;
        for (int64_t i = 1; i <= N; i++) {
            if (_is_prime[i]) {
                mob[i] = -1;
                _primes.push_back(i);
            }
            for (auto it : _primes) {
                if (i * it > N) {
                    break;
                }
                mob[i * it] = !!(i % it) * -mob[i];
                _is_prime[i * it] = 0;
                if (i % it == 0) {
                    break;
                }
            }
        }
        return mob;
    }

    static int64_t mob(int n) {
        static vector<int> m;
        if (m.size() <= n) {
            m = mobius(2 * n);
        }
        return m[n];
    }

    int64_t cur = 0;
    vector<int> div;

public:
    coprimality(int N) : div(N + 1, 0) {}

    void add(int x) {
        auto divs = divisors(x);
        int coprime = 0;
        for (auto d : divs) {
            coprime += mob(d) * div[d];
        }
        cur += coprime;

        for (auto d : divs) {
            div[d]++;
        }
    }

    void remove(int x) {
        auto divs = divisors(x);
        for (auto d : divs) {
            div[d]--;
        }

        int coprime = 0;
        for (auto d : divs) {
            coprime += mob(d) * div[d];
        }
        cur -= coprime;
    }

    int64_t coprime_pairs() {
        return cur;
    }

    int count_coprime(int x) {
        auto divs = divisors(x);
        int coprime = 0;
        for (auto d : divs) {
            coprime += mob(d) * div[d];
        }
        return coprime;
    }
};
