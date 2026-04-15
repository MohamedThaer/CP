template <class node, class T = int>
class segment_tree {
private:
    int size;
    vector<node> seg;

    void build(int x, int lx, int rx, const vector<T> &a) {
        if (lx == rx) {
            seg[x].apply(lx, rx, a[lx]);
            return;
        }
        int mid = (lx + rx) >> 1;
        int left = x + 1;
        int right = x + 2 * (mid - lx + 1);

        build(left, lx, mid, a);
        build(right, mid + 1, rx, a);
        seg[x].merge(seg[left], seg[right]);
    }

    void modify(int x, int lx, int rx, int l, int r, const T value) {
        if (l <= lx && rx <= r) {
            seg[x].apply(lx, rx, value);
            return;
        }
        int mid = (lx + rx) >> 1;
        int left = x + 1;
        int right = x + 2 * (mid - lx + 1);

        seg[x].propagate(seg[left], seg[right], lx, rx, mid);

        if (l <= mid) modify(left, lx, mid, l, r, value);
        if (mid + 1 <= r) modify(right, mid + 1, rx, l, r, value);

        seg[x].merge(seg[left], seg[right]);
    }

    node query(int x, int lx, int rx, int l, int r) {
        if (l <= lx && rx <= r) return seg[x];

        int mid = (lx + rx) >> 1;
        int left = x + 1;
        int right = x + 2 * (mid - lx + 1);

        seg[x].propagate(seg[left], seg[right], lx, rx, mid);

        if (r <= mid) return query(left, lx, mid, l, r);
        if (l > mid)  return query(right, mid + 1, rx, l, r);

        node ret;
        ret.merge(query(left, lx, mid, l, r), query(right, mid + 1, rx, l, r));
        return ret;
    }

    pair<int, node> find(int x, int lx, int rx, node *last, int l, int r, int dir,
            const function<bool(const node&)> &pred) {
        if (l <= lx && rx <= r) {
            node cur_node = seg[x];
            if (last) {
                dir ? cur_node.merge(*last, seg[x]) : cur_node.merge(seg[x], *last);
            }
            if (!pred(cur_node)) return {-1, cur_node};
            if (lx == rx) return {lx, cur_node};
        }

        int mid = (lx + rx) >> 1;
        int left = x + 1;
        int right = x + 2 * (mid - lx + 1);

        seg[x].propagate(seg[left], seg[right], lx, rx, mid);

        if (r <= mid) return find(left, lx, mid, last, l, r, dir, pred);
        if (l > mid)  return find(right, mid + 1, rx, last, l, r, dir, pred);

        auto res = dir ? find(left, lx, mid, last, l, r, dir, pred)
                : find(right, mid + 1, rx, last, l, r, dir, pred);

        if (res.first != -1) return res;

        return dir ? find(right, mid + 1, rx, &res.second, l, r, dir, pred)
                : find(left, lx, mid, &res.second, l, r, dir, pred);
    }

public:
    segment_tree(int n, const T init_value) : size(n), seg(2 * n) {
        vector<T> a(n, init_value);
        build(0, 0, size - 1, a);
    }

    segment_tree(const vector<T> &a) : size((int)a.size()), seg(2 * size) {
        build(0, 0, size - 1, a);
    }

    void update(int l, int r, const T value) {
        modify(0, 0, size - 1, l, r, value);
    }

    node query(int l, int r) {
        return query(0, 0, size - 1, l, r);
    }

    int find_first(int l, int r, const function<bool(const node&)> &pred) {
        return find(0, 0, size - 1, nullptr, l, r, 1, pred).first;
    }

    int find_last(int l, int r, const function<bool(const node&)> &pred) {
        return find(0, 0, size - 1, nullptr, l, r, 0, pred).first;
    }
};

struct node {
    int64_t sum = 0, lazy = -1;

    void apply(int lx, int rx, int64_t val = 0) {
        sum = (rx - lx + 1) * val;
        lazy = val;
    }

    void merge(const node &a, const node &b) {
        sum = a.sum + b.sum;
    }

    void propagate(node &left, node &right, int lx, int rx, int mid) {
        // if (lazy != -1) {
        //     left.apply(lx, mid, lazy);
        //     right.apply(mid + 1, rx, lazy);
        //     lazy = -1;
        // }
    }
};
