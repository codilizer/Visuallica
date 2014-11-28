#include "BDcm.h"
#include <QImage>
#include <QPixmap>
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmjpeg/djdecode.h"
#include "dcmtk/dcmdata/dcrledrg.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include <QDebug>
#include "DcmPixmap.h"

BDcm::BDcm()
{

}

BDcm::~BDcm()
{

}
bool BDcm::Load(QString filePath)
{

	DJDecoderRegistration::registerCodecs();
	DcmRLEDecoderRegistration::registerCodecs();

	//QPixmap *pixmap = NULL;

	DcmFileFormat dfile;// = new DcmFileFormat();


	//OFCondition cond = dfile->loadFile(filePath.toAscii().data(), EXS_Unknown, EGL_withoutGL, DCM_MaxReadLength, ERM_autoDetect);

	//OFCondition cond = dfile.loadFile(filePath.toStdString().data(), EXS_Unknown, EGL_withoutGL, DCM_MaxReadLength, ERM_autoDetect);
	// ERM_autoDetect로 하면 특정 파일에서 멎어버리는 현상(loadFile함수가 리턴하지 않음)때문에 ERM_fileOnly로 변경하였음
	OFCondition cond = dfile.loadFile(filePath.toStdString().data(), EXS_Unknown, EGL_withoutGL, DCM_MaxReadLength, ERM_fileOnly);
	if (cond.bad() || cond.status() != OF_ok)
	{
		qDebug() << "BDcm::Load(), condition error! ";

		//if (dfile) {
		//	dfile->clear();
		//}
		DJDecoderRegistration::cleanup();
		DcmRLEDecoderRegistration::cleanup();

		return false;

	}

	E_TransferSyntax xfer = dfile.getDataset()->getOriginalXfer();
	DicomImage *pDCMImage = new DicomImage(&dfile, xfer);

	QImage *pImage = ImageFromDcm(pDCMImage, &dfile);

	//pImage->save("c:\\test2.bmp"); // now can show on QLable or save

	int bRetunResult = false;

	QPixmap pixmap;

	if (pImage)
	{
		if (pDCMImage->isMonochrome())
			bRetunResult = pixmap.convertFromImage(*pImage, Qt::MonoOnly);
		else
		{
			bRetunResult = pixmap.convertFromImage(*pImage);
		}
	}
	else{
		bRetunResult = false;
	}

	if (pImage) {
		delete pImage;
		pImage = NULL;
	}

	if (pDCMImage) {
		delete pDCMImage;
		pDCMImage = NULL;
	}

	DJDecoderRegistration::cleanup();
	DcmRLEDecoderRegistration::cleanup();

	return bRetunResult;
}



