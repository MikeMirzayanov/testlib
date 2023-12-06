#include <bits/stdc++.h>
using namespace std;
int main(){
    string run1,run2,num;
    stringstream tmp;
    system("mkdir data");
    system("g++ -o gen.exe gen.cpp");
    system("g++ -o std.exe std.cpp");
    for(int i=1;i<=10;i++){
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
    string op1=mvop+" data.zip data";
    string op2=cpop+" gen.cpp data";
    string op3=cpop+" std.cpp data";
    string op4=cpop+" run.cpp data";
    string op5=rmop+" *.exe *.o run";

    system(op1.c_str());//将data.zip 移动至 data 文件夹
    system(op2.c_str());//复制 gen.cpp 文件至 data 文件夹
    system(op3.c_str());//复制 std.cpp 文件至 data 文件夹
    system(op4.c_str());//复制 run.cpp 文件至 data 文件夹
    system(op5.c_str());//复制 run.cpp 文件至 data 文件夹


	return 0;
}
