#include "testlib.h"
#include "treegenerator.h"
//#include <bits/stdc++.h>
using namespace std;
using namespace tree_generator_by_ouuan;
typedef long long ll;
const int N=2e5+5;
int x[N];
int s[N];
int main(int argc, char *argv[]){
	registerGen(argc, argv, 1);
    /*****************************/
	int k=opt<int>(1);
	int L=1,R=1000;
	if(k>=1&&k<=5){
		L=1;R=10;
	}else{
		L=10;R=100;
	}
    int n=rnd.next(L,R);
    int x=rnd.next(2,10);
    Tree t;

    t.complete(n,x,0);////生成n个节点的x-1叉随机树 ，1为根
    cout<<n<<" "<<x-1<<endl;
    cout<<t;


	return 0;
}
