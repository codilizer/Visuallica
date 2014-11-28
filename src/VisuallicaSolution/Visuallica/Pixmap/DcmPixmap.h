#ifndef DCMPIXMAP_H
#define DCMPIXMAP_H

#include "BPixmap.h"

class QImage;
class DicomImage;


class DcmPixmap : public BPixmap
{
public:
	DcmPixmap::DcmPixmap();
	DcmPixmap::~DcmPixmap();

	virtual bool isDcmGrayImage();


	virtual void SetOriginalImage(QImage *);
	virtual QImage* GetOriginalImage();

	virtual void Scale(double xFactor, double yFactor);
	virtual void Rotate(int degree);
	QImage* SetWindow(double center, double width);


	virtual QImage* GetTransformedImage();
	virtual void ResetTransform();



	void SetOriginalDcmImage(DicomImage*);
	DicomImage* GetOriginalDcmImage();

	void GetWindowValue(double &windowCenter, double &windowWidth);
	

	void SetDcmSize(int width, int height);
	void GetDcmSize(int& width, int& height);

	QString GetPatientName();
	void SetPatientName(QString);

	QString GetPatientID();
	void SetPatientID(QString);

	QString GetPatientSex();
	void SetPatientSex(QString);

	QString GetPatientBirthDate();
	void SetPatientBirthDate(QString);

	QString GetModality();
	void SetModality(QString);

	QString GetInstitutionName();
	void SetInstitutionName(QString);

	QString GetRefferingPhysicianName();
	void SetRefferingPhysicianName(QString);

	QString GetStudyDate();
	void SetStudyDate(QString);

	QString GetStudyTime();
	void SetStudyTime(QString);

	QString GetSeriesDescription();
	void SetSeriesDescription(QString);

private:
	DicomImage * m_pOriginalDicomImage;	// 원본만 있으면 충분.
	DicomImage * m_pTransformedDicomImage;
	//QImage * m_pOriginalImage;
	//QImage * m_pImage;	// 하나만 있으면 충분.
	

	int m_iDcmWidth;
	int m_iDcmHeight;

	// dcm tag
	QString m_sPatientName;
	QString m_sPatientId;
	QString m_sPatientSex;
	QString m_sPatientBirthdate;
	QString m_sModality;
	QString m_sInstitutionName;
	QString m_sRefferingPhysicianName;
	QString m_sStudyDate;
	QString m_sStudyTime;
	QString m_sSeriesDescription;

};


#endif // DCMPIXMAP_H
