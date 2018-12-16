#include "ofApp.h"

/*
    If you are struggling to get the device to connect ( especially Windows Users )
    please look at the ReadMe: in addons/ofxKinect/README.md
*/

//--------------------------------------------------------------
void ofApp::setup() {
   ofSetLogLevel(OF_LOG_VERBOSE);

	//kinect initialization
	myInitKnt(kinect1, 1);
	myInitKnt(kinect2, 2);
	myInitKnt(kinect3, 3);
    myInitKnt(kinect4, 4);
    myInitKnt(kinect5, 5);
    myInitKnt(kinect6, 6);

	KNT_CUT = 20;
	KNT_WIDTH = kinect1.width - KNT_CUT*2; 
	KNT_HEIGHT = kinect1.height - KNT_CUT*2;

	//grayImage initialization
	grayDpthImgKnt1.allocate(KNT_WIDTH, KNT_HEIGHT);//or constant value
	grayDpthImgKnt2.allocate(KNT_WIDTH, KNT_HEIGHT);
	grayDpthImgKnt3.allocate(KNT_WIDTH, KNT_HEIGHT);//or constant value
    grayDpthImgKnt4.allocate(KNT_WIDTH, KNT_HEIGHT);//or constant value
    grayDpthImgKnt5.allocate(KNT_WIDTH, KNT_HEIGHT);
    grayDpthImgKnt6.allocate(KNT_WIDTH, KNT_HEIGHT);//or constant value
    grayDpthImgMerge.allocate(KNT_WIDTH * 3, KNT_HEIGHT * 2);

	ofSetFrameRate(60);

	//GUI
	myInitGUI();
    
    //Video Server
    ofx::HTTP::SimpleIPVideoServerSettings settings;
    
    settings.setHost(HOST);
    settings.setPort(7890);
    
    settings.ipVideoRouteSettings.setMaxClientConnections(2);
    
    server.setup(settings);
    
    server.start();
}

//--------------------------------------------------------------
void ofApp::update() {
	
	ofBackground(100, 100, 100);

	kinect1.update();
	//kinect2.update();
	//kinect3.update();
	//kinect4.update();
	//kinect5.update();
	//kinect6.update();

	//gui work
	kinect1.setCameraTiltAngle(angelKnt1);
	//kinect2.setCameraTiltAngle(angelKnt1);
	//kinect3.setCameraTiltAngle(angelKnt1);
	//kinect4.setCameraTiltAngle(angelKnt1);
	//kinect5.setCameraTiltAngle(angelKnt1);
	//kinect6.setCameraTiltAngle(angelKnt1);
	kinect1.setDepthClipping(nearClipping, farClipping);//not work,ignore
	//kinect2.setDepthClipping(nearClipping, farClipping);//not work,ignore
	//kinect3.setDepthClipping(nearClipping, farClipping);//not work,ignore
	//kinect4.setDepthClipping(nearClipping, farClipping);//not work,ignore
	//kinect5.setDepthClipping(nearClipping, farClipping);//not work,ignore
	//kinect6.setDepthClipping(nearClipping, farClipping);//not work,ignore
	
	// there is a new frame and we are connected
	if(kinect1.isFrameNew()) {
		//filter pixels by constant range from distance
		grayDpthImgKnt1.setFromPixels(filterDpthPoint(kinect1).getPixels());
		grayDpthImgKnt2.setFromPixels(filterDpthPoint(kinect1).getPixels());
		grayDpthImgKnt3.setFromPixels(filterDpthPoint(kinect1).getPixels());
		grayDpthImgKnt4.setFromPixels(filterDpthPoint(kinect1).getPixels());
		grayDpthImgKnt5.setFromPixels(filterDpthPoint(kinect1).getPixels());
		grayDpthImgKnt6.setFromPixels(filterDpthPoint(kinect1).getPixels());
		
		grayDpthImgMerge.setFromPixels(LongMergeImg(
			TransMergeImg(TransMergeImg(grayDpthImgKnt1, grayDpthImgKnt2), grayDpthImgKnt3),
			TransMergeImg(TransMergeImg(grayDpthImgKnt4, grayDpthImgKnt5), grayDpthImgKnt6)).getPixels());
        
        
		
		// load grayscale depth image from the kinect source
        server.send(grayDpthImgMerge.getPixels());
	}
    

}

//--------------------------------------------------------------
void ofApp::draw() {
    grayDpthImgMerge.draw(10, 10, 960, 720);
	gui.draw();	
}



//--------------------------------------------------------------
void ofApp::exit() {
	kinect1.setCameraTiltAngle(0); // zero the tilt on exit
	kinect1.close();
    /*
	kinect2.setCameraTiltAngle(0); // zero the tilt on exit
	kinect2.close();
    kinect3.setCameraTiltAngle(0); // zero the tilt on exit
    kinect3.close();
    kinect4.setCameraTiltAngle(0); // zero the tilt on exit
    kinect4.close();
    kinect5.setCameraTiltAngle(0); // zero the tilt on exit
    kinect5.close();
    kinect6.setCameraTiltAngle(0); // zero the tilt on exit
    kinect6.close();*/
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key) {
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{

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


ofxCvGrayscaleImage ofApp::filterDpthPoint(ofxKinect &knt) {

	int w = KNT_WIDTH;
	int h = KNT_HEIGHT;

	ofxCvGrayscaleImage gryImg;
	gryImg.allocate(w, h);
	ofPixels pxlImg = gryImg.getPixels();

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if ((knt.getDistanceAt(j + 10, i + 10) < maxDepth)
				&& (knt.getDistanceAt(j + 10, i + 10) > minDepth)) {
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
