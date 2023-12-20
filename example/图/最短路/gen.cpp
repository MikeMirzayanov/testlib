#include "testlib.h"
#include "treegenerator.h"
#include <bits/stdc++.h>
using namespace std;
using namespace tree_generator_by_ouuan;
const int N=1e5+5;
set<pair<int,int> >st;
int d[N];
int main(int argc, char *argv[]){
    registerGen(argc, argv, 1);
    /*****************************/
    int k=opt<int>(1);
    int Ln=100000,Rn=100000,Lm=200000,Rm=200000;
	if(k>=1&&k<=5){
		Ln=2,Rn=1000;
		Rm=10000;
	}
	int n=rnd.next(Ln,Rn);
	int m=Rm;
	int s=1;
	printf("%d %d %d\n",n,m,s);
	d[1]=0;
	for(int i=2;i<=n;i++){
		int u=i;
		int l=max(i-3,1),r=i-1;
		int v=(rnd.next(1,n)%(r-l+1))+l;
		d[u] = d[v] + 1;
		int t = rnd.next(1,10000);
		printf("%d %d %d\n", u,v,t);
	}
	for(int i = 1;i <= m - (n - 1);++i){
		int u = rnd.next(1,n);
		int v = rnd.next(1,n);
		while(u==v){
			u = rnd.next(1,n);
			v = rnd.next(1,n);			
		}
		int l = d[u] - d[v], r = l + 3;
		int t = (rnd.next(1,10000) % (r - l + 1)) + l;
		printf("%d %d %d\n", u,v,t);
	} 
	return 0;
}
