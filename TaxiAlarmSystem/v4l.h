/**********************************************************************
*
*
* �� �� ��:  v4l.h
*
* ��������:  ��Ƶ���ϵͳͷ�ļ�
*
*
*/

#ifndef _V4L_H
#define _V4L_H

#ifdef  _cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <unistd.h>
#include <error.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <linux/videodev.h>
#include <sys/types.h>
#include <string.h>



	/*�ɼ���ͼ�����󳤺Ϳ�*/
#define MAX_WIDTH 320
#define MAX_HEIGHT 240

	/*�豸�ļ�*/
#define DEFAULT_DEVICE "/dev/video0"


	/*�Զ������ݽṹ,����v4l ���õ������ݽṹ*/
	typedef struct v4l_struct
	{
		int fd;/*�豸��*/
		struct video_capability capability;/*�����豸�Ļ�����Ϣ(�豸���ơ�֧�ֵ������С�ֱ��ʡ��ź�Դ��Ϣ��)*/
		struct video_channel channel[8];/*���ڸ����ź�Դ������*/
		struct video_picture picture;/*�豸�ɼ���ͼ��ĸ�������*/
		struct video_window  window; /*��������capture area����Ϣ*/
		struct video_mmap mmap;   /*����mmap*/        
		struct video_mbuf mbuf;  /*����mmap����ӳ���֡����Ϣ*/
		unsigned char *buffer ;/*ͼ�����ݴ����*/
		unsigned char *map;/*mmap��ʽ��ȡ����ʱ,���ݵ��׵�ַ*/
		int frame_current;
		int frame_using[2]; /*���֡��״̬0 ��ʾ����,1��ʾ������*/
	}v4l_device;

	/*��������*/
	int v4l_open(const char *dev, v4l_device *vd);/*���豸*/

	int v4l_close(v4l_device *vd);/*�ر��豸*/

	int v4l_get_capability(v4l_device *vd);/*��video_capability ����Ϣ*/

	int v4l_get_picture(v4l_device *vd);

	int v4l_get_channels(v4l_device *vd);

	int v4l_get_window(v4l_device *vd);

	int v4l_get_mbuf(v4l_device *vd);

	unsigned char *v4l_get_address(v4l_device *vd);

	int v4l_init_mbuf(v4l_device *vd);

	int v4l_munmap(v4l_device *vd) ;

	int v4l_grab_frame(v4l_device *vd, int frame);

	int v4l_grab_sync(v4l_device *vd) ;

	void v4l_grab_movie(v4l_device *vd);

	int v4l_set_picture(v4l_device *vd) ;


#ifdef  _cplusplus
}
#endif


#endif

