12.1) LCS (Longest Common Substring among many strings)
(From screenshot: builds suffix automaton on smallest string, then scans others)

Used for
Longest common substring (contiguous) among k strings.
Core idea
Build suffix automaton (SAM) for one string (usually the shortest).
For each other string, walk automaton and track longest match ending at each position.
Keep global maximum over all strings.
Input
A list of strings v (read until EOF in your snippet).
Output
Prints the length of LCS (longest common substring).
Important components (from snippet)
suffix_automaton sa(smallest_string)
can_add(char c) / next_state(state,c) checks transitions.
Two pointers window technique while scanning a string.
Example
Strings: ["ababc", "babca", "abcba"]
Longest common substring is "abc" length = 3.

When to use
Multi-string LCS (substring) with total length up to ~2e5–1e6.
12.2) PairRelation (compare suffixes across concatenated strings)
Used for
Problems where you have multiple strings and need:
compare substrings across strings lexicographically
count occurrences of one string’s substring inside another
Often used in “string relations” / “count occurrences across many strings”.
Core idea
Concatenate all strings into one big string with unique separators.
Build suffix array (or suffix array per string + merged).
Use LCP + binary search to count matches.
Input
vector v
Output / Queries
cmp(...) compare two substrings from two strings.
count(ida, idb) count occurrences of string v[ida] in v[idb] (as in your comment).
Important functions (from screenshot)
PairRelation() / PairRelation(v)
int cmp(int a,int la,int ra, int b,int lb,int rb)
Input: substring ranges in string a and b
Output: -1/0/1 comparison (like strcmp)
int count(int ida, int idb)
Output: number of occurrences of v[ida] in v[idb]
Example
v[0] = "aba", v[1] = "ababa"
count(0,1) = 2 ("aba" at positions 0 and 2)

12.3) PalindromicTree (Eertree)
Used for
Online palindromic substrings:
count distinct palindromes
count occurrences of each palindrome
longest palindromic suffix at each step
Very common in palindrome problems.
Core idea
Nodes represent distinct palindromes.
Two root nodes: length -1 and 0.
When adding a character, jump suffix links until you can extend.
Input
string s
Output
structure with nodes; you can query counts / distinct palindromes.
Important fields in node (from screenshot comments)
nxt[26]: transitions by adding char at both ends
len: palindrome length
st, en: substring indices (in original string) of one occurrence
link: suffix link to longest proper palindromic suffix
cnt: number of palindromes in suffix chain (often “distinct suffix pal count”)
oc: occurrences count (after propagation)
Important functions
PalindromicTree() / PalindromicTree(string s)
int extend(int pos)
Input: position in s to add
Output: 1 if new palindrome node created else 0
int next_state(int cur, char c)
Output: next node after extending with c
void calc_occurrences()
Effect: pushes occurrence counts from longer palindromes to suffix links.
Example
s="ababa"

distinct palindromes: a,b,aba,bab,ababa
longest palindrome suffix after each step can be tracked via last.
12.4) PersistentBinaryTrie< BITS >
Used for (XOR queries on prefix versions)
Typical tasks:

For each prefix (version), insert number.
Query:
max xor with x in version
min xor with x
kth smallest number / kth xor-related
count of values with xor < k
range queries via (version r) - (version l-1)
Core idea
Each insertion creates a new root (persistent).
Node stores cnt (#numbers passing here), and children 0/1.
Range query uses difference between two versions.
Inputs
version index (int)
value x (uint64_t)
delta for add/remove (default +1)
Outputs
depending on query: uint64 result or counts.
Important functions (from your pages)
PersistentBinaryTrie() creates version 0 empty.
void insert(int version, uint64_t x, int delta=1)
Input: version index to modify (creates that version’s root)
Effect: inserts x into that version
uint64_t max_xor(int version, uint64_t x)
uint64_t min_xor(int version, uint64_t x) uses ~x trick
uint64_t max_xor(int vl, int vr, uint64_t x) (range version diff)
uint64_t kth_order(int vl, int vr, uint64_t k)
Output: k-th smallest value in range multiset (by count difference)
uint64_t xor_less_than_k(int version, uint64_t x, uint64_t k)
Output: count of values y with (x xor y) < k in that version
uint64_t xor_less_than_k(int vl,int vr, x,k) range
uint64_t xor_greater_equal_to_k(...) = total - less_than
int count_all(int version) total inserted in that version
int count_value(int version, uint64_t x) frequency of x
int push_back(int version) clones root pointer (new version = same as old)
int cur_version() returns last version index
Example (range max xor)
Array: [5, 1, 7]

Build versions:
v0 = {}
v1 insert 5
v2 insert 1
v3 insert 7
Query max xor with x=2 in range [2..3] (values {1,7}):
max_xor(v1, v3, 2) should return max(1^2=3, 7^2=5) => 5
12.5) PersistentTrie<ALPHA, FIRST>
Used for
Persistent string trie with versions:

