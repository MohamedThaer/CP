// if you want it faster use vector not map but memory will be high n * 26
//--------------------------END POSITIONS-----------------------------

// we call two substrings t1 and t2 endpos equivalent,if their ending sets coincide: endpos(t1) = endpos(t2).
// It turns out, that in a suffix machine endpos-equivalent substrings correspond to the same state.

//----------------------------SUFFIX LINKS----------------------------

// [suffix link] link(v) leads to the state that corresponds to the longest suffix of string in (v) that is in another endpos equivalence class.

//----------------------------IMPORTANT THINGS----------------------------

// For each state v  one or multiple substrings match ,  We denote by longest(v) the longest such string, and through len(v) its lenght .
// We denote by shortest(v) the shortest substring in it with length minlen(v) .
// OBSERVATION : Then all the strings corresponding to this state are different suffixes of the string longest(v) and have all possible lengths in the interval [minlen(v) , len(v)].
// we can express the minlen(v) = len(link(v)) + 1.
const int N = 1e5 + 5;
int idx[2 * N];
vector<int> adj[2 * N];

struct SuffixAutomaton {
    struct state {
        int len;
        int link; // the suffix length
        map<char, int> next;
    };
    int mx_len; // max length of the string
    int sz;     // index of next state
    int last;   // index of last state we created
    vector<state> st;
    vector<int> pos;
    vector<vector<int>> up;

    SuffixAutomaton(int n) {
        mx_len = n;
        st = vector<state>(2 * n);
        st[0].len = 0; // initial state as empty string
        st[0].link = -1;
        sz = 1;
        last = 0;
        // additions
    }

    void build(string &s) {
        pos.resize(s.size() + 1);
        for (int i = 0; i < s.size(); ++i) {
            char c = s[i];
            insert(c), pos[i] = last;
            idx[last] = i + 1;
        }
    }

    void insert(char c) {
        int cur = sz++;
        st[cur].len = st[last].len + 1;
        int p = last;
        while (p != -1 && !st[p].next.count(c)) {
            st[p].next[c] = cur;
            p = st[p].link;
        }
        if (p == -1) {
            st[cur].link = 0;
        }
        else {
            int q = st[p].next[c];
            if (st[p].len + 1 == st[q].len) {
                st[cur].link = q;
            }
            else {
                int clone = sz++;
                st[clone].len = st[p].len + 1;
                st[clone].next = st[q].next;
                st[clone].link = st[q].link;
                while (p != -1 && st[p].next[c] == q) {
                    st[p].next[c] = clone;
                    p = st[p].link;
                }
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }

    // Returns the number of occurrences of pattern p in the original text.

    // Prepare binary lifting table on suffix links to answer queries in O(log n)
    void prepare_lifting() {
        int LOG = 1;
        while ((1 << LOG) <= sz)
            ++LOG;
        up.assign(sz, vector<int>(LOG, -1));
        for (int v = 0; v < sz; ++v)
            up[v][0] = st[v].link;
        for (int k = 1; k < LOG; ++k) {
            for (int v = 0; v < sz; ++v) {
                int mid = up[v][k - 1];
                up[v][k] = (mid == -1 ? -1 : up[mid][k - 1]);
            }
        }
    }

    // Return state id that represents substring s[l..r].
    // Assumes:
    //   - build(s) was called and pos filled
    //   - prepare_lifting() was called
    // indices l and r are 0-based, 0 <= l <= r < n
    // If invalid query returns -1.
    int state_of_substring(int l, int r) {
        int L = r - l + 1;
        int cur = pos[r];
        int LOG = up.empty() ? 0 : (int)up[0].size();
        for (int k = LOG - 1; k >= 0; --k) {
            if (up[cur][k] != -1 && st[up[cur][k]].len >= L) {
                cur = up[cur][k];
            }
        }
        // now cur is the ancestor with minimal len that still >= L,
        // and its link has len < L (or link == -1)
        return cur;
    }

    void pre() {
        for (int i = 1; i < sz; ++i) {
            adj[st[i].link].push_back(i);
        }
    }
};