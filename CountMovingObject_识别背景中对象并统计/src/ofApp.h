#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

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
		
		ofVideoGrabber vidGrabber; //视频输入
		ofxCvColorImage colorImg; //OpenCV处理的彩色图像 p1
		ofxCvGrayscaleImage grayImage; //OpenCV处理的灰度图像 p2
		ofxCvGrayscaleImage grayBg; //背景灰度图 p3
		ofxCvGrayscaleImage grayDiff; //灰度图 p2 与 p3 差异
		ofxCvGrayscaleImage grayDiff_2; //灰度图 p2 与 p3 差异
		ofxCvContourFinder contourFinder; //提取轮廓
		bool bLearnBakground; //是否记录背景
		int threshold; //背景差分限值阈值

		//确定运动物体范围，并标记上+
		ofxCvContourFinder 	contourFinder_2;
		vector<ofPoint> obj;	//记录运动物体中心点
};
