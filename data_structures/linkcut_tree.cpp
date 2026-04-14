/*
 * LCT(n): n --> nodes in the forest (one indexed)
 * make_root(u) --> make u the root in its tree
 * link(u, v) --> make edge between u and v
 * cut(u) --> remove edge between u and parent of u
 * cut(u, v) --> remove edge between u and v
 * get_parent(u) --> return parent of u in its tree considering current root
 * find_root(u) --> return root of the tree that contains node u
 * connected(u, v) --> is u and v on the same tree
 * depth(u) --> distance between u and the current root of its tree
 * lca(u, v) --> lowest common ancestor with cuurent root of their tree
 * is_root(u) --> u is the root in its tree or not
 * component_size(u) --> size of node u tree
 * subtree_size(u) --> with the current root of node u tree get subtree size
 * component_sum(u) --> sum of all tree values (node u tree)
 * subtree_sum(u) --> sum of all subtree values considering current root (node u tree)
 * subtree_query(u, root) --> sum of all subtree values considering parameter root (node u tree) (don't affect current root)
 * query(u, v) --> sum on path from u to v
 * upd(u, x) --> add x to node u value
 * upd(u, v, x) --> add x to all path from u to v
 */
struct LCT {
    struct node {
        int p = 0, c[2] = {0, 0}, pp = 0;
        bool flip = 0;
        int sz = 0, ssz = 0, vsz = 0; // sz -> aux tree size, ssz = subtree size in rep tree, vsz = virtual tree size
        int64_t val = 0, sum = 0, lazy = 0, subsum = 0, vsum = 0;
        node() {}
        node(int x) {
            val = x;
            sum = x;
            sz = 1;
            lazy = 0;
            ssz = 1;
            vsz = 0;
            subsum = x;
            vsum = 0;
        }
    };

    vector<node> t;
    LCT() {}
    LCT(int n) : t(n + 1) {}

    // <independant splay tree code>
    int dir(int x, int y) { return t[x].c[1] == y; }
    void set(int x, int d, int y) {
        if (x) t[x].c[d] = y, pull(x);
        if (y) t[y].p = x;
    }
    void pull(int x) {
        if (!x) return;
        int &l = t[x].c[0], &r = t[x].c[1];
        push(l);
        push(r);
        t[x].sum = t[l].sum + t[r].sum + t[x].val;
        t[x].sz = t[l].sz + t[r].sz + 1;
        t[x].ssz = t[l].ssz + t[r].ssz + t[x].vsz + 1;
        t[x].subsum = t[l].subsum + t[r].subsum + t[x].vsum + t[x].val;
    }
    void push(int x) {
        if (!x) return;
        int &l = t[x].c[0], &r = t[x].c[1];
        if (t[x].flip) {
            swap(l, r);
            if (l) t[l].flip ^= 1;
            if (r) t[r].flip ^= 1;
            t[x].flip = 0;
        }
        if (t[x].lazy) {
            t[x].val += t[x].lazy;
            t[x].sum += t[x].lazy * t[x].sz;
            t[x].subsum += t[x].lazy * t[x].ssz;
            t[x].vsum += t[x].lazy * t[x].vsz;
            if (l) t[l].lazy += t[x].lazy;
            if (r) t[r].lazy += t[x].lazy;
            t[x].lazy = 0;
        }
    }
    void rotate(int x, int d) {
        int y = t[x].p, z = t[y].p, w = t[x].c[d];
        swap(t[x].pp, t[y].pp);
        set(y, !d, w);
        set(x, d, y);
        set(z, dir(z, y), x);
    }
    void splay(int x) {
        for (push(x); t[x].p;) {
            int y = t[x].p, z = t[y].p;
            push(z);
            push(y);
            push(x);
            int dx = dir(y, x), dy = dir(z, y);
            if (!z)
                rotate(x, !dx);
            else if (dx == dy)
                rotate(y, !dx), rotate(x, !dx);
            else
                rotate(x, dy), rotate(x, dx);
        }
    }
    // </independant splay tree code>

