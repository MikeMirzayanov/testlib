/*

MIT License

Copyright (c) 2019 ouuan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

// See https://github.com/ouuan/Tree-Generator to get latest version or bug tracker.

#ifndef TREE_GENERATOR_BY_OUUAN_
#define TREE_GENERATOR_BY_OUUAN_ 1

#include <queue>
#include <chrono>
#include <random>
#include <vector>
#include <string>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/assoc_container.hpp>

namespace tree_generator_by_ouuan
{
    typedef std::pair<int, int> pii; 

    std::mt19937 defaultRNG(std::chrono::steady_clock::now().time_since_epoch().count());

    int defaultRandInt(int l, int r)
    {
        int out = defaultRNG() % (r - l + 1) + l;
        return out >= l ? out : out + r - l + 1; 
    }

    int (*randint)(int, int) = defaultRandInt;

    void defaultOutputEdge(std::ostream& os, int u, int pa)
    {
        if (randint(0, 1) == 0) os << u + 1 << ' ' << pa + 1 << std::endl;
        else os << pa + 1 << ' ' << u + 1 << std::endl;
    }

    void (*outputEdge)(std::ostream&, int, int) = defaultOutputEdge;

    class Tree
    {
    private:
        std::vector<int> p, id, eid;

    public:
        Tree()
        {
            p.push_back(-1);
            id.push_back(0);
        }
        Tree(int n)
        {
            assert(n > 0);
            p.push_back(-1);
            id.push_back(0);
            if (n > 1) random(n - 1, 0);
        }
        Tree(const std::string& s)
        {
            p.push_back(-1);
            id.push_back(0);

            std::function<unsigned int(const std::string&, unsigned int)> findComma = [](const std::string& str, unsigned int sta)
            {
                while (sta < str.size() && str[sta] != ',') ++sta;
                return sta;
            };

            std::function<unsigned int(const std::string&, unsigned int)> findLetter = [](const std::string& str, unsigned int sta)
            {
                while (sta < str.size() && !isalpha(str[sta])) ++sta;
                return sta;
            };

            std::function<void(const std::string &)> error = [](const std::string & func)
            {
                std::cerr << "Error: the number of parameters for " << func << " is not correct." << std::endl;
                exit(1);
            };

            unsigned int pos = 0;

            while (pos < s.size())
            {
                if (pos + 1 >= s.size())
                {
                    std::cerr << "Error: can't recognize the tree type abbreviation, it's too short.\n";
                    exit(1);
                }
                std::string type = s.substr(pos, 2);
                pos += 2;
                for_each(type.begin(), type.end(), [](char& x){x = tolower(x);});
                int nextLetter = findLetter(s, pos);
                if (type == "lh")
                {
                    int nextComma = findComma(s, pos);
                    assert(nextComma < nextLetter);
                    int n = atoi(s.substr(pos, nextComma - pos).c_str());
                    pos = nextComma + 1;
                    nextComma = findComma(s, pos);
                    assert(nextComma < nextLetter);
                    double low, high;
                    sscanf(s.substr(pos, nextComma - pos).c_str(), "%lf", &low);
                    pos = nextComma + 1;
                    nextComma = findComma(s, pos);
                    assert(nextComma < nextLetter);
                    sscanf(s.substr(pos, nextComma - pos).c_str(), "%lf", &high);
                    pos = nextComma + 1;
                    nextComma = findComma(s, pos);
                    assert(nextComma >= nextLetter);
                    int pa = atoi(s.substr(pos, nextComma - pos).c_str());
                    pos = nextLetter;
                    lowhigh(n, low, high, pa);
                    continue;
                }
                if (type == "lm")
                {
                    int nextComma = findComma(s, pos);
                    assert(nextComma < nextLetter);
                    int n = atoi(s.substr(pos, nextComma - pos).c_str());
                    pos = nextComma + 1;
                    nextComma = findComma(s, pos);
                    assert(nextComma < nextLetter);
                    int k = atoi(s.substr(pos, nextComma - pos).c_str());
                    pos = nextComma + 1;
                    nextComma = findComma(s, pos);
                    assert(nextComma < nextLetter);
                    double low, high;
                    sscanf(s.substr(pos, nextComma - pos).c_str(), "%lf", &low);
                    pos = nextComma + 1;
                    nextComma = findComma(s, pos);
                    assert(nextComma < nextLetter);
                    sscanf(s.substr(pos, nextComma - pos).c_str(), "%lf", &high);
                    pos = nextComma + 1;
                    nextComma = findComma(s, pos);
                    assert(nextComma >= nextLetter);
                    int pa = atoi(s.substr(pos, nextComma - pos).c_str());
                    pos = nextLetter;
                    lowhighMaxDegree(n, k, low, high, pa);
                    continue;
                }
                std::vector<int> par;
                while (1)
                {
                    int nextComma = findComma(s, pos);
                    par.push_back(atoi(s.substr(pos, nextComma - pos).c_str()));
                    pos = nextComma + 1;
                    if (nextComma >= nextLetter)
                    {
                        pos = nextLetter;
                        break;
                    }
                }
                if (type == "nd")
                {
                    if (par.size() == 1) addNode(par[0]);
                    else error("addNode");
                }
                else if (type == "rd")
                {
                    if (par.size() == 2) random(par[0], par[1]);
                    else error("random");
                }
                else if (type == "tl")
                {
                    if (par.size() == 3) tall(par[0], par[1], par[2]);
                    else error("tall");
                }
                else if (type == "ch")
                {
                    if (par.size() == 2) chain(par[0], par[1]);
                    else error("chain");
                }
                else if (type == "st")
                {
                    if (par.size() == 2) star(par[0], par[1]);
                    else error("star");
                }
                else if (type == "fl")
                {
                    if (par.size() == 2) flower(par[0], par[1]);
                    else error("flower");
                }
                else if (type == "md")
                {
                    if (par.size() == 3) maxDegree(par[0], par[1], par[2]);
                    else error("maxDegree");
                }
                else if (type == "cp")
                {
                    if (par.size() == 3) complete(par[0], par[1], par[2]);
                    else error("complete");
                }
                else if (type == "bi")
                {
                    if (par.size() == 2) binary(par[0], par[1]);
                    else error("binary");
                }
                else if (type == "cb")
                {
                    if (par.size() == 2) completeBinary(par[0], par[1]);
                    else error("completeBinary");
                }
                else if (type == "sw")
                {
                    if (par.size() == 2) silkworm(par[0], par[1]);
                    else error("silkworm");
                }
                else if (type == "al")
                {
                    if (par.size() == 3) addLeaves(par[0], par[1], par[2]);
                    else error("addLeaves");
                }
                else
                {
                    std::cerr << "Error: can't recognize the tree type abbreviation " << type << "." << std::endl;
                    exit(1);
                }
            }
        }
        int size() const { return id.size(); }
        void addNode(int pa)
        {
            assert(pa >= 0);
            assert(pa < size());
            id.push_back(id.size());
            p.push_back(pa);
            eid.push_back(id.size() - 1);
        }
        void random(int n, int pa)
        {
            int sz = size();
            assert(n > 0);
            assert(pa >= 0);
            assert(pa < sz);
            addNode(pa);
            if (n == 1) return;
            if (n == 2)
            {
                addNode(sz);
                return;
            }
            std::vector<int> prufer, cnt;
            std::vector<std::vector<int> > g;
            g.resize(n);
            cnt.resize(n, 0);
            for (int i = 0; i < n - 2; ++i)
            {
                int x = randint(0, n - 1);
                prufer.push_back(x);
                ++cnt[x];
            }
            std::priority_queue<int> q;
            for (int i = 0; i < n; ++i) if (!cnt[i]) q.push(i);
            for (auto v : prufer)
            {
                int u = q.top();
                g[u].push_back(v);
                g[v].push_back(u);
                q.pop();
                if (--cnt[v] == 0) q.push(v);
            }
            int x = q.top();
            q.pop();
            int y = q.top();
            g[x].push_back(y);
            g[y].push_back(x);

            std::queue<int> bfs;

            bfs.push(0);
            int _id = sz;

            while (!bfs.empty())
            {
                int u = bfs.front();
                cnt[u] = 1;
                bfs.pop();
                for (auto v : g[u])
                {
                    if (cnt[v] == 0)
                    {
                        addNode(_id);
                        bfs.push(v);
                    }
                }
                ++_id;
            }
        }
        void lowhigh(int n, double low, double high, int pa)
        {
            int sz = size();
            assert(n > 0);
            assert(low >= 0);
            assert(high <= 1);
            assert(high >= low);
            assert(pa >= 0);
            assert(pa < sz);
            addNode(pa);
            for (int i = 1; i < n; ++i) addNode(randint(round((i - 1) * low), round((i - 1) * high)) + sz);
        }
        void tall(int n, int k, int pa)
        {
            int sz = size();
            assert(n > 0);
            assert(k > 0);
            assert(pa >= 0);
            assert(pa < sz);
            addNode(pa);
            for (int i = sz + 1; i < sz + n; ++i) addNode(randint(std::max(sz, i - k), i - 1));
        }
        void chain(int n, int pa)
        {
            assert(n > 0);
            assert(pa >= 0);
            assert(pa < size());
            tall(n, 1, pa);
        }
        void star(int n, int pa)
        {
            int sz = size();
            assert(n > 0);
            assert(pa >= 0);
            assert(pa < sz);
            addNode(pa);
            for (int i = sz + 1; i < sz + n; ++i) addNode(sz);
        }
        void flower(int n, int pa)
        {
            assert(n > 0);
            assert(pa >= 0);
            assert(pa < size());
            star(n, pa);
        }
        void maxDegree(int n, int k, int pa)
        {
            int sz = size();
            assert(n > 0);
            assert(k >= 2);
            assert(pa >= 0);
            assert(pa < sz);
            addNode(pa);
            __gnu_pbds::tree<pii, __gnu_pbds::null_type, std::less<pii>, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update> remain;
            remain.insert(pii(sz, k - 1));
            for (int i = sz + 1; i < sz + n; ++i)
            {
                auto it = remain.find_by_order(randint(0, remain.size() - 1));
                int u = it->first;
                int t = it->second;
                remain.erase(it);
                if (t > 1) remain.insert(pii(u, t - 1));
                addNode(u);
                remain.insert(pii(i, k - 1));
            }
        }
        void lowhighMaxDegree(int n, int k, double low, double high, int pa)
        {
            int sz = size();
            assert(n > 0);
            assert(k >= 2);
            assert(low >= 0);
            assert(high <= 1);
            assert(pa >= 0);
            assert(pa < sz);
            addNode(pa);
            __gnu_pbds::tree<pii, __gnu_pbds::null_type, std::less<pii>, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update> remain;
            remain.insert(pii(sz, k - 1));
            for (int i = sz + 1; i < sz + n; ++i)
            {
                auto it = remain.find_by_order(randint(round((remain.size() - 1) * low), round((remain.size() - 1) * high)));
                int u = it->first;
                int t = it->second;
                remain.erase(it);
                if (t > 1) remain.insert(pii(u, t - 1));
                addNode(u);
                remain.insert(pii(i, k - 1));
            }
        }
        void complete(int n, int k, int pa)
        {
            int sz = size();
            assert(n > 0);
            assert(k >= 2);
            assert(pa >= 0);
            assert(pa < sz);
            addNode(pa);
            for (int i = sz + 1; i < sz + n; ++i) addNode(sz + ceil(1.0 * (i - sz) / (k - 1) - 1e-9) - 1);
        }
        void binary(int n, int pa)
        {
            assert(n > 0);
            assert(pa >= 0);
            assert(pa < size());
            maxDegree(n, 3, pa);
        }
        void completeBinary(int n, int pa)
        {
            assert(n > 0);
            assert(pa >= 0);
            assert(pa < size());
            complete(n, 3, pa);
        }
        void silkworm(int n, int pa)
        {
            int sz = size();
            assert(n > 0);
            assert(pa >= 0);
            assert(pa < sz);
            int chain_len = (n + 1) / 2;
            chain(chain_len, pa);
            for (int i = sz; i + chain_len < sz + n; ++i) addNode(i);
        }
        void addLeaves(int n, int l, int r)
        {
            assert(n > 0);
            assert(l >= 0);
            assert(r < size());
            assert(l <= r);
            for (int i = 1; i <= n; ++i) addNode(randint(l, r));
        }
        void shuffleNodes(int from = 0)
        {
            for (int i = 0; i < from; ++i) id[i] = i;
            for (unsigned int i = from; i < id.size(); ++i)
            {
                id[i] = i;
                std::swap(id[i], id[randint(from, i)]);
            }
        }
        void shuffleEdges()
        {
            for (unsigned int i = 0; i < eid.size(); ++i)
            {
                eid[i] = i + 1;
                std::swap(eid[i], eid[randint(0, i)]);
            }
        }
        void resize(int n)
        {
            assert(n > 0);
            if (size() < n) addLeaves(n - size(), 0, size() - 1);
            else if (size() > n)
            {
                p.resize(n);
                id.resize(n);
                eid.resize(n - 1);
                for (int i = 0; i < n; ++i) id[i] = i;
                for (int i = 0; i < n - 1; ++i) eid[i] = i + 1;
            }
        }
        void printEdge(int edgeID, std::ostream& os = std::cout) const
        {
            outputEdge(os, id[eid[edgeID]], id[p[eid[edgeID]]]);
        }
        int parent(int u) const
        {
            return p[u];
        }
        friend std::ostream& operator<<(std::ostream& os, const Tree& t)
        {
            for (unsigned int i = 0; i < t.eid.size(); ++i) t.printEdge(i, os);
            return os;
        }
    };
}

#endif
