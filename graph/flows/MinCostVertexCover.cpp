
class MinCostVertexCover {
private:
    const int64_t INF = 1E18;

    int64_t total = 0;
    int n, m;
    max_flow mf;

public:
    MinCostVertexCover(int n, int m, vector<int> costL, vector<int> costR) : n(n), m(m), mf(n + m + 2) {
        for (int i = 0; i < n; ++i) {
            total += costL[i];
            mf.add_edge(n + m, i, costL[i]);
        }
        for (int i = 0; i < m; ++i) {
            total += costR[i];
            mf.add_edge(i + n, n + m + 1, costR[i]);
        }
    }

    void add_edge(int u, int v) { mf.add_edge(u, v + n, INF); }
    int64_t MinCost() { return mf.flow(n + m, n + m + 1); }
    int64_t MaxCostIndependentSet() { return total - MinCost(); }

    pair<vector<int>, vector<int>> GetVertexCoverSet() {
        vector<bool> cut = mf.min_cut();
        vector<int> left, right;

        for (int i = 0; i < n; ++i) {
            if (!cut[i]) {
                left.push_back(i);
            }
        }

        for (int i = 0; i < m; ++i) {
            if (cut[i + n]) {
                right.push_back(i);
            }
        }
        return make_pair(left, right);
    }

    pair<vector<int>, vector<int>> GetIndependentSet() {
        vector<bool> cut = mf.min_cut();
        vector<int> left, right;

        for (int i = 0; i < n; ++i) {
            if (cut[i]) {
                left.push_back(i);
            }
        }

        for (int i = 0; i < m; ++i) {
            if (!cut[i + n]) {
                right.push_back(i);
            }
        }
        return make_pair(left, right);
    }
};
