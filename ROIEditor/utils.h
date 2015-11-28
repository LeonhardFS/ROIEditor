//
//  utils.h
//  ROIEditor
//
//  Created by Leonhard Spiegelberg on 27.11.15.
//
//

#ifndef utils_h
#define utils_h

#include <QFileInfo>

inline bool fileExists(QString path) {
    QFileInfo checkFile(path);
    // check if file exists and that it is not a directory
    if (checkFile.exists() && checkFile.isFile()) {
        return true;
    } else {
        return false;
    }
}

#endif /* utils_h */
