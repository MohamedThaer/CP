class MaxCostAntiChain {
private:
    const int64_t INF = 1E18;

    int n;
    int64_t total = 0;
    max_flow mf;

public:
    // nodes of the DAG
    MaxCostAntiChain(int n, vector<int> cost) : n(n), mf(n + n + 2) {
        for (int i = 0; i < n; ++i) {
            total += cost[i];
            mf.add_edge(n + n, i, cost[i]);
            mf.add_edge(i + n, n + n + 1, cost[i]);
        }
    }

    // make sure no cycles occur
    void add_edge(int u, int v) {
        if (u != v) {
            mf.add_edge(u, v + n, INF);
        }
    }

    int64_t MaxCost() { return total - mf.flow(n + n, n + n + 1); }
};
