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
		
		ofVideoGrabber vidGrabber; //��Ƶ����
		ofxCvColorImage colorImg; //OpenCV����Ĳ�ɫͼ�� p1
		ofxCvGrayscaleImage grayImage; //OpenCV����ĻҶ�ͼ�� p2
		ofxCvGrayscaleImage grayBg; //�����Ҷ�ͼ p3
		ofxCvGrayscaleImage grayDiff; //�Ҷ�ͼ p2 �� p3 ����
		ofxCvGrayscaleImage grayDiff_2; //�Ҷ�ͼ p2 �� p3 ����
		ofxCvContourFinder contourFinder; //��ȡ����
		bool bLearnBakground; //�Ƿ��¼����
		int threshold; //���������ֵ��ֵ

		//ȷ���˶����巶Χ���������+
		ofxCvContourFinder 	contourFinder_2;
		vector<ofPoint> obj;	//��¼�˶��������ĵ�
};
