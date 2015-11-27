//
//  ImageBay.cpp
//  ROIEditor
//
//  Created by Leonhard Spiegelberg on 27.11.15.
//
//

#include "ImageBay.h"

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

