
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "rgb2bmp.h"
#include "log.h"

/****************************
RGB加上头部信息转换成BMP
参数說明：
rgb_buffer        :RGB数据文件中的信息
nData             :RGB数据的长度
nWidth            :图像宽度的像素数
nHeight           :图像高度的像素数
fp1               :所存放的文件
*****************************/
int RGB2BMP(char *rgb_buffer,int nWidth,int nHeight,FILE*fp1)
{
    log_debug("RGB to BMP... ") ;

	BmpHead m_BMPHeader;     
	InfoHead  m_BMPInfoHeader ;  

	//设置文件头信息
	char bfType[2]={'B','M'};
	m_BMPHeader.imageSize=3*nWidth*nHeight+54;
	m_BMPHeader.blank=0;
	m_BMPHeader.startPosition=54;

	//将文件头信息往保存图片中写
	fwrite(bfType,1,sizeof(bfType),fp1);
	fwrite(&m_BMPHeader.imageSize,1,sizeof(m_BMPHeader.imageSize),fp1);
	fwrite(&m_BMPHeader.blank,1,sizeof(m_BMPHeader.blank),fp1);
	fwrite(&m_BMPHeader.startPosition,1,sizeof(m_BMPHeader.startPosition),fp1) ;


	//设置位图信息头
	m_BMPInfoHeader.Length=40;		//设置位图信息头的大小，sizeof(InfoHead)
	m_BMPInfoHeader.width=nWidth;	
	m_BMPInfoHeader.height=nHeight;
	m_BMPInfoHeader.colorPlane=1;	//固定合适
	m_BMPInfoHeader.bitColor=24;	// 1(黑白二色图)、4(16色图)、8(256色图)、24(真彩色图)。
	m_BMPInfoHeader.zipFormat=0;	// 0 BI_RGB表示不压缩。
	m_BMPInfoHeader.realSize=3*nWidth*nHeight;
	m_BMPInfoHeader.xPels=0;
	m_BMPInfoHeader.yPels=0;
	m_BMPInfoHeader.colorUse=0;	//固定合适，可参考结构体定义
	m_BMPInfoHeader.colorImportant=0;

	// write bmp header infomation to file
	fwrite(&m_BMPInfoHeader.Length,1,sizeof(m_BMPInfoHeader.Length),fp1);
	fwrite(&m_BMPInfoHeader.width,1,sizeof(m_BMPInfoHeader.width),fp1);
	fwrite(&m_BMPInfoHeader.height,1,sizeof(m_BMPInfoHeader.height),fp1);
	fwrite(&m_BMPInfoHeader.colorPlane,1,sizeof(m_BMPInfoHeader.colorPlane),fp1);
	fwrite(&m_BMPInfoHeader.bitColor,1,sizeof(m_BMPInfoHeader.bitColor),fp1);
	fwrite(&m_BMPInfoHeader.zipFormat,1,sizeof(m_BMPInfoHeader.zipFormat),fp1);
	fwrite(&m_BMPInfoHeader.realSize,1,sizeof(m_BMPInfoHeader.realSize),fp1);
	fwrite(&m_BMPInfoHeader.xPels,1,sizeof(m_BMPInfoHeader.xPels),fp1);
	fwrite(&m_BMPInfoHeader.yPels,1,sizeof(m_BMPInfoHeader.yPels),fp1);
	fwrite(&m_BMPInfoHeader.colorUse,1,sizeof(m_BMPInfoHeader.colorUse),fp1);
	fwrite(&m_BMPInfoHeader.colorImportant,1,sizeof(m_BMPInfoHeader.colorImportant),fp1);
	fwrite(rgb_buffer,3*nWidth*nHeight,1,fp1);		//write picture data to file

	return 0;
}

/************************************************************
** Function name:  write_data
** Descriptions:保存BMP图片
** Input: 	*filename 保存图片的名称
**             *rgb_buffer 内存中图片的信息
**              width   图片的宽度
**              height  图片的高度
** Output : 无
** Created by: lpp
** Created Date: 2014-5-16
************************************************************/
int  write_data(char *filename,unsigned char *rgb_buffer,int width,int height)
{   
    log_debug("begin to save %s picture \n",filename) ;

	FILE* fp;
	long nData = width*height;
	long total = nData*3;	// total data capacity

	long i =0;

	unsigned char* pVisit = (unsigned char*) malloc(total*sizeof(char));  //allocate memory,the size is total data capacity
	unsigned char* tmp = pVisit;
	unsigned char *init_address = pVisit ;		  //save the primary the allocated memory address ,for later free

	if(pVisit == NULL)
	{
		printf("malloc failed \n") ;
		exit(0) ;
	}
  //  printf("malloc success \n") ;

	if((fp =fopen(filename,"wb")) == NULL) 
	{
		printf("cannot open %s \n",filename) ;
		return -1 ;
	}

 //   printf("open file %s success \n",filename) ;
 //   printf("rgb_buffer = %p \n",rgb_buffer) ;
    while(i<total)
	{
        *pVisit = *rgb_buffer ;
       rgb_buffer++;
        pVisit++ ;
        //equal to -->     *pVisit++ = *rgb_buffer++ ;
		i++;
	}

    RGB2BMP((char *)tmp,width,height,fp);
	if(NULL != init_address )
	{
		free(init_address );
		init_address  = NULL ;
	}
	fclose(fp);
 //   printf("delete end \n") ;

	return 0;
}

