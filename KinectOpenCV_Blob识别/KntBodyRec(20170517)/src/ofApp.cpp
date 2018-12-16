#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);

	//kinect initialization
	myInitKnt(kinect1, 1);
	myInitKnt(kinect2, 2);
	myInitKnt(kinect3, 3);

	KNT_CUT = 30;

	KNT_WIDTH = kinect1.width - KNT_CUT *2;
	KNT_HEIGHT = kinect1.height - KNT_CUT *2;

	//grayImage initialization
	grayDpthImgKnt1.allocate(KNT_WIDTH, KNT_HEIGHT);//or constant value
	grayDpthImgKnt2.allocate(KNT_WIDTH, KNT_HEIGHT);
	grayDpthImgKnt3.allocate(KNT_WIDTH, kinect1.height);//or constant value
	grayDpthRecImg.allocate(KNT_WIDTH * 3, KNT_HEIGHT * 2);
	grayDpthImgMerge.allocate(KNT_WIDTH * 3, KNT_HEIGHT * 3);

	ofSetFrameRate(60);

	//GUI
	myInitGUI();

	//OSC sender
	sender.setup("192.168.31.201", 12000);

	//Video grabber
	videoGrabber.setCameraName("Kinect");
	videoGrabber.setURI("http://192.168.31.159:7890/ipvideo");
	videoGrabber.connect();

	//vector initialization
	initMan2BlobVectot(30);
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(100, 100, 100);

	videoGrabber.update();
	kinect1.update();
	//kinect2.update();
	//kinect3.update();

	//gui work
	kinect1.setCameraTiltAngle(angelKnt1);
	//kinect2.setCameraTiltAngle(angelKnt1);
	//kinect3.setCameraTiltAngle(angelKnt1);
	kinect1.setDepthClipping(nearClipping, farClipping);//not work,ignore
	//kinect2.setDepthClipping(nearClipping, farClipping);//not work,ignore
	//kinect3.setDepthClipping(nearClipping, farClipping);//not work,ignore

	if (kinect1.isFrameNew()) {
		//filter pixels by constant range from distance
		grayDpthImgKnt1.setFromPixels(filterDpthPoint(kinect1).getPixels());
		grayDpthImgKnt2.setFromPixels(filterDpthPoint(kinect1).getPixels());
		grayDpthImgKnt3.setFromPixels(filterDpthPoint(kinect1).getPixels());

		//HTTP receive
		grayDpthRecImg.setFromPixels(videoGrabber.getPixels());

		//merge Img
		//grayDpthImgMerge.setFromPixels(LongMergeImg(TransMergeImg(TransMergeImg(grayDpthImgKnt1, grayDpthImgKnt2), grayDpthImgKnt3),
		//	grayDpthRecImg).getPixels());

		
		grayDpthImgMerge.setFromPixels(LongMergeImg(LongMergeImg(
			TransMergeImg(TransMergeImg(grayDpthImgKnt1, grayDpthImgKnt2), grayDpthImgKnt3),
			TransMergeImg(TransMergeImg(grayDpthImgKnt1, grayDpthImgKnt2), grayDpthImgKnt3)),
			TransMergeImg(TransMergeImg(grayDpthImgKnt1, grayDpthImgKnt2), grayDpthImgKnt3)).getPixels());
		

		//start blob
		grayDpthImgMerge.blurGaussian(blurGaussian);
		groupByBlob(grayDpthImgMerge);
	}

}

