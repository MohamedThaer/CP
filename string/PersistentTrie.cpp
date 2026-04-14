/*
PersistentTrie
- Template parameters:
    ALPHA = alphabet size (default 26)
    FIRST = char value that maps to index 0 (default 'a')
- Usage:
    PersistentTrie<> trie;                // lowercase 'a'..'z'
    trie.insert(0, "moh");
    trie.push_back(0);
    cout << trie.count_words(1, "moh") << '\n'; // 1
    trie.insert(1, "moh", -1);
    cout << trie.count_words(1, "moh") << '\n'; // 0
    cout << trie.count_words(0, "moh") << '\n'; // 1

- To change alphabet (e.g., full ASCII): set ALPHA=128, FIRST=0
*/
template <int ALPHA = 26, char FIRST = 'a'>
class PersistentTrie {
private:
    struct node {
        array<int, ALPHA> nxt;
        int prefix = 0, word = 0;
        node() {
            nxt.fill(0);
        }
    };

    // version 0 is empty trie
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

    bool valid(int nd) { return nd != 0 && tree[nd].prefix > 0; }

public:
    PersistentTrie() {
        tree.emplace_back();        // index 0: blank node (used as null or empty base)
        roots.push_back(new_node()); // root of version 0 (an empty node)
    }

    void insert(int version, const string &s, int delta = 1) {
        int new_root = clone_node(roots[version]);
        roots[version] = new_root;

        int cur = new_root;
        tree[cur].prefix += delta;

        for (auto ch : s) {
            int to = ch - FIRST;
            int nd = (!tree[cur].nxt[to] ? new_node() : clone_node(tree[cur].nxt[to]));
            tree[cur].nxt[to] = nd;
            cur = nd;
            tree[cur].prefix += delta;
        }

        tree[cur].word += delta;
    }

    int count_prefix(int version, const string &p) {
        int cur = roots[version];
        for (auto ch : p) {
            int to = ch - FIRST;
            if (!valid(tree[cur].nxt[to])) return 0;
            cur = tree[cur].nxt[to];
        }
        return tree[cur].prefix;
    }

    int count_words(int version, const string &s) {
        int cur = roots[version];
        for (auto ch : s) {
            int to = ch - FIRST;
            if (!valid(tree[cur].nxt[to])) return 0;
            cur = tree[cur].nxt[to];
        }
        return tree[cur].word;
    }

    int push_back(int version) {
        roots.push_back(roots[version]);
        return roots.size() - 1;
    }

    int cur_version() { return roots.size() - 1; }
};
