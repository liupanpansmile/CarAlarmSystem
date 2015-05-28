/**********************************************************************
*
*
* 文 件 名:  v4l.c
*
* 内容描述:  视频监控系统源文件
*
*
*/

#include "v4l.h"
#include "log.h"

/*********************************************************************
* 函数名:v4l_open
* 功  能: 打开设备
* 输  入: dev
*                 vd
* 输  出: 无
* 返  回:  -1----失败
*              0----成功
*/
int v4l_open(const char *dev, v4l_device *vd )
{
	if( !dev )
	{
		dev=DEFAULT_DEVICE ;
	}

	if( ( vd->fd = open( dev, O_RDWR ) )  < 0 )
	{
		log_error( "v4l_open error" );
		return -1;
	}
	vd->picture.palette=VIDEO_PALETTE_RGB565;
	log_debug( "open v4l ok \n" );
	return 0;
}

/*********************************************************************
* 函数名: v4l_get_capability
* 功  能: 获取设备属性
* 输  入: vd
* 输  出: 无
* 返  回:  -1----失败
*              0----成功
*/
int v4l_get_capability( v4l_device *vd )
{
	if( ioctl( vd->fd, VIDIOCGCAP, &( vd->capability ) ) <0 )
	{
		log_error( "v4l_get_capability failed ");
		return -1 ;
	}
	/*
	log_debug( "\nname=%s\n,maxwidth=%d, maxheight=%d\nminwidth=%d, minheight=%d\nchannels=%d, type=%d,audios=%d\n",
	vd->capability.name,
	vd->capability.maxwidth,vd->capability.maxheight,vd->capability.minwidth,
	vd->capability.minheight,vd->capability.channels, vd->capability.type,
	vd->capability.audios );
	*/
	return 0; 
}

/*********************************************************************
* 函数名:v4l_get_picture
* 功  能:获取图片属性
* 输  入: vd
* 输  出: 无
* 返  回:  -1----失败
*                   0----成功
*/
int v4l_get_picture( v4l_device *vd )
{
	if( ioctl( vd->fd,VIDIOCGPICT,&( vd->picture ) ) < 0 )
	{
		log_error("get picture properties failed") ;
		return -1;
	} 
	/*
	log_debug("brightness=%d,hue=%d \ncolour=%d,contrast=%d \nwhiteness=%d,depth=%d,palette=%d\n" , 
	vd->picture.brightness,vd->picture.hue, 
	vd->picture.colour,vd->picture.contrast,vd->picture.whiteness,
	vd->picture.depth,vd->picture.palette ) ;
	*/
	return 0;
}

/*********************************************************************
* 函数名:v4l_set_picture
* 功  能:设置图片属性
* 输  入: vd
* 输  出: 无
* 返  回:  -1----失败
*              0----成功
*/
int v4l_set_picture( v4l_device *vd ) 
{
	if( ioctl( vd->fd, VIDIOCSPICT, &( vd->picture ) ) < 0 )
	{
		log_error("set picture properties failed") ;
		return -1;
	}
	return 0;
}

/*********************************************************************
* 函数名:v4l_get_channels
* 功  能:获取通道信息
* 输  入: vd
* 输  出: 无
* 返  回:  -1----失败
*             0----成功
*/
int v4l_get_channels( v4l_device *vd )
{
	int i;
	for( i=0;i < vd->capability.channels ; i++ )
	{
		vd->channel[i].channel = i;               //确定通道

		if( ioctl( vd->fd , VIDIOCGCHAN, &( vd->channel[i] ) ) <0 )
		{
			log_debug( "v4l_get_channel failed" );
			return -1;
		}
	}
	return 0;
}

/*********************************************************************
* 函数名:v4l_get_window(v4l_device *vd);
* 功  能:获取window信息
* 输  入: vd
* 输  出: 无
* 返  回:  -1----失败
*            0----成功
*/
int v4l_get_window(v4l_device *vd)
{
	if( ioctl( vd->fd, VIDIOCGWIN, &( vd->window ) ) < 0 )
	{
		log_error("get the video overlay  window failed") ;
		return -1;
	}
	return 0;
}


