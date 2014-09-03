

/*-------------------------------------------------

	文件说明：
				自定义的dcm文件类，派生自dcmtk开源库的底层
				dcm类：DcmFileFormat。
				采用了适配器（Adapter）设计	模式，将CxImage类库
				常用图像格式（如 jpg/bmp/png/tiff 等）的编码接口
				融合到DcmFileFormat类中，形成新的类。

	Author：
				zssure
	Date：	
				2013-04-11
  -------------------------------------------------*/

#ifndef _TDCMFILEFORMAT_H
#define _TDCMFILEFORMAT_H

//包含基础类DcmFileFormat的头文件，因为继承时编译器需要知道父类的大小
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"//主要负责图像的显示，如调整窗宽窗位
//包含系统文件


namespace THU_STD_NAMESPACE{




//define the image buffer type
enum THUIMAGEBUFFERTYPE{
	THU_GRAY_DIB=8,THU_RGB_DIB=24
};
enum THUIMAGESTREAMTYPE{
	THU_JPEG,THU_BMP
};


class TDcmFileFormat:public DcmFileFormat
{
public:
	TDcmFileFormat();
	TDcmFileFormat(DcmFileFormat DcmFile);
	TDcmFileFormat(DcmFileFormat DcmFile,THUIMAGEBUFFERTYPE ImageBufferType);
	TDcmFileFormat(const char* pDcmFileName,THUIMAGEBUFFERTYPE ImageBufferType=THU_GRAY_DIB);
	TDcmFileFormat(const TDcmFileFormat& OtherDcmFileFormat);
	TDcmFileFormat& operator =(const TDcmFileFormat& OtherDcmFileFormat);
	virtual ~TDcmFileFormat();
	
	//适配CxImage开源库的接口
public:

	/*--------------------------------------
	  Func:saveToDcm
	  Purpose: 
				save the result of the operation on dcm file
	  Paras:
				mDcmFileName,the dcm file name
	  --------------------------------------*/
	void saveToDcm(const char* pDcmFileName);
	/*--------------------------------------
	  Func:saveToBmp
	  Purpose: 
				change the dcm file to bmp format
	  Paras:
				mBmpFileName:the name of the bmp file
				bits:the bits of one pixel stored in memory
					 default value is 8 bits,i.e. the buffer is a pallete bmp image;
					 if bits=24,the buffer is a RGB bmp image.
	  --------------------------------------*/

	void saveToBmp(const char* pBmpFileName);
	/*--------------------------------------
	  Func:saveToJpg
	  Purpose: 
				change the dcm file to jpg format
	  Paras:
				mJpgFileName:the name of the jpg file
				bits:the bits of one pixel stored in memory
					 default value is 8 bits,i.e. the buffer is a pallete bmp image;
					 if bits=24,the buffer is a RGB bmp image.
	  --------------------------------------*/

	void saveToJpg(const char* pJpgFileName);
	/*--------------------------------------
	  Func:saveToJpgBySize
	  Purpose: 
				change the dcm file to jpg format
	  Paras:
				mJpgFileName:the name of the jpg file
				
				bits:the bits of one pixel stored in memory
					 default value is 8 bits,i.e. the buffer is a pallete bmp image;
					 if bits=24,the buffer is a RGB bmp image.

				 ImageWidth:	the width of the image
				 ImageHeight:	the height of the image

	  --------------------------------------*/
	void saveToJpgBySize(const char* pJpgFileName,long ImageWidth,long ImageHeight);
	/*--------------------------------------
	  Func:saveToBmpBySize
	  Purpose: 
				change the dcm file to jpg format
	  Paras:
				mJpgFileName:the name of the jpg file
				
				bits:the bits of one pixel stored in memory
					 default value is 8 bits,i.e. the buffer is a pallete bmp image;
					 if bits=24,the buffer is a RGB bmp image.

				ImageWidth:		the width of the image
				ImageHeight:	the height of the image
	  --------------------------------------*/
	void saveToBmpBySize(const char* pBmpFileName,long ImageWidth,long ImageHeight);
	

	
public://dcm图像基本信息接口

	inline unsigned short int getWidth(){
		//字段（0028,0010）Rows的VR为US：16位无符号整数
		unsigned short DcmRows;
		this->getDataset()->findAndGetUint16(DCM_Columns,DcmRows);
		return DcmRows;
	};
	inline unsigned short int getHeight(){
		//字段（0028.0011)Columns的VR为US：16位无符号整数
		unsigned short DcmColumns;
		this->getDataset()->findAndGetUint16(DCM_Rows,DcmColumns);
		return DcmColumns;
	};
	inline unsigned short int getBitsAllocated(){
		//字段（0028,0100）BitsAllocated的VR为US：16位无符号整数
		unsigned short DcmBitsAllocated;
		this->getDataset()->findAndGetUint16(DCM_BitsAllocated,DcmBitsAllocated);
		return DcmBitsAllocated;

	};
	inline unsigned short int getBitsStored(){
		unsigned short DcmBitsStored;
		this->getDataset()->findAndGetUint16(DCM_BitsStored,DcmBitsStored);
		return DcmBitsStored;
	};
	inline unsigned short int getHighBit(){
		unsigned short DcmHighBit;
		this->getDataset()->findAndGetUint16(DCM_HighBit,DcmHighBit);
		return DcmHighBit;
	};
	inline double getWindowWidth(){
		double DcmWindowWidth;
		this->getDataset()->findAndGetFloat64(DCM_WindowWidth,DcmWindowWidth);
		return DcmWindowWidth;

	};
	inline double getWindowCenter(){
		double DcmWindowCenter;
		this->getDataset()->findAndGetFloat64(DCM_WindowCenter,DcmWindowCenter);
		return DcmWindowCenter;
	};
	inline double getRescaleSlope(){
		double DcmRescaleSlope;
		this->getDataset()->findAndGetFloat64(DCM_RescaleSlope,DcmRescaleSlope);
		return DcmRescaleSlope;

	};
	inline double getRescaleIntercept(){
		double DcmRescaleIntercept;
		this->getDataset()->findAndGetFloat64(DCM_RescaleIntercept,DcmRescaleIntercept);
		return DcmRescaleIntercept;

	};
	void* getPixelData();//获得真实像素数据指针

