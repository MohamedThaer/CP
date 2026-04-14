auto divisor_analysis(vector<pair<int, int>> v) {
    using Z = mint<(int)1e9 + 7>;
    using Z2 = mint<(int)1e9 + 6>;

    auto calc_sum = [&](Z f, int k) { // (F^1 + F^2 + ... + F^k)
        return f * (f.power(k) - 1) / (f - 1);
    };

    auto calc_mul = [&](Z f, int k) { // (F^1 * F^2 * ... * F^k)
        return f.power(k * (k + 1LL) / 2);
    };

    Z count = 1, sum = 1, product = 1;
    Z2 cnt = 1;
    for (auto [f, k] : v) {
        count *= (k + 1);
        sum += calc_sum(f, k) * sum;
        product *= calc_mul(f, k).power(cnt.value) * product.power(k);
        cnt *= (k + 1);
    }
    return tuple{count, sum, product};
}