Insert word in a version
Query:
how many words equal to s
how many words with prefix p
occurrences list (get_occ)
plus ability to “activate/deactivate” words and query count in a text (used later).
Core idea
Persistent cloning of trie nodes along inserted path.
Each node stores:
prefix: count of words passing through
word: count of words ending here
nxt[ALPHA] child pointers
plus extra for Aho later: link, cnt, occ
Inputs
version index
string s
delta (+1 insert, -1 remove)
Outputs
counts and occurrence information.
Important functions (from your code)
PersistentTrie() creates version 0 empty.
void insert(int version, const string s, int delta=1)
Effect: creates new root for that version, updates prefix/word
int count_prefix(int version, const string &p)
Output: how many inserted strings in this version have prefix p
int count_words(int version, const string &s)
Output: count of exact word occurrences
int push_back(int version) → new version same root
int cur_version() → last version index
Example
Version 0 empty
Insert "moh" into version 1:

count_prefix(1,"mo") = 1
count_words(1,"moh") = 1
count_words(1,"mo") = 0
12.6) Aho-Corasick (static)
Used for
Multi-pattern matching:

Given many patterns, count how many appear in a text
Find occurrences positions / counts
Common: censorship, keyword search, DNA patterns.
Core idea
Trie of patterns + failure links (like KMP on trie).
While scanning text, follow transitions; on each node add pattern counts.
Input
vector patterns (lowercase in your code)
Output
ability to compute count in a query string.
Important components (from your static class)
Node fields:

child[26]
cnt : number of patterns ending at this node (or weighted)
link: failure link
occ: ids of patterns ending here (used for activation trick)
Functions:

constructor aho_corasick_static(words)
builds trie
builds failure links BFS
int count(const string &s)
Output: total matches in s (sum of cnt along traversal)
vector<vector<int>> get_occ(const string &s) (in dynamic version too)
Output: for each position i, list of pattern ids that end at i (based on occ lists)
Example
patterns = ["he","she","hers","his"], text="ahishers"
count(text) = 4 (his, she, he, hers) depending overlaps.

12.7) Aho-Corasick (dynamic / rebuild blocks)
Used for
Online insert patterns + query text count, without rebuilding full automaton each time.

Core idea (your code)
Maintain buckets li[20] like binary decomposition by size.
When inserting a new word:
merge buckets like binary addition
rebuild automaton for the merged bucket
Query by summing results from all existing automata.
Functions (from your class)
void insert(const string &s)
int get_count(string s)
Output: total matches across all inserted patterns in s
vector<vector<int>> get_occ(const string &s) merge occurrence lists from buckets
Example
Insert patterns: "a", then "ab", then "bab"
Query "babab" counts matches.

12.8) binary_trie (non-persistent XOR trie)
Used for
Maintain multiset of numbers and answer:

max xor with x
min xor with x
count of y such that (x xor y) < k
count of y such that (x xor y) >= k
kth smallest number (by order) in trie
Input
numbers int64_t (LOG default 62)
count increments/decrements
Output
xor results or counts.
Important functions (from your class)
void insert(int64_t x, int cnt=1)
Effect: add/remove x occurrences
int64_t max_xor(int64_t x)
int64_t min_xor(int64_t x) = max_xor(~x)
int64_t xor_less_than_k(int64_t x, int64_t k)
int64_t xor_greater_equal_to_k(int64_t x, int64_t k) = total - less
int64_t kth_order(int64_t k)
Output: k-th smallest inserted value (0-indexed usually; check your usage)
Helper valid(node) ensures node exists and cnt>0.
Example
Insert: 5,1,7

max_xor(2) = 7^2=5
xor_less_than_k(2,4): count y with y^2 < 4 → (1^2=3) yes, (5^2=7) no, (7^2=5) no => 1
12.9) bitset_matching
Used for
Fast pattern matching / substring search on small alphabet using bitsets.

