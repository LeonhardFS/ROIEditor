//
//  ImageBay.h
//  ROIEditor
//
//  Created by Leonhard Spiegelberg on 27.11.15.
//
//

#ifndef ImageBay_h
#define ImageBay_h

#include <vector>
#include <string>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QImageReader>
#include <QDebug>

using namespace std;

class ImageBay;
class AnnotatedImageFile;

// rectangular region of interest
template<typename T> class RegionOfInterest {
public:
    T x, y, w, h;
    
    RegionOfInterest():x(0), y(0), w(0), h(0) {}
    RegionOfInterest(const RegionOfInterest& roi):x(roi.x), y(roi.y), w(roi.w), h(roi.h) {}
    RegionOfInterest(const T _x, const T _y, const T _w, const T _h):x(_x), y(_y), w(_w), h(_h) {}
    
    inline bool containsPoint(const T px, const T py) {
        if(px > x + w)return false;
        if(py > y + h)return false;
        if(px < x) return false;
        if(py < y)return false;
        return true;
    }
    
    
};

template<typename T> class PointOfInterest {
public:
    T x, y;
};

// hold the file name and its annotations
class AnnotatedImageFile {
private:
    ImageBay *parent;
public:
    AnnotatedImageFile(ImageBay *bay):parent(bay) {}
    
    QString sFileName;
    unsigned int width, height;
    RegionOfInterest<double> ROI;
    
    // make sure the update does not push the ROI out of the image
    void clampedUpdate(double updateX, double updateY);
};

// stores a selection of image files with their corresponding regions of interest
class ImageBay {
private:
    vector<AnnotatedImageFile*> Images;
    bool changes = false;
    
    // release all allocated memory
    void cleanUp();
    
    // alter the change status
    void notifyChange() {
        changes = true;
    }
public:
    ImageBay():changes(false) {}
    
    ~ImageBay() {
        cleanUp();
    }
   
    // add single file to ImageBay
    void addFile(const QString sFileName);
    
    // add directory to ImageBay
    void addDirectory(const QString& sDir);
    
    // save annotations to file
    void saveToCSV(const QString& sFileName);
    
    // load annotations from file
    void loadFromCSV(const QString& sFileName);
    
    // get number of images
    inline unsigned int count() {return Images.size();}
    
    // get image
    inline AnnotatedImageFile* get(const unsigned int index) {
        if(!Images.empty())if(index < count())return Images[index];
        return NULL;
    }
    
    // static helper function to check if the ext stems from a graphics file
    static inline bool isImageFileExt(const QString& s) {
        if(s.compare("jpg") == 0)return true;
        if(s.compare("jpeg") == 0)return true;
        if(s.compare("png") == 0)return true;
        if(s.compare("bmp") == 0)return true;
        if(s.compare("tif") == 0)return true;
        if(s.compare("tiff") == 0)return true;
        return false;
    }
    
    // has anything changed ?
    bool changed() {return changes;}
    
    friend class AnnotatedImageFile;
    
};

#endif /* ImageBay_h */
