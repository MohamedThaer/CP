class SegTree {
private:
    using T = int;
    vector<T> tree;
    int n = 1;
    void build(const vector<T> &arr) {
        while (n < arr.size())
            n *= 2;
        tree.resize(n * 2, 0);
        for (int i = 0; i < arr.size(); i++)
            update(i, arr[i]);
    }
    T merge(const T &a, const T &b) {
        return a + b;
    }

public:
    SegTree(int size) {
        while (n < size)
            n *= 2;
        tree.resize(n * 2, 0);
    }
    SegTree(const vector<T> &nums) {
        build(nums);
    }
    void update(int idx, T val) {
        idx += n;
        tree[idx] = val;
        for (idx /= 2; idx >= 1; idx /= 2) {
            tree[idx] = merge(tree[idx * 2], tree[(idx * 2) + 1]);
        }
    }
    T query(int l, int r) {
        if (l > r)
            swap(l, r);
        T ret = 0;
        l += n, r += n;
        while (l <= r) {
            if (l % 2)
                ret = merge(ret, tree[l++]);
            if (!(r % 2))
                ret = merge(ret, tree[r--]);
            l /= 2, r /= 2;
        }
        return ret;
    }
};
