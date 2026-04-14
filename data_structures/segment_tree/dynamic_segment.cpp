
template <class node, int64_t DEFAULT = 0>
class seg_tree {
private:
    int64_t LX, RX;
    vector<node> seg;

    int create_node(int &x, int64_t lx, int64_t rx) {
        if (x == -1) {
            x = seg.size();
            seg.emplace_back();
            seg[x].apply(lx, rx, DEFAULT);
        }
        return x;
    }

    template <class... M>
    void modify(int x, int64_t lx, int64_t rx, int64_t l, int64_t r, const M &...val) {
        if (l <= lx && rx <= r) {
            return seg[x].apply(lx, rx, val...);
        }
        int64_t mid = lx + rx >> 1;
        int left = create_node(seg[x].left, lx, mid);
        int right = create_node(seg[x].right, mid + 1, rx);
        seg[x].propagate(seg[left], seg[right], lx, rx, mid);
        if (l <= mid) {
            modify(left, lx, mid, l, r, val...);
        }
        if (mid + 1 <= r) {
            modify(right, mid + 1, rx, l, r, val...);
        }
        seg[x].merge(seg[left], seg[right]);
    }

    node query(int x, int64_t lx, int64_t rx, int64_t l, int64_t r) {
        if (l <= lx && rx <= r) {
            return seg[x];
        }
        int64_t mid = lx + rx >> 1;
        int left = create_node(seg[x].left, lx, mid);
        int right = create_node(seg[x].right, mid + 1, rx);
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

    pair<int, node> find(int x, int64_t lx, int64_t rx, node *last, int64_t l, int64_t r, int dir, const auto &F) {
        if (l <= lx && rx <= r) {
            node cur_node = seg[x];
            if (last) {
                dir ? cur_node.merge(*last, seg[x]) : cur_node.merge(seg[x], *last);
            }
            if (!F(cur_node)) return {-1, cur_node};
            if (lx == rx) return {lx, cur_node};
        }
        int64_t mid = lx + rx >> 1;
        int left = create_node(seg[x].left, lx, mid);
        int right = create_node(seg[x].right, mid + 1, rx);
        seg[x].propagate(seg[left], seg[right], lx, rx, mid);
        if (r < mid + 1) {
            return find(left, lx, mid, last, l, r, dir, F);
        }
        if (mid < l) {
            return find(right, mid + 1, rx, last, l, r, dir, F);
        }
        auto [idx, nd] = (dir ? find(left, lx, mid, last, l, r, dir, F) : find(right, mid + 1, rx, last, l, r, dir, F));
        if (~idx) return {idx, nd};
        return dir ? find(right, mid + 1, rx, &nd, l, r, dir, F) : find(left, lx, mid, &nd, l, r, dir, F);
    }

public:
    seg_tree(int64_t LX, int64_t RX) : seg(1), LX(LX), RX(RX) {
        seg[0].apply(LX, RX, DEFAULT);
    }

    template <class... M>
    void update(int64_t l, int64_t r, const M &...val) {
        modify(0, LX, RX, l, r, val...);
    }

    node query(int64_t l, int64_t r) {
        return query(0, LX, RX, l, r);
    }

    int find_first(int64_t l, int64_t r, const auto &F) {
        return find(0, LX, RX, nullptr, l, r, 1, F).first;
    }

    int find_last(int64_t l, int64_t r, const auto &F) {
        return find(0, LX, RX, nullptr, l, r, 0, F).first;
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
