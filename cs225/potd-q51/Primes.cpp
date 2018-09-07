#include <vector>
#include "Primes.h"

std::vector<int> *genPrimes(int n) {
    std::vector<int> *v = new std::vector<int>();
    // your code here
    bool prime[n+1];
    for(int i = 0; i < n+1; i++)
        prime[i] = true;
    for (int p=2; p*p<=n; p++)
    {
        // If prime[p] is not changed, then it is a prime
        if (prime[p] == true)
        {
            // Update all multiples of p
            for (int i=p*2; i<=n; i += p)
                prime[i] = false;
        }
    }
    for (int p=2; p<=n; p++)
       if (prime[p])
         v->push_back(p);

    return v;
}