    // making it a root in the rep. tree
    void make_root(int u) {
        access(u);
        int l = t[u].c[0];
        t[l].flip ^= 1;
        swap(t[l].p, t[l].pp);
        t[u].vsz += t[l].ssz;
        t[u].vsum += t[l].subsum;
        set(u, 0, 0);
    }
    // make the path from root to u a preferred path
    // returns last path-parent of a node as it moves up the tree
    int access(int _u) {
        int last = _u;
        for (int v = 0, u = _u; u; u = t[v = u].pp) {
            splay(u);
            splay(v);
            t[u].vsz -= t[v].ssz;
            t[u].vsum -= t[v].subsum;
            int r = t[u].c[1];
            t[u].vsz += t[r].ssz;
            t[u].vsum += t[r].subsum;
            t[v].pp = 0;
            swap(t[r].p, t[r].pp);
            set(u, 1, v);
            last = u;
        }
        splay(_u);
        return last;
    }
    void link(int u, int v) { // u -> v
        // assert(!connected(u, v));
        make_root(v);
        access(u);
        splay(u);
        t[v].pp = u;
        t[u].vsz += t[v].ssz;
        t[u].vsum += t[v].subsum;
    }
    void cut(int u) { // cut par[u] -> u, u is non root vertex
        access(u);
        assert(t[u].c[0] != 0);
        t[t[u].c[0]].p = 0;
        t[u].c[0] = 0;
        pull(u);
    }
    void cut(int u, int v) {
        if (v == get_parent(u)) {
            cut(u);
        }
        else {
            cut(v);
        }
    }
    // parent of u in the rep. tree
    int get_parent(int u) {
        access(u);
        splay(u);
        push(u);
        u = t[u].c[0];
        push(u);
        while (t[u].c[1]) {
            u = t[u].c[1];
            push(u);
        }
        splay(u);
        return u;
    }
    // root of the rep. tree containing this node
    int find_root(int u) {
        access(u);
        splay(u);
        push(u);
        while (t[u].c[0]) {
            u = t[u].c[0];
            push(u);
        }
        splay(u);
        return u;
    }
    bool connected(int u, int v) {
        return find_root(u) == find_root(v);
    }
    // depth in the rep. tree
    int depth(int u) {
        access(u);
        splay(u);
        return t[u].sz;
    }
    int lca(int u, int v) {
        // assert(connected(u, v));
        if (u == v) return u;
        if (depth(u) > depth(v)) swap(u, v);
        access(v);
        return access(u);
    }
    int is_root(int u) {
        return get_parent(u) == 0;
    }
    int component_size(int u) {
        return t[find_root(u)].ssz;
    }
    int subtree_size(int u) {
        int p = get_parent(u);
        if (p == 0) {
            return component_size(u);
        }
        cut(u);
        int ans = component_size(u);
        link(p, u);
        return ans;
    }
    int64_t component_sum(int u) {
        return t[find_root(u)].subsum;
    }
    int64_t subtree_sum(int u) {
        int p = get_parent(u);
        if (p == 0) {
            return component_sum(u);
        }
        cut(u);
        int64_t ans = component_sum(u);
        link(p, u);
        return ans;
    }
    // sum of the subtree of u when root is specified
    int64_t subtree_query(int u, int root) {
        int cur = find_root(u);
        make_root(root);
        int64_t ans = subtree_sum(u);
        make_root(cur);
        return ans;
    }
    // path sum
    int64_t query(int u, int v) {
        int cur = find_root(u);
        make_root(u);
        access(v);
        int64_t ans = t[v].sum;
        make_root(cur);
        return ans;
    }
    void upd(int u, int x) {
        access(u);
        splay(u);
        t[u].val += x;
    }
    // add x to the nodes on the path from u to v
    void upd(int u, int v, int x) {
        int cur = find_root(u);
        make_root(u);
        access(v);
        t[v].lazy += x;
        make_root(cur);
    }
};