/*********************************************************************
* 函数名:v4l_get_mbuf
* 功  能:获取内存映射信息
* 输  入: vd
* 输  出: 无
* 返  回:  -1----失败
*              0----成功
*/
int v4l_get_mbuf( v4l_device *vd )
{
	if( ioctl( vd->fd,VIDIOCGMBUF,&( vd->mbuf ) ) <0 )
	{
		log_error( "memory map buffer info failed" );
		return -1;
	}
	if( ( vd->map = ( unsigned char * )mmap( 0, vd->mbuf.size, 
		PROT_READ | PROT_WRITE, MAP_SHARED, vd->fd, 0 ) ) < 0 )
	{
		log_error("v4l_mmap_init:mmap");
		return -1;
	}
	return 0 ;
}

/*********************************************************************
* 函数名: v4l_init_mbuff
* 功  能:初始化内存映射信息
* 输  入: vd
* 输  出: 无
* 返  回:  0----成功
*/
int v4l_init_mbuf(v4l_device *vd)
{
	log_debug("init v4l mbuf") ;
	//vd->mmap.frame = 10 ; 
	vd->mmap.width = MAX_WIDTH;
	vd->mmap.height = MAX_HEIGHT;
	vd->mmap.format = vd->picture.palette;
	vd->frame_current = 0;
	vd->frame_using[0] = 0;
	vd->frame_using[1] = 0;
	return 0;
}

/*********************************************************************
* 函数名: v4l_get_address
*
* 功  能:获取数据在图像的地址
*/
unsigned char *v4l_get_address(v4l_device *vd)
{
	return (vd->map + vd->mbuf.offsets[vd->frame_current]);
}

/*********************************************************************
*
*
* 函数名: v4l_grab_frame
* 功  能:捕获帧
*
*
*/
int v4l_grab_frame(v4l_device *vd, int frame)
{
   log_debug("begin grab_frame\n");
	if (vd->frame_using[frame]) 
	{
	//	fprintf(stderr, "v4l_grab_frame: frame %d is already used.\n", frame);
		log_error("v4l_grab_frame: frame %d is already used.", frame);
		return -1;
	}

	vd->mmap.frame = frame;

	if ( ioctl(vd->fd, VIDIOCMCAPTURE, &(vd->mmap ) ) < 0 ) 
	{
		perror( "grab frame failed" );
		return -1;
	}

	vd->frame_using[frame] = 1;
	vd->frame_current = frame;
	log_debug("end grab frame") ;
	return 0;    
}

/*********************************************************************
* 函数名: v4l_grab_sync
*
* 功  能:与内存映射捕获一致
*/
int v4l_grab_sync(v4l_device *vd)
{
	log_debug("begin  grab_sync");
	if (ioctl(vd->fd, VIDIOCSYNC, &(vd->frame_current)) < 0) 
	{
		perror("v4l_grab_sync");
	}
	vd->frame_using[vd->frame_current] = 0;
	log_debug("end grab_sync\n");
	return 0;
}

/*********************************************************************
* 函数名: v4l_munmap
* 功  能:停止内存映射
*/
int v4l_munmap( v4l_device *vd )
{
	if ( munmap( vd->map, vd->mbuf.size ) < 0 ) 
	{
		log_error( "v4lmunmap:munmap");
		return -1;
	}

	return 0;
}

/*********************************************************************
* 函数名: v4l_close
* 功  能:关闭设备
*/
int v4l_close(v4l_device *vd)
{
	log_debug("close v4l") ;
	close(vd->fd);   
	return 0;
}

/*********************************************************************
* 函数名: v4l_grab_movie
* 功  能:捕获连续图像
*/
void  v4l_grab_movie(v4l_device *vd)
{  
	v4l_grab_frame(vd, vd->frame_current);      /*获取下一 帧*/
	v4l_grab_sync(vd);                                     /*等待传完一 帧*/
	vd->buffer = v4l_get_address(vd);              /*得到这一帧的地址*/
	vd->frame_current = (vd->frame_current+1)%2; /* 下一帧的frame*/

}



