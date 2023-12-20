#include <bits/stdc++.h>
using namespace std;
const int N=105;
int dis[N][N];
int main()
{	
	int a,b,c;
	int n,m;
	while(cin>>n>>m&&(n||m)){
		memset(dis,0x3f,sizeof(dis));
		for(int i=1;i<=m;i++){
			cin>>a>>b>>c;
			dis[a][b]=dis[b][a]=min(c,dis[a][b]);
		}
		for(int i=1;i<=n;i++) dis[i][i]=0;
		for(int k=1;k<=n;k++){
			for(int i=1;i<=n;i++){
				for(int j=1;j<=n;j++){
					if(dis[i][j]>dis[i][k]+dis[k][j])
						dis[i][j]=dis[i][k]+dis[k][j];
				}
			}
		}
		cout<<dis[1][n]<<endl;
	}
	
	return 0;
}
