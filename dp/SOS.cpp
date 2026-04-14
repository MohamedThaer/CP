
namespace SOS {
    // s' \$ s defines all subsets of s
    int power_two_size(int n) { return (__lg(n - 1) + 1); }
    // z(f(s))=\sum_{s' \$ s}{f(s')}
    // O(B * 2 ^ B)
    // zeta transform is actually SOS DP
    template <class T>
    vector<T> zeta_transform(vector<T> f) {
        int B = power_two_size(f.size());
        f.resize(1 << B);
        for (int i = 0; i < B; i++) {
            for (int mask = 0; mask < (1 << B); mask++) {
                if ((mask & (1 << i)) != 0) {
                    f[mask] += f[mask ^ (1 << i)]; // you can change the operator from + to min/gcd to find min/gcd of all f[submasks]
                }
            }
        }
        return f;
    }
    // z(f(s))=\sum_{s' is supermask of s}{f(s')}
    // O(B * 2 ^ B)
    // zeta transform is actually SOS DP
    template <class T>
    vector<T> zeta_transform_for_supermasks(vector<T> f) {
        int B = power_two_size(f.size());
        f.resize(1 << B);
        for (int i = 0; i < B; i++) {
            for (int mask = (1 << B) - 1; mask >= 0; mask--) {
                if ((mask & (1 << i)) == 0) f[mask] += f[mask ^ (1 << i)];
            }
        }
        return f;
    }
    // mu(f(s))=\sum_{s' \$ s}{(-1)^|s\s'| * f(s')}
    // O(B * 2 ^ B)
    template <class T>
    vector<T> mobius_transform(vector<T> f) {
        int B = power_two_size(f.size());
        f.resize(1 << B);
        for (int i = 0; i < B; i++) {
            for (int mask = 0; mask < (1 << B); mask++) {
                if ((mask & (1 << i)) != 0) {
                    f[mask] -= f[mask ^ (1 << i)];
                }
            }
        }
        return f;
    }
    template <class T>
    vector<T> mobius_transform_for_supermasks(vector<T> f) {
        int B = power_two_size(f.size());
        f.resize(1 << B);
        for (int b = 0; b < B; b++) {
            for (int mask = 0; mask < (1 << B); mask++) {
                if ((mask & (1 << b)) == 0) {
                    f[mask] -= f[mask | (1 << b)];
                }
            }
        }
        return f;
    }
    template <class T>
    vector<T> inverse_zeta_transform(vector<T> f) {
        return mobius_transform(f);
    }
    template <class T>
    vector<T> inverse_mobius_transform(vector<T> f) {
        return zeta_transform(f);
    }

    // no empty subset
    vector<Z> and_subsets(vector<Z> frq) {
        int B = power_two_size(frq.size());
        frq.resize(1 << B);
        frq = SOS::zeta_transform_for_supermasks(frq);
        for (int i = 0; i < (1 << B); ++i) {
            frq[i] = Z(2).power(frq[i].value) - 1;
        }
        frq = SOS::mobius_transform_for_supermasks(frq);
        return frq;
    }

    // no empty subset
    vector<Z> or_subsets(vector<Z> frq) {
        int B = power_two_size(frq.size());
        frq.resize(1 << B);
        frq = SOS::zeta_transform(frq);
        for (int i = 0; i < (1 << B); ++i) {
            frq[i] = Z(2).power(frq[i].value) - 1;
        }
        frq = SOS::mobius_transform(frq);
        return frq;
    }

    // f*g(s)=sum_{s' \$ s} {F(s')*G(s\s')}
    // O(B * B * 2 ^ B)
    /* c[i|j] += [i&j==0]*a[i]*b[j] */
    template <class T>
    vector<T> subset_sum_convolution(vector<T> F, vector<T> G) {
        int B = max(power_two_size(F.size()), power_two_size(G.size()));
        F.resize(1 << B), G.resize(1 << B);
        vector<vector<T>> f(B + 1, vector<T>(1 << B, 0));
        vector<vector<T>> g(B + 1, vector<T>(1 << B, 0));

        for (int mask = 0; mask < (1 << B); mask++) {
            f[__builtin_popcount(mask)][mask] = F[mask];
            g[__builtin_popcount(mask)][mask] = G[mask];
        }

        for (int k = 0; k <= B; k++) {
            // do SOS -> f[k][mask] will be = number of submasks of mask such that |submask| = k
            // same for g[k][mask]
            for (int i = 0; i <= B; i++) {
                for (int mask = 0; mask < (1 << B); mask++) {
                    if (mask >> i & 1) {
                        f[k][mask] += f[k][mask ^ (1 << i)];
                        g[k][mask] += g[k][mask ^ (1 << i)];
                    }
                }
            }
        }

        vector<vector<T>> h(B + 1, vector<T>(1 << B, 0));
        // h[k][mask] = number of pairs (u, v) such that u belongs to F
        // and v belongs to G and (u | v) is a submask of mask and |u| + |v| = k
        for (int mask = 0; mask < (1 << B); mask++) {
            for (int k = 0; k <= B; k++) {
                for (int i = 0; i <= k; i++) {
                    h[k][mask] += f[i][mask] * g[k - i][mask];
                }
            }
        }

        for (int k = 0; k <= B; k++) {
            // do inverse SOS -> h[k][mask] will be = same as above but now (u | v) = mask
            for (int i = 0; i <= B; i++) {
                for (int mask = 0; mask < (1 << B); mask++) {
                    if (mask >> i & 1) {
                        h[k][mask] -= h[k][mask ^ (1 << i)];
                    }
                }
            }
        }

        // and, when u | v = mask and |u| + |v| = |mask|, that means u & v = 0 and we are done!
        vector<T> ans(1 << B, 0);
        for (int mask = 0; mask < (1 << B); mask++) {
            ans[mask] = h[__builtin_popcount(mask)][mask];
        }
        return ans;
    }
}