	/*--------------------------------------
	  Func:getImageStream
	  Purpose: 
				convert the pixel data in the dcm file into JPEG or BMP type, 
				and save as a BYTE stream
	  Paras:
				imageStream:the BYTE stream of the pixel data in the dcm file,
							it must be initialized to NULL
				size:the size of image stream,the starting value is zero
				type:the type of the image stream
	  return:
				void
	  --------------------------------------*/

	void getImageStream(BYTE** pImageStream,long& StreamSize,THUIMAGESTREAMTYPE Type=THU_JPEG);
	/*--------------------------------------
	  Func:getImageStreamBySize
	  Purpose: 
				convert the pixel data in the dcm file into JPEG or BMP type, then resample the image
				in order to change its size to ImageWidth*ImageHeight,and save as a BYTE stream
	  Paras:
				imageStream:the BYTE stream of the pixel data in the dcm file,
							it must be initialized to NULL
				size:the size of image stream,the starting value is zero
				type:the type of the image stream
	  return:
				void
	  --------------------------------------*/
	void getImageStreamBySize(BYTE** pImageStream,long& StreamSize,long ImageWidth,long ImageHeight,THUIMAGESTREAMTYPE Type=THU_JPEG);

public://图像处理函数
	
	void setWindow() ;//使用默认的窗宽、窗位
	void setWindow(const double WindowCenter,const double WindowWidth);//自定义窗宽，窗位

protected:
	/*--------------------------------------
	  Func:checkCompressed
	  Purpose: 
				check the transfer syntax of the dcm file compressed or not
	  return:
				0,the dcm file's transfersyntax is uncompressed
				1,the dcm file's transfersyntax is compressed
	  --------------------------------------*/

	bool checkCompressed();//检测传输语义，看图像是否是压缩图像


	//在对dcm原始数据进行修改后，需要调用FlushImagebuffer()来将dcm中的图像信息加载到图像缓冲区中，为之后的图像格式转换做准备。
	/*--------------------------------------
	  Func:FlushImageBuffer
	  Purpose:
			refresh the image buffer,mImageBuffer, store the result of the last operation
	  Paras:
			bits:the bits of one pixel stored in memory
				 default value is 8 bits,i.e. the buffer is a pallete bmp image;
				 if bits=24,the buffer is a RGB bmp image.
	 Tips:
			All of constructors of the TDcmFileFormat don't call the FlushImageBuffer() function expilictly,so
			the image buffer,mImageBuffer,is null.
			Whenever you want to save your operation on the dcm file to JPG/BMP file, you should call the 
			FlushImageBuffer() function to refresh the image buffer.
	  --------------------------------------*/
	void FlushImageBuffer();//刷新图像缓冲区，当对图像进行处理后，需要刷新图像缓冲区,
	/*--------------------------------------
	  Func:createBmpFileHeader
	  Purpose:
			create the file header of teh image buffer,mImageBuffer
	  Paras:
			bits:the bits of one pixel stored in memory
				 default value is 8 bits,i.e. the buffer is a pallete bmp image;
				 if bits=24,the buffer is a RGB bmp image.
  	  Return:
			the file header of the image buffer,mImageBuffer
	  --------------------------------------*/

	BITMAPFILEHEADER* createBmpFileHeader();
	/*--------------------------------------
	  Func:createBmpInfoHeader
	  Purpose:
			create the information header of the image buffer,mImageBuffer
	  Paras:
			bits:the bits of one pixel stored in memory
				 default value is 8 bits,i.e. the buffer is a pallete bmp image;
				 if bits=24,the buffer is a RGB bmp image.
	  Return:
			the information header of the image buffer,mImageBuffer
	  --------------------------------------*/
	BITMAPINFOHEADER* createBmpInfoHeader();
	/*--------------------------------------
	  Func:createBmpPalette
	  Purpose:
			create the palette of the image buffer,mImageBuffer
	  Return:
			the image buffer's palette
	  --------------------------------------*/
	RGBQUAD* createBmpPalette();


private:

	void* m_pImageBuffer;//the image buffer,storing the pixel data of dcm file
	THUIMAGEBUFFERTYPE m_ImageBufferType;

};
};

#endif

/*
*	2013-04-11 the first version has been complete
*	
*	2013-04-17 make the TDcmFileFormat class in the namespace THU_STD_NAMESPACE,
*			   in oder to avoid class confusion
*
*   2013-04-26 add the image stream function:getImageStream();
*			   add the enum variable THUIMAGESTREAMTYPE
*
*	2013-04-30 check the naming rules by the file: <<Google Code Rules>>
*
*   2013-05-21 add the new function getImageStreamBySize()
*			   in order to resize the image
*
*	2013-05-22 add new functions: saveToJpgBySize, and saveToBmpBySize
*			   in order to resize the image
*
*	2013-06-18 修改生成bmp文件头的函数createBmpFileHeader，将Bits==8修改为Bits==24
*
*	2013-06-20 修改FlushImageBuffer函数，添加PhotometricInterpretation字段的判别
*			   能够对RGB格式的图像（例如超声图像）进行处理，将原本简单利用DicomImage类
*			   的createWindowsDIB函数来生成BMP数据流进行扩充，能够手动生成RGB模式的数据流
*
*
*
*/