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
        perror( "v4l_open error" );
        return -1;
    }
    vd->picture.palette=VIDEO_PALETTE_RGB565;
     // printf("v4l_open... video_mbuf  size=%u,\n", vd->mbuf.size);

    #ifdef DEBUG_PRINT
    printf( "open v4l ok \n" );
    #endif  
    
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
        perror( "v4l_get_capability" );
        return -1 ;
    }
/*
    #ifdef DEBUG_PRINT
    printf( "\nname=%s\n,maxwidth=%d, maxheight=%d\nminwidth=%d, minheight=%d\nchannels=%d, type=%d,audios=%d\n",
              vd->capability.name,
              vd->capability.maxwidth,vd->capability.maxheight,vd->capability.minwidth,
              vd->capability.minheight,vd->capability.channels, vd->capability.type,
              vd->capability.audios );
    #endif
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
         return -1;
    } 

    /*
    #ifdef DEBUG_PRINT
    printf("brightness=%d,hue=%d \ncolour=%d,contrast=%d \nwhiteness=%d,depth=%d,palette=%d\n" , 
        vd->picture.brightness,vd->picture.hue, 
        vd->picture.colour,vd->picture.contrast,vd->picture.whiteness,
        vd->picture.depth,vd->picture.palette ) ;
    #endif
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
              perror( "v4l_get_channel" );
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
         perror( "get_mbuf:" );
         return -1;
    }
    
    if ( ( vd->map = ( unsigned char * )mmap( 0, vd->mbuf.size, 
    	PROT_READ | PROT_WRITE, MAP_SHARED, vd->fd, 0 ) ) < 0 )
   {
       perror("v4l_mmap_init:mmap");
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
    //vd->mmap.frame = 10 ; 
    vd->mmap.width = MAX_WIDTH;
    vd->mmap.height = MAX_HEIGHT;
    vd->mmap.format = vd->picture.palette;
    vd->frame_current = 0;
    vd->frame_using[0] = 0;
    vd->frame_using[1] = 0;

    #ifdef DEBUG_PRINT
    printf( "\nbegin to init device:" );
    #endif
    
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

#ifdef DEBUG_PRINT
  //   printf("enter grab_frame\n");
#endif
     if (vd->frame_using[frame]) 
     {
         fprintf(stderr, "v4l_grab_frame: frame %d is already used.\n", frame);
         return -1;
     }

     vd->mmap.frame = frame;
     
     if ( ioctl(vd->fd, VIDIOCMCAPTURE, &(vd->mmap ) ) < 0 ) 
     {
            perror( "v4l_grab_frame" );
            return -1;
     }
     
      vd->frame_using[frame] = 1;
      vd->frame_current = frame;

      return 0;    
}

/*********************************************************************
* ������: v4l_grab_sync
*
* ��  ��:���ڴ�ӳ�䲶��һ��
*/
int v4l_grab_sync(v4l_device *vd)
{
    // printf("enter grab_sync\n");
     if (ioctl(vd->fd, VIDIOCSYNC, &(vd->frame_current)) < 0) 
     {
         perror("v4l_grab_sync");
      }

      vd->frame_using[vd->frame_current] = 0;
    //  printf("leave grab_sync\n");
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
        perror( "v4lmunmap:munmap" );
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
     close(vd->fd);
     
     return 0;
}

/*********************************************************************
* ������: v4l_grab_movie
* ��  ��:��������ͼ��
*/
void  v4l_grab_movie(v4l_device *vd)
 {
    #ifdef DEBUG_PRINT
   {
      //     printf("video_mbuf include size=%u,frame=%d,offdet=%d\n", \
      //         vd->mbuf.size, vd->mbuf.frames, vd->mbuf.offsets[1]);
          // printf("Hello \n") ;
   }
     #endif
     
     v4l_grab_frame(vd, vd->frame_current);      /*��ȡ��һ ֡*/
     v4l_grab_sync(vd);                                     /*�ȴ�����һ ֡*/
     vd->buffer = v4l_get_address(vd);              /*�õ���һ֡�ĵ�ַ*/
     vd->frame_current = (vd->frame_current+1)%2; /* ��һ֡��frame*/
    
 }



