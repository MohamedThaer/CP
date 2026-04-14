
/**
 * @brief O(N log N) Tree Hashing with O(1) Subtree Query.
 *
 * This struct first runs a precalculation (DFS) from a given root.
 * During this single DFS, it computes and stores the canonical hash
 * for every subtree.
 *
 * After precalculation, you can query the hash of any subtree
 * (rooted at node 'u') in O(1) time.
 *
 * NOTE: The hashes are all relative to the initial root chosen
 * for the precalculation.
 */
class TreeHasher {
private:
    int n;
    vector<vector<int>> adj;

    static map<vector<int>, int> hash_map;
    static int next_id;

    vector<int> subtree_size;
    vector<int> subtree_hashes;

    /**
     * @brief Internal DFS to compute and store all subtree hashes.
     * @param u Current node.
     * @param p Parent node (to avoid cycles).
     * @return The canonical integer hash for the subtree rooted at u.
     */
    int dfs_hash(int u, int p) {
        vector<int> child_hashes;
        for (int v : adj[u]) {
            if (v != p) {
                child_hashes.push_back(dfs_hash(v, u));
            }
        }
        sort(child_hashes.begin(), child_hashes.end());
        if (hash_map.find(child_hashes) == hash_map.end()) {
            hash_map[child_hashes] = next_id++;
        }
        return subtree_hashes[u] = hash_map[child_hashes];
    }

    void dfs_sizes(int u, int p) {
        subtree_size[u] = 1;
        for (int v : adj[u]) {
            if (v != p) {
                dfs_sizes(v, u);
                subtree_size[u] += subtree_size[v];
            }
        }
    }

    vector<int> get_centroids() {
        dfs_sizes(0, -1);
        vector<int> centroids;
        int u = 0;
        int p = -1;
        bool found = false;
        while (!found) {
            found = true;
            for (int v : adj[u]) {
                if (v != p && subtree_size[v] > n / 2) {
                    found = false;
                    p = u;
                    u = v;
                    break;
                }
            }
        }
        centroids.push_back(u);
        for (int v : adj[u]) {
            if (v != p && subtree_size[v] == n / 2) {
                centroids.push_back(v);
            }
        }
        return centroids;
    }

public:
    TreeHasher(int num_nodes) : n(num_nodes),
                                adj(n),
                                subtree_size(n),
                                subtree_hashes(n) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    /**
     * @brief Runs the precalculation for the entire tree from a given root.
     * @param root The root node to start the DFS from.
     * @return The hash of the entire tree (rooted at 'root').
     */
    int set_root(int root) {
        return dfs_hash(root, -1);
    }

    /**
     * @brief Gets the precalculated hash of a subtree in O(1).
     * Must be called AFTER set_root().
     * @param u The root node of the subtree you want to query.
     * @return The canonical hash for that subtree.
     */
    int get_hash(int u) {
        return subtree_hashes[u];
    }

    /**
     * @brief Computes the canonical hash for an *unrooted* tree.
     * This also fills the subtree_hashes vector, but the hashes
     * will be relative to the chosen centroid.
     * @return The canonical unrooted hash.
     */
    int get_unrooted_hash() {
        vector<int> centroids = get_centroids();
        int min_hash = -1;

        for (int centroid : centroids) {
            // This precalculates all hashes relative to the centroid
            int current_hash = set_root(centroid);
            if (min_hash == -1 || current_hash < min_hash) {
                min_hash = current_hash;
                // If this is the new best hash, we keep the
                // 'subtree_hashes' vector computed from this centroid.
            }
        }
        return min_hash;
    }
};
map<vector<int>, int> TreeHasher::hash_map;
int TreeHasher::next_id;
