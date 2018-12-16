#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxHTTP.h"
#include "ofxOsc.h"
#include "ofxGui.h"

#define HOST "192.168.31.159"
#define PORT 7890

class Man2Blob {

public:
	int manID;
	int blobID;
	bool isUsed;
	ofPoint p;
};

class myBlob {

public:
	int blobID;
	bool isUsed;
	ofPoint p;
};

class ofApp : public ofBaseApp {
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	
	
	//*******************
	//*      VAR        *
	//*******************

	//kinect group
	int KNT_WIDTH;
	int KNT_HEIGHT;
	int KNT_CUT;
	
	ofxKinect kinect1;
	ofxKinect kinect2;
	ofxKinect kinect3;
    ofxKinect kinect4;
    ofxKinect kinect5;
    ofxKinect kinect6;
    

	//deal with kinect pixels
	ofxCvGrayscaleImage grayDpthImgMerge;
	ofxCvGrayscaleImage grayDpthImgKnt1;
	ofxCvGrayscaleImage grayDpthImgKnt2;
	ofxCvGrayscaleImage grayDpthImgKnt3;
    ofxCvGrayscaleImage grayDpthImgKnt4;
    ofxCvGrayscaleImage grayDpthImgKnt5;
    ofxCvGrayscaleImage grayDpthImgKnt6;
	//gui
	ofxPanel gui;
	ofParameter<float> nearClipping;
	ofParameter<float> farClipping;
	ofParameter<float> minDepth;
	ofParameter<float> maxDepth;
	ofParameter<float> blurGaussian;
	ofParameter<float> minArea;
	ofParameter<float> maxArea;
	ofParameter<float> nConsidered;
	ofParameter<int>   angelKnt1;
	ofParameter<int>   angelKnt2;
	ofParameter<float> m2bDistance;
    
	//HTTP sender
    ofx::HTTP::SimpleIPVideoServer server;
    string serverURL;
    ofImage colorPixels;
	
	//*******************
	//*    FUNCTION     *
	//*******************

	//initialization kinect
	void myInitKnt(ofxKinect &knt, int id);

	//set gui
	void myInitGUI();

	//filter pixels by constant range from distance
	ofxCvGrayscaleImage filterDpthPoint(ofxKinect &knt);

	//merge img
	ofxCvGrayscaleImage mergeImg(ofxCvGrayscaleImage img1, ofxCvGrayscaleImage img2);
	ofxCvGrayscaleImage TransMergeImg(ofxCvGrayscaleImage img1, ofxCvGrayscaleImage img2);
	ofxCvGrayscaleImage LongMergeImg(ofxCvGrayscaleImage img1, ofxCvGrayscaleImage img2);
	
    
};
