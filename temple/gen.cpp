#include "testlib.h"
#include "treegenerator.h"
#include <bits/stdc++.h>
using namespace std;
using namespace tree_generator_by_ouuan;
typedef long long ll;
const int N=5e4+5;
bool vis[205][205];
/*
输入包括多组数据。每组数据第一行是两个整数N、M（N<=100，M<=10000），N表示成都的大街上有几个路口，标号为1的路口是商店所在地，标号为N的路口是赛场所在地，M则表示在成都有几条路。N=M=0表示输入结束。接下来M行，每行包括3个整数A，B，C（1<=A,B<=N,1<=C<=1000）,表示在路口A与路口B之间有一条路，我们的工作人员需要C分钟的时间走过这条路。
输入保证至少存在1条商店到赛场的路线。
*/
int t[N];
int main(int argc, char *argv[]){
    registerGen(argc, argv, 1);
    /*****************************/
    int k=opt<int>(1);
    int Ln,Rn;
	if(k>=1&&k<=5){
		Ln=2,Rn=10;
	}else if(k<=8){
		Ln=11,Rn=100;
	}else{
		Ln=100,Rn=100;
	}
	int T=rnd.next(1,100);//数据组数
	for(int i=1;i<=T;i++){
		int n=rnd.next(Ln,Rn);//随机点的数量
		int m=rnd.next(n*(n-1)/2,10000);//随机边的数量
		if(k>=9) m=10000;
		printf("%d %d\n",n,m);
		for(int j=1;j<=m;j++){
			int a=rnd.next(1,n);
			int b=rnd.next(1,n);
			int c=rnd.next(1,1000);
			printf("%d %d %d\n",a,b,c);
		}
	}
	printf("0 0");
	
	return 0;
}
