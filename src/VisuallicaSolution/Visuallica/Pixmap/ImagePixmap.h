#ifndef IMAGEPIXMAP_H
#define IMAGEPIXMAP_H

#include "BPixmap.h"

class QImage;


class ImagePixmap : public BPixmap
{
public:
	ImagePixmap();
	~ImagePixmap();


	virtual bool isDcmGrayImage();

	virtual void SetOriginalImage(QImage *);

	virtual QImage* GetOriginalImage();
	virtual void Scale(double xFactor, double yFactor);
	virtual void Rotate(int degree);
	virtual QImage* GetTransformedImage();
	virtual void ResetTransform();
protected:
	

private:

	QImage * m_pOriginalImage;
	QImage * m_pTransformedImage;


};


#endif // IMAGEPIXMAP_H