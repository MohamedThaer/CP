class DynamicConnectivity {
private:
    enum Type {
        comp_cnt,
        is_connected,
    };

    struct Edge {
        int u, v;
    };

    struct Query {
        Type type;
        int u, v;
        int time;
    };

    vector<tuple<int, int, Edge>> edge_intervals;
    vector<Query> all_queries;

    int n, time = 0;
    rollback_dsu dsu;
    unordered_map<long long, int> edge_start_time;

    static long long key(int a, int b) {
        if (a > b) swap(a, b);
        return ((long long)a << 32) | (long long)b;
    }

    void add_interval(vector<vector<Edge>> &seg, int idx, int l, int r, int ql, int qr, Edge e) {
        if (ql >= r || qr <= l) return;
        if (ql <= l && r <= qr) {
            seg[idx].push_back(e);
            return;
        }
        int mid = l + (r - l) / 2;
        add_interval(seg, idx * 2, l, mid, ql, qr, e);
        add_interval(seg, idx * 2 + 1, mid, r, ql, qr, e);
    }

    void dfs(const vector<vector<Edge>> &seg, int idx, int l, int r,
             const vector<vector<int>> &queries_by_time, vector<int> &results) {
        size_t snap = dsu.snapshot();
        for (auto &e : seg[idx])
            dsu.merge(e.u, e.v);

        if (l + 1 == r) {
            for (int original_index : queries_by_time[l]) {
                const auto &q = all_queries[original_index];
                if (q.type == Type::is_connected) { // is_connected
                    results[original_index] = dsu.same_set(q.u, q.v);
                }
                else if (q.type == Type::comp_cnt) { // component_count
                    results[original_index] = dsu.comp_cnt();
                }
            }
        }
        else {
            int mid = l + (r - l) / 2;
            dfs(seg, idx * 2, l, mid, queries_by_time, results);
            dfs(seg, idx * 2 + 1, mid, r, queries_by_time, results);
        }
        dsu.rollback_to(snap);
    }

public:
    DynamicConnectivity(int num_nodes) : n(num_nodes), dsu(n) {}

    void add_edge(int u, int v) {
        edge_start_time[key(u, v)] = time++;
    }

    void remove_edge(int u, int v) {
        auto it = edge_start_time.find(key(u, v));
        if (it != edge_start_time.end()) {
            edge_intervals.emplace_back(it->second, time, Edge{u, v});
            edge_start_time.erase(it);
        }
        time++;
    }

    void query_is_connected(int u, int v) {
        all_queries.push_back({Type::is_connected, u, v, time++});
    }

    void query_component_count() {
        all_queries.push_back({Type::comp_cnt, 0, 0, time++});
    }

    vector<int> run() {
        int capacity = time;

        for (auto const &[edge_key, t_add] : edge_start_time) {
            int u = int(edge_key >> 32);
            int v = int(edge_key & 0xFFFFFFFF);
            if (t_add < capacity) {
                edge_intervals.emplace_back(t_add, capacity, Edge{u, v});
            }
        }
        edge_start_time.clear();

        vector<vector<Edge>> seg;
        if (capacity > 0) {
            seg.assign(4 * capacity + 5, {});
            for (const auto &interval : edge_intervals) {
                add_interval(seg, 1, 0, capacity, get<0>(interval), get<1>(interval), get<2>(interval));
            }
        }

        vector<vector<int>> queries_by_time(capacity);
        for (int i = 0; i < all_queries.size(); ++i) {
            if (all_queries[i].time < capacity) {
                queries_by_time[all_queries[i].time].push_back(i);
            }
        }

        vector<int> final_answers(all_queries.size());
        if (capacity > 0) {
            dfs(seg, 1, 0, capacity, queries_by_time, final_answers);
        }

        return final_answers;
    }
};
