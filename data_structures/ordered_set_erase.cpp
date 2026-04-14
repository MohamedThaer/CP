template <class M>
void erase(ordered_set<M> &x, M v) {
    int rank = x.order_of_key(v);                        // Number of elements that are less than v in t
    auto it = x.find_by_order(rank); // Iterator that points to the (rank+1)th element in t
    x.erase(it);
}
