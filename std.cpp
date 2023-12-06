#include <bits/stdc++.h>
using namespace std;
const int N=10005;
int L[N],H[N],R[N],n;
int a[N<<1],b[N<<1];
int main(){
	int l,h,r;
	while(cin>>l>>h>>r){
		n++;
		L[n]=l;H[n]=h;R[n]=r;
		a[2*n-1]=l;
		a[2*n]=r;
	}
	sort(a+1,a+2*n+1);
	int m=unique(a+1,a+2*n+1)-a-1;
	for(int i=1;i<=n;i++){
		int kl=lower_bound(a+1,a+m+1,L[i])-a;
		int kr=lower_bound(a+1,a+m+1,R[i])-a;
		for(int j=kl;j<kr;j++){
			b[j]=max(b[j],H[i]);
		}
	}
	for(int i=1;i<=m;i++){
		if(b[i]!=b[i-1]){
			cout<<a[i]<<" "<<b[i]<<" ";
		}
	}
	return 0;
}
