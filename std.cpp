#include <bits/stdc++.h>
using namespace std;
const int N=1e6+5;
vector<int> G[N];
int dp[N];
void dfs(int u,int f){
    dp[u]=1;
    for(auto v:G[u]){
        if(f==v) continue;
        dfs(v,u);
        dp[u]+=dp[v];
    }
}
int main(){
    int n,m,u,v;
    scanf("%d%d",&n,&m);
    for(int i=1;i<n;i++){
        scanf("%d%d",&u,&v);
        G[u].push_back(v);
        G[v].push_back(u);
    }
    dfs(1,1);
    for(int i=1;i<=m;i++){
        scanf("%d",&u);
        printf("%d\n",dp[u]);
    }
	return 0;
}
