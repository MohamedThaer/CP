class dr_string {
private:
    static const int64_t mod = (1LL << 61) - 1;
    static int64_t base;
    static vector<int64_t> pw, powsum;

    struct node {
        int len, lazy = -1;
        int64_t hash, hash_rev;

        void apply(int lx, int rx, int64_t val = 0) {
            lazy = val;
            len = rx - lx + 1;
            hash = hash_rev = val * (__int128_t)powsum[len] % mod;
        }

        void merge(const node &a, const node &b) {
            len = a.len + b.len;
            hash = (a.hash + b.hash * (__int128_t)pw[a.len]) % mod;
            hash_rev = (a.hash_rev * (__int128_t)pw[b.len] + b.hash_rev) % mod;
        }

        void propagate(node &left, node &right, int lx, int rx, int mid) {
            if (lazy != -1) {
                left.apply(lx, mid, lazy);
                right.apply(mid + 1, rx, lazy);
                lazy = -1;
            }
        }
    };

    segment_tree<node> sg = segment_tree<node>(1);

public:
    dr_string(const auto &a) {
        while (pw.size() <= a.size()) {
            powsum.push_back((pw.back() + powsum.back()) % mod);
            pw.push_back((__int128_t)pw.back() * base % mod);
        }
        sg = segment_tree<node>(vector<int64_t>(a.begin(), a.end()));
    }

    void update(int l, int r, int64_t val) {
        sg.update(l, r, val);
    }

    int64_t substr(int l, int r) {
        return sg.query(l, r).hash;
    }

    int64_t substr_rev(int l, int r) {
        return sg.query(l, r).hash_rev;
    }

    bool is_palindrome(int l, int r) {
        return substr(l, r) == substr_rev(l, r);
    }
};
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
int64_t dr_string::base = (mod >> 2) + rng() % (mod >> 1);
vector<int64_t> dr_string::pw = {1}, dr_string::powsum = {0};
