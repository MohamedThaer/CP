const int64_t inf = 1e18;
class segtree_beats {
private:
    struct node {
        int64_t max_v = -inf, smax_v = -inf, max_c = 0;
        int64_t min_v = inf, smin_v = inf, min_c = 0;
        int64_t sum = 0;
        int64_t ladd = 0, lval = inf;

        void update_max(int lx, int rx, int64_t val) { // val > smax_v
            int len = rx - lx + 1;
            sum += (val - max_v) * max_c;

            if (max_v == min_v) {
                max_v = min_v = val;
            }
            else if (max_v == smin_v) {
                max_v = smin_v = val;
            }
            else {
                max_v = val;
            }

            if (lval != inf && val < lval) {
                lval = val;
            }
        }

        void update_min(int lx, int rx, int64_t val) { // val < smin_v
            int len = rx - lx + 1;
            sum += (val - min_v) * min_c;

            if (max_v == min_v) {
                max_v = min_v = val;
            }
            else if (smax_v == min_v) {
                min_v = smax_v = val;
            }
            else {
                min_v = val;
            }

            if (lval != inf && lval < val) {
                lval = val;
            }
        }

        void assign_val(int lx, int rx, int64_t val) {
            int len = rx - lx + 1;
            max_v = val;
            smax_v = -inf;
            min_v = val;
            smin_v = inf;
            max_c = min_c = len;

            sum = val * len;
            lval = val;
            ladd = 0;
        }

        void add_val(int lx, int rx, int64_t val) {
            int len = rx - lx + 1;
            max_v += val;
            if (smax_v != -inf) smax_v += val;
            min_v += val;
            if (smin_v != inf) smin_v += val;

            sum += len * val;
            if (lval != inf) {
                lval += val;
            }
            else {
                ladd += val;
            }
        }

        void merge(node left, node right) {
            sum = left.sum + right.sum;

            if (left.max_v < right.max_v) {
                max_v = right.max_v;
                max_c = right.max_c;
                smax_v = max(left.max_v, right.smax_v);
            }
            else if (left.max_v > right.max_v) {
                max_v = left.max_v;
                max_c = left.max_c;
                smax_v = max(left.smax_v, right.max_v);
            }
            else {
                max_v = left.max_v;
                max_c = left.max_c + right.max_c;
                smax_v = max(left.smax_v, right.smax_v);
            }

            if (left.min_v < right.min_v) {
                min_v = left.min_v;
                min_c = left.min_c;
                smin_v = min(left.smin_v, right.min_v);
            }
            else if (left.min_v > right.min_v) {
                min_v = right.min_v;
                min_c = right.min_c;
                smin_v = min(left.min_v, right.smin_v);
            }
            else {
                min_v = left.min_v;
                min_c = left.min_c + right.min_c;
                smin_v = min(left.smin_v, right.smin_v);
            }
        }

        void propagate(node &left, node &right, int lx, int rx, int mid) {
            if (lval != inf) {
                left.assign_val(lx, mid, lval);
                right.assign_val(mid + 1, rx, lval);
                lval = inf;
                return;
            }
            if (ladd != 0) {
                left.add_val(lx, mid, ladd);
                right.add_val(mid + 1, rx, ladd);
                ladd = 0;
            }
            if (max_v < left.max_v) {
                left.update_max(lx, mid, max_v);
            }
            if (left.min_v < min_v) {
                left.update_min(mid + 1, rx, min_v);
            }

            if (max_v < right.max_v) {
                right.update_max(lx, mid, max_v);
            }
            if (right.min_v < min_v) {
                right.update_min(mid + 1, rx, min_v);
            }
        }
    };

    int size;
    vector<node> seg;

