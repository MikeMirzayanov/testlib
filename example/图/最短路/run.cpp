#include <bits/stdc++.h>
using namespace std;
int main(){
    string run1,run2,num;
    stringstream tmp;
    system("mkdir data");
    system("g++ -o gen.exe gen.cpp");
    system("g++ -o std.exe std.cpp");
    for(int i=1;i<=20;i++){
        tmp.clear();
        tmp<<i;
        tmp>>num;
        // 拼接生成第i个输入数据与对应的输出数据的指令
        #if _WIN32 //windowns平台
            run1="gen.exe ";
            run2="std.exe <data/";
        #elif __linux__ //linux平台
            run1="./gen.exe ";
            run2="./std.exe <data/";
        #endif // __linux__
        run1=run1+num+" >data/"+num+".in";//生成输入文件
        run2=run2+num+".in >data/"+num+".out";//生成输出文件
        system(run1.c_str());//执行生成.in文件指令
        system(run2.c_str());//执行生成.out文件指令
    }

    system("zip -jr data.zip data");//将输入输出数据打包为 data.zip压缩包

    string mvop="mv",cpop="cp",rmop="rm";
    #ifdef _WIN32//定义windows中的移动、复制、删除命令
    mvop="move";
    cpop="cp";
    rmop="del";
    #endif // _WIN32

    string op5=rmop+" *.exe *.o run";
    system(op5.c_str());//


	return 0;
}
