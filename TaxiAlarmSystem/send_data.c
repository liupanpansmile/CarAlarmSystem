#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>    // for socket
#include <sys/socket.h>    // for socket
#include <stdio.h>        // for printf
#include <stdlib.h>        // for exit
#include <string.h>        // for bzero
#include <netinet/in.h>
#include <arpa/inet.h>

#include "send_data.h"
#include "log.h"

#define BUFFER_SIZE                     4096
#define FILE_NAME_MAX_SIZE				512

#define SEND_OK                         1
#define SEND_ERROR                      0
#define SEND_FINISH					   -3

#define     CALL_POLICE_RET             3
#define 	SAFE						1
#define 	DANGER						0

#pragma pack(8)

/*********************************************************************************************************
** Function name: send_to_server
** Descriptions: ����Ϣ���͵���������
** Input: send_data�������Ϣ�Ľṹ��ָ��
		  server_ip_address	��������IP
** Output : ����0��ʾ������ɷ��ء�
** Created by: lpp
** Created Date: 2014-5-27
********************************************************************************************************/

int send_to_server(AllInfo* send_data,char *server_ip_address,int port)
{
   
    int send_flag;
    int recv_safe_flag =-1;
 
    // ����һ��socket��ַ�ṹclient_addr, ����ͻ�����internet��ַ�Ͷ˿�
    struct sockaddr_in client_addr;

    bzero(&client_addr, sizeof(client_addr));
    client_addr.sin_family = AF_INET;				 // internetЭ����
    client_addr.sin_addr.s_addr = htons(INADDR_ANY); // INADDR_ANY��ʾ�Զ���ȡ������ַ
    client_addr.sin_port = htons(0);				 // auto allocated, ��ϵͳ�Զ�����һ�����ж˿�

    // ��������internet����Э��(TCP)����socket����client_socket����ͻ���socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
		log_error("Create Socket Failed!\n");
        exit(1);
    }
    // �ѿͻ��˵�socket�Ϳͻ��˵�socket��ַ�ṹ��
    if (bind(client_socket, (struct sockaddr*)&client_addr, sizeof(client_addr)))
    {
        log_error("Client Bind Port Failed!\n");
        exit(1);
    }

    // ����һ��socket��ַ�ṹserver_addr,�����������internet��ַ�Ͷ˿�
    struct sockaddr_in  server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;

    // ��������IP��ַ���Գ���Ĳ���
    if (inet_aton(server_ip_address, &server_addr.sin_addr) == 0)
    {
        log_error("Server IP Address Error!\n");
        exit(1);
    }
    server_addr.sin_port = htons(port);
    socklen_t server_addr_length = sizeof(server_addr);
    
	// ����������������������ӳɹ���client_socket����ͻ��˺ͷ������˵�һ��socket����
    if (connect(client_socket, (struct sockaddr*)&server_addr, server_addr_length) < 0)
    {
        log_error("Can Not Connect To %s!\n", server_ip_address);
        exit(1);
    }

    /*
	������Ϣ������˼�룺
	1.���Ƚ��ṹ�����Ϣ���͵���������
	2.���ȷ��͵����ݴ�С����
	3.����ָ����С������
	4.���շ������˵ķ����źţ��ж������Ƿ���ȷ�Ľ��գ����û����ȷ�Ľ��գ����ط�
	*/

	//	���ṹ�����Ϣ���͵���������

	send(client_socket,send_data,sizeof(AllInfo),0);
	
    char buffer[BUFFER_SIZE];
    bzero(buffer, sizeof(buffer));    
    FILE *fp = fopen(send_data->filename, "r");
    if (fp == NULL)
    {
        log_error("File:\t%s Can Not Open To read!\n", send_data->filename);
        exit(1);
    }
    int file_block_length = 0;
    int send_len = 0 ;
    log_debug("���ڷ������ݣ����Ժ�... ") ;
    while( (file_block_length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
    {
        //printf("client is trans size_file_block!  %d\n ",file_block_length);
        send_len = send(client_socket,&file_block_length,sizeof(int ),0);        //�Ƚ����ݵĴ�С���͹�ȥ���������Դ˴�С����
      //  printf("want to ���͵�len = %d \n",send_len) ;
        if ( (send_len =send(client_socket, buffer, BUFFER_SIZE, 0) ) < 0)
        {
            log_error("Send File:\t%s Failed!\n", send_data->filename);
            break;
        }
      //  printf("ʵ�ʷ��͵�len = %d \n",send_len) ;
        
        recv(client_socket,&send_flag,sizeof(int),0);  //  ���ܵ��ӿͻ��˷�������������
        //printf("recv flag = %d \n",send_flag) ;
        while(send_flag == SEND_ERROR)  //��û����ȫ���ܳɹ�ʱ�򣬽���ǰ���ݼ������͸��ͻ���
        {
        	log_debug("���ݶ�ʧ \n");
            send(client_socket,&file_block_length,sizeof(int ),0);
			send(client_socket, buffer, file_block_length, 0);
            recv(client_socket,&send_flag,sizeof(int),0);
        }
        bzero(buffer, sizeof(buffer));
    }
   
   
    fclose(fp);

   
   // receive processing results from server 
    while(1)
    {
    	send_flag = recv(client_socket,&recv_safe_flag,sizeof(int),0) ;
        //printf("send_flag = %d \n",send_flag) ;
    	if(send_flag == sizeof(int))
    	{
    		send(client_socket,&send_flag,sizeof(send_flag),0) ;
    		log_debug("send ok !");
    		break ;
    	}
    	else
    	{
    		send_flag = -1 ;
    		send(client_socket,&send_flag,sizeof(send_flag),0) ;
    		log_debug("send error \n") ;
        }
    }
    log_debug("���ݷ������... \n") ;
   
    close(client_socket) ;	
    return recv_safe_flag ;
}

