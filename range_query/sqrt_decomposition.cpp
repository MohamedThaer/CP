class SQblocks {
private:
    struct node {
        int64_t sum = 0, lazy = -1;

        void apply(int lb, int rb, int64_t val) { // range assign val from lb to rb
            sum = (rb - lb + 1) * val;
            lazy = val;
        }

        template <class M>
        void propagate(vector<M> &v, int lb, int rb) {
            if (lazy != -1) {
                for (int i = lb; i <= rb; ++i) {
                    v[i] = lazy;
                }
                lazy = -1;
            }
        }
    };

    int n, SQ;
    vector<int64_t> v;
    vector<node> block;

    node &blk(int i) { return block[i / SQ]; }
    pair<int, int> get_blk_range(int i) { return {i / SQ * SQ, min(n, i / SQ * SQ + SQ) - 1}; }

    void add(int i) {
        blk(i).sum += v[i];
    }

    void remove(int i) {
        blk(i).sum -= v[i];
    }

public:
    SQblocks(int _n) : n(_n), v(n), SQ(sqrt(n) + 1), block(SQ) {
        for (int i = 0; i < n; i++) {
            add(i);
        }
    }

    void update_point(int i, int64_t val) {
        remove(i);
        v[i] = val;
        add(i);
    }

    template <class... M>
    void update_range(int l, int r, const M &...val) {
        while (l <= r) {
            if (l % SQ || r - l + 1 < SQ) {
                auto [lb, rb] = get_blk_range(l);
                blk(l).propagate(v, lb, rb);
                update_point(l, val...);
                l += 1;
            }
            else {
                auto [lb, rb] = get_blk_range(l);
                blk(l).apply(lb, rb, val...);
                l += SQ;
            }
        }
    }

    int64_t query(int l, int r) {
        int64_t ret = 0;
        while (l <= r) {
            if (l % SQ || r - l + 1 < SQ) {
                auto [lb, rb] = get_blk_range(l);
                blk(l).propagate(v, lb, rb);
                ret += v[l];
                l += 1;
            }
            else {
                ret += blk(l).sum;
                l += SQ;
            }
        }
        return ret;
    }
};
