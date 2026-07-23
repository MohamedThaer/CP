uint64_t hash_num(uint64_t x) {
    static const uint64_t SEED = chrono::steady_clock::now().time_since_epoch().count();
    x += 0x9e3779b97f4a7c15 + SEED;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
}

struct MultisetHash {
    uint64_t h = 0;
    void add(int v) { h += hash_num(v); }
    void remove(int v) { h -= hash_num(v); }
    bool operator==(const MultisetHash &o) const { return h == o.h; }
};

struct SubarrayHash {
    vector<uint64_t> pre;

    SubarrayHash(const vector<int> &a) {
        int sz = a.size();
        pre.assign(sz + 1, 0);
        for (int i = 0; i < sz; i++) {
            pre[i + 1] = pre[i] + hash_num(a[i]);
        }
    }

    uint64_t subarr(int l, int r) {
        return pre[r + 1] - pre[l];
    }
};
