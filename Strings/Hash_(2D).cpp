class hash_2d {
private:
    static const int64_t mod = (1LL << 61) - 1;
    vector<int64_t> pow_r{1}, pow_c{1};
    vector<vector<int64_t>> hash;

public:
    hash_2d(const auto &grid) {
          mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
          static const int64_t base_r = (mod >> 2) + rng() % (mod >> 1);
          static const int64_t base_c = (mod >> 3) + rng() % (mod >> 2);

        int n = grid.size();
        int m = grid.empty() ? 0 : grid[0].size();

        for (int i = 0; i < n; i++) {
            pow_r.push_back((__int128_t)pow_r[i] * base_r % mod);
        }
        for (int i = 0; i < m; i++) {
            pow_c.push_back((__int128_t)pow_c[i] * base_c % mod);
        }

        hash.assign(n + 1, vector<int64_t>(m + 1, 0));

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                int64_t top = (__int128_t)hash[i][j + 1] * base_r % mod;
                int64_t left = (__int128_t)hash[i + 1][j] * base_c % mod;
                int64_t top_left = (__int128_t)hash[i][j] * base_r % mod * base_c % mod;

                int64_t cur = (top + left - top_left + grid[i][j]) % mod;
                hash[i + 1][j + 1] = cur + (cur < 0 ? mod : 0);
            }
        }
    }

    int64_t query(int r1, int c1, int r2, int c2) {
        int r_len = r2 - r1 + 1;
        int c_len = c2 - c1 + 1;

        int64_t ans = hash[r2 + 1][c2 + 1];
        int64_t top = (__int128_t)hash[r1][c2 + 1] * pow_r[r_len] % mod;
        int64_t left = (__int128_t)hash[r2 + 1][c1] * pow_c[c_len] % mod;
        int64_t top_left = (__int128_t)hash[r1][c1] * pow_r[r_len] % mod * pow_c[c_len] % mod;

        ans = (ans - top - left + top_left) % mod;
        return ans + (ans < 0 ? mod : 0);
    }
};
