
vector<int> prefix_function(const string &s) {
    int n = s.size(), k = 0;
    vector<int> pref(n);
    for (int i = 1; i < n; ++i) {
        while (k > 0 && s[i] != s[k]) {
            k = pref[k - 1];
        }
        k += (s[i] == s[k]);
        pref[i] = k;
    }
    return pref;
}

vector<int> KMP(const string &str, const string &pat) {
    int k = 0;
    vector<int> pref = prefix_function(pat), ret;
    for (int i = 0; i < str.size(); ++i) {
        while (k > 0 && str[i] != pat[k]) {
            k = pref[k - 1];
        }
        k += (str[i] == pat[k]);
        if (k == pat.size()) {
            ret.push_back(i - k + 1);
            k = pref[k - 1];
        }
    }
    return ret;
}