// called from ThumbnailWorker
bool BDcm::Load(QPixmap *paPixmap, QString filePath)
{

	DJDecoderRegistration::registerCodecs();
	DcmRLEDecoderRegistration::registerCodecs();

	DcmFileFormat dfile;// = new DcmFileFormat();


	//OFCondition cond = dfile->loadFile(filePath.toAscii().data(), EXS_Unknown, EGL_withoutGL, DCM_MaxReadLength, ERM_autoDetect);

	//OFCondition cond = dfile.loadFile(filePath.toStdString().data(), EXS_Unknown, EGL_withoutGL, DCM_MaxReadLength, ERM_autoDetect);
	// ERM_autoDetect로 하면 특정 파일에서 멎어버리는 현상(loadFile함수가 리턴하지 않음)때문에 ERM_fileOnly로 변경하였음
	// ERM_fileOnly로 하면 특정 DCM파일을 못 읽어 들이는 현상 때문에 ERM_autoDetect로 다시 변경하였음
	//OFCondition cond = dfile.loadFile(filePath.toStdString().data(), EXS_Unknown, EGL_withoutGL, DCM_MaxReadLength, ERM_fileOnly);
	OFCondition cond = dfile.loadFile(filePath.toStdString().data(), EXS_Unknown, EGL_withoutGL, DCM_MaxReadLength, ERM_autoDetect);
	if (cond.bad() || cond.status() != OF_ok)
	{
		qDebug() << "BDcm::Load(), condition error! ";

		//if (dfile) {
		//	dfile->clear();
		//}
		DJDecoderRegistration::cleanup();
		DcmRLEDecoderRegistration::cleanup();

		return false;

	}

	E_TransferSyntax xfer = dfile.getDataset()->getOriginalXfer();
	DicomImage *pDCMImage = new DicomImage(&dfile, xfer);
	
	QImage *pImage = ImageFromDcm(pDCMImage, &dfile);

	//pImage->save("c:\\test2.bmp"); // now can show on QLable or save

	int bRetunResult = false;

	if (pImage)
	{
		if (pDCMImage->isMonochrome())
			bRetunResult = paPixmap->convertFromImage(*pImage, Qt::MonoOnly);
		else
		{
			bRetunResult = paPixmap->convertFromImage(*pImage);
		}
	}
	else{
		bRetunResult = false;
	}

	if (pImage) {
		delete pImage;
		pImage = NULL;
	}

	DJDecoderRegistration::cleanup();
	DcmRLEDecoderRegistration::cleanup();

	QImage* windowImage;

	windowImage = SetWindow(pDCMImage, 3000, 1000);


	if (pDCMImage) {
		delete pDCMImage;
		pDCMImage = NULL;
	}

	if (windowImage)
		delete windowImage;

	return bRetunResult;
}


// called from ImageViewWorker
bool BDcm::Load(DcmPixmap* paDcmPixmap, QString filePath)
{

	DJDecoderRegistration::registerCodecs();
	DcmRLEDecoderRegistration::registerCodecs();

	DcmFileFormat dfile; // = new DcmFileFormat();


	//OFCondition cond = dfile->loadFile(filePath.toAscii().data(), EXS_Unknown, EGL_withoutGL, DCM_MaxReadLength, ERM_autoDetect);

	OFCondition cond = dfile.loadFile(filePath.toStdString().data(), EXS_Unknown, EGL_withoutGL, DCM_MaxReadLength, ERM_autoDetect);
	if (cond.bad() || cond.status() != OF_ok)
	{
		//if (dfile) {
		//	delete dfile;
		//	dfile = NULL;
		//}
		DJDecoderRegistration::cleanup();
		DcmRLEDecoderRegistration::cleanup();

		qDebug() << "BDcm::Load(), condition error! ";
		return false;

	}

	E_TransferSyntax xfer = dfile.getDataset()->getOriginalXfer();
	DicomImage *pDCMImage = new DicomImage(&dfile, xfer);

	QImage *pImage = ImageFromDcm(pDCMImage, &dfile);

	//pImage->save("c:\\test2.bmp"); // now can show on QLable or save


	int bRetunResult = false;
	if (pImage)
	{
		bRetunResult = paDcmPixmap->convertFromImage(*pImage, Qt::MonoOnly);

		//paDcmPixmap->SetOriginalImage(pImage); 
		if (pImage)
		{
			delete pImage;
			pImage = NULL;
		}

		paDcmPixmap->SetOriginalDcmImage(pDCMImage);
		paDcmPixmap->SetDcmSize(pDCMImage->getWidth(), pDCMImage->getHeight());

		// tag listing
		OFCondition tagReadingStatus;
		DcmDataset *dataset = dfile.getDataset();

		const char* pStrPatinetName = NULL;
		tagReadingStatus = dataset->findAndGetString(DCM_PatientName, pStrPatinetName);
		if (tagReadingStatus.good())
			paDcmPixmap->SetPatientName(pStrPatinetName);
		else
			qDebug() << "Error: Couldn't read tag [DCM_PatientName]" << endl;

		const char* pStrPatientID = NULL;
		tagReadingStatus = dataset->findAndGetString(DCM_PatientID, pStrPatientID);
		if (tagReadingStatus.good())
			paDcmPixmap->SetPatientID(pStrPatientID);
		else
			qDebug() << "Error: Couldn't read tag [DCM_PatientID]" << endl;


		const char* pStrPatientSex = NULL;
		tagReadingStatus = dataset->findAndGetString(DCM_PatientSex, pStrPatientSex);
		if (tagReadingStatus.good())
			paDcmPixmap->SetPatientSex(pStrPatientSex);
		else
			qDebug() << "Error: Couldn't read tag [pStrPatientSex]" << endl;

		const char* pStrPatientBirthdate = NULL;
		tagReadingStatus = dataset->findAndGetString(DCM_PatientBirthDate, pStrPatientBirthdate);
		if (tagReadingStatus.good())
			paDcmPixmap->SetPatientBirthDate(pStrPatientBirthdate);
		else
			qDebug() << "Error: Couldn't read tag [DCM_PatientBirthDate]" << endl;


		const char* pStrModality = NULL;
		tagReadingStatus = dataset->findAndGetString(DCM_Modality, pStrModality);
		if (tagReadingStatus.good())
			paDcmPixmap->SetModality(pStrModality);
		else
			qDebug() << "Error: Couldn't read tag [DCM_Modality]" << endl;

		const char* pStrInstitutionName = NULL;
		tagReadingStatus = dataset->findAndGetString(DCM_InstitutionName, pStrInstitutionName);
		if (tagReadingStatus.good())
			paDcmPixmap->SetInstitutionName(pStrInstitutionName);
		else
			qDebug() << "Error: Couldn't read tag [DCM_InstitutionName]" << endl;

		const char* pStrRefferingPhysicianName = NULL;
		tagReadingStatus = dataset->findAndGetString(DCM_ReferringPhysicianName, pStrRefferingPhysicianName);
		if (tagReadingStatus.good())
			paDcmPixmap->SetRefferingPhysicianName(pStrRefferingPhysicianName);
		else
			qDebug() << "Error: Couldn't read tag [DCM_ReferringPhysicianName]" << endl;

		const char* pStrStudyDate = NULL;
		tagReadingStatus = dataset->findAndGetString(DCM_StudyDate, pStrStudyDate);
		if (tagReadingStatus.good())
			paDcmPixmap->SetStudyDate(pStrStudyDate);
		else
			qDebug() << "Error: Couldn't read tag [DCM_StudyDate]" << endl;

		const char* pStrStudyTime = NULL;
		tagReadingStatus = dataset->findAndGetString(DCM_StudyTime, pStrStudyTime);
		if (tagReadingStatus.good())
			paDcmPixmap->SetStudyTime(pStrStudyTime);
		else
			qDebug() << "Error: Couldn't read tag [DCM_StudyTime]" << endl;

		const char* pStrSeriesDescription = NULL;
		tagReadingStatus = dataset->findAndGetString(DCM_SeriesDescription, pStrSeriesDescription);
		if (tagReadingStatus.good())
			paDcmPixmap->SetSeriesDescription(pStrSeriesDescription);
		else
			qDebug() << "Error: Couldn't read tag [DCM_SeriesDescription]" << endl;

	}

	DJDecoderRegistration::cleanup();
	DcmRLEDecoderRegistration::cleanup();

	return bRetunResult;
}


QImage * BDcm::ImageFromDcm(DicomImage* paSrcDcmImage, DcmFileFormat* pDcmFileFormat)
{
	int i;
	QColor color;
	QImage *paTargetImage = NULL;
	
	uchar *px;
	//uchar pixel[4];
	
	const int width = (int)(paSrcDcmImage->getWidth());
	const int height = (int)(paSrcDcmImage->getHeight());

	if (0 == width || 0 == height)
		return NULL;

	if (paSrcDcmImage->isMonochrome())
	{
		paTargetImage = new QImage(width, height, QImage::Format_Indexed8);
		//img->setNumColors(256);
		paTargetImage->setColorCount(256);

		// define gray palette here
		for (i = 0; i < 256; i++) {
			color.setRgb(i, i, i);
			paTargetImage->setColor(i, color.rgb());
		}

		OFCondition statusWindowCenter, statusWindowWidth;
		DcmDataset *dataset = pDcmFileFormat->getDataset();
		Float64 windowWidth = 0;
		Float64 windowCenter = 0;
		statusWindowWidth = dataset->findAndGetFloat64(DCM_WindowWidth, windowWidth);
		statusWindowCenter = dataset->findAndGetFloat64(DCM_WindowCenter, windowCenter);
		if (statusWindowWidth.good() && statusWindowCenter.good())
		{
			paSrcDcmImage->setWindow(windowCenter, windowWidth);
		}
		else
		{
			qDebug() << "Error: Couldn't get the value of window tag " << endl;
			paSrcDcmImage->setMinMaxWindow();
		}

		const void * pDicomOutputData = paSrcDcmImage->getOutputData(16);

		uchar pixel[2];
		unsigned char * pd;
		pd = (unsigned char *)pDicomOutputData;
		for (int y = 0; y < (long)height; y++)
		{
			px = paTargetImage->scanLine(y);
			for (int x = 0; x < (ulong)width; x++)
			{

				//pixel[3] = (unsigned char)(*(pd + 4 * y*width + 4 * x));
				//pixel[2] = (unsigned char)(*(pd + 4 * y*width + 4 * x + 1));
				//pixel[1] = (unsigned char)(*(pd + 4 * y*width + 4 * x + 2));
				//pixel[0] = (unsigned char)(*(pd + 4 * y*width + 4 * x + 3));

				pixel[1] = (unsigned char)(*(pd + 2 * y*width + 2 * x));
				pixel[0] = (unsigned char)(*(pd + 2 * y*width + 2 * x + 1));


				Uint16 orgVal = ((pixel[1] & 0xff) << 8) | (pixel[0] & 0xff);
				//qDebug() << "orgVal : " << orgVal;

				px[x] = orgVal;

				//if (orgVal <= expf((float)windowCenter - 0.5 - ((float)windowWidth - 1) / 2))
				//	px[x] = (unsigned char)0;
				//else if (orgVal > expf((float)windowCenter - 0.5 + ((float)windowWidth - 1) / 2))
				//	px[x] = (unsigned char)255;
				//else
				//	px[x] = (unsigned char)expf((((float)orgVal - ((float)windowCenter - 0.5)) / ((float)windowWidth - 1) + 0.5) * 255);

				//pd++;


			}
		}
	}
	else
	{
		uchar pixel[3];
		paTargetImage = new QImage(width, height, QImage::Format_RGB32);
		void * pDicomDibits = NULL;
		paSrcDcmImage->createWindowsDIB(pDicomDibits, 0, 0, 24, 0, 1);
		unsigned char * pd;
		pd = (unsigned char *)pDicomDibits;
		for (int y = 0; y < (long)height; y++)
		{
			px = paTargetImage->scanLine(y);
			for (int x = 0; x < (ulong)width; x++)
			{
				QRgb *row = (QRgb*)px;

				pixel[2] = (unsigned char)(*(pd + 3 * y*width + 3 * x));
				pixel[1] = (unsigned char)(*(pd + 3 * y*width + 3 * x + 1));
				pixel[0] = (unsigned char)(*(pd + 3 * y*width + 3 * x + 2));

				//pixel[3] = (unsigned char) (*pd++);

				row[x] = qRgb(pixel[0], pixel[1], pixel[2]);

			}
		}

		if (pDicomDibits)
			delete [] pDicomDibits;
	}

	//pImage->save("c:\\test2.bmp"); // now can show on QLable or save

	return paTargetImage;

}


QImage * BDcm::ConvertFromDcm(DicomImage* paSrcDcmImage)
{
	int i;
	QColor color;
	QImage *paTargetImage = NULL;

	uchar *px;
	//uchar pixel[4];
	
	const int width = (int)(paSrcDcmImage->getWidth());
	const int height = (int)(paSrcDcmImage->getHeight());

	if (0 == width || 0 == height)
		return NULL;

	if (paSrcDcmImage->isMonochrome())
	{
		paTargetImage = new QImage(width, height, QImage::Format_Indexed8);
		//img->setNumColors(256);
		paTargetImage->setColorCount(256);

		// define gray palette here
		for (i = 0; i < 256; i++) {
			color.setRgb(i, i, i);
			paTargetImage->setColor(i, color.rgb());
		}
		
		const void * pDicomOutputData = paSrcDcmImage->getOutputData(16);

		uchar pixel[2];
		unsigned char * pd;
		pd = (unsigned char *)pDicomOutputData;
		for (int y = 0; y < (long)height; y++)
		{
			px = paTargetImage->scanLine(y);
			for (int x = 0; x < (ulong)width; x++)
			{

				//pixel[3] = (unsigned char)(*(pd + 4 * y*width + 4 * x));
				//pixel[2] = (unsigned char)(*(pd + 4 * y*width + 4 * x + 1));
				//pixel[1] = (unsigned char)(*(pd + 4 * y*width + 4 * x + 2));
				//pixel[0] = (unsigned char)(*(pd + 4 * y*width + 4 * x + 3));

				pixel[1] = (unsigned char)(*(pd + 2 * y*width + 2 * x));
				pixel[0] = (unsigned char)(*(pd + 2 * y*width + 2 * x + 1));

				Uint16 orgVal = ((pixel[1] & 0xff) << 8) | (pixel[0] & 0xff);
				//qDebug() << "orgVal : " << orgVal;

				px[x] = orgVal;

				//if (orgVal <= expf((float)windowCenter - 0.5 - ((float)windowWidth - 1) / 2))
				//	px[x] = (unsigned char)0;
				//else if (orgVal > expf((float)windowCenter - 0.5 + ((float)windowWidth - 1) / 2))
				//	px[x] = (unsigned char)255;
				//else
				//	px[x] = (unsigned char)expf((((float)orgVal - ((float)windowCenter - 0.5)) / ((float)windowWidth - 1) + 0.5) * 255);

				//pd++;


			}
		}
	}
	else
	{
		uchar pixel[3];
		paTargetImage = new QImage(width, height, QImage::Format_RGB32);
		void * pDicomDibits = NULL;
		paSrcDcmImage->createWindowsDIB(pDicomDibits, 0, 0, 24, 0, 1);
		unsigned char * pd;
		pd = (unsigned char *)pDicomDibits;
		for (int y = 0; y < (long)height; y++)
		{
			px = paTargetImage->scanLine(y);
			for (int x = 0; x < (ulong)width; x++)
			{
				QRgb *row = (QRgb*)px;

				pixel[2] = (unsigned char)(*(pd + 3 * y*width + 3 * x));
				pixel[1] = (unsigned char)(*(pd + 3 * y*width + 3 * x + 1));
				pixel[0] = (unsigned char)(*(pd + 3 * y*width + 3 * x + 2));

				//pixel[3] = (unsigned char) (*pd++);

				row[x] = qRgb(pixel[0], pixel[1], pixel[2]);

			}
		}

		//if (pDicomDibits)
		//	free((void*) pDicomDibits);
		if (pDicomDibits)
			delete[] pDicomDibits;
	}

	//pImage->save("c:\\test2.bmp"); // now can show on QLable or save

	return paTargetImage;
}


bool BDcm::ImageToDCM(QImage* paSrcImage, DicomImage *paTargetDcm)
{





	return true;
}

bool BDcm::GetWindow(DicomImage* paImg, double &center, double &width)
{
	return paImg->getWindow(center, width);
}

QImage* BDcm::SetWindow(DicomImage* paSrcDcmImage, double windowCenter, double windowWidth)
{

	int i;
	QColor color;
	QImage *paTargetImage = NULL;

	void * ptrForDelete = NULL;
	uchar *px;
	//uchar pixel[4];
	uchar pixel[2];
	const int width = (int)(paSrcDcmImage->getWidth());
	const int height = (int)(paSrcDcmImage->getHeight());

	if (paSrcDcmImage->isMonochrome())
	{
		paTargetImage = new QImage(width, height, QImage::Format_Indexed8);
		//img->setNumColors(256);
		paTargetImage->setColorCount(256);

		// define gray palette here
		for (i = 0; i < 256; i++) {
			color.setRgb(i, i, i);
			paTargetImage->setColor(i, color.rgb());
		}

		paSrcDcmImage->setWindow(windowCenter, windowWidth);
		//paSrcDcmImage->setMinMaxWindow();
		// What else should I set before presenting the image?
		const void * pDicomData = paSrcDcmImage->getOutputData(16);
		// The brightness only occurs with greyscale images.. with RGB
		// images I get very nice looking images though I've not modified
		// any member variables at all.

		//paSrcDcmImage->createWindowsDIB(pDicomDibits, 0, 0, 32, 0, 1);

		unsigned char * pd;
		pd = (unsigned char *)pDicomData;
		for (int y = 0; y < (long)height; y++)
		{
			px = paTargetImage->scanLine(y);
			for (int x = 0; x < (ulong)width; x++)
			{

				//pixel[3] = (unsigned char)(*(pd + 4 * y*width + 4 * x));
				//pixel[2] = (unsigned char)(*(pd + 4 * y*width + 4 * x + 1));
				//pixel[1] = (unsigned char)(*(pd + 4 * y*width + 4 * x + 2));
				//pixel[0] = (unsigned char)(*(pd + 4 * y*width + 4 * x + 3));


				pixel[1] = (unsigned char)(*(pd + 2 * y*width + 2 * x));
				pixel[0] = (unsigned char)(*(pd + 2 * y*width + 2 * x + 1));

				// 구리게 나왓음
				//Uint16 orgVal = ((pixel[0] & 0xff) << 8) | (pixel[1] & 0xff);
				//Uint16 orgVal = ((pixel[2] & 0xff) << 8) | (pixel[3] & 0xff);
				Uint16 orgVal = ((pixel[1] & 0xff) << 8) | (pixel[0] & 0xff);
				//Uint16 orgVal = ( pixel[1] << 8) | pixel[0];
				//qDebug() << "orgVal : " << orgVal;

				px[x] = orgVal;

				//if (orgVal <= expf((float)windowCenter - 0.5 - ((float)windowWidth - 1) / 2))
				//	px[x] = (unsigned char)0;
				//else if (orgVal > expf((float)windowCenter - 0.5 + ((float)windowWidth - 1) / 2))
				//	px[x] = (unsigned char)255;
				//else
				//	px[x] = (unsigned char)expf((((float)orgVal - ((float)windowCenter - 0.5)) / ((float)windowWidth - 1) + 0.5) * 255);

				//pd++;


			}
		}
	}
	else
	{
		return NULL;

	}

	//pImage->save("c:\\test2.bmp"); // now can show on QLable or save

	return paTargetImage;

}

DicomImage* BDcm::Copy(DicomImage* paSrcImage)
{
	return paSrcImage->createDicomImage();
}

int BDcm::Flip(DicomImage* paSrcDcmImage, int isHorizontal, int isVertical)
{
	int ret = paSrcDcmImage->flipImage(isHorizontal, isVertical);

	return ret;
}

bool BDcm::Rotate(DicomImage* paSrcDcmImage, int degree)
{
	return paSrcDcmImage->rotateImage(degree);
}
