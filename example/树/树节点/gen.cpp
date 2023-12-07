#include "testlib.h"
#include "treegenerator.h"
//#include <bits/stdc++.h>
/*
## 输入格式
第1行为一个整数n,m,对应节点的总数与查询的次数。
第$2\sim n$ 行，每行两个整数x,y，代表x与y相连。
后m行，每行一个整数，对应查询的节点编号。
*/
using namespace std;
using namespace tree_generator_by_ouuan;
typedef long long ll;
int main(int argc, char *argv[]){
        registerGen(argc, argv, 1);
        /*****************************/
        int k=opt<int>(1);
        int L,R;
        //根据测试点是第几组决定随机数范围
        if(k>=1&&k<=5){
            L=5;R=1e4;
        }else if(k<=8){
            L=1e4;R=1e6;
        }else{
            L=1e6;R=1e6;
        }
        int n=rnd.next(L,R);//随机节点的总数
        int x=rnd.next(2,5);//随机树的分支上限
        int m=rnd.next(L,R);//随机查询的次数
        Tree t;
        t.complete(n,x,0);////生成n+1个节点的x-1叉随机树 ，1为根
        cout<<n+1<<" "<<m<<endl;
        cout<<t;
        for(int i=1;i<=m;i++){
            cout<<rnd.next(1,n)<<endl;//随机查询的节点编号
        }
	return 0;
}
