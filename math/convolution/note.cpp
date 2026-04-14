2.1) FFT (Fast Fourier Transform)
USED FOR

Fast polynomial/array convolution:
c[k] = Σ a[i] * b[k-i]
Big integer multiplication
String matching tricks (convert chars to numbers)
CORE IDEA (SIMPLE)

Naive multiplication is O(n^2).
FFT transforms arrays to “frequency domain”.
Multiply point-by-point.
Inverse FFT returns coefficients.
INPUTS

vector a
vector b
OUTPUT

vector c (size = |a| + |b| - 1)
ASSUMPTIONS / CONSTRAINTS

Uses floating point -> rounding errors possible for large values.
Must pad length to power of 2:
n >= |a| + |b| - 1
IMPORTANT FUNCTIONS

fft(a, invert)

Input : array a (complex), invert=false (FFT), invert=true (inverse)
Output: modifies a in place
Why : core transform
multiply(a, b)

Input : arrays a, b
Output: convolution result
Why : wrapper:
pad -> FFT(a), FFT(b) -> pointwise multiply
-> inverse FFT -> round
SMALL EXAMPLE

a = [1,2]
b = [3,4]
c = [3,10,8]
WHEN TO USE

Need fast convolution and values are not extremely large.
WHEN NOT TO USE

Need exact modulo answer -> use NTT.
Values too large -> precision risk.
2.2) FFT APPLICATIONS
A) Polynomial Power: poly_pow(poly, p)
USED FOR

Repeated convolution / generating functions / counting ways
IDEA

Binary exponentiation using convolution:
ans = [1]
while p > 0:
if p odd: ans = ans * poly
poly = poly * poly
p >>= 1
EXAMPLE

poly = [1,1] represents (1 + x)
poly^2 = [1,2,1]
B) Big Integer Multiplication (strings)
USED FOR

Multiply huge numbers given as strings
STEPS

Convert digits to array (reversed)
Convolve
Carry handling
EXAMPLE

"123" * "45" = "5535"
2.3) FWHT (Walsh–Hadamard Transform)
XOR / AND / OR Convolution
USED FOR

Bitmask problems where indices combine as:
XOR: i xor j = k
AND: i & j = k
OR : i | j = k
COMMON PATTERNS
Mask DP, subset DP, counting mask pairs
CORE IDEA (SIMPLE)

Special transform for bitmasks.
After transform: convolution becomes pointwise multiplication.
Apply inverse transform to return result.
INPUTS

arrays a, b with size n = 2^m
OUTPUT

array c with size n
ASSUMPTIONS / CONSTRAINTS

n must be a power of 2.
IMPORTANT FUNCTIONS

hadamard(a, inv, mode)

Input : array a, mode in {XOR, AND, OR}, inv=inverse?
Output: modifies a in place
Why : core FWHT transform
multiply(a, b, mode)

Input : arrays a, b and mode
Output: convolution under XOR/AND/OR
Why : wrapper:
transform both -> pointwise multiply -> inverse
TINY XOR EXAMPLE (IDEA)

If a[1] = 2 and b[2] = 4,
they contribute to c[1 xor 2 = 3] by 2*4 = 8.
2.4) NTT (Number Theoretic Transform)
USED FOR

Exact polynomial convolution modulo prime MOD
DP / combinatorics with exact mod results
CORE IDEA (SIMPLE)

Same as FFT steps, but in modular arithmetic (no floating errors).
INPUTS

arrays a, b (values modulo MOD)
OUTPUT

c = a * b (mod MOD)
ASSUMPTIONS / CONSTRAINTS (IMPORTANT)

MOD must be NTT-friendly:
MOD = c * 2^k + 1
Need primitive root for MOD
Pad length to power of 2
IMPORTANT FUNCTIONS

ntt(A, invert)

Input : vector A, invert flag
Output: modifies A in place
Why : core transform
multiply(a, b)

Input : arrays a, b
Output: exact convolution mod MOD
NOTE (YOUR TEMPLATE)

Uses 3 NTT primes + CRT to rebuild large integer results.
2.5) GCD CONVOLUTION
COMPUTES

C[k] = Σ A[i] * B[j] where gcd(i, j) = k
USED FOR

Counting pairs grouped by gcd
Number theory “gcd class” sums
CORE IDEA (SIMPLE)

Use transform over multiples (zeta), multiply, then invert (Möbius).
INPUTS

A[1..n], B[1..n] (usually 1-based)
OUTPUT

C[1..n]
ASSUMPTIONS / CONSTRAINTS

Complexity about O(n log n) via looping over multiples.
IMPORTANT FUNCTIONS

gcd_zeta(v)

Output: v[d] = Σ v[m] for all m multiple of d
Why : prepares for gcd grouping
gcd_mobius(v)

Output: inverse of gcd_zeta
Why : recovers exact gcd = k contributions
multiply(A, B)

Does : gcd_zeta(A), gcd_zeta(B)
pointwise multiply
gcd_mobius(result)
2.6) LCM CONVOLUTION
COMPUTES

C[k] = Σ A[i] * B[j] where lcm(i, j) = k
USED FOR

Counting pairs grouped by lcm
Divisor-based number theory patterns
CORE IDEA (SIMPLE)

Use transform over divisors (zeta), multiply, then invert (Möbius).
INPUTS

A[1..n], B[1..n] (1-based)
OUTPUT

C[1..n]
IMPORTANT FUNCTIONS

DivisorZetaTransform(v)

Output: v[x] = Σ v[d] for all d | x
Why : prepares for lcm grouping
DivisorMobiusTransform(v)

Output: inverse transform
Why : isolates exact lcm = x contributions
multiply(A, B)

Does : zeta(A), zeta(B)
pointwise multiply
mobius inverse