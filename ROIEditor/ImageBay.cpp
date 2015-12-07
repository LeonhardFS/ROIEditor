//
//  ImageBay.cpp
//  ROIEditor
//
//  Created by Leonhard Spiegelberg on 27.11.15.
//
//

#include "ImageBay.h"
#include "utils.h"
#include <fstream>
#include <sstream>
#include <string>
#include <QDebug>

// 128px default ROI size
#define DEFAULT_ROI_SIZE 128


void AnnotatedImageFile::clampedUpdate(double updateX, double updateY) {
    double new_x = ROI.x + updateX;
    double new_y = ROI.y + updateY;
    
    // clamp it
    new_x = max(0., min(width - ROI.w - 1, new_x));
    new_y = max(0., min(height - ROI.h - 1, new_y));
    
    ROI.x = new_x;
    ROI.y = new_y;
    
    if(parent)
        parent->notifyChange();
}


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
    // clear vector
    Images.clear();
    changes = false;
}

// add image file to list
void ImageBay::addFile(const QString sFileName) {
    
    AnnotatedImageFile * aif = new AnnotatedImageFile(this);
    
    aif->sFileName = sFileName;
    
    
    // per default set the ROI to the center
    // use QT Image reader to speed up the process
    QImageReader img(sFileName);
    double w = img.size().width();
    double h = img.size().height();
    
    aif->ROI = RegionOfInterest<double>(w / 2. - DEFAULT_ROI_SIZE/2., h / 2. - DEFAULT_ROI_SIZE / 2., DEFAULT_ROI_SIZE, DEFAULT_ROI_SIZE);
    
    aif->width = w;
    aif->height = h;
    
    // add to list
    Images.push_back(aif);
}

// add whole directory to list
void ImageBay::addDirectory(const QString &sDir) {
    QDir dir(sDir);
    
    // include not the special dirs . and ..
    QFileInfoList fil = dir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries);
    
    // iterate over files and directories...
    if(!fil.empty())
    for (QList<QFileInfo>::iterator it = fil.begin();it != fil.end(); ++it) {
        QFileInfo fileInfo = *it;
        if(fileInfo.isFile()) {
            // file
            if (ImageBay::isImageFileExt(fileInfo.suffix()))
                addFile(fileInfo.absoluteFilePath());
        }
        else {
            if(fileInfo.isDir())
                addDirectory(fileInfo.absolutePath());
        }
    }
}

// saving to CSV file
void ImageBay::saveToCSV(const QString &sFileName) {
    
    string sep = ",";
    
    // format is
    // x | y | w | h | imw | imh | filename | abs.path
    ofstream ofs;
    ofs.open(sFileName.toStdString().c_str());
    // write header
    ofs<< "x"<<sep<<"y"<<sep<<"w"<<sep<<"h"<<sep<<"imw"<<sep<<"imh"<<sep;
    ofs<<"filename"<<sep<<"absolutepath"<<endl;
    
    if (!Images.empty()) {
        for(vector<AnnotatedImageFile*>::const_iterator it = Images.begin();
            it != Images.end(); ++it) {
            
            AnnotatedImageFile *aif = *it;
            QFileInfo fileInfo(aif->sFileName);
            
            ofs<<(int)aif->ROI.x<<sep;
            ofs<<(int)aif->ROI.y<<sep;
            ofs<<(int)aif->ROI.w<<sep;
            ofs<<(int)aif->ROI.h<<sep;
            ofs<<(int)aif->width<<sep;
            ofs<<(int)aif->height<<sep;
            ofs<<fileInfo.fileName().toStdString()<<sep;
            ofs<<fileInfo.filePath().toStdString()<<endl;
        }
    }
    ofs.close();
    
    // file was saved, no changes!
    changes = false;
}

// load data from csv
void ImageBay::loadFromCSV(const QString &sFileName) {
    // first clean up
    cleanUp();
    
    // now load the data
    ifstream ifs;
    ifs.open(sFileName.toStdString().c_str());
    
    string line;
    while(getline(ifs, line))
    {
        stringstream iss(line);
        
        // parse the csv row
        vector<string> row;
        string cell;
        while(getline(iss, cell, ',' ) )
            row.push_back(cell);
        
        // check if image file exists
        if(fileExists(QString(row[7].c_str()))) {
            
            // add a new annotated image to list if it exists
            AnnotatedImageFile *aif = new AnnotatedImageFile(this);
            
            aif->ROI.x = atoi(row[0].c_str());
            aif->ROI.y = atoi(row[1].c_str());
            aif->ROI.w = atoi(row[2].c_str());
            aif->ROI.h = atoi(row[3].c_str());
            aif->width = atoi(row[4].c_str());
            aif->height = atoi(row[5].c_str());
            aif->sFileName = QString(row[7].c_str());
            
            Images.push_back(aif);
        }
        
    }
}

