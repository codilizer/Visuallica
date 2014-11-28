#ifndef BDCM_H
#define BDCM_H

#include "QString.h"

class QPixmap;
class DcmPixmap;
class DicomImage;
class QImage;
class DcmFileFormat;

class BDcm
{
public:
	static BDcm& GetInstance()
	{
		static BDcm instance;
		return instance;
	}

public:
	bool Load(QString filePath);
	bool Load(QPixmap*, QString filePath);
	bool Load(DcmPixmap*, QString filePath);

	//bool DcmToImage(DicomImage* paSrcDcm, QImage *paTargetImage);
	// ������ ó�� ����(���ʷε��� ���)
	QImage * ImageFromDcm(DicomImage* paSrcDcmImage, DcmFileFormat* pDff);

	// ������ ó�� ������
	QImage * ConvertFromDcm(DicomImage* paSrcDcmImage);
	bool ImageToDCM(QImage* paSrcImage, DicomImage *paTargetDcm);

	bool GetWindow(DicomImage* paImg, double &center, double &width);
	QImage* SetWindow(DicomImage* paImg, double center, double width);

	DicomImage* Copy(DicomImage*);
	int Flip(DicomImage* paSrcDcmImage, int isHorizontal, int isVertical);
	bool Rotate(DicomImage* paSrcDcmImage, int degree);

private:
	BDcm();
	~BDcm();
};

#endif