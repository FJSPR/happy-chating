# happy-chating
Unix网络编程下的网络聊天室学习项目
受《linux高性能服务器编程》9.6节的启发，准备自己实现一个聊天室，逐步丰富功能。
***
***简介***

这个版本是[NingbinWang](https://github.com/NingbinWang/ChatRoom) 的删减版和9.6的组合，以后会逐渐完善。
王宁斌的聊天室有太多的数据库操作，对于主要精力为网络编程的同学来说，太多的数据库操作会分散精力；
所以我删除了一大半关于数据库的操作，使聊天室逻辑简洁一些。
聊天室的博文在[基于LINUX的多功能聊天室](https://www.cnblogs.com/samuelwnb/p/4265519.html) 思路还是很清晰的

***环境配置***

本聊天室用到了sqlite，所以需要安装：

1. sudo apt-get install sqlite3  //安装sqlite3
2. sudo apt-get install libsqlite3-dev //安装对应的api

以上两项都安装后才可以使用

server文件夹中是服务器端代码，client文件夹中是客户端代码

***使用方法：***

    服务器：1、make
	      2、./server
	客户端：1、make
		   2、./client 127.0.0.1 8000
