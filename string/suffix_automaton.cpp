
class suffix_automaton {
private:
    vector<int64_t> dp, dis_cnt, sub_cnt;

    void extend(char c) {
        int p = last;
        if (t[p].nxt.count(c)) {
            int q = t[p].nxt[c];
            if (t[q].len == t[p].len + 1) {
                last = q;
                return;
            }
            int clone = sz++;
            t[clone] = t[q];
            t[clone].len = t[p].len + 1;
            t[q].link = clone;
            last = clone;
            while (p != -1 && t[p].nxt[c] == q) {
                t[p].nxt[c] = clone;
                p = t[p].link;
            }
            return;
        }
        int cur = sz++;
        t[cur].len = t[last].len + 1;
        t[cur].firstpos = t[cur].len;
        p = last;
        while (p != -1 && !t[p].nxt.count(c)) {
            t[p].nxt[c] = cur;
            p = t[p].link;
        }
        if (p == -1)
            t[cur].link = 0;
        else {
            int q = t[p].nxt[c];
            if (t[p].len + 1 == t[q].len)
                t[cur].link = q;
            else {
                int clone = sz++;
                t[clone] = t[q];
                t[clone].len = t[p].len + 1;
                while (p != -1 && t[p].nxt[c] == q) {
                    t[p].nxt[c] = clone;
                    p = t[p].link;
                }
                t[q].link = t[cur].link = clone;
            }
        }
        last = cur;
    }

public:
    // len -> largest string length of the corresponding endpos-equivalent class
    // link -> longest suffix that is another endpos-equivalent class.
    // firstpos -> 1 indexed end position of the first occurrence of the largest string of that node
    // minlen(v) -> smallest string of node v = len(link(v)) + 1
    // terminal nodes -> store the suffixes
    struct node {
        int len, link, firstpos;
        map<char, int> nxt;
    };
    int sz, last;
    vector<node> t;
    vector<int> terminal;
    vector<vector<int>> g;

    suffix_automaton() {}

    suffix_automaton(int n) : t(2 * n), terminal(2 * n), dp(2 * n, -1), dis_cnt(2 * n, -1), sub_cnt(2 * n, -1), g(2 * n) {
        sz = 1;
        last = 0;
        t[0].len = 0;
        t[0].link = -1;
        t[0].firstpos = 0;
    }

    void append(char c, int val = 1) {
        extend(c);
        terminal[last] = val;
    }

    suffix_automaton(const string &s) : suffix_automaton((int)s.size()) {
        for (auto c : s) {
            append(c, 1);
        }
        build_tree();
    }

    void build_tree() {
        for (int i = 1; i < sz; i++) {
            g[t[i].link].push_back(i);
        }
    }

    int next_state(int cur, char c) {
        return t[cur].nxt.count(c) ? t[cur].nxt[c] : -1;
    }

    // for any path p from initial state to i-th state ---> how many time p appears in the string
    int64_t cnt(int i) { // number of times i-th node occurs in the string
        if (dp[i] != -1) return dp[i];
        int64_t ret = terminal[i];
        for (auto &it : g[i]) {
            ret += cnt(it);
        }
        return dp[i] = ret;
    }

    // count distinct substrings that could be achieved starting from node
    int64_t count_distinct(int node) {
        if (~dis_cnt[node]) return dis_cnt[node];

        int64_t ret = 1;
        for (auto [c, nxt] : t[node].nxt) {
            ret += count_distinct(nxt);
        }
        return dis_cnt[node] = ret;
    }

    // for all distinct substrings starting from this node, find the count of them in the actual string
    int64_t count_substrings(int node) {
        if (~sub_cnt[node]) return sub_cnt[node];

        int64_t ret = node ? cnt(node) : 0;
        for (auto [c, nxt] : t[node].nxt) {
            ret += count_substrings(nxt);
        }
        return sub_cnt[node] = ret;
    }

    string kth_distinct_substring(int cur, int64_t k) {
        if (k <= 0) {
            return "";
        }

        for (auto [c, nxt] : t[cur].nxt) {
            if (count_distinct(nxt) < k) {
                k -= count_distinct(nxt);
            }
            else {
                return string(1, c) + kth_distinct_substring(nxt, k - 1);
            }
        }
        return "";
    }

    string kth_distinct_substring(int64_t k) {
        return kth_distinct_substring(0, k);
    }

    string kth_substring(int cur, int64_t k) {
        if (k <= 0) {
            return "";
        }

        for (auto [c, nxt] : t[cur].nxt) {
            if (count_substrings(nxt) < k) {
                k -= count_substrings(nxt);
            }
            else {
                return string(1, c) + kth_substring(nxt, k - cnt(nxt));
            }
        }
        return "";
    }

    string kth_substring(int64_t k) {
        return kth_substring(0, k);
    }

    vector<int> substring_distripution() {
        int n = t.size() / 2;
        vector<int> ret(n + 2);
        for (int i = 1; i < sz; ++i) {
            int l = t[t[i].link].len + 1, r = t[i].len, o = cnt(i);
            ret[l]++, ret[r + 1]--;
        }

        for (int i = 1; i <= n; ++i) {
            ret[i] += ret[i - 1];
        }
        return ret;
    }
};
