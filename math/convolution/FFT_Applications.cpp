vector<int> poly_pow(vector<int> poly, int p) {
      vector<int> ans{ 1 };
      while (p) {
         if (p & 1) ans = multiply(ans, poly);
         poly = multiply(poly, poly);
         p >>= 1;
      }
      return ans;
   }

   string mul_two_big_int(const string& s1, const string& s2) {
      int n = s1.size(), m = s2.size();
      vector<int> poly1(n), poly2(m);
      for (int i = 0; i < n; ++i) {
         poly1[n - i - 1] = s1[i] - '0';
      }
      for (int i = 0; i < m; ++i) {
         poly2[m - i - 1] = s2[i] - '0';
      }
      vector<int> ans = multiply(poly1, poly2);
      int k = ans.size();
      for (int i = 0; i < k - 1; ++i) {
         ans[i + 1] += ans[i] / 10;
         ans[i] = ans[i] % 10;
      }
      string final = to_string(ans[k - 1]);
      for (int i = k - 2; i >= 0; --i) {
         final += (char)(ans[i] + '0');
      }
      for (int i = 0; i < k; ++i) {
         if (final[i] != '0') return final.substr(i);
      }
      return "0";
   }

   string big_int_pow(string s, int p) {
      string ans = "1";
      while (p) {
         if (p & 1) ans = mul_two_big_int(ans, s);
         s = mul_two_big_int(s, s);
         p >>= 1;
      }
      return ans;
   }

   vector<int>str_match(const string& s, const string& patt) {
      int n = s.size(), m = patt.size();
      vector<int> poly1(n), poly2(m);
      vector<int> ans_match(n);
      int shift = m - 1;
      for (int i = 0; i < 26; ++i) {
         for (int j = 0; j < n; ++j) {
            poly1[j] = (s[j] - 'a') == i;
         }
         for (int j = 0; j < m; ++j) {
            poly2[j] = (patt[-j + shift] - 'a') == i;
         }
         vector<int> ans = multiply(poly1, poly2);
         for (int j = 0; j < n; ++j) {
            ans_match[j] += ans[j + shift];
         }
      }
      return ans_match;
   }

   vector<int>wild_card_cnt(const string& s, const string& patt) {
      int n = s.size(), m = patt.max_size();
      vector<int> ans_match = str_match(s, patt);
      vector<int> pos;
      int wild_cnt = count(patt.begin(), patt.end(), '*');
      for (int i = 0; i < n; ++i) {
         if (ans_match[i] == m - wild_cnt) {
            pos.push_back(i);
         }
      }
      return pos;
   }
