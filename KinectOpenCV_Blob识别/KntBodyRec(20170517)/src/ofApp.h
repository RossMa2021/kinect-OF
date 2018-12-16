#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "IPVideoGrabber.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxGui.h"

// listen on port 12345
#define PORT 12345
#define NUM_MSG_STRINGS 20

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

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

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

		//deal with kinect pixels
		ofxCvGrayscaleImage grayDpthImgMerge;
		ofxCvGrayscaleImage grayDpthImgKnt1;
		ofxCvGrayscaleImage grayDpthImgKnt2;
		ofxCvGrayscaleImage grayDpthImgKnt3;

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


		//blob
		ofxCvContourFinder 	contourFinder;
		vector<myBlob> obj;	//object's centers

		//OSC sender
		ofxOscSender sender;

		//HTTP receive
		ofx::Video::IPVideoGrabber videoGrabber;
		ofImage receivedImage;
		ofxCvGrayscaleImage grayDpthRecImg;

		//man mapping blob
		vector<Man2Blob> m2b;

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

		//blob
		void groupByBlob(ofxCvGrayscaleImage img);
		void drawBlobMark(int diffw, int diffh, int w, int h);

		//OSC sender
		void myOSCsender(int id, float x, float y);

		//vector initialization
		void initMan2BlobVectot(int num);

		//manId mapping blobID
		void mapBlobId2ManID(vector<myBlob> &obj, vector<Man2Blob> &m2b);
		
};
