struct eertree {
    static const int A = 26;
    static const char base = 'a';

    struct node {
        int len, link, occ, first_pos;
        int nxt[A];

        node(int _len = 0) {
            len = _len;
            link = 0;
            occ = 0;
            first_pos = -1;
            memset(nxt, 0, sizeof nxt);
        }
    };

    vector<node> tree;
    string s;
    int suff;                 // node of longest suffix palindrome
    vector<int> suf_node;     // node of longest pal suffix for each prefix

    eertree() {
        init();
    }

    void init() {
        tree.clear();
        s.clear();
        suf_node.clear();

        tree.push_back(node(-1)); // 0 -> odd root
        tree.push_back(node(0));  // 1 -> even root

        tree[0].link = 0;
        tree[1].link = 0;
        suff = 1;
    }

    inline int get(char c) {
        return c - base;
    }

    int get_link(int v, int pos) {
        while (true) {
            int curlen = tree[v].len;
            if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == s[pos]) break;
            v = tree[v].link;
        }
        return v;
    }

    // add one character, return node id of longest palindrome ending here
    int add_char(char c) {
        s.push_back(c);
        int pos = (int)s.size() - 1;
        int x = get(c);

        int cur = get_link(suff, pos);

        if (!tree[cur].nxt[x]) {
            int now = (int)tree.size();
            tree.push_back(node(tree[cur].len + 2));
            tree[cur].nxt[x] = now;
            tree[now].first_pos = pos;

            if (tree[now].len == 1) {
                tree[now].link = 1;
            } else {
                int link_candidate = get_link(tree[cur].link, pos);
                tree[now].link = tree[link_candidate].nxt[x];
            }
        }

        suff = tree[cur].nxt[x];
        tree[suff].occ++;
        suf_node.push_back(suff);
        return suff;
    }

    // build from string
    void build(const string &t) {
        for (char c : t) add_char(c);
    }

    // propagate occurrences from longer palindromes to suffix links
    void push_occurrences() {
        for (int i = (int)tree.size() - 1; i >= 2; i--) {
            tree[tree[i].link].occ += tree[i].occ;
        }
    }

    // number of distinct palindromes
    int distinct_palindromes() const {
        return (int)tree.size() - 2;
    }

    // longest palindrome suffix length of current string
    int longest_suffix_len() const {
        return tree[suff].len;
    }

    // longest palindrome ending at each position
    vector<int> longest_ending_at_each_pos() const {
        vector<int> ret;
        ret.reserve(suf_node.size());
        for (int id : suf_node) ret.push_back(tree[id].len);
        return ret;
    }

    // all palindrome lengths ending at current suffix chain
    vector<int> suffix_palindrome_lengths() const {
        vector<int> ret;
        int u = suff;
        while (u > 1) {
            ret.push_back(tree[u].len);
            u = tree[u].link;
        }
        return ret;
    }

    // check if whole current string is palindrome
    bool whole_string_is_palindrome() const {
        return !s.empty() && tree[suff].len == (int)s.size();
    }

    // get occurrence count of palindrome node (after push_occurrences)
    int count_occ(int node_id) const {
        return tree[node_id].occ;
    }

    // restore one palindrome string from node id
    string get_palindrome(int node_id) const {
        int len = tree[node_id].len;
        int r = tree[node_id].first_pos;
        int l = r - len + 1;
        return s.substr(l, len);
    }

    // list all distinct palindromes
    vector<string> get_all_palindromes() const {
        vector<string> ret;
        for (int i = 2; i < (int)tree.size(); i++) {
            ret.push_back(get_palindrome(i));
        }
        return ret;
    }

    // traverse suffix-palindrome chain of position pos
    vector<int> pal_nodes_ending_at_pos(int pos) const {
        vector<int> ret;
        if (pos < 0 || pos >= (int)suf_node.size()) return ret;
        int u = suf_node[pos];
        while (u > 1) {
            ret.push_back(u);
            u = tree[u].link;
        }
        return ret;
    }
};
