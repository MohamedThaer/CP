#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;
template <class M>
using ordered_set = tree<M, null_type, less<M>, rb_tree_tag, tree_order_statistics_node_update>;
template <class M>
using ordered_multiset = tree<M, null_type, less_equal<M>, rb_tree_tag, tree_order_statistics_node_update>;
