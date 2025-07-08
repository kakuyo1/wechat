#include <iostream>
#include <boost/asio.hpp>
#include "CServer.h"
#include "ConfigIniManager.h"
#include <hiredis/hiredis.h>
#include <string>
#include <cstring>
#include <cstdio>
#include <cassert>
#include "RedisManager.h"

// void TestRedis() {
// 	//连接redis 需要启动才可以进行连接
// //redis默认监听端口为6379 可以再配置文件中修改
// 	redisContext* c = redisConnect("127.0.0.1", 6379);
// 	if (c->err)
// 	{
// 		printf("Connect to redisServer failed:%s\n", c->errstr);
// 		redisFree(c);
//         return;
// 	}
// 	printf("Connect to redisServer Success\n");

// 	std::string redis_password = "17379922448Ab";
// 	redisReply* r = (redisReply*)redisCommand(c, "AUTH %s", redis_password.c_str());
// 	if (r->type == REDIS_REPLY_ERROR) {
// 		printf("Redis认证失败!%s\n", r->str);
//         freeReplyObject(r);
//         redisFree(c);
//         return;
// 	}else {
// 		printf("Redis认证成功!\n");
// 	}

// 	//为redis设置key
// 	const char* command1 = "set stest1 value1";

// 	//执行redis命令行
//     r = (redisReply*)redisCommand(c, command1);

// 	//如果返回NULL则说明执行失败
// 	if (NULL == r)
// 	{
// 		printf("Execut command1 failure\n");
// 		redisFree(c);
//         return;
// 	}

// 	//如果执行失败则释放连接
// 	if (!(r->type == REDIS_REPLY_STATUS && (strcmp(r->str, "OK") == 0 || strcmp(r->str, "ok") == 0)))
// 	{
// 		printf("Failed to execute command[%s]\n", command1);
// 		freeReplyObject(r);
// 		redisFree(c);
//         return;
// 	}

// 	//执行成功 释放redisCommand执行后返回的redisReply所占用的内存
// 	freeReplyObject(r);
// 	printf("Succeed to execute command[%s]\n", command1);

// 	const char* command2 = "strlen stest1";
// 	r = (redisReply*)redisCommand(c, command2);

// 	//如果返回类型不是整形 则释放连接
// 	if (r->type != REDIS_REPLY_INTEGER)
// 	{
// 		printf("Failed to execute command[%s]\n", command2);
// 		freeReplyObject(r);
// 		redisFree(c);
//         return;
// 	}

// 	//获取字符串长度
// 	int length = r->integer;
// 	freeReplyObject(r);
// 	printf("The length of 'stest1' is %d.\n", length);
// 	printf("Succeed to execute command[%s]\n", command2);

// 	//获取redis键值对信息
// 	const char* command3 = "get stest1";
// 	r = (redisReply*)redisCommand(c, command3);
// 	if (r->type != REDIS_REPLY_STRING)
// 	{
// 		printf("Failed to execute command[%s]\n", command3);
// 		freeReplyObject(r);
// 		redisFree(c);
//         return;
// 	}
// 	printf("The value of 'stest1' is %s\n", r->str);
// 	freeReplyObject(r);
// 	printf("Succeed to execute command[%s]\n", command3);

// 	const char* command4 = "get stest2";
// 	r = (redisReply*)redisCommand(c, command4);
// 	if (r->type != REDIS_REPLY_NIL)
// 	{
// 		printf("Failed to execute command[%s]\n", command4);
// 		freeReplyObject(r);
// 		redisFree(c);
//         return;
// 	}
// 	freeReplyObject(r);
// 	printf("Succeed to execute command[%s]\n", command4);

// 	//释放连接资源
// 	redisFree(c);
// }

// void TestRedisMgr() {
//     assert(RedisManager::GetInstance()->Set("blogwebsite","llfc.club"));
//     std::string value="";
//     assert(RedisManager::GetInstance()->Get("blogwebsite", value) );
//     std::cout << value << std::endl;
//     assert(RedisManager::GetInstance()->Get("nonekey", value) == false);
//     assert(RedisManager::GetInstance()->HSet("bloginfo","blogwebsite", "llfc.club"));
//     assert(RedisManager::GetInstance()->HGet("bloginfo","blogwebsite") != "");
//     assert(RedisManager::GetInstance()->Exists("bloginfo"));
//     assert(RedisManager::GetInstance()->Delete("bloginfo"));
//     assert(RedisManager::GetInstance()->Delete("bloginfo") == false);
//     assert(RedisManager::GetInstance()->Exists("bloginfo") == false);
//     assert(RedisManager::GetInstance()->LPush("lpushkey1", "lpushvalue1"));
//     assert(RedisManager::GetInstance()->LPush("lpushkey1", "lpushvalue2"));
//     assert(RedisManager::GetInstance()->LPush("lpushkey1", "lpushvalue3"));
//     assert(RedisManager::GetInstance()->RPop("lpushkey1", value));
//     std::cout << "RPop lpushkey1: " << value << std::endl;
//     assert(RedisManager::GetInstance()->RPop("lpushkey1", value));
//     std::cout << "RPop lpushkey1: " << value << std::endl;
//     assert(RedisManager::GetInstance()->LPop("lpushkey1", value));
//     std::cout << "LPop lpushkey1: " << value << std::endl;
//     assert(RedisManager::GetInstance()->LPop("lpushkey2", value)==false);
//     RedisManager::GetInstance()->Close();
// }

int main() {
    try{
        // Load configuration
        auto& config_manager = ConfigIniManager::Instance();
        std::string port_str = config_manager["GateServer"]["Port"]; //调用的是const std::string& operator[](const std::string& key) const;
        unsigned short port = static_cast<unsigned short>(std::atoi(port_str.c_str()));

        boost::asio::io_context ioc; //only for acceptor

        // handle signals to stop the server gracefully
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc](const boost::system::error_code& ec, int signo) {
            if (ec) {
                std::cerr << "Error handling signal: " << ec.message() << std::endl;
                return;
            }
            std::cout << "Stopping GateServer..." << std::endl;
            ioc.stop(); // Stop the io_context to exit the event loop
        });

        // Create and start the server
        auto server = std::make_shared<CServer>(ioc, port);
        server->Start();
        std::cout << "GateServer started on port " << port << std::endl;

        ioc.run(); // Run the io_context to start processing events
    } catch (const std::exception& e) {
        std::cerr << "Exception in main: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}