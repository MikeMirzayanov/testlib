#include "testlib.h"
#include "treegenerator.h"
#include <bits/stdc++.h>
using namespace std;
using namespace tree_generator_by_ouuan;
typedef long long ll;
const int N=5e4+5;
bool vis[205][205];
/*
第一行包含两个正整数 $N,M$，表示了村庄的数目与公路的数量。

第二行包含 $N$ 个非负整数 $t_0,t_1,\cdots,t_{N-1}$，表示了每个村庄重建完成的时间，数据保证了 $t_0 \le t_1 \le \cdots \le t_{N-1}$。

接下来 $M$ 行，每行 $3$ 个非负整数 $i,j,w$，$w$ 为不超过 $10000$ 的正整数，表示了有一条连接村庄 $i$ 与村庄 $j$ 的道路，长度为 $w$，保证 $i\neq j$，且对于任意一对村庄只会存在一条道路。

接下来一行也就是 $M+3$ 行包含一个正整数 $Q$，表示 $Q$ 个询问。

接下来 $Q$ 行，每行 $3$ 个非负整数 $x,y,t$，询问在第 $t$ 天，从村庄 $x$ 到村庄 $y$ 的最短路径长度为多少，数据保证了 $t$ 是不下降的。
*/
int t[N];
int main(int argc, char *argv[]){
    registerGen(argc, argv, 1);
    /*****************************/
    int k=opt<int>(1);
    int Ln,Rn,Lq,Rq;
    //根据测试点是第几组决定随机数范围
    if(k>=1 && k<=3){
		Ln=1,Rn=50;
		Lq=1,Rq=100;
	}else if(k<=5){
		Ln=1,Rn=50;
		Lq=1,Rq=100;
	}else if(k<=7){
		Ln=51,Rn=100;
		Lq=101,Rq=50000;
	}else if(k<=9){
		Ln=100,Rn=200;
		Lq=101,Rq=50000;
	}else{
		Ln=200,Rn=200;
		Lq=50000,Rq=50000;
	}
	int n=rnd.next(Ln,Rn);
	int m=rnd.next(0,n*(n-1)/2);
	int q=rnd.next(Lq,Rq);
	
	printf("%d %d\n",n,m);//给出村庄的数量和边的数量
	for(int i=1;i<=n;i++){
		t[i]=rnd.next(1,100000);
		if(k>=6&&k<=7) t[i]=0;//20%的ti=0
	}
	sort(t+1,t+n+1);//村庄重建完成时间不下降
	for(int i=1;i<=n;i++){
		printf("%d ",t[i]);
	}
	printf("\n");
	
	//给定m条边 ，不能有重边
	for(int i=1;i<=m;i++){
		int x=rnd.next(0,n-1),y=rnd.next(0,n-1);
		while(x==y||vis[x][y]){//x!=y 且 不能有重边
			x=rnd.next(0,n-1);
			y=rnd.next(0,n-1);
		}
		vis[x][y]=vis[y][x]=1;
		int w=rnd.next(1,100000);//随机边权值
		printf("%d %d %d\n",x,y,w);
	}
	//q次询问
	printf("%d\n",q);
	for(int i=1;i<=q;i++){
		t[i]=rnd.next(1,100000);
	}
	sort(t+1,t+1+q);
	for(int i=1;i<=q;i++){
		int x=rnd.next(0,n-1);
		int y=rnd.next(0,n-1);
		printf("%d %d %d\n",x,y,t[i]);
	}
	
	return 0;
}
