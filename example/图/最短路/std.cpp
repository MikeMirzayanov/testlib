#include <bits/stdc++.h>
using namespace std;
const int N=1e5+5;
struct node{
	int v,w;
	friend bool operator <(const node A,const node B){
		return A.w>B.w;
	}
};
vector<node> G[N];
int n,m,s;
int dis[N];
bool vis[N];
void Dijkstra(int st){
	priority_queue<node> q;
	q.push(node{st,0});
	dis[st]=0;
	while(!q.empty()){
		node cur=q.top();
		q.pop();
		int u=cur.v;
		if(vis[u]) continue;
		
		vis[u]=1;
		for(auto e:G[u]){
			int v=e.v,w=e.w;
			if(dis[u]+w<dis[v]){
				dis[v]=dis[u]+w;
				q.push(node{v,dis[v]});
			}
		}
	}
}
int main()
{
	memset(dis,0x3f,sizeof(dis));
	int u,v,w;
	scanf("%d%d%d",&n,&m,&s);
	for(int i=1;i<=m;i++){
		scanf("%d%d%d",&u,&v,&w);
		G[u].push_back(node{v,w});
	}
	Dijkstra(s);
	for(int i=1;i<=n;i++){
		printf("%d ",dis[i]);
	}
	
	return 0;
}
