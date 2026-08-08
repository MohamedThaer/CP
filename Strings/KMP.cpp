
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

vector<vector<int> > compute_automaton(string s) {
    s += '#';
    vector<int> pi = prefix_function(s);
    int n = s.size();
    vector<vector<int> > aut(n, vector<int>(26, 0));
    for (int i = 0; i < n; i++) {
        for (int c = 0; c < 26; c++) {
            if (s[i] - 'a' == c)
                aut[i][c] = i + 1;
            else if (i)
                aut[i][c] = aut[pi[i - 1]][c];
        }
    }
    return aut;
}

vector<pair<int, int>> get_prefix_suffix_occurrences(const string &s) {
    int n = s.size();
    vector<int> pi = prefix_function(s);
    vector<int> freq(n + 1, 0);
    for (int i = 0; i < n; i++) {
        freq[pi[i]]++;
    }
    for (int i = n - 1; i > 0; i--) {
        freq[pi[i - 1]] += freq[i];
    }
    for (int i = 0; i <= n; i++) {
        freq[i]++;
    }
    vector<pair<int, int>> ans;
    int len = n;
    while (len > 0) {
        ans.push_back({len, freq[len]});
        len = pi[len - 1];
    }
    reverse(ans.begin(), ans.end());
    return ans;
}
