#include "DcmPixmap.h"
#include <QImage>
#include "dcmtk/dcmimgle/dcmimage.h"
#include <QDate>
#include <QTime>
#include <QLocale>
#include "BDcm.h"


DcmPixmap::DcmPixmap()
{
	m_pOriginalDicomImage = NULL;

	m_pTransformedDicomImage = NULL;

	m_iDcmWidth = 0;
	m_iDcmHeight = 0;
}

DcmPixmap::~DcmPixmap()
{
	if (m_pOriginalDicomImage)
	{
		delete m_pOriginalDicomImage;
		m_pOriginalDicomImage = NULL;
	}

	if (m_pTransformedDicomImage)
	{
		delete m_pTransformedDicomImage;
		m_pTransformedDicomImage = NULL;
	}
}

bool DcmPixmap::isDcmGrayImage()
{
	if (m_pOriginalDicomImage)
	{
		return m_pOriginalDicomImage->isMonochrome();
	}
	else{
		return false;
	}

	return false;

}

void DcmPixmap::SetOriginalImage(QImage *paImage)
{
	
}

QImage* DcmPixmap::GetOriginalImage()
{
	return NULL;

}

void DcmPixmap::Scale(double xFactor, double yFactor)
{
	int isHorizontalFlip, isVerticalFlip;
	if (xFactor == -1 && yFactor == 1) // vertical, DCMTK는 Qt와 vertical의 의미가 반대이다.
	{
		isVerticalFlip = 0;
		isHorizontalFlip = 1;
	}
	else
	{
		isVerticalFlip = 1;
		isHorizontalFlip = 0;
	}

	BDcm::GetInstance().Flip(m_pTransformedDicomImage, isHorizontalFlip, isVerticalFlip);
}

void DcmPixmap::Rotate(int degree)
{
	BDcm::GetInstance().Rotate(m_pTransformedDicomImage, degree);
}

QImage* DcmPixmap::SetWindow(double windowCenter, double windowWidth)
{
	return BDcm::GetInstance().SetWindow(m_pTransformedDicomImage, windowCenter, windowWidth);
}

QImage* DcmPixmap::GetTransformedImage()
{
	return BDcm::GetInstance().ConvertFromDcm(m_pTransformedDicomImage);
}

void DcmPixmap::ResetTransform()
{
	double windowCenter, windowWidth;

	BDcm::GetInstance().GetWindow(m_pTransformedDicomImage, windowCenter, windowWidth);

	if (m_pTransformedDicomImage)
	{
		delete m_pTransformedDicomImage;
		m_pTransformedDicomImage = NULL;
	}

	m_pTransformedDicomImage = BDcm::GetInstance().Copy(m_pOriginalDicomImage);
	BDcm::GetInstance().SetWindow(m_pTransformedDicomImage, windowCenter, windowWidth);
	
}


void DcmPixmap::SetOriginalDcmImage(DicomImage* paDicomImage)
{
	if (m_pOriginalDicomImage)
	{
		delete m_pOriginalDicomImage;
		m_pOriginalDicomImage = NULL;
	}

	m_pOriginalDicomImage = paDicomImage;


	if (m_pTransformedDicomImage)
	{
		delete m_pTransformedDicomImage;
		m_pTransformedDicomImage = NULL;
	}

	m_pTransformedDicomImage = BDcm::GetInstance().Copy(m_pOriginalDicomImage);

}

DicomImage* DcmPixmap::GetOriginalDcmImage()
{
	return m_pOriginalDicomImage;
}

void DcmPixmap::GetWindowValue(double &windowCenter, double &windowWidth)
{
	if (m_pTransformedDicomImage)
		BDcm::GetInstance().GetWindow(m_pTransformedDicomImage, windowCenter, windowWidth);

}

void DcmPixmap::SetDcmSize(int width, int height)
{
	m_iDcmWidth = width;
	m_iDcmHeight = height;
}

void DcmPixmap::GetDcmSize(int& width, int& height)
{
	width = m_iDcmWidth;
	height = m_iDcmHeight;
}

QString DcmPixmap::GetPatientName()
{
	return m_sPatientName;
}
void DcmPixmap::SetPatientName(QString aString)
{
	m_sPatientName = aString;
}

QString DcmPixmap::GetPatientID()
{
	return m_sPatientId;
}
void DcmPixmap::SetPatientID(QString aString)
{
	m_sPatientId = aString;
}

QString DcmPixmap::GetPatientSex()
{
	return m_sPatientSex;
}
void DcmPixmap::SetPatientSex(QString aString)
{
	m_sPatientSex = aString;

}

QString DcmPixmap::GetPatientBirthDate()
{
	return m_sPatientBirthdate;

}
void DcmPixmap::SetPatientBirthDate(QString aString)
{
	QDate birthdate = QDate::fromString(aString, "yyyyMMdd");

	m_sPatientBirthdate = QLocale::system().toString(birthdate, QLocale::ShortFormat); 
}

QString DcmPixmap::GetModality()
{
	return m_sModality;
}
void DcmPixmap::SetModality(QString aString)
{
	m_sModality = aString;
}

QString DcmPixmap::GetInstitutionName()
{
	return m_sInstitutionName;
}
void DcmPixmap::SetInstitutionName(QString aString)
{
	m_sInstitutionName = aString;
}

QString DcmPixmap::GetRefferingPhysicianName()
{
	return m_sRefferingPhysicianName;
}
void DcmPixmap::SetRefferingPhysicianName(QString aString)
{
	m_sRefferingPhysicianName = aString;
}

QString DcmPixmap::GetStudyDate()
{
	return m_sStudyDate;
}
void DcmPixmap::SetStudyDate(QString aString)
{
	QDate studydate = QDate::fromString(aString, "yyyyMMdd");

	m_sStudyDate = QLocale::system().toString(studydate, QLocale::ShortFormat);
}

QString DcmPixmap::GetStudyTime()
{
	return m_sStudyTime;
}
void DcmPixmap::SetStudyTime(QString aString)
{
	QTime time = QTime::fromString(aString, "hhmmss");
	m_sStudyTime = time.toString("hh:mm:ss");
}
QString DcmPixmap::GetSeriesDescription()
{
	return m_sSeriesDescription;
}
void DcmPixmap::SetSeriesDescription(QString aString)
{
	m_sSeriesDescription = aString;
}



