#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
int a[205][205],dp[205][205],t[205];
int main(){
	memset(a,0x3f,sizeof(a));
	memset(dp,0x3f,sizeof(dp));
	int n,m,x,y,z,ti,q,i,j,k=0;
	cin>>n>>m;
	for(i=0;i<n;++i)cin>>t[i];
	for(i=1;i<=m;++i){
		cin>>x>>y>>z;
		a[x][y]=a[y][x]=z;
		dp[x][y]=dp[y][x]=z;
	}
	cin>>q;
	while(q--){
		cin>>x>>y>>ti;
		if(t[x]>ti||t[y]>ti){
			cout<<-1<<endl;
			continue;
		}
		for(;k<n;++k){
			if(t[k]>ti)break;
			for(i=0;i<n;++i)for(j=0;j<n;++j)if(dp[i][j]>dp[i][k]+dp[k][j])dp[i][j]=dp[i][k]+dp[k][j];
		}
		if(dp[x][y]==0x3f3f3f3f)cout<<-1<<endl;
		else if(x==y)cout<<0<<endl;
		else cout<<dp[x][y]<<endl;
	}
	return 0;
}
