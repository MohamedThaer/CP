

/*
Math Guide
<---------------------------------------------------------------->

extended euclid: x*a + y*b = gcd(a, b)
bezout: different pairs from {x, y}
diophantine: x*a + y*b = c ----solvable-if---> (c % gcd(a, b) == 0)
CRT:
x = R1 (mod M1)
x = R2 (mod M2)
x = R3 (mod M3)

/////////////////////////////////////////////////////

a*x = a*y (mod M)
x = y (mod M / gcd(a, M))

/////////////////////////////////////////////////////

(x + y)^p = x^p + y^p (mod p)

/////////////////////////////////////////////////////

modular equation:
x*a = b (mod M)
x*a - b = y*M
x*a - y*M = b

solve diophantine(a, M, b)

/////////////////////////////////////////////////////

a^phi(M) = 1 (mod M) --if--> gcd(a, M) = 1

/////////////////////////////////////////////////////

x = 0
for(d:divisors(n)) x += mobius(d)
--> x = [n = 1]

/////////////////////////////////////////////////////

x = 0
for(d:divisors(n)) x += phi(d)
--> x = n

/////////////////////////////////////////////////////

---> d|y check if d divides y
gcd(x, y) = summation(i, 1, N) phi(i) * [d|x] * [d|y]

/////////////////////////////////////////////////////

geometric sequence: (1+r+r^2+...+r^n) = (r^(n+1) - 1) / (r - 1)
(1^2+2^2+3^2+...+n^2) = n*(n+1)*(2*n+1)/6

//////////////////////////////////////////////////////

integer division lemma: (a/b)/c = a/(b*c)

///////////////////////////////////////////////////////

The Chicken McNugget Theorem states that for any two relatively prime positive integers m,n
the greatest integer that cannot be written in the form am+bn
for nonnegative integers a,b is mn−m−n (equivalent to (n - 1)(m - 1) - 1)

////////////////////////////////////////////////////////

tail_sum_identity:
for any nonnegative randon variable X:
E[X] = sum(t, 1, inf) { propability(X >= t) }

///////////////////////////////////////////////////////////

Bertrand's ballot theorem:

string of A and B where (count(A) > count(B))

propability of A always greater than B in any prefix of the string is
(count(A) - count(B)) / (count(A) + count(B))

valid string: AABAABB

<---------------- another variation ------------------->
string of A and B where (count(A) >= count(B))

propability of A always greater than or equal to B in any prefix of the string is
(count(A) + 1 - count(B)) / (count(A) + 1)

valid string: AABBAABB
////////////////////////////////////////////////////////////
every number can be written as sum of three triangular numbers
every number of form 8*n+3 can be wrritten as sum of three odd squares

*/