    template <class M>
    void build(int x, int lx, int rx, const vector<M> &a) {
        if (lx == rx) {
            return seg[x].assign_val(lx, rx, a[lx]);
        }
        int mid = lx + rx >> 1;
        int left = x + 1;
        int right = x + 2 * (mid - lx + 1);
        build(left, lx, mid, a);
        build(right, mid + 1, rx, a);
        seg[x].merge(seg[left], seg[right]);
    }

    void update_min(int x, int lx, int rx, int l, int r, int64_t val) {
        if (seg[x].max_v <= val) {
            return;
        }
        if (l <= lx && rx <= r && seg[x].smax_v < val) {
            seg[x].update_max(lx, rx, val);
            return;
        }
        int mid = lx + rx >> 1;
        int left = x + 1;
        int right = x + 2 * (mid - lx + 1);
        seg[x].propagate(seg[left], seg[right], lx, rx, mid);
        if (l <= mid) {
            update_min(left, lx, mid, l, r, val);
        }
        if (mid + 1 <= r) {
            update_min(right, mid + 1, rx, l, r, val);
        }
        seg[x].merge(seg[left], seg[right]);
    }

    void update_max(int x, int lx, int rx, int l, int r, int64_t val) {
        if (val <= seg[x].min_v) {
            return;
        }
        if (l <= lx && rx <= r && val < seg[x].smin_v) {
            seg[x].update_min(lx, rx, val);
            return;
        }
        int mid = lx + rx >> 1;
        int left = x + 1;
        int right = x + 2 * (mid - lx + 1);
        seg[x].propagate(seg[left], seg[right], lx, rx, mid);
        if (l <= mid) {
            update_max(left, lx, mid, l, r, val);
        }
        if (mid + 1 <= r) {
            update_max(right, mid + 1, rx, l, r, val);
        }
        seg[x].merge(seg[left], seg[right]);
    }

    void add_val(int x, int lx, int rx, int l, int r, int64_t val) {
        if (l <= lx && rx <= r) {
            seg[x].add_val(lx, rx, val);
            return;
        }
        int mid = lx + rx >> 1;
        int left = x + 1;
        int right = x + 2 * (mid - lx + 1);
        seg[x].propagate(seg[left], seg[right], lx, rx, mid);
        if (l <= mid) {
            add_val(left, lx, mid, l, r, val);
        }
        if (mid + 1 <= r) {
            add_val(right, mid + 1, rx, l, r, val);
        }
        seg[x].merge(seg[left], seg[right]);
    }

    void assign_val(int x, int lx, int rx, int l, int r, int64_t val) {
        if (l <= lx && rx <= r) {
            seg[x].assign_val(lx, rx, val);
            return;
        }
        int mid = lx + rx >> 1;
        int left = x + 1;
        int right = x + 2 * (mid - lx + 1);
        seg[x].propagate(seg[left], seg[right], lx, rx, mid);
        if (l <= mid) {
            assign_val(left, lx, mid, l, r, val);
        }
        if (mid + 1 <= r) {
            assign_val(right, mid + 1, rx, l, r, val);
        }
        seg[x].merge(seg[left], seg[right]);
    }

    node query(int x, int lx, int rx, int l, int r) {
        if (l <= lx && rx <= r) {
            return seg[x];
        }
        int mid = lx + rx >> 1;
        int left = x + 1;
        int right = x + 2 * (mid - lx + 1);
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
    segtree_beats(const vector<int64_t> &v) {
        size = v.size();
        seg.resize(2 * size);
        build(0, 0, size - 1, v);
    }

    void update_min(int l, int r, int64_t val) {
        update_min(0, 0, size - 1, l, r, val);
    }

    void update_max(int l, int r, int64_t val) {
        update_max(0, 0, size - 1, l, r, val);
    }

    void add_val(int l, int r, int64_t val) {
        add_val(0, 0, size - 1, l, r, val);
    }

    void assign_val(int l, int r, int64_t val) {
        assign_val(0, 0, size - 1, l, r, val);
    }

    node query(int l, int r) {
        return query(0, 0, size - 1, l, r);
    }
};
