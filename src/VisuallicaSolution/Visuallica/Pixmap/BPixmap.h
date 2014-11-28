#ifndef BPIXMAP_H
#define BPIXMAP_H

#include <QPixmap>

class QImage;
class DicomImage;

class BPixmap : public QPixmap
{
public:
	BPixmap();
	~BPixmap();

	virtual bool isDcmGrayImage() = 0;  // Pure Virtual Method

	virtual void SetOriginalImage(QImage *) = 0;

	virtual QImage* GetOriginalImage() = 0;
	virtual void Scale(double xFactor, double yFactor) = 0;
	virtual void Rotate(int degree) = 0;
	virtual QImage* GetTransformedImage() = 0;
	virtual void ResetTransform() = 0;

private:
	//DicomImage * m_pDicomImage;
	//QImage * m_pImage;
};


#endif // BPIXMAP_H