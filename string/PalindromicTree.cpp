struct PalindromicTree {
    /*
        -> nxt[26]: Array for transitions. nxt[c] is the index of the node representing the palindrome formed by adding character c to both ends of the current palindrome. (The index \$0\$ to \$25\$ maps to 'a' to 'z').
        -> len: length of the palindromic substring represented by this node.
        -> st, en: The 0-based start and end indices of one occurrence of this palindrome in the original string s.
        -> oc: The total number of times the palindrome appears in the original string s.
        -> link: Suffix Link: The index of the node representing the longest proper palindromic suffix of the current node's palindrome. This is the key for efficient construction.
        -> cnt: Suffix Count: The number of distinct palindromic suffixes of the string this node represents (including itself).
    */
    struct node {
        int nxt[26]{}, len, st, en, link, cnt, oc;
    };
    string s;
    vector<node> t;
    int sz, last;

    PalindromicTree() {}
    PalindromicTree(string _s) {
        s = _s;
        int n = s.size();
        t.clear();
        t.resize(n + 9);
        sz = 2, last = 2;
        t[1].len = -1, t[1].link = 1;
        t[2].len = 0, t[2].link = 1;
        for (int i = 0; i < s.size(); i++) {
            extend(i);
        }
    }

    int extend(int pos) { // returns 1 if it creates a new palindrome
        int cur = last, curlen = 0;
        int ch = s[pos] - 'a';
        while (1) {
            curlen = t[cur].len;
            if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == s[pos]) break;
            cur = t[cur].link;
        }
        if (t[cur].nxt[ch]) {
            last = t[cur].nxt[ch];
            t[last].oc++;
            return 0;
        }
        sz++;
        last = sz;
        t[sz].oc = 1;
        t[sz].len = t[cur].len + 2;
        t[cur].nxt[ch] = sz;
        t[sz].en = pos;
        t[sz].st = pos - t[sz].len + 1;
        if (t[sz].len == 1) {
            t[sz].link = 2;
            t[sz].cnt = 1;
            return 1;
        }
        while (1) {
            cur = t[cur].link;
            curlen = t[cur].len;
            if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == s[pos]) {
                t[sz].link = t[cur].nxt[ch];
                break;
            }
        }
        t[sz].cnt = 1 + t[t[sz].link].cnt;
        return 1;
    }

    int next_state(int cur, char c) {
        return t[cur].nxt[c - 'a'];
    }

    void calc_occurrences() {
        for (int i = sz; i >= 3; i--)
            t[t[i].link].oc += t[i].oc;
    }
};
