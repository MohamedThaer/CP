class dr_string {
private:
    static const int64_t mod = (1LL << 61) - 1;
    vector<int64_t> pow{1}, hash{0};

public:
    dr_string(const auto &a) {
        static const int64_t base = (mod >> 2) + chrono::steady_clock::now().time_since_epoch().count() % (mod >> 1);

        for (int i = 0; i < a.size(); i++) {
            pow.push_back((__int128_t)pow[i] * base % mod);
            hash.push_back(((__int128_t)hash[i] * base + a[i]) % mod);
        }
    }

    int64_t substr(int l, int r) {
        int64_t ret = hash[r + 1] - (__int128_t)hash[l] * pow[r - l + 1] % mod;
        return ret + (ret < 0 ? mod : 0);
    }

    int64_t merge(int64_t h1, int64_t h2, int len2) {
        return ((__int128_t)h1 * pow[len2] + h2) % mod;
    }
};
