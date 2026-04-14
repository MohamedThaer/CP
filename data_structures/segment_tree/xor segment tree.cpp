template <class node>
class XORSegmentTree {
private:
    int size, lg;
    vector<node> seg;

    template <class... M>
    void build(int x, int lx, int rx, const vector<M> &...a) {
        if (lx == rx) {
            if (lx < (a.size(), ...)) seg[x].apply(lx, rx, a[lx]...);
            return;
        }
        int mid = (lx + rx) >> 1;
        int left = 2 * x + 1;
        int right = 2 * x + 2;
        build(left, lx, mid, a...);
        build(right, mid + 1, rx, a...);
        seg[x].merge(seg[left], seg[right]);
    }

    template <class... M>
    void modify(int x, int lx, int rx, int l, int r, const M &...value) {
        if (l <= lx && rx <= r) {
            return seg[x].apply(lx, rx, value...);
        }
        int mid = (lx + rx) >> 1;
        int left = 2 * x + 1;
        int right = 2 * x + 2;
        seg[x].propagate(seg[left], seg[right], lx, rx, mid);
        if (l <= mid) {
            modify(left, lx, mid, l, r, value...);
        }
        if (mid + 1 <= r) {
            modify(right, mid + 1, rx, l, r, value...);
        }
        seg[x].merge(seg[left], seg[right]);
    }

    // layer = LOG - 1
    // return sum of a[p ^ xr] over all l <= p <= r
    // O(LOG)
    node query(int x, int lx, int rx, int l, int r, int xr, int layer) {
        if (l > r || lx > r || rx < l) return node{};
        if (l <= lx && rx <= r) return seg[x];

        int mid = (lx + rx) >> 1;
        int left = 2 * x + 1;
        int right = 2 * x + 2;
        seg[x].propagate(seg[left], seg[right], lx, rx, mid);

        node ret;
        if (xr >> layer & 1) {
            // 1 at this bit: query children and swap ranges
            int i1 = l, j1 = min(mid, r), i2 = max(l, mid + 1), j2 = r;
            ret.merge(query(right, mid + 1, rx, mid + 1 + i1 - lx, mid + 1 + j1 - lx, xr, layer - 1),
                      query(left, lx, mid, lx + i2 - (mid + 1), lx + j2 - (mid + 1), xr, layer - 1));
        }
        else {
            // 0 at this bit: query children in same order
            ret.merge(query(left, lx, mid, l, r, xr, layer - 1),
                      query(right, mid + 1, rx, l, r, xr, layer - 1));
        }
        return ret;
    }

public:
    template <class... M>
    XORSegmentTree(const vector<M> &...a) {
        int n = (a.size(), ...);
        size = 1;
        lg = 0;
        while (size < n) {
            size <<= 1;
            lg++;
        }
        seg.assign(2 * size, {});
        build(0, 0, size - 1, a...);
    }

    template <class... M>
    XORSegmentTree(int n, M... x) {
        size = 1;
        lg = 0;
        while (size < n) {
            size <<= 1;
            lg++;
        }
        seg.assign(2 * size, {});
        build(0, 0, size - 1, vector<M>(n, x)...);
    }

    template <class... M>
    void update(int l, int r, const M &...value) {
        modify(0, 0, size - 1, l, r, value...);
    }

    node query(int l, int r, int x) {
        return query(0, 0, size - 1, l, r, x, lg - 1);
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
        if (lazy != -1) {
            left.apply(lx, mid, lazy);
            right.apply(mid + 1, rx, lazy);
            lazy = -1;
        }
    }
};
