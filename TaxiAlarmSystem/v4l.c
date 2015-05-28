/**********************************************************************
*
*
* �� �� ��:  v4l.c
*
* ��������:  ��Ƶ���ϵͳԴ�ļ�
*
*
*/

#include "v4l.h"
#include "log.h"

/*********************************************************************
* ������:v4l_open
* ��  ��: ���豸
* ��  ��: dev
*                 vd
* ��  ��: ��
* ��  ��:  -1----ʧ��
*              0----�ɹ�
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
* ������: v4l_get_capability
* ��  ��: ��ȡ�豸����
* ��  ��: vd
* ��  ��: ��
* ��  ��:  -1----ʧ��
*              0----�ɹ�
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
* ������:v4l_get_picture
* ��  ��:��ȡͼƬ����
* ��  ��: vd
* ��  ��: ��
* ��  ��:  -1----ʧ��
*                   0----�ɹ�
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
* ������:v4l_set_picture
* ��  ��:����ͼƬ����
* ��  ��: vd
* ��  ��: ��
* ��  ��:  -1----ʧ��
*              0----�ɹ�
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
* ������:v4l_get_channels
* ��  ��:��ȡͨ����Ϣ
* ��  ��: vd
* ��  ��: ��
* ��  ��:  -1----ʧ��
*             0----�ɹ�
*/
int v4l_get_channels( v4l_device *vd )
{
	int i;
	for( i=0;i < vd->capability.channels ; i++ )
	{
		vd->channel[i].channel = i;               //ȷ��ͨ��

		if( ioctl( vd->fd , VIDIOCGCHAN, &( vd->channel[i] ) ) <0 )
		{
			log_debug( "v4l_get_channel failed" );
			return -1;
		}
	}
	return 0;
}

/*********************************************************************
* ������:v4l_get_window(v4l_device *vd);
* ��  ��:��ȡwindow��Ϣ
* ��  ��: vd
* ��  ��: ��
* ��  ��:  -1----ʧ��
*            0----�ɹ�
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
* ������:v4l_get_mbuf
* ��  ��:��ȡ�ڴ�ӳ����Ϣ
* ��  ��: vd
* ��  ��: ��
* ��  ��:  -1----ʧ��
*              0----�ɹ�
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
* ������: v4l_init_mbuff
* ��  ��:��ʼ���ڴ�ӳ����Ϣ
* ��  ��: vd
* ��  ��: ��
* ��  ��:  0----�ɹ�
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
* ������: v4l_get_address
*
* ��  ��:��ȡ������ͼ��ĵ�ַ
*/
unsigned char *v4l_get_address(v4l_device *vd)
{
	return (vd->map + vd->mbuf.offsets[vd->frame_current]);
}

/*********************************************************************
*
*
* ������: v4l_grab_frame
* ��  ��:����֡
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
* ������: v4l_grab_sync
*
* ��  ��:���ڴ�ӳ�䲶��һ��
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
* ������: v4l_munmap
* ��  ��:ֹͣ�ڴ�ӳ��
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
* ������: v4l_close
* ��  ��:�ر��豸
*/
int v4l_close(v4l_device *vd)
{
	log_debug("close v4l") ;
	close(vd->fd);   
	return 0;
}

/*********************************************************************
* ������: v4l_grab_movie
* ��  ��:��������ͼ��
*/
void  v4l_grab_movie(v4l_device *vd)
{  
	v4l_grab_frame(vd, vd->frame_current);      /*��ȡ��һ ֡*/
	v4l_grab_sync(vd);                                     /*�ȴ�����һ ֡*/
	vd->buffer = v4l_get_address(vd);              /*�õ���һ֡�ĵ�ַ*/
	vd->frame_current = (vd->frame_current+1)%2; /* ��һ֡��frame*/

}



