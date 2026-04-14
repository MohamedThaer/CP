class graph_segtree {
private:
    int size;
    vector<int> idx;
    vector<vector<pair<int, int64_t>>> g; // with segment structure initially

    void add_edge(int a, int b, int64_t c) {
        g[a].push_back({b, c});
    }

    void build(int x, int lx, int rx) {
        if (lx == rx) {
            idx[lx] = x;
            return;
        }
        int mid = lx + rx >> 1;
        int left = x + 1;
        int right = x + 2 * (mid - lx + 1);

        add_edge(x, left, 0);
        add_edge(x, right, 0);
        add_edge((lx == mid ? left : left + 2 * size), x + 2 * size, 0);
        add_edge((mid + 1 == rx ? right : right + 2 * size), x + 2 * size, 0);

        build(left, lx, mid);
        build(right, mid + 1, rx);
    }

    vector<int> nodes;
    void get_in(int x, int lx, int rx, int l, int r) {
        if (l <= lx && rx <= r) {
            return nodes.push_back(x);
        }
        int mid = lx + rx >> 1;
        int left = x + 1;
        int right = x + 2 * (mid - lx + 1);
        if (l <= mid) {
            get_in(left, lx, mid, l, r);
        }
        if (mid + 1 <= r) {
            get_in(right, mid + 1, rx, l, r);
        }
    }

    void get_out(int x, int lx, int rx, int l, int r) {
        if (l <= lx && rx <= r) {
            return nodes.push_back(lx == rx ? x : x + 2 * size);
        }
        int mid = lx + rx >> 1;
        int left = x + 1;
        int right = x + 2 * (mid - lx + 1);
        if (l <= mid) {
            get_out(left, lx, mid, l, r);
        }
        if (mid + 1 <= r) {
            get_out(right, mid + 1, rx, l, r);
        }
    }

    vector<int> get_in(int l, int r) {
        nodes.resize(0);
        get_in(0, 0, size - 1, l, r);
        return nodes;
    }

    vector<int> get_out(int l, int r) {
        nodes.resize(0);
        get_out(0, 0, size - 1, l, r);
        return nodes;
    }

public:
    graph_segtree(int n) : size(n), idx(n), g(4 * n) {
        build(0, 0, size - 1);
    }

    void add_edges(int l1, int r1, int l2, int r2, int64_t c) {
        for (auto a : get_out(l1, r1)) {
            for (auto b : get_in(l2, r2)) {
                add_edge(a, b, c);
            }
        }
    }

    vector<int64_t> RunDijkstra(vector<int> src = {1}) {
        for (auto &it : src) {
            it = idx[it];
        }
        auto dis = dijkstra(g, src);
        vector<int64_t> ret(size);
        for (int i = 0; i < size; ++i) {
            ret[i] = dis[idx[i]];
        }
        return ret;
    }
};
