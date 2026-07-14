template <int ALPHABET = 26, char BASE = 'a'>
struct Trie {
    struct Node {
        int nxt[ALPHABET];
        int words, prefixes;
        Node() {
            memset(nxt, -1, sizeof(nxt));
            words = prefixes = 0;
        }
    };

    vector<Node> tree;

    bool valid(int u) {
        return u != -1 && tree[u].prefixes > 0;
    }

    int new_node() {
        tree.emplace_back();
        return tree.size() - 1;
    }

    void insert(const string &s, int cnt = 1) {
        int u = 0;
        tree[u].prefixes += cnt;
        for (char c : s) {
            int v = c - BASE;
            if (tree[u].nxt[v] == -1)
                tree[u].nxt[v] = new_node();
            u = tree[u].nxt[v];
            tree[u].prefixes += cnt;
        }
        tree[u].words += cnt;
    }

    int count_word(const string &s) {
        int u = 0;
        for (char c : s) {
            int v = c - BASE;
            if (!valid(tree[u].nxt[v]))
                return 0;
            u = tree[u].nxt[v];
        }
        return tree[u].words;
    }

    int count_prefix(const string &s) {
        int u = 0;
        for (char c : s) {
            int v = c - BASE;
            if (!valid(tree[u].nxt[v]))
                return 0;
            u = tree[u].nxt[v];
        }
        return tree[u].prefixes;
    }

    void erase(const string &s, int cnt = 1) {
        cnt = min(cnt, count_word(s));
        if (!cnt)
            return;
        int u = 0;
        tree[u].prefixes -= cnt;
        for (char c : s) {
            u = tree[u].nxt[c - BASE];
            tree[u].prefixes -= cnt;
        }
        tree[u].words -= cnt;
    }

    void dfs(int u, string &cur, vector<string> &res) {
        if (tree[u].words)
            res.push_back(cur);
        for (int i = 0; i < ALPHABET; i++) {
            if (!valid(tree[u].nxt[i]))
                continue;
            cur.push_back(BASE + i);
            dfs(tree[u].nxt[i], cur, res);
            cur.pop_back();
        }
    }

    vector<string> autocomplete(const string &prefix) {
        vector<string> res;
        int u = 0;
        for (char c : prefix) {
            int v = c - BASE;
            if (!valid(tree[u].nxt[v]))
                return res;
            u = tree[u].nxt[v];
        }
        string cur = prefix;
        dfs(u, cur, res);
        return res;
    }
    // k-th lexicographical string (1-based)
    string get_kth(int k) {
        if (k <= 0 || k > tree[0].prefixes)
            return "";
        string res;
        int u = 0;
        while (true) {
            if (k <= tree[u].words)
                return res;
            k -= tree[u].words;
            for (int i = 0; i < ALPHABET; i++) {
                int v = tree[u].nxt[i];
                if (!valid(v))
                    continue;
                if (k <= tree[v].prefixes) {
                    res.push_back(BASE + i);
                    u = v;
                    break;
                }
                k -= tree[v].prefixes;
            }
        }
    }
    
    int count_less(const string &s) {
        int u = 0;
        int ans = 0;
        for (char c : s) {
            int v = c - BASE;
            ans += tree[u].words;
            for (int i = 0; i < v; i++)
                if (valid(tree[u].nxt[i]))
                    ans += tree[tree[u].nxt[i]].prefixes;
            if (!valid(tree[u].nxt[v]))
                return ans;
            u = tree[u].nxt[v];
        }
        return ans;
    }

    string longest_prefix(const string &s) {
        string res;
        int u = 0;
        for (char c : s) {
            int v = c - BASE;
            if (!valid(tree[u].nxt[v]))
                break;
            res.push_back(c);
            u = tree[u].nxt[v];
        }
        return res;
    }

    string longest_word_prefix(const string &s) {
        string res;
        int u = 0;
        for (int i = 0; i < (int)s.size(); i++) {
            int v = s[i] - BASE;
            if (!valid(tree[u].nxt[v]))
                break;
            u = tree[u].nxt[v];
            if (tree[u].words)
                res = s.substr(0, i + 1);
        }
        return res;
    }

    string unique_prefix(const string &s) {
        string res;
        int u = 0;
        for (char c : s) {
            u = tree[u].nxt[c - BASE];
            res.push_back(c);
            if (tree[u].prefixes == 1)
                break;
        }
        return res;
    }

    int distinct_words() {
        int ans = 0;
        for (auto &node : tree)
            ans += (node.words > 0);
        return ans;
    }

    int size() {
        return tree[0].prefixes;
    }
};
