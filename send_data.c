#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>    // for socket
#include <sys/socket.h>    // for socket
#include <stdio.h>        // for printf
#include <stdlib.h>        // for exit
#include <string.h>        // for bzero
#include <netinet/in.h>
#include <arpa/inet.h>

#include "send_data.h"

#define SERVER_PORT    				  10001
#define BUFFER_SIZE                      4096
#define FILE_NAME_MAX_SIZE         512

#define SEND_OK                       1
#define SEND_ERROR                 0
#define SEND_FINISH					 -3

#define     CALL_POLICE_RET     3
#define 	SAFE  1
#define 	DANGER 0

#pragma pack(8)

/*********************************************************************************************************
** Function name: send_to_server
** Descriptions: 将信息发送到服务器端
** Input: send_data，存放信息的结构体指针
		  server_ip_address	服务器的IP
** Output : 返回0表示正常完成返回。
** Created by: lpp
** Created Date: 2014-5-27
********************************************************************************************************/

int send_to_server(AllInfo* send_data,char *server_ip_address)
{
   
    int send_flag;
    int recv_safe_flag =-1;
 
    // 设置一个socket地址结构client_addr, 代表客户机的internet地址和端口
    struct sockaddr_in client_addr;

    bzero(&client_addr, sizeof(client_addr));
    client_addr.sin_family = AF_INET;				 // internet协议族
    client_addr.sin_addr.s_addr = htons(INADDR_ANY); // INADDR_ANY表示自动获取本机地址
    client_addr.sin_port = htons(0);				 // auto allocated, 让系统自动分配一个空闲端口

    // 创建用于internet的流协议(TCP)类型socket，用client_socket代表客户端socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        printf("Create Socket Failed!\n");
        exit(1);
    }
    // 把客户端的socket和客户端的socket地址结构绑定
    if (bind(client_socket, (struct sockaddr*)&client_addr, sizeof(client_addr)))
    {
        printf("Client Bind Port Failed!\n");
        exit(1);
    }

    // 设置一个socket地址结构server_addr,代表服务器的internet地址和端口
    struct sockaddr_in  server_addr;
    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    // 服务器的IP地址来自程序的参数
    if (inet_aton(server_ip_address, &server_addr.sin_addr) == 0)
    {
        printf("Server IP Address Error!\n");
        exit(1);
    }
    server_addr.sin_port = htons(SERVER_PORT);
    socklen_t server_addr_length = sizeof(server_addr);
    
	// 向服务器发起连接请求，连接成功后client_socket代表客户端和服务器端的一个socket连接
    if (connect(client_socket, (struct sockaddr*)&server_addr, server_addr_length) < 0)
    {
        printf("Can Not Connect To %s!\n", server_ip_address);
        exit(1);
    }

    /*
	发送信息的总体思想：
	1.首先将结构体的信息发送到服务器端
	2.将先发送的数据大小发送
	3.发送指定大小的数据
	4.接收服务器端的反馈信号，判断数据是否正确的接收，如果没有正确的接收，则重发
	*/

	//	将结构体的信息发送到服务器端

    //printf("sizeof(AllInfo) = %d\n",sizeof(AllInfo)) ;
	send(client_socket,send_data,sizeof(AllInfo),0);
	
   
    char buffer[BUFFER_SIZE];
    bzero(buffer, sizeof(buffer));    
    FILE *fp = fopen(send_data->filename, "r");
    if (fp == NULL)
    {
        printf("File:\t%s Can Not Open To read!\n", send_data->filename);
        exit(1);
    }
    int file_block_length = 0;
    int send_len = 0 ;
    printf("正在发送数据，请稍候... \n") ;
    while( (file_block_length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
    {
 
        //printf("client is trans size_file_block!  %d\n ",file_block_length);
		
        send_len = send(client_socket,&file_block_length,sizeof(int ),0);        //先将数据的大小发送过去，服务器以此大小接收
      //  printf("want to 发送的len = %d \n",send_len) ;
        if ( (send_len =send(client_socket, buffer, BUFFER_SIZE, 0) ) < 0)
        {
            printf("Send File:\t%s Failed!\n", send_data->filename);
            break;
        }
      //  printf("实际发送的len = %d \n",send_len) ;
        
        recv(client_socket,&send_flag,sizeof(int),0);  //  接受到从客户端反馈回来得数据
        //printf("recv flag = %d \n",send_flag) ;
        while(send_flag == SEND_ERROR)  //当没有完全接受成功时候，将先前数据继续发送给客户端
        {
        	printf("数据丢失 \n");
            send(client_socket,&file_block_length,sizeof(int ),0);
			send(client_socket, buffer, file_block_length, 0);
            recv(client_socket,&send_flag,sizeof(int),0);
        }
        bzero(buffer, sizeof(buffer));
    }
   
   
    fclose(fp);

   // printf("File:\t%s Transfer Finished!\n", send_data->filename);
    
    while(1)
    {
    	send_flag = recv(client_socket,&recv_safe_flag,sizeof(int),0) ;
        //printf("send_flag = %d \n",send_flag) ;
    	if(send_flag == sizeof(int))
    	{
    		send(client_socket,&send_flag,sizeof(send_flag),0) ;
    		printf("send ok !\n");
    		break ;
    	}
    	else
    	{
    		send_flag = -1 ;
    		send(client_socket,&send_flag,sizeof(send_flag),0) ;
    		printf("send error \n") ;
        }
    }
    printf("数据发送完毕... \n") ;
   // printf("recv_safe_flag = %d\n",recv_safe_flag) ;
   // printf("send_flag = %d \n",send_flag) ;
    close(client_socket) ;	//
    return recv_safe_flag ;
}

