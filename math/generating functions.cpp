


/*
geometric: (1+x+x^2+...+x^n) = (x^(n+1) - 1) / (x - 1)

Generating Functions: summation i from zero to infinity
1 / (1 - ax)             = (a*x)^i
1 / (1 - x)^n            = ncr(n+i-1, i) * x^i
1 / (1 + x)              = (-x)^i
1 / (1 - x)^2            = (i + 1)*(x^i)
x / (1 - x)^2            = i*(x^i)
1 / sqrt(1 - 4*x)        = ncr(2*i, i)*(x^i)
x / (1 - x - x^2)        = Fib(i)*(x^i)
[1 - sqrt(1 - 4*x)] / 2x = C(i) *(x^i)   (C stands for catalan)
sigma1(i) * (x^i) = i*(x^i)/(1-(x^i))
fib[i]=a*fib[i - 1]+b*fib[i - 2] : x/(1-a*x-b*x^2)
f(i)=a*f(i-1)+b*f(i-2)+c*f(i-3) : [f(0)+(f(1)-a*f(0))*x+(f(2)-a*f(1)-b*f(0))*x^2]/(1-a*x-b*x^2-c*x^3)

Euler's Pentagonal Number Theorem:
mul(i,1,inf) {1 - x^i} = 1 - x - x^2 + x^5 + x^7 - x^12 + ......
where powers are in two forms: m * (3 * m - 1) / 2, m * (3 * m + 1) / 2
and signs are this pattern: + - - +


binomial theorem:
(x + 1)^n = summation(i, 0, n) { ncr(n, i) }
*/
