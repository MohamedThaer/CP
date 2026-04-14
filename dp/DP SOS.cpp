/*

const int N = ...;
int A[1 << N]{}, F[1 << N]{}, dp[1 << N][N]{};

// (3^N)
for (int mask = 0; mask < (1 << N); ++mask) {
    F[mask] = A[0];
    for (int s = mask; s; s = (s - 1) & mask) {
        F[mask] += A[s];
    }
}

// recursive
memset(dp, -1, sizeof dp);
function<int(int, int)> solve = [&](int mask, int i) {
    if (i == -1) {
        return A[mask];
    }
    auto &ret = dp[mask][i];
    if (~ret) {
        return ret;
    }
    ret = solve(mask, i - 1);
    if (mask & (1 << i)) {
        ret += solve(mask ^ (1 << i), i - 1);
    }
    return ret;
};


// iterative
for (int mask = 0; mask < (1 << N); ++mask) {
    dp[mask][0] = A[mask];
    if (mask & 1) {
        dp[mask][0] += A[mask ^ 1];
    }
    for (int i = 1; i < N; ++i) {
        dp[mask][i] = dp[mask][i - 1];
        if (mask & (1 << i)) {
            dp[mask][i] += dp[mask ^ (1 << i)][i - 1];
        }
    }
    F[mask] = dp[mask][N - 1];
}


// memory optimized
for (int mask = 0; mask < (1 << N); ++mask) {
    F[mask] = A[mask];
}
for (int i = 0; i < N; ++i) {
    for (int mask = 0; mask < (1 << N); ++mask) {
        if (mask & (1 << i)) {
            F[mask] += F[mask ^ (1 << i)];
        }
    }
}

*/
