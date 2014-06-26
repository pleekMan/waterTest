//
//  VideoManager.cpp
//  waterTest
//
//  Created by FYPF+OTROS on 26/06/14.
//
//

#include "VideoManager.h"

void VideoManager::setup(string videoPath){
    
    ofDirectory dir(videoPath);
    //dir.allowExt("mov");
    dir.listDir(); // POPULATE WITH FILES IN DIR
    
    for(int i = 0; i < dir.numFiles(); i++){
        ofLogNotice(dir.getPath(i));
        ofVideoPlayer currentVideo;
        currentVideo.loadMovie(dir.getPath(i));
        videos.push_back(currentVideo);
    }
}

void VideoManager::update(){
    
}

void VideoManager::render(){
    
}