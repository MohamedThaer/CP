template<int BITS = 31>
struct PersistentBinaryTrie {
    struct Node {
        int child[2];
        int cnt;
        Node() {
            child[0] = child[1] = 0;
            cnt = 0;
        }
    };
    vector<Node> tree;
    vector<int> roots;
    PersistentBinaryTrie() {
        tree.emplace_back();
        roots.push_back(new_node()); // v0
    }
    int new_node() {
        tree.emplace_back();
        return tree.size() - 1;
    }
    int clone_node(int u) {
        tree.push_back(tree[u]);
        return tree.size() - 1;
    }
    bool valid(int u) {
        return u && tree[u].cnt;
    }
    int diff(int a, int b) {
        return tree[b].cnt - tree[a].cnt;
    }
    void insert(int version, int64_t x, int delta = 1) {
        roots[version] = clone_node(roots[version]);
        int cur = roots[version];
        tree[cur].cnt += delta;
        for (int bit = BITS - 1; bit >= 0; bit--) {
            int b = (x >> bit) & 1;
            if (!tree[cur].child[b])
                tree[cur].child[b] = new_node();
            else
                tree[cur].child[b] = clone_node(tree[cur].child[b]);
            cur = tree[cur].child[b];
            tree[cur].cnt += delta;
        }
    }
    // {xor value , element} in range (vl,vr]
    pair<uint64_t, uint64_t> max_xor(int vl, int vr, uint64_t x) {
        int L = roots[vl];
        int R = roots[vr];
        uint64_t xr = 0, val = 0;
        for (int bit = BITS - 1; bit >= 0; bit--) {
            int b = (x >> bit) & 1;
            if (diff(tree[L].child[b ^ 1], tree[R].child[b ^ 1]) > 0) {
                L = tree[L].child[b ^ 1];
                R = tree[R].child[b ^ 1];
                xr |= 1ULL << bit;
                val |= 1ULL * (b ^ 1) << bit;
            } else {
                L = tree[L].child[b];
                R = tree[R].child[b];

                val |= 1ULL * b << bit;
            }
        }

        return {xr, val};
    }
    // k-th smallest in range (vl,vr]
    uint64_t kth(int vl, int vr, int k) {
        int L = roots[vl];
        int R = roots[vr];
        uint64_t ans = 0;
        for (int bit = BITS - 1; bit >= 0; bit--) {
            int leftL = tree[L].child[0];
            int leftR = tree[R].child[0];
            int cntLeft = diff(leftL, leftR);
            if (k <= cntLeft) {
                L = leftL;
                R = leftR;
            } else {
                k -= cntLeft;
                L = tree[L].child[1];
                R = tree[R].child[1];
                ans |= 1ULL << bit;
            }
        }

        return ans;
    }
    int64_t size(int version) {
        return tree[roots[version]].cnt;
    }
    int push_back(int version) {
        roots.push_back(roots[version]);
        return roots.size() - 1;
    }
    int cur_version() {
        return roots.size() - 1;
    }
    
};
