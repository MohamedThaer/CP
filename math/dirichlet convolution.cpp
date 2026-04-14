

/**
multiplicative functions:
I(x): I(p^k) = 1
ID(x): ID(p^k) = p^k
E(x): E(p^k) = [p^k = 1]
ID_a(x): ID_a(p^k) = p^(a*k)
phi(x): phi(p^k) = p^k - p^(k-1)
mobius(x): mu(p^k) = [k = 0] - [k = 1]
sigma_a(x): sum of all divisors raised to a-th power ----> sigma_a(p^k) = summation(i,0,k){p^(a*i)}

-------------------------------------------
(f * g)(n) = sum; for(d : divisors(n)) sum += f(d) * g(n / d)
lets call the dirichlet convolution of f, g ----> c = (f * g)

Prefix sum of multiplicative functions :
  p_f : the prefix sum of f(x) (1 <= x <= T).
  p_g : the prefix sum of g(x) (1 <= x <= N).
  p_c : the prefix sum of f * g(x) (1 <= x <= N).
  T : the threshold, generally should be n ^ (2 / 3). for n = 1e9, T = 1e6
Magic Functions for different f(x)
For f(x) = phi(x): Id(x) = phi * I(x) i.e. p_c = prefix sum of Id(x), p_g = prefix sum of I(x). Here Id(n) = n, I(n) = 1
For f(x) = mu(x): e(x) = mu * I(x). Here e(x) = x == 1 ? 1 : 0
Complexity: O(n^(2/3))
**/
class Dirichlet {
private:
    // solution for f(x) = phi(x)
    vector<Z> dp;

public:
    Dirichlet(int64_t N) {
        int threshold = powl(N, 2.0 / 3.0);
        factorizer fac(threshold);

        dp.assign(threshold, 0);
        for (int i = 1; i < dp.size(); i++) {
            dp[i] = dp[i - 1] + fac.phi(i);
        }
    }

    Z p_c(int64_t n) {
        if (n % 2 == 0) return (Z)(n / 2) * (n + 1);
        return (Z)((n + 1) / 2) * n;
    }

    Z p_g(int64_t n) {
        return n;
    }

    map<int64_t, Z> mp;
    Z p_f(int64_t x) { // O(sqrt(n))
        if (x < dp.size()) return dp[x];
        if (mp.find(x) != mp.end()) return mp[x];

        Z ans = 0;
        for (int64_t i = 2, last; i <= x; i = last + 1) {
            last = x / (x / i);
            ans += p_f(x / i) * (p_g(last) - p_g(i - 1));
        }
        ans = p_c(x) - ans;
        ans /= p_g(1) - p_g(0);
        return mp[x] = ans;
    }
};
