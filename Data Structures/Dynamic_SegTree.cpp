
template<class node, int64_t DEFAULT = 0>
class dynamic_seg_tree {
private:
    int64_t LX, RX;
    vector<node> seg;

    int create_node(int child_idx, int64_t lx, int64_t rx) {
        if (child_idx == -1) {
            int new_idx = seg.size();
            seg.emplace_back();
            seg[new_idx].apply(lx, rx, DEFAULT);
            return new_idx;
        }
        return child_idx;
    }

    void modify(int x, int64_t lx, int64_t rx, int64_t l, int64_t r, int64_t val) {
        if (l <= lx && rx <= r) {
            return seg[x].apply(lx, rx, val);
        }
        int64_t mid = (lx + rx) >> 1;

        int left = create_node(seg[x].left, lx, mid);
        int right = create_node(seg[x].right, mid + 1, rx);

        seg[x].left = left;
        seg[x].right = right;

        seg[x].propagate(seg[left], seg[right], lx, rx, mid);

        if (l <= mid) {
            modify(left, lx, mid, l, r, val);
        }
        if (mid + 1 <= r) {
            modify(right, mid + 1, rx, l, r, val);
        }
        seg[x].merge(seg[left], seg[right]);
    }

    node query(int x, int64_t lx, int64_t rx, int64_t l, int64_t r) {
        if (l <= lx && rx <= r) {
            return seg[x];
        }
        int64_t mid = (lx + rx) >> 1;

        int left = create_node(seg[x].left, lx, mid);
        int right = create_node(seg[x].right, mid + 1, rx);

        seg[x].left = left;
        seg[x].right = right;

        seg[x].propagate(seg[left], seg[right], lx, rx, mid);

        if (r < mid + 1) {
            return query(left, lx, mid, l, r);
        }
        if (mid < l) {
            return query(right, mid + 1, rx, l, r);
        }
        node ret;
        ret.merge(query(left, lx, mid, l, r), query(right, mid + 1, rx, l, r));
        return ret;
    }

public:
    dynamic_seg_tree(int64_t LX, int64_t RX) : seg(1), LX(LX), RX(RX) {
        seg[0].apply(LX, RX, DEFAULT);
    }

    void update(int64_t l, int64_t r, int64_t val) {
        modify(0, LX, RX, l, r, val);
    }

    node query(int64_t l, int64_t r) {
        return query(0, LX, RX, l, r);
    }
};

struct node {
    int64_t sum = 0, lazy = 0;
    int left = -1, right = -1;

    void apply(int64_t lx, int64_t rx, int64_t val = 0) {
        val %= MOD;
        sum = (sum + (rx - lx + 1) % MOD * val) % MOD;
        lazy = (lazy + val) % MOD;
    }

    void merge(const node &a, const node &b) {
        sum = (a.sum + b.sum) % MOD;
    }

    void propagate(node &left, node &right, int64_t lx, int64_t rx, int64_t mid) {
        if (lazy != 0) {
            left.apply(lx, mid, lazy);
            right.apply(mid + 1, rx, lazy);
            lazy = 0;
        }
    }
};

struct node {
    int64_t sum = 0, lazy = -1;
    int left = -1, right = -1;

    void apply(int64_t lx, int64_t rx, int64_t val = 0) {
        sum = (rx - lx + 1) * val;
        lazy = val;
    }

    void merge(const node &a, const node &b) {
        sum = a.sum + b.sum;
    }

    void propagate(node &left, node &right, int64_t lx, int64_t rx, int64_t mid) {
        // if (lazy != -1) {
        //     left.apply(lx, mid, lazy);
        //     right.apply(mid + 1, rx, lazy);
        //     lazy = -1;
        // }
    }
};
