
void solve() {
    vector<string> v;
    string s;
    while (cin >> s) {
        v.push_back(s);
    }
    int n = v.size();
    s = *min_element(v.begin(), v.end(), [&](string a, string b) { return a.size() < b.size(); });

    vector<suffix_automaton> sa(v.begin(), v.end());

    vector<int> cur(n, 0);
    int ans = 0;
    for (int l = 0, r = 0; l < s.size(); ++l) {
        r = max(r, l);

        auto can_add = [&](char c) {
            for (int i = 0; i < n; ++i) {
                if (sa[i].next_state(cur[i], c) == -1) {
                    return 0;
                }
            }
            return 1;
        };

        auto add = [&](char c) {
            for (int i = 0; i < n; ++i) {
                cur[i] = sa[i].next_state(cur[i], c);
            }
        };

        while (r < s.size() && can_add(s[r])) {
            add(s[r++]);
        }

        ans = max(ans, r - l);

        auto remove_first = [&]() {
            int len = r - l;
            for (int i = 0; i < n; ++i) {
                if (sa[i].t[sa[i].t[cur[i]].link].len >= len - 1) {
                    cur[i] = sa[i].t[cur[i]].link;
                }
            }
        };
        if (r - l > 0) remove_first();
    }

    cout << ans;
}
