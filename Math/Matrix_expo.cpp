template <class T>
struct Matrix {
    T D = 0;
    vector<vector<T>> a;
    Matrix(int n, int m) : a(n, vector<T>(m, D)) {}
    Matrix(int n) : a(n, vector<T>(n, D)) {
        for (int i = 0; i < n; i++) a[i][i] = 1;
    }

    Matrix operator*(const Matrix& other) const {
        int n = a.size(), m = other.a[0].size(), k = a[0].size();
        Matrix res(n, m);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                for (int x = 0; x < k; x++) {
                    res.a[i][j] = res.a[i][j] + a[i][x] * other.a[x][j];
                }
            }
        }
        return res;
    }

    Matrix power(long long p) {
        Matrix res(a.size()),base = *this;
        while (p) {
            if (p & 1) res = res * base;
            base = base * base;
            p >>= 1;
        }
        return res;
    }
};
// Rizk 

#pragma GCC optimize("Ofast,unroll-loops,no-stack-protector,fast-math")
#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define int ll
const int MOD = 1000000007, N = 2e5 + 5, inf = 1LL << 60;

vector<vector<int> > mat_mul(vector<vector<int> > &a, vector<vector<int> > &b) {
    int r1 = a.size(), c1 = a[0].size(), r2 = b.size(), c2 = b[0].size();
    assert(c1==r2);
    vector<vector<int> > res(r1, vector<int>(c2));
    for (int i = 0; i < r1; i++)
        for (int k = 0; k < c1; k++)
            if (a[i][k] != 0)
                for (int j = 0; j < c2; j++)
                    res[i][j] = (res[i][j] + a[i][k] * b[k][j]) % MOD;
    return res;
}

vector<vector<int> > mat_pow(vector<vector<int> > &a,int n) {
    int k = a.size();
    vector res(k, vector<int>(k));
    for (int i = 0; i < k; i++)
        res[i][i] = 1;
    while (n) {
        if (n & 1)res = mat_mul(res, a);
        a = mat_mul(a, a);
        n >>= 1;
    }
    return res;
}

vector<vector<vector<int> > > help(vector<vector<int> > &mat,int n,int bits) {
    vector arr(bits, vector(n, vector<int>(n)));
    for (int i = 0; i < n; i++)arr[0][0][i] = mat[0][i];
    for (int i = 1; i < n; i++)arr[0][i][i - 1] = mat[i][i - 1];
    for (int i = 1; i < bits; i++) // bits*n*n
        arr[i] = mat_mul(arr[i - 1], arr[i - 1]);
    return arr;
}

void get_result(vector<vector<vector<int> > > &arr, vector<vector<int> > &ans,int mask,int bits) {
    for (int j = 0; j < bits; j++)
        if (mask & (1ll << j))
            ans = mat_mul(arr[j], ans);
}

int32_t main() {
    ios_base::sync_with_stdio(0),
            cin.tie(0), cout.tie(0);
    int m, n, q;
    cin >> m >> n >> q;
    vector<int> a(m);
    for (int i = 0; i < m; i++) {
        cin >> a[i];
        if (a[i] < 0)a[i] += MOD;
    }
    reverse(a.begin(), a.end());
    vector<vector<int> > mat(m, vector<int>(m));
    for (int i = 0; i < m; i++)mat[0][i] = 1;
    for (int i = 1; i < m; i++)mat[i][i - 1] = 1;
    vector arr = help(mat, m, 61);
    vector<pair<int,int> > v;
    while (q--) {
        int idx, val;
        cin >> idx >> val;
        if (val < 0)val += MOD;
        if (idx == n) {
            cout << val;
            return 0;
        }
        v.emplace_back(idx, val);
    }
    sort(v.begin(), v.end());
    int last = m;
    v.emplace_back(n + 1, -inf);
    for (int i = 0; i < v.size(); i++) {
        int mask = v[i].first - last - 1;
        vector ans(m, vector<int>(1));
        for (int j = 0; j < m; j++)ans[j][0] = a[j];
        get_result(arr, ans, mask);
        a[0] = v[i].second;
        for (int j = 1; j < m; j++)a[j] = ans[j - 1][0];
        last = v[i].first;
        if (v[i].second == -inf)cout << ans[0][0] << '\n';
    }
}
