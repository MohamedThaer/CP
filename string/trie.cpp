class trie {
private:
    struct node {
        int word = 0, prefix = 0, child[26];
        node() {
            memset(child, -1, sizeof child);
        }
    };
    vector<node> tree = vector<node>(1);

    bool valid(int node) { return node != -1 && 0 < tree[node].prefix; }

public:
    void insert(const string &s, int cnt = 1) {
        int cur = 0;
        tree[cur].prefix += cnt;
        for (auto c : s) {
            int to = c - 'a';
            if (tree[cur].child[to] == -1) {
                tree[cur].child[to] = tree.size();
                tree.emplace_back();
            }
            cur = tree[cur].child[to];
            tree[cur].prefix += cnt;
        }
        tree[cur].word += cnt;
    }

    int count_word(const string &s) {
        int cur = 0;
        for (auto c : s) {
            int to = c - 'a';
            if (!valid(tree[cur].child[to])) {
                return 0;
            }
            cur = tree[cur].child[to];
        }
        return tree[cur].word;
    }

    int count_prefix(const string &s) {
        int cur = 0;
        for (auto c : s) {
            int to = c - 'a';
            if (!valid(tree[cur].child[to])) {
                return 0;
            }
            cur = tree[cur].child[to];
        }
        return tree[cur].prefix;
    }
};
