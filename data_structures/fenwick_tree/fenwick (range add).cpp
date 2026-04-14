class range_fenwick {
private:
    fenwick_tree M, A;

public:
    range_fenwick(int n) : M(n), A(n) {}

    void add(int l, int r, int64_t x) {
        M.add(l, x);
        A.add(l, -x * (l - 1));
        M.add(r, -x);
        A.add(r, x * r);
    }

    int64_t get(int i) {
        return M.get(i) * i + A.get(i);
    }

    int64_t query(int l, int r) {
        return get(r) - get(l - 1);
    }
};