//--------------------------------------------------------------
void ofApp::draw(){
	grayDpthImgMerge.draw(10, 10, 960, 720);
	gui.draw();
	contourFinder.draw(10, 10, 960, 720);
	drawBlobMark(10, 10, 960, 720);
	kinect1.draw(10, 10);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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

//kinect device initialization
void ofApp::myInitKnt(ofxKinect &knt, int id) {
	knt.setRegistration(true);
	knt.init(false, false);
	knt.open();
	if (knt.isConnected()) {
		ofLogNotice() << "kinect[" << id << "]sensor-emitter dist: " << knt.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "kinect[" << id << "]sensor-camera dist:  " << knt.getSensorCameraDistance() << "cm";
		ofLogNotice() << "kinect[" << id << "]zero plane pixel size: " << knt.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "kinect[" << id << "]zero plane dist: " << knt.getZeroPlaneDistance() << "mm";
	}
	knt.setCameraTiltAngle(0);
	knt.setCameraTiltAngle(0);
}

void ofApp::myInitGUI() {
	//gui
	gui.setup();
	gui.add(nearClipping.set("nearClipping", 50, 1, 500));
	gui.add(farClipping.set("farClipping", 500, 1, 5000));
	gui.add(minDepth.set("minDepth", 1, 1, 5000));
	gui.add(maxDepth.set("maxDepth", 50, 1, 5000));
	gui.add(blurGaussian.set("blurGaussian", 3, 1, 9));
	gui.add(minArea.set("minArea", 10, 1, 1000));
	gui.add(maxArea.set("maxArea", 10000, 5000, 20000));
	gui.add(nConsidered.set("nConsidered", 20, 1, 100));
	gui.add(angelKnt1.set("angelKnt1", 0, 1, 100));
	gui.add(angelKnt2.set("angelKnt2", 0, 1, 100));
	gui.add(m2bDistance.set("m2bDistance", 1, 0.001, 5));
}

//init vector
void ofApp::initMan2BlobVectot(int num) {
	for (int i = 0; i < num; i++) {
		Man2Blob *temp = new Man2Blob();
		(*temp).manID = i;
		(*temp).blobID = -1;
		(*temp).p.x = -1;
		(*temp).p.y = -1;
		(*temp).isUsed = false;
		m2b.push_back(*temp);
	}
}

ofxCvGrayscaleImage ofApp::filterDpthPoint(ofxKinect &knt) {

	int w = KNT_WIDTH;
	int h = KNT_HEIGHT;

	ofxCvGrayscaleImage gryImg;
	gryImg.allocate(w, h);
	ofPixels pxlImg = gryImg.getPixels();

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if ((knt.getDistanceAt(j+ KNT_CUT, i+ KNT_CUT) < maxDepth)
				&& (knt.getDistanceAt(j+ KNT_CUT, i+ KNT_CUT) > minDepth)) {
				pxlImg[i*w + j] = 255;
			}
			else {
				pxlImg[i*w + j] = 0;
			}

		}
	}

	gryImg.getPixels() = pxlImg;

	return gryImg;
}

//Transverse stitching, same height
ofxCvGrayscaleImage ofApp::TransMergeImg(ofxCvGrayscaleImage img1, ofxCvGrayscaleImage img2) {

	ofxCvGrayscaleImage imgRes;

	int h = img1.height;
	int w1 = img1.width;
	int w2 = img2.width;
	int wRes = w1 + w2;

	imgRes.allocate(wRes, h);

	ofPixels pxlimg1 = img1.getPixels();
	ofPixels pxlimg2 = img2.getPixels();
	ofPixels pxlimgRes = imgRes.getPixels();


	for (int i = 0; i < h; i++) {
		for (int j = 0; j < wRes; j++) {
			if (j < w1) {
				pxlimgRes[i * wRes + j] = pxlimg1[i*w1 + j];
			}
			else {
				pxlimgRes[i * wRes + j] = pxlimg2[i*w2 + j - w1];
			}
		}
	}

	imgRes.getPixels() = pxlimgRes;

	return imgRes;
}

//Longitudinal stitching,same width
ofxCvGrayscaleImage ofApp::LongMergeImg(ofxCvGrayscaleImage img1, ofxCvGrayscaleImage img2) {

	ofxCvGrayscaleImage imgRes;

	int w = img1.width;
	int h1 = img1.height;
	int h2 = img2.height;
	int hRes = h1 + h2;

	imgRes.allocate(w, hRes);

	ofPixels pxlimg1 = img1.getPixels();
	ofPixels pxlimg2 = img2.getPixels();
	ofPixels pxlimgRes = imgRes.getPixels();


	for (int i = 0; i < hRes; i++) {
		for (int j = 0; j < w; j++) {
			if (i < h1) {
				pxlimgRes[i * w + j] = pxlimg1[i*w + j];
			}
			else {
				pxlimgRes[i * w + j] = pxlimg2[(i - h1)*w + j];
			}
		}
	}

	imgRes.getPixels() = pxlimgRes;

	return imgRes;
}

void ofApp::groupByBlob(ofxCvGrayscaleImage img) {
	contourFinder.findContours(img, minArea, maxArea, nConsidered, false);
	//Store objects' centers
	vector<ofxCvBlob>  &blobs = contourFinder.blobs;
	int n = blobs.size();	//Get number of blobs
	obj.resize(n);		//Resize obj array
	for (int i = 0; i<n; i++) {
		obj[i].p = blobs[i].centroid;	//Fill obj array
		obj[i].isUsed = false;
		//cout << "obj[i].isUsed " << obj[i].isUsed << endl;
		//cout <<"Blob id[" <<i<<"],coordinates("<<obj[i].p <<")."<< endl;
		//myOSCsender(i, obj[i].p.x, obj[i].p.y);
	}


	if (n > 0) {
		mapBlobId2ManID(obj, m2b);
	}

}

