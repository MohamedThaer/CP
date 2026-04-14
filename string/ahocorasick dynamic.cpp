
class aho_corasick_static {
private:
    struct node {
        int child[26], cnt, link;
        vector<int> occ;
        node() {
            cnt = 0, link = -1;
            memset(child, -1, sizeof child);
        }
    };
    vector<node> tree;

public:
    aho_corasick_static() { tree = vector<node>(1); }
    aho_corasick_static(const vector<pair<string, int>> &words) {
        tree = vector<node>(1);

        for (auto &[s, idx] : words) {
            int cur = 0;
            for (auto it : s) {
                int to = it - 'a';
                if (tree[cur].child[to] == -1) {
                    tree[cur].child[to] = tree.size();
                    tree.emplace_back(node());
                }
                cur = tree[cur].child[to];
            }
            tree[cur].cnt++;
            tree[cur].occ.push_back(idx);
        }

        queue<int> Q;
        Q.push(0);
        while (!Q.empty()) {
            int node = Q.front();
            Q.pop();
            for (int i = 0; i < 26; ++i) {
                int v = tree[node].child[i];
                if (~v) {
                    int j = tree[node].link;
                    while (~j && tree[j].child[i] == -1) {
                        j = tree[j].link;
                    }
                    tree[v].link = (~j ? tree[j].child[i] : 0);
                    tree[v].cnt += tree[tree[v].link].cnt;
                    for (auto &it : tree[tree[v].link].occ) {
                        tree[v].occ.push_back(it);
                    }
                    Q.push(v);
                }
            }
        }
    }

    int count(const string &s) {
        int64_t cur = 0, ans = 0;
        for (auto it : s) {
            int to = it - 'a';
            while (~cur && tree[cur].child[to] == -1) {
                cur = tree[cur].link;
            }
            cur = (~cur ? tree[cur].child[to] : 0);
            ans += tree[cur].cnt;
        }
        return ans;
    }

    vector<vector<int>> get_occ(const string &s) {
        int cur = 0, i = 0;
        vector<vector<int>> ret(s.size());
        for (auto it : s) {
            int to = it - 'a';
            while (~cur && tree[cur].child[to] == -1) {
                cur = tree[cur].link;
            }
            cur = (~cur ? tree[cur].child[to] : 0);
            for (auto id : tree[cur].occ) {
                ret[i].push_back(id);
            }
            i++;
        }
        return ret;
    }
};

class aho_corasick {
private:
    int sz = 0;
    vector<pair<string, int>> li[20];
    aho_corasick_static ac[20];

public:
    void insert(const string &s) {
        int pos = 0;
        for (int l = 0; l < 20; l++) {
            if (li[l].empty()) {
                pos = l;
                break;
            }
        }
        for (int bef = 0; bef < pos; bef++) {
            for (auto &it : li[bef]) {
                li[pos].push_back(it);
            }
            li[bef].clear();
            ac[bef] = aho_corasick_static();
        }

        li[pos].push_back({s, sz++});
        ac[pos] = aho_corasick_static(li[pos]);
    }

    int get_count(string &s) {
        int ret = 0;
        for (int l = 0; l < 20; l++) {
            if (li[l].size()) {
                ret += ac[l].count(s);
            }
        }
        return ret;
    }

    vector<vector<int>> get_occ(const string &s) {
        vector<vector<int>> occ(s.size());
        for (int l = 0; l < 20; l++) {
            if (li[l].size()) {
                auto aco = ac[l].get_occ(s);
                for (int i = 0; i < s.size(); ++i) {
                    occ[i].insert(occ[i].end(), aco[i].begin(), aco[i].end());
                }
            }
        }
        return occ;
    }
};
