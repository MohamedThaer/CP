

/*
PersistentBinaryTrie
- Template parameter:
    BITS = number of bits to store (default 31 -> supports values up to < 2^31)
- Usage:
    PersistentBinaryTrie<31> trie;

*/
template <int BITS = 31>
class PersistentBinaryTrie {
    struct node {
        int nxt[2]{}, cnt = 0;
    };

    vector<int> roots;
    vector<node> tree;

    int new_node() {
        tree.emplace_back();
        return tree.size() - 1;
    }

    int clone_node(int v) {
        tree.push_back(tree[v]);
        return tree.size() - 1;
    }

    bool valid(int nd) { return nd != 0 && tree[nd].cnt > 0; }
    bool diff_count(int a, int b) { return abs(tree[b].cnt - tree[a].cnt); }

public:
    PersistentBinaryTrie() {
        tree.emplace_back();
        roots.push_back(new_node()); // root for version 0
    }

    void insert(int version, uint64_t x, int delta = 1) {
        int new_root = clone_node(roots[version]);
        roots[version] = new_root;
        int cur = new_root;

        tree[cur].cnt += delta;
        for (int b = BITS - 1; b >= 0; --b) {
            int bit = (x >> b) & 1;
            int nd = (!tree[cur].nxt[bit] ? new_node() : clone_node(tree[cur].nxt[bit]));
            tree[cur].nxt[bit] = nd;
            cur = nd;
            tree[cur].cnt += delta;
        }
    }

    // max xor of x with elements present in the given version
    // returns -1 if trie empty (no numbers)
    uint64_t max_xor(int version, uint64_t x) {
        int cur = roots[version];
        if (tree[cur].cnt == 0) return -1;
        uint64_t ans = 0;
        for (int b = BITS - 1; b >= 0; --b) {
            int bit = (x >> b) & 1;
            if (valid(tree[cur].nxt[!bit])) {
                ans |= (1ULL << b);
                cur = tree[cur].nxt[!bit];
            }
            else {
                cur = tree[cur].nxt[bit];
            }
        }
        return ans;
    }

    uint64_t min_xor(int version, uint64_t x) {
        return ~max_xor(version, ~x);
    }

    // max xor of x with elements present in the given version
    // returns -1 if trie empty (no numbers)
    uint64_t max_xor(int vl, int vr, uint64_t x) {
        int curl = roots[vl], curr = roots[vr];
        uint64_t ans = 0;
        for (int b = BITS - 1; b >= 0; --b) {
            int bit = (x >> b) & 1;
            if (diff_count(tree[curl].nxt[!bit], tree[curr].nxt[!bit])) {
                ans |= (1ULL << b);
                curl = tree[curl].nxt[!bit];
                curr = tree[curr].nxt[!bit];
            }
            else {
                curl = tree[curl].nxt[bit];
                curr = tree[curr].nxt[bit];
            }
        }
        return ans;
    }

    uint64_t kth_order(int version, uint64_t k) {
        uint64_t cur = roots[version], ret = 0;
        for (int bit = BITS - 1; ~bit; bit--) {
            int left = tree[cur].nxt[0];
            if (left && tree[left].cnt >= k) {
                cur = left;
            }
            else {
                k -= tree[left].cnt;
                cur = tree[cur].nxt[1];
                ret += (1ULL << bit);
            }
        }
        return ret;
    }

    uint64_t kth_order(int vl, int vr, uint64_t k) {
        uint64_t cura = roots[vl], curb = roots[vr], ret = 0;
        for (int bit = BITS - 1; ~bit; bit--) {
            int lefta = tree[cura].nxt[0];
            int leftb = tree[curb].nxt[0];
            if (abs(tree[leftb].cnt - tree[lefta].cnt) >= k) {
                cura = lefta;
                curb = leftb;
            }
            else {
                k -= abs(tree[leftb].cnt - tree[lefta].cnt);
                cura = tree[cura].nxt[1];
                curb = tree[curb].nxt[1];
                ret += (1ULL << bit);
            }
        }
        return ret;
    }

    uint64_t xor_less_than_k(int version, uint64_t x, uint64_t k) {
        uint64_t cur = roots[version], ret = 0;
        for (int bit = BITS - 1; ~bit; --bit) {
            if (!valid(cur)) return ret;
            int to = (x >> bit) & 1;
            if ((k >> bit) & 1) {
                if (valid(tree[cur].nxt[to])) {
                    ret += tree[tree[cur].nxt[to]].cnt;
                }
                cur = tree[cur].nxt[!to];
            }
            else {
                cur = tree[cur].nxt[to];
            }
        }
        return ret;
    }

    uint64_t xor_less_than_k(int vl, int vr, uint64_t x, uint64_t k) {
        return xor_less_than_k(vr, x, k) - xor_less_than_k(vl, x, k);
    }

    uint64_t xor_greater_equal_to_k(int version, uint64_t x, uint64_t k) {
        return tree[roots[version]].cnt - xor_less_than_k(version, x, k);
    }

    uint64_t xor_greater_equal_to_k(int vl, int vr, uint64_t x, uint64_t k) {
        return xor_greater_equal_to_k(vr, x, k) - xor_greater_equal_to_k(vl, x, k);
    }

    // total number of inserted items in that version
    int count_all(int version) { return tree[roots[version]].cnt; }

    int count_value(int version, uint64_t x) {
        int cur = roots[version];
        for (int b = BITS - 1; b >= 0; --b) {
            int bit = (x >> b) & 1;
            if (valid(tree[cur].nxt[bit])) return 0;
            cur = tree[cur].nxt[bit];
        }
        return tree[cur].cnt;
    }

    int push_back(int version) {
        roots.push_back(roots[version]);
        return roots.size() - 1;
    }

    int cur_version() { return roots.size() - 1; }
};