Often: find all occurrences of pattern in text quickly (O(n/word_size)).
Core idea
Precompute bitset for each character positions in text.
For pattern, shift-and combine bitsets to find valid endpoints.
Important functions (from screenshot)
void set_text(const string &s)
void update(int idx, char ch) update text character
int pattern_count(const string &pat, int l, int r)
count occurrences of pat in text segment [l..r]
vector<int> pattern_pos(const string &pat, int l, int r)
return starting positions
Inputs
text length ≤ N (compile-time)
pattern lowercase (as masks use 'a')
Example
text="ababa"
pattern="aba"
positions: 0 and 2 → count=2

12.10) hashing (2D) — dr_string2D
Used for
2D rolling hash on a matrix of characters/strings:

compare sub-rectangles quickly
detect repeated blocks/patterns in grids.
Core idea
Precompute prefix hashes with two bases (row-base and col-base).
Sub-rectangle hash extracted in O(1).
Constructor
dr_string2D(const vector<string> &mat)

Input: matrix of chars (n rows, m cols)
Output: builds pow arrays and prefix-hash tables.
Query
int64_t substr(int x1,int y1,int x2,int y2) // 1-indexed in comment

Input: rectangle corners
Output: hash value mod (2^61-1).
Example
Compare two 2×2 blocks in a grid by comparing hashes.

12.11) hashing (const time) — dr_string
Used for
Standard rolling hash for string:

O(1) substring hash
compare substrings equality quickly.
Core idea
prefix hash + powers under mod (2^61-1)
Constructor
dr_string(const string &a)

Input: string a
Output: stores prefix hash and pow.
Functions (typical; your snippet shows substr/merge)
int64_t substr(int l, int r)
Input: substring [l..r] (your code later uses 1-index in update version; here likely 0-index—need confirm from full file)
Output: hash
int64_t merge(int64_t h1, int64_t h2, int len2)
Output: hash of concat
Example
Check if s[l1..r1] == s[l2..r2] by comparing hashes.

12.12) hashing (with update) — dr_string + segtree
Used for
Dynamic string:

point updates (change character)
query substring hash
check palindrome in substring using reverse hash
Core idea
Maintain segment tree where each node stores:
len
forward hash and reverse hash
Update range sets a character (your code uses range update but usually used for point update)
Palindrome check: forward hash == reverse hash for [l..r]
Important functions (from your code)
dr_string(const auto &a) builds powers and segtree from initial string
void update(int l, int r, int64_t val)
used as update on segment tree (often l=r for single char)
int64_t substr(int l, int r)
int64_t substr_rev(int l, int r)
bool is_palindrome(int l, int r) returns substr == substr_rev
Indexing in your code: looks 1-indexed in public methods:

substr(int l,int r) calls segtree query(l,r)
So treat l,r as 1..n.
Example
s="abca"

update(2,2,'b') etc
is_palindrome(1,3) checks "abc" false
12.13) KMP
Used for
Find all occurrences of pattern in text in O(n+m)
Prefix-function building used in many tasks.
Functions (from code)
vector<int> prefix_function(const string &s)
Input: s
Output: pi[i] = longest proper prefix of s[0..i] which is also suffix.
vector<int> KMP(const string &str, const string &pat)
Input: text str, pattern pat
Output: starting positions where pat occurs in str (0-indexed)
Example
str="ababa", pat="aba" → positions [0,2]

12.14) manacher
Used for
Find longest palindromic substring in O(n)
Check palindrome in O(1) after preprocessing.
Core idea
Transform string with separators to handle even/odd uniformly.
Compute palindrome radius around each center.
Class API (from your code)
manacher(string t) preprocess
int even(int i) returns length of longest even palindrome centered between i-1 and i (in original indexing logic)
int odd(int i) returns length of longest odd palindrome centered at i
bool is_palindrome(int l, int r)
Input: substring indices [l..r] (your code uses 1-indexed likely; verify in usage)
Output: true if s[l..r] palindrome
Example
s="abacaba"

longest palindrome length 7
is_palindrome(1,3) for "aba" true
12.15) prefix_function (standalone)
Same as KMP’s prefix_function; used alone for borders, periodicity, etc.

Common tasks

smallest period of string
count occurrences of each prefix
find borders.
12.16) suffix_array (plus internal sparse_table for RMQ on LCP)
Used for
lexicographic order of suffixes
LCP queries
substring search/count
number of distinct substrings
kth lexicographic substring
Core idea
Build SA with doubling algorithm O(n log n)
Build LCP array
Build RMQ sparse table on LCP for O(1) LCP(i,j)
Important components visible
template<class M> class sparse_table
sparse_table(vector<M> a, merge)
M query(int l,int r) inclusive
template<class T> class suffix_array
Key members:

