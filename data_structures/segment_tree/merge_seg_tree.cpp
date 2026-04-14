template <class node>
class merge_seg_tree {
private:
    int size;
    vector<node> seg;

    template <class... M>
    void build(int x, int lx, int rx, const vector<M> &...a) {
        if (lx == rx) {
            return seg[x].apply(lx, rx, a[lx]...);
        }
        int mid = lx + rx >> 1;
        int left = x + 1;
        int right = x + 2 * (mid - lx + 1);
        build(left, lx, mid, a...);
        build(right, mid + 1, rx, a...);
        seg[x].merge(seg[left], seg[right]);
    }

    vector<int> nodes;
    void get(int x, int lx, int rx, int l, int r) {
        if (l <= lx && rx <= r) {
            return nodes.push_back(x);
        }
        int mid = lx + rx >> 1;
        int left = x + 1;
        int right = x + 2 * (mid - lx + 1);
        if (l <= mid) {
            get(left, lx, mid, l, r);
        }
        if (mid + 1 <= r) {
            get(right, mid + 1, rx, l, r);
        }
    }

public:
    template <class... M>
    merge_seg_tree(const vector<M> &...a) : size((a.size(), ...)), seg(2 * size) {
        build(0, 0, size - 1, a...);
    }

    int query(int l, int r, int a, int b) {
        nodes.clear(), get(0, 0, size - 1, l, r);

        int ret = 0;
        for (auto nd : nodes) {
            auto &v = seg[nd].v;
            ret += upper_bound(v.begin(), v.end(), b) - lower_bound(v.begin(), v.end(), a);
        }
        return ret;
    }
};

struct node {
    vector<int> v;

    void apply(int lx, int rx, int64_t val = 0) {
        v = {int(val)};
    }

    void merge(const node &a, const node &b) {
        std::merge(a.v.begin(), a.v.end(), b.v.begin(), b.v.end(), back_inserter(v));
    }
};
