
/*
bool is_winning(state) {
    ret = 0;
    ret |= !is_winning(next_state_1);
    ret |= !is_winning(next_state_n);
    return ret;
}

normal nim: xor is positive ? first : second
reversed nim: (all ones ? (odd arr len? second : first ) : (xor is positive ? first : second))

int grundy(state) {
    return mex({grundy(next_state_1), grundy(next_state_2), ..., grundy(next_state_n)});
}

tree nim (remove any subtree except root): grundy[node] = xor(grundy[ch_1]+1, grundy[ch_2]+1, ..., grundy[ch_k]+1)
graph grundy = bridge tree grundy

Consider a two player game on a graph with a specified vertex (root).
In each turn, a player eliminates one edge.
Then, if a subgraph that is disconnected from the root, it is removed.
If a player cannot select an edge (i.e., the graph is singleton),
he will lose.
Compute the Grundy number of the given graph.

We will use two principles:
1. Colon Principle:
    Grundy number of a tree is the xor of
    Grundy number of child subtrees.

2. Fusion Principle:
    Consider a pair of adjacent vertices u, v
    that has another path (i.e., they are in a cycle). Then,
    we can contract u and v without changing Grundy number.

*/
