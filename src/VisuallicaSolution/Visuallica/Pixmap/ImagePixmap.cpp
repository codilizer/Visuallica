#include "ImagePixmap.h"
#include <QImage>


ImagePixmap::ImagePixmap()
{
	m_pOriginalImage = NULL;
	m_pTransformedImage = NULL;

}

ImagePixmap::~ImagePixmap()
{
	if (m_pOriginalImage)
	{
		delete m_pOriginalImage;
		m_pOriginalImage = NULL;
	}

	if (m_pTransformedImage)
	{
		delete m_pTransformedImage;
		m_pTransformedImage = NULL;
	}

}

bool ImagePixmap::isDcmGrayImage()
{
	return false;
}

void ImagePixmap::SetOriginalImage(QImage * paImage)
{
	if (m_pOriginalImage)
	{
		delete m_pOriginalImage;
		m_pOriginalImage = NULL;
	}

	m_pOriginalImage = paImage;

	if (m_pTransformedImage)
	{
		delete m_pTransformedImage;
		m_pTransformedImage = NULL;
	}

	m_pTransformedImage = new QImage(m_pOriginalImage->copy(m_pOriginalImage->rect()));
}

QImage* ImagePixmap::GetOriginalImage()
{
	return m_pOriginalImage;
}

void ImagePixmap::Scale(double xFactor, double yFactor)
{
	QMatrix matrix;
	matrix.scale(xFactor, yFactor);
	QImage tempImage = m_pTransformedImage->transformed(matrix);

	if (m_pTransformedImage){
		delete m_pTransformedImage;
		m_pTransformedImage = NULL;
	}

	m_pTransformedImage = new QImage(tempImage);
}

void ImagePixmap::Rotate(int degree)
{
	QMatrix matrix;
	matrix.rotate(degree);
	QImage tempImage = m_pTransformedImage->transformed(matrix);

	if (m_pTransformedImage){
		delete m_pTransformedImage;
		m_pTransformedImage = NULL;
	}

	m_pTransformedImage = new QImage(tempImage);
}

QImage* ImagePixmap::GetTransformedImage()
{
	return m_pTransformedImage;
}

void ImagePixmap::ResetTransform()
{
	if (m_pTransformedImage)
	{
		delete m_pTransformedImage;
		m_pTransformedImage = NULL;
	}

	m_pTransformedImage = new QImage(m_pOriginalImage->copy(m_pOriginalImage->rect()));

}