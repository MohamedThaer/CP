template <int ALPHA = 26, char BASE = 'a'>
struct PersistentTrie {
    struct Node {
        int nxt[ALPHA];
        int prefix = 0, word = 0;

        Node() {
            nxt.fill(0);
        }
    };

    vector<Node> tree;
    vector<int> roots;

    PersistentTrie() {
        tree.emplace_back();
        roots.push_back(new_node());  // v0
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
        return u && tree[u].prefix > 0;
    }

    void insert(int version, const string &s, int delta = 1) {
        roots[version] = clone_node(roots[version]);
        int cur = roots[version];
        tree[cur].prefix += delta;
        for (char c : s) {
            int id = c - BASE;
            if (!tree[cur].nxt[id])
                tree[cur].nxt[id] = new_node();
            else
                tree[cur].nxt[id] = clone_node(tree[cur].nxt[id]);
            cur = tree[cur].nxt[id];
            tree[cur].prefix += delta;
        }
        tree[cur].word += delta;
    }

    int count_prefix(int version, const string &s) {
        int cur = roots[version];
        for (char c : s) {
            int id = c - BASE;
            if (!valid(tree[cur].nxt[id]))
                return 0;
            cur = tree[cur].nxt[id];
        }
        return tree[cur].prefix;
    }

    int count_word(int version, const string &s) {
        int cur = roots[version];
        for (char c : s) {
            int id = c - BASE;
            if (!valid(tree[cur].nxt[id]))
                return 0;

            cur = tree[cur].nxt[id];
        }
        return tree[cur].word;
    }

    int push_back(int version) {
        roots.push_back(roots[version]);
        return roots.size() - 1;
    }

    int cur_version() {
        return roots.size() - 1;
    }

    int versions() {
        return roots.size();
    }

};
