/**********************************************************************
*
*
* 文 件 名:  v4l.h
*
* 内容描述:  视频监控系统头文件
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



	/*采集的图像的最大长和宽*/
#define MAX_WIDTH 320
#define MAX_HEIGHT 240

	/*设备文件*/
#define DEFAULT_DEVICE "/dev/video0"


	/*自定义数据结构,包含v4l 中用到的数据结构*/
	typedef struct v4l_struct
	{
		int fd;/*设备号*/
		struct video_capability capability;/*包含设备的基本信息(设备名称、支持的最大最小分辨率、信号源信息等)*/
		struct video_channel channel[8];/*关于各个信号源的属性*/
		struct video_picture picture;/*设备采集的图象的各种属性*/
		struct video_window  window; /*包含关于capture area的信息*/
		struct video_mmap mmap;   /*用于mmap*/        
		struct video_mbuf mbuf;  /*利用mmap进行映射的帧的信息*/
		unsigned char *buffer ;/*图像数据存放区*/
		unsigned char *map;/*mmap方式获取数据时,数据的首地址*/
		int frame_current;
		int frame_using[2]; /*这个帧的状态0 表示可用,1表示不可用*/
	}v4l_device;

	/*函数声明*/
	int v4l_open(const char *dev, v4l_device *vd);/*打开设备*/

	int v4l_close(v4l_device *vd);/*关闭设备*/

	int v4l_get_capability(v4l_device *vd);/*读video_capability 中信息*/

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

