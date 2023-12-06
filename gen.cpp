#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=2e5+5;
int x[N];
int s[N];
int main(int argc, char *argv[]){
	registerGen(argc, argv, 1);
	int k=opt<int>(1);
	int L=1,R=1000;
	if(k>=1&&k<=5){
		L=4;R=100000;
	}else{
		L=1e8;R=2e9;
	}
	int n=5000;
	for(int i=1;i<=n;i++){
		int l,h,r;
		l=rnd.next(L,R);
		h=rnd.next(1,R);
		r=rnd.next(l+1,R);
		printf("%d %d %d\n",l,h,r);
	}
	return 0;
}
