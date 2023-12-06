# testlibOI

## 介绍

本项目源自于[Testlib](https://github.com/MikeMirzayanov/testlib)。结合实际工作中的需求进行了一些本土化处理。

Testlib 是一套历史悠久的出题辅助工具库。它可以用于书写数据生成器（Generator），数据校验器（Validator），答案检查器（checker），交互器（interactor）。

著名编程竞赛平台 Codeforces 是 Testlib 的最大使用者。在 Codeforces 上出题，上面提到的四种程序都需要使用 Testlib 完成。

## 快速入门

1. 在`gen.cpp`文件中书写数据生成器
2. 在`std.cpp`文件中放入标程
3. 编译运行 `run.cpp`文件
4. 在`data`文件夹中找到生成的数据与备份的`gen.cpp`、`std.cpp`、`run.cpp`文件

## 数据生成器使用说明

使用Testlib写数据生成器的一个重要特点就是，如果参数保持一致，在不同环境(Windows、Linux)下数据生成器的结果都是一样的。

在Testlib中我们使用的随机数工具为`rnd`对象与`shuffle()`函数。

下面是一个`rnd`对象的成员函数列表:

| 函数                | 描述                                                         |
| ------------------- | ------------------------------------------------------------ |
| rnd.next()          | 生成一个 $[0,1)$之间的浮点数。                               |
| rnd.next(R)         | 如果传入的参数是整数，生成一个 $[0,R]$之间的整数。如果传入的参数是浮点数。生成一个 $[0,R)$之间的浮点数。 |
| rnd.next(L,R)       | 如果传入的参数是整数，生成一个 $[L,R]$之间的整数。如果传入的参数是浮点数。生成一个 $[L,R)$之间的浮点数。 |
| rnd.any(c)          | 返回容器 c 内的一个随机元素。对 vector 和 string 都可以使用。 |
| rnd.wnext(i,t)      | 不均匀随机数生成器（具体定义比较长，下面会给出）。`rnd.wnext(4,2)` 相当于 `max(rnd.next(4),rnd.next(4))`；`rnd.wnext(4,-2) 相当于 min(rnd.next(4),rnd.next(4))`；`rnd.wnext(4,0) `相当于 `rnd.next(4)`。 |
| rnd.next("a\|b\|c") | 等概率返回 `a`，`b`，`c` 这三个字符串中的一个。              |