T a (string/vector)
vector<int> suf suffix array
vector<int> lcp_ LCP between adjacent in SA
vector<int> group ranks
sparse_table<int> sp RMQ on LCP
Key functions (from screenshot)

constructor suffix_array(T a)
int lcp(int i, int j)
Input: positions in SA? (in your code it does i=group[i] etc)
Output: lcp length between suffixes starting at i and j in the original string
int cmp(pair<int,int> sub1, pair<int,int> sub2)
compare two substrings (l,r) using lcp and ranks
T kth_distinct_substring(int64_t k)
returns k-th distinct substring in lexicographic order
Also count(T sub) exists in screenshot:

counts occurrences of sub by binary search on SA.
Example: count occurrences
s="banana"
sub="ana" occurs 2 times
sa.count("ana") == 2

12.17) suffix_automaton (SAM, map-based)
Used for
count distinct substrings
count occurrences of substrings
find k-th substring (distinct or with multiplicity)
substring distribution by length
Core idea
Build automaton of all substrings.
Each state represents an endpos-equivalence class.
len = max length in state, link suffix link.
Important fields (from your class)
t[state].len
t[state].link
t[state].nxt (map<char,int> in your code)
terminal[state] occurrences of end positions (initially set for each appended char)
dp / dis_cnt / sub_cnt memo arrays:
count_distinct(state)
count_substrings(state) (with multiplicity/occurrence)
Important functions
suffix_automaton(), suffix_automaton(string s) builds
void append(char c, int val=1)
void build_tree() builds link-tree adjacency
int next_state(int cur, char c) → next state or -1
int64_t cnt(int i) → occurrences count of state i in original string (DP on link tree)
int64_t count_distinct(int node) → number of distinct substrings starting at node
int64_t count_substrings(int node) → number of substrings with multiplicity
string kth_distinct_substring(int64_t k)
string kth_substring(int64_t k)
vector<int> substring_distribution()
returns array where ret[L] = number of substrings of length ≤ L? (your code builds prefix sums; used for queries)
Example
s="ababa"

#distinct substrings = 9 (you can compute via SAM)
kth_distinct_substring(1) = "a" etc (depending k indexing)
12.18) suffix_automaton2 (array-based + lifting for substring id)
Used for
Faster SAM when alphabet small (or still map but with extra arrays):
represent a specific substring state for s[l..r]
count occurrences of pattern quickly
supports building with positions and binary lifting.
Important parts visible in comments
pos[i]: state after processing prefix ending at i
up[v][k]: 2^k ancestor along suffix links
Key functions visible
prepare_lifting()
int state_of_substring(int l, int r)
Input: substring indices
Output: SAM state representing that substring
If invalid returns -1
void build_tree() adjacency for suffix links
Use pattern
build(s)
prepare_lifting()
state = state_of_substring(l,r)
answer occurrences from that state (if you stored occurrences)
12.19) trie (normal trie)
Used for
Insert many words
Query:
count words equal to s
count words with prefix p
Node fields (from your code)
word: words ending here
prefix: words passing through
child[26] initialized to -1
Important functions
void insert(const string &s, int cnt=1)
increases prefix counts along path and word at end
int count_word(const string &s) exact count
int count_prefix(const string &s) prefix count
Example
Insert: "ab", "ab", "abc"

count_word("ab") = 2
count_prefix("ab") = 3
12.20) z_function
Used for
Pattern matching
Find occurrences of pattern in text via pat + '#' + text
String periodicity and borders
Function
vector<int> z_function(string s)

Input
string s
Output
z[i] = length of longest prefix of s that matches substring starting at i
Example
s="aaaaa"
z = [0,4,3,2,1]

Pattern matching example:
pat="aba", text="abacaba"
build t = "aba#abacaba"
positions where z[i] == |pat| give matches.

Integration in contest workflow (quick)
Need find pattern occurrences once: KMP or Z-function
Need many substring queries / kth substring: suffix_array or suffix_automaton
Need palindrome queries / longest palindrome: manacher or PalindromicTree
Need multi-pattern search: Aho-Corasick
Need XOR max/min / kth in ranges: PersistentBinaryTrie
Need dynamic string equality/palindrome: hashing with update
Need static string equality: hashing const time