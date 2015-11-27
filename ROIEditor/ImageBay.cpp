//
//  ImageBay.cpp
//  ROIEditor
//
//  Created by Leonhard Spiegelberg on 27.11.15.
//
//

#include "ImageBay.h"


// 128px default ROI size
#define DEFAULT_ROI_SIZE 128

// delete all allocated space
void ImageBay::cleanUp() {
    if(!Images.empty())
        for(vector<AnnotatedImageFile*>::iterator it = Images.begin();
            it != Images.end(); ++it) {
            AnnotatedImageFile *ptr = *it;
            if (ptr) {
                delete ptr;
                ptr = NULL;
            }
        }
}

// add image file to list
void ImageBay::addFile(const QString sFileName) {
    
    AnnotatedImageFile * aif = new AnnotatedImageFile;
    
    aif->sFileName = sFileName;
    
    
    // per default set the ROI to the center
    QImage img;
    img.load(sFileName);
    double w = img.size().width();
    double h = img.size().height();
    
    aif->ROI = RegionOfInterest<double>(w / 2. - DEFAULT_ROI_SIZE/2., h / 2. - DEFAULT_ROI_SIZE / 2., DEFAULT_ROI_SIZE, DEFAULT_ROI_SIZE);
    
    // add to list
    Images.push_back(aif);
}

// add whole directory to list
void ImageBay::addDirectory(const QDir &dir) {
    // iterate over files and directories...
    QFileInfoList fil = dir.entryInfoList();
    for (QList<QFileInfo>::iterator it = fil.begin();it != fil.end(); ++it) {
        QFileInfo fileInfo = *it;
        if(fileInfo.isFile()) {
            // file
            if (ImageBay::isImageFileExt(fileInfo.suffix()))
                addFile(fileInfo.absoluteFilePath());
        }
            else {
                if(fileInfo.isDir())
                    addDirectory(fileInfo.absoluteDir());
            }
    }
}