//manId mapping blobID
void ofApp::mapBlobId2ManID(vector<myBlob> &obj, vector<Man2Blob> &m2b) {
	int nBlob = obj.size();
	int nM2b = m2b.size();

	//get m2b used count
	int nM2bUsed = 0;
	for (int i = 0; i < nM2b; i++) {
		if (m2b[i].isUsed == true) {
			nM2bUsed++;
		}
	}

	cout << "nBlob: " << nBlob
		<< ",nM2bUsed: " << nM2bUsed
		<< ",nM2b: " << nM2b << endl;


	//no people leave or enter
	if (nBlob == nM2bUsed) {
		for (int i = 0; i < nM2b; i++) {
			if (m2b[i].isUsed == true) {
				for (int j = 0; j < nBlob; j++) {
					if (obj[j].isUsed == false) {
						if (m2b[i].p.distance(obj[j].p) < 0.9) {
							m2b[i].p = obj[j].p;
							m2b[i].isUsed = true;
							obj[j].isUsed = true;
							break;
						}
					}
				}
			}
		}

	}
	//people enter
	else if (nBlob > nM2bUsed) {
		for (int i = 0; i < nM2b; i++) {
			if (m2b[i].isUsed == true) {
				for (int j = 0; j < nBlob; j++) {
					if (obj[j].isUsed == false) {
						if (m2b[i].p.distance(obj[j].p) < 0.9) {
							m2b[i].p = obj[j].p;
							m2b[i].isUsed = true;
							obj[j].isUsed = true;
							break;
						}
					}
				}
			}
		}
		for (int i = 0; i < nM2b; i++) {
			if (m2b[i].isUsed == false) {
				for (int j = 0; j < nBlob; j++) {
					if (obj[j].isUsed == false) {
						m2b[i].p = obj[j].p;
						m2b[i].isUsed = true;
						obj[j].isUsed = true;
						break;
					}
				}
			}
		}
	}
	//people leave
	else if (nBlob < nM2bUsed) {
		for (int i = 0; i < nM2b; i++) {
			if (m2b[i].isUsed == true) {
				m2b[i].isUsed = false;
				for (int j = 0; j < nBlob; j++) {
					if (obj[j].isUsed == false) {
						if (m2b[i].p.distance(obj[j].p) < 0.9) {
							m2b[i].p = obj[j].p;
							m2b[i].isUsed = true;
							obj[j].isUsed = true;
							break;
						}
					}
				}
			}
		}
		for (int i = 0; i < nM2b; i++) {
			if (m2b[i].isUsed == false) {
				m2b[i].p.x = -1;
				m2b[i].p.y = -1;
				m2b[i].isUsed = false;

			}
		}
	}

	//send
	for (int i = 0; i < nM2b; i++) {
		//if (m2b[i].isUsed == true) {
			cout << "M2B ID[" << i << "],coordinates(" << m2b[i].p << ")." << endl;
			myOSCsender(i, m2b[i].p.x, m2b[i].p.y);
		//}

	}
}

void ofApp::myOSCsender(int id, float x, float y) {
	ofxOscMessage m;
	//char ch[25];
	//itoa(id,ch,10);
	string str = "/" + to_string(id);
	m.setAddress(str);
	m.addFloatArg(x);
	m.addFloatArg(y);
	sender.sendMessage(m, false);
}

void ofApp::drawBlobMark(int diffw, int diffh, int w, int h) {
	float rateWidth = (float)w / (float)KNT_WIDTH / 3;
	float rateHeight = (float)h / (float)KNT_HEIGHT / 3;
	for (int i = 0; i<obj.size(); i++) {
		ofPoint p;
		p.x = obj[i].p.x * rateWidth + diffw;
		p.y = obj[i].p.y * rateHeight + diffh;
		ofLine(p.x - 20, p.y, p.x + 20, p.y);
		ofLine(p.x, p.y - 20, p.x, p.y + 20);
	}
}

void ofApp::exit() {
	kinect1.setCameraTiltAngle(0);
	kinect1.close();
	kinect2.setCameraTiltAngle(0);
	kinect2.close();
	kinect3.setCameraTiltAngle(0);
	kinect3.close();

}