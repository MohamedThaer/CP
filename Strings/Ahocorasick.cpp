struct AC {
    int N, P;
    const int K = 26;
    const char CH = 'a';
    vector<vector<int> > next;
    vector<int> link, out_link;
    vector<vector<int> > out;
    //vector<vector<int> > dp;

    AC() : N(0), P(0) { node(); }

    int node() {
        next.emplace_back(K, 0);
        link.emplace_back(0);
        out_link.emplace_back(0);
        out.emplace_back();
        return N++;
    }

    int get(char c) {
        return c - CH;
    }

    int add(const string &patt) {
        int u = 0;
        for (auto &c: patt) {
            if (!next[u][get(c)]) next[u][get(c)] = node();
            u = next[u][get(c)];
        }
        out[u].push_back(P);
        return P++;
    }

    void compute() {
        queue<int> q;
        for (q.push(0); !q.empty();) {
            int u = q.front();
            q.pop();
            for (int c = 0; c < K; ++c) {
                int v = next[u][c];
                if (!v) next[u][c] = next[link[u]][c];
                else {
                    link[v] = u ? next[link[u]][c] : 0;
                    out_link[v] = out[link[v]].empty() ? out_link[link[v]] : link[v];
                    q.push(v);
                }
            }
        }
       // dp.assign(N, vector<int>(K, -1));
    }

    int advance(int u, char c) {
        // if (!u || next[u][get(c)]) return next[u][get(c)];
        //
        // int &ret = dp[u][get(c)];
        // if (~ret) return ret;
        // ret = advance(link[u], c);
        // return ret;
        while (u && !next[u][get(c)]) u = link[u];
        u = next[u][get(c)];
        return u;
    }
    void match(const string &text) {
        int u = 0;
        for(int i = 0; i < text.size(); i++){
            u = advance(u, text[i]);
            int tmp = u;
            while(tmp){
                for(int p: out[tmp]){
                    // pattern p ends at index i
                }
                tmp = out_link[tmp];
            }
        }
    }

    bool forbidden(int u) {
        return !out[u].empty() || out_link[u];
    }

    void precompute_outputs() {
        vector<vector<int>> all(N);
        for (int i = 0; i < N; i++) {
            all[i] = out[i];
            int tmp = out_link[i];
            while (tmp) {
                for (int idx : out[tmp]) {
                    all[i].push_back(idx);
                }
                tmp = out_link[tmp];
            }
        }

        out = move(all);
    }
};

template<class M>
M unique(M x) {
    sort(x.begin(), x.end());
    x.erase(unique(x.begin(), x.end()), x.end());
    return x;
}
