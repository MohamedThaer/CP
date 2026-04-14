
// returns the number of solutions to the equation
// x_1 + x_2 + ... + x_n <= s and 0 <= l <= x_i <= r
Z AlgebricSolutionCount(int n, int s, int l, int r) {
    if (s < 1LL * l * n || l > r) return 0;
    s -= l * n;
    r -= l;
    Z ans = 0;
    for (int k = 0; k <= n; k++) {
        Z cur = ncr(s - k - k * r + n, n) * ncr(n, k);
        if (k & 1) {
            ans -= cur;
        }
        else {
            ans += cur;
        }
    }
    return ans;
}
