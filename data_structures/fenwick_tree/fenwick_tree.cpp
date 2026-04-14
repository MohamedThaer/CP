class fenwick_tree {
private:
    vector<int64_t> fenw;

public:
    fenwick_tree(int n) {
        fenw.assign(n + 1, 0);
    }

    void add(int i, int64_t val) {
        ++i;
        while (i < fenw.size()) {
            fenw[i] += val;
            i += (i & -i);
        }
    }

    int64_t get(int i) {
        ++i;
        int64_t ret = 0;
        while (i) {
            ret += fenw[i];
            i -= (i & -i);
        }
        return ret;
    }

    void update(int i, int64_t val) {
        add(i, val - query(i, i));
    }

    int64_t query(int l, int r) {
        return get(r) - get(l - 1);
    }

    // first position i where (v <= prefix[i])
    // requires monotocity
    int bit_search(int64_t v) {
        int64_t sum = 0;
        int pos = 0;

        for (int i = __lg((int)fenw.size() - 1); i >= 0; i--) {
            if (pos + (1 << i) < fenw.size() and sum + fenw[pos + (1 << i)] < v) {
                sum += fenw[pos + (1 << i)];
                pos += (1 << i);
            }
        }
        return pos;
    }
};
