vector<int> prefix_function(const string &s) {
    int n = s.size(), k = 0;
    vector<int> pref(n);
    for (int i = 1; i < n; ++i) {
        while (k > 0 && s[i] != s[k]) {
            k = pref[k - 1];
        }
        if (s[i] == s[k]) {
            k++;
        }
        pref[i] = k;
    }
    return pref;
}
