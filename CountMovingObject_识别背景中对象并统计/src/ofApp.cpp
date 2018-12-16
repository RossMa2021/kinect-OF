#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0, 0, 0);

	//视频输入
	vidGrabber.initGrabber(320, 240);

	//图像容器实例化
	colorImg.allocate(320, 240); //p1
	grayImage.allocate(320, 240); //p2
	grayBg.allocate(320, 240); //p3 背景
	grayDiff.allocate(320, 240); // p2 p3 的差

	bLearnBakground = true;
	threshold = 100;
}

//--------------------------------------------------------------
void ofApp::update(){

	vidGrabber.update();

	if (vidGrabber.isFrameNew()) {
		colorImg.setFromPixels(vidGrabber.getPixelsRef());//p1
		grayImage = colorImg; //p2

		//取第一帧作为背景
		if (bLearnBakground == true) {
			grayBg = grayImage;//p
			bLearnBakground = false;
		} 

		//获取p2 p3 差分
		grayDiff.absDiff(grayBg, grayImage);
		
		grayDiff.threshold(threshold);

		grayDiff_2 = grayDiff;
		
		//获取轮廓
		contourFinder.findContours(grayDiff, 20, (340 * 240) / 3, 10, true);

		//灰度图像，将寻找斑点。参数(灰度像素图,最小斑点像素，最大斑点像素，最大斑点数量，斑点是否无孔)
		//Find contours
		contourFinder_2.findContours(grayDiff_2, 10, 10000, 20, false);
		//将坐标存入
		//Store objects' centers
		vector<ofxCvBlob>  &blobs = contourFinder_2.blobs;
		int n = blobs.size();	//Get number of blobs
		obj.resize(n);		//Resize obj array
		for (int i = 0; i < n; i++) {
			obj[i] = blobs[i].centroid;	//Fill obj array
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw(){
	//1-1
	colorImg.draw(10, 10);
	//1-2
	grayImage.draw(340, 10);
	//2-1
	grayBg.draw(10, 260);
	//2-2
	grayDiff.draw(340, 260);

	//1-3
	for (int i = 0; i < contourFinder.nBlobs; i++) {
		contourFinder.blobs[i].draw(670, 10);
	}

	//2-3
	//grayDiff.draw(670, 260);
	//只显示十字线
	for (int i = 0; i < obj.size(); i++) {
		ofPoint p = obj[i] + ofPoint(670, 260);
		ofLine(p.x - 20, p.y, p.x + 20, p.y);
		ofLine(p.x, p.y - 20, p.x, p.y + 20);
	}
	ofSetLineWidth(1);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case '1': //スペースキーで背景を学習モードに
		bLearnBakground = true;
		break;
	case '2': //[+]キーで背景の閾値のレベルを上げる(最大255)
		threshold++;
		if (threshold > 255) threshold = 255;
		break;
	case '3': //[-]キーで背景の閾値のレベルを下げる(最小0)
		threshold--;
		if (threshold < 0) threshold = 0;
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
