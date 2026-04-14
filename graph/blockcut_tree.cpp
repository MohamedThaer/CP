
/**
 * @brief Implements the Block-Cut Tree data structure. (Definitive Version)
 * This class correctly finds biconnected components (BCCs), identifies
 * articulation points, and constructs the block-cut tree. It handles all
 * graph structures including disconnected graphs and isolated vertices.
 */
class BlockCutTree {
private:
    int n, timer;
    vector<vector<int>> g;
    vector<int> tin, low;
    vector<pair<int, int>> st; // Edge stack

    // BCC and Articulation Point properties
    vector<vector<int>> bcc;
    vector<bool> isArtic;

    // Tree properties
    vector<vector<int>> tree;
    vector<int> id;            // Maps an articulation point to its tree node index
    vector<int> node_to_block; // Maps a non-articulation point to its block's tree node index

    void init(int N) {
        n = N;
        timer = 0;
        g.assign(n, {});
        tin.assign(n, -1);
        low.assign(n, -1);
        st.clear();
        bcc.clear();
        isArtic.assign(n, false);
        tree.clear();
        id.assign(n, -1);
        node_to_block.assign(n, -1);
    }

    void dfs(int u, int p = -1) {
        tin[u] = low[u] = timer++;
        int children = 0;

        for (int v : g[u]) {
            if (v == p) continue;

            if (tin[v] != -1) {        // Visited node
                if (tin[v] < tin[u]) { // Back edge to an ancestor
                    low[u] = min(low[u], tin[v]);
                    st.emplace_back(u, v);
                }
            }
            else { // Unvisited node (tree edge)
                children++;
                st.emplace_back(u, v);
                dfs(v, u);
                low[u] = min(low[u], low[v]);

                // Condition for articulation point and new BCC
                if (low[v] >= tin[u]) {
                    if (p != -1) { // A non-root node is an AP if this condition is met
                        isArtic[u] = true;
                    }

                    unordered_set<int> nodes_in_bcc;
                    while (true) {
                        auto e = st.back();
                        st.pop_back();
                        nodes_in_bcc.insert(e.first);
                        nodes_in_bcc.insert(e.second);
                        if (e.first == u && e.second == v) break;
                    }
                    bcc.emplace_back(nodes_in_bcc.begin(), nodes_in_bcc.end());
                }
            }
        }
        // Root of the DFS tree is an AP if it has more than one child.
        if (p == -1 && children > 1) {
            isArtic[u] = true;
        }
    }

public:
    BlockCutTree(int n = 0) {
        if (n > 0) init(n);
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    /**
     * @brief Finds all BCCs and articulation points, then builds the tree.
     */
    void build_tree() {
        // Step 1: Find all edge-based BCCs and all articulation points
        for (int i = 0; i < n; ++i) {
            if (tin[i] == -1) {
                if (g[i].empty()) continue; // Skip isolated vertices for now
                dfs(i);
                if (!st.empty()) {
                    unordered_set<int> nodes_in_bcc;
                    while (!st.empty()) {
                        auto e = st.back();
                        st.pop_back();
                        nodes_in_bcc.insert(e.first);
                        nodes_in_bcc.insert(e.second);
                    }
                    bcc.emplace_back(nodes_in_bcc.begin(), nodes_in_bcc.end());
                }
            }
        }

        // Step 2: Handle isolated vertices
        vector<bool> covered(n, false);
        for (const auto &component : bcc) {
            for (int node : component) {
                covered[node] = true;
            }
        }
        for (int i = 0; i < n; ++i) {
            if (!covered[i]) {
                bcc.push_back({i});
            }
        }

        // Step 3: Construct the Block-Cut Tree
        int bccCount = bcc.size();
        int articCount = 0;
        for (int i = 0; i < n; ++i) {
            if (isArtic[i]) {
                id[i] = bccCount + articCount++;
            }
        }

        tree.assign(bccCount + articCount, {});

        for (int i = 0; i < bccCount; ++i) {
            for (int v : bcc[i]) {
                if (isArtic[v]) {
                    // Connect block node `i` to articulation point node `id[v]`
                    int u_artic = id[v];
                    int v_block = i;
                    tree[u_artic].push_back(v_block);
                    tree[v_block].push_back(u_artic);
                }
                else {
                    // Map non-AP node `v` to its block node `i`
                    node_to_block[v] = i;
                }
            }
        }
    }

    /**
     * @brief Returns the adjacency list of the constructed block-cut tree.
     * Nodes 0 to bcc.size()-1 are block nodes.
     * Nodes bcc.size() and onwards are articulation point nodes.
     */
    vector<vector<int>> get_tree() {
        return tree;
    }

    /**
     * @brief Checks if an original graph node `u` is an articulation point.
     */
    bool is_articulation(int u) const {
        return isArtic[u];
    }

    /**
     * @brief Gets the index of the node in the block-cut tree that represents
     * the original graph node `u`.
     * @return The index of the articulation point node if `u` is an AP,
     * otherwise the index of the block node that contains `u`.
     */
    int tree_idx(int u) {
        if (is_articulation(u)) {
            return id[u];
        }
        else {
            return node_to_block[u];
        }
    }

    vector<vector<int>> get_bccs() {
        return bcc;
    }
};
