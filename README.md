# Testlib

`toString` - number to string

`toHumanReadableString` - number to string in human readable (scientific) notation

`rnd.next(n)` -> random number from interval $[0, n-1]$

`rnd.next(a,b)` -> random number from interval $[a, b]$

`rnd.wnext(n, k)` -> random number from interval $[0, n-1]$ with distribution $x^{sgn(k) * (abs(k) + 1)}$ (take max (or min depenending on the sign) from $abs(k)$ generations)

`rnd.any(begin, end)` -> random value from iterator interval

`rnd.wany(begin, end)` -> random value from iterator interval (with distribution, simililar to `rnd.wnext`)

`rnd.perm(n, first)` -> permutation of size $n$ (starts from $first$)

`rnd.distinct(n, a, b)` -> $n$ distinct values from the interval $[a, b]$

`rnd.distinct(n, lim)` -> $n$ distinct values from the interval $[0, lim-1]$

`rnd.partition(n, sum, mini)` -> $n$ numbers, with the sum equal to $sum$, each not less than $mini$

`rnd.partition(n, sum)` -> $n$ numbers, with the sum equal to $sum$

pattern:

- `next(rnd)` -> generated string matching pattern
- `matches(str)` -> check if matches

`trim(str)` -> string with trimed whitespace

`registerGen(seed)` -> register generator with seed equal to $seed$

`split(str, separators)` -> splits string on every occurance of a character in $seperators$

`println(x)` -> `cout << x << endl;`

`println(a,b)` -> `cout << a << ' ' << b << endl;`
itd.

`println(begin, end)` -> prints values from a interval of iterators

# Readwriter

## Graph

Class for graph structure

- construction of random graphs
  - clique
  - silkworm
  - jellyfish
  - forest (trivial and prufer codes)
  - paths
  - starfish
  - bounded degree trees
- formatting and printing
- some helper functions

# Samples

## Generator

```cpp
#include "testlib/readwriter.h"
using namespace std;

int main() {
    int seed;
    cin >> seed;
    registerGen(seed);
    int numOfNodes = rnd.next(10, 15);
    int numberOfTrees = rnd.next(3, 4);
    Graph g = Graph::construct_forest_graph(numOfNodes, numberOfTrees);
    g.printTo(cout, Solution);
}
```

## Solution

```cpp
#include "testlib/graph.h"
using namespace std;

int numberOfNodes(vector<vector<int>> graph) {
    return (int)graph.size();
}

int main() {
    Graph g = Graph::read_graph(cin);
    cout << numberOfNodes(g) << '\n';
}
```
