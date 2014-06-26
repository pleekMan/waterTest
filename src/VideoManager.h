//
//  VideoManager.h
//  waterTest
//
//  Created by FYPF+OTROS on 26/06/14.
//
//

#ifndef __waterTest__VideoManager__
#define __waterTest__VideoManager__

#include <iostream>

#endif /* defined(__waterTest__VideoManager__) */

#include "ofMain.h"

class VideoManager {
    
    
public:
    VideoManager(){};
    
    void setup(string videoPath);
    void update();
    void render();
    
private:
    
    vector<ofVideoPlayer> videos;
    
};