#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	video.loadMovie("fruits.mov");	//Load the video file
	video.play();					//Start the video to play	
	drawMode = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
	video.update();	//Decode the new frame if needed

	//Do computing only if the new frame was obtained
	if ( video.isFrameNew() ) {
		//Getting a new frame
		image.setFromPixels( video.getPixelsRef() );

		//抽取图像加速处理(通过压缩图像得到？)
		//Decimate image for speed-up processing
		if ( !imageDecimated.bAllocated ) {
			imageDecimated.allocate( image.width * 0.5,
				image.height * 0.5 );
		}
		imageDecimated.scaleIntoMe( image, CV_INTER_NN );

		//转为灰度图像
		//Convert to grayscale image
		grayImage = imageDecimated;	

		//执行平滑抑制噪声
		//模糊的使用高斯模糊。高斯模糊通常是减少图像噪声和减少细节
		//Smoothing image
		blurred = grayImage;
		blurred.blurGaussian( 9 );

		//取第一帧作为背景，前提是视频起始，运动物体不在画中
		//Store first frame to background image
		if ( !background.bAllocated ) {
			background = blurred;	
		}

		//差分法，获取差异，存入diff
		//Find difference of the frame and background
		diff = blurred;
		diff -= background;

		//增加图像的对比度。该值是应用于图像的阈值级别，
		//执行阈值，以获得一个二进制图像，其中的白色区域对应于在原始图像上的明亮的对象。
		//Thresholding for obtaining binary image
		mask = diff;
		mask.threshold( 40 );
		//Here value 40 is the threshold parameter. 
		//It should be adjusted for good results when using another videos than in example.

		//获取高亮斑点的中心坐标
		//灰度图像，将寻找斑点。参数(灰度像素图,最小斑点像素，最大斑点像素，最大斑点数量，斑点是否无孔)
		//Find contours
		contourFinder.findContours( mask, 10, 10000, 20, false );

		//将坐标存入
		//Store objects' centers
		vector<ofxCvBlob>  &blobs = contourFinder.blobs;
		int n = blobs.size();	//Get number of blobs
		obj.resize( n );		//Resize obj array
		for (int i=0; i<n; i++) {
			obj[i] = blobs[i].centroid;	//Fill obj array
		}

		//渲染
		//Additional rendering - updates img if needed
		if ( drawMode == 1 ) {
			generateImg( mask.width, mask.height );
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if ( drawMode == 0 ) {
		ofBackground(255, 255, 255);	//Set the background color
		int w = imageDecimated.width;
		int h = imageDecimated.height;
		ofSetColor( 255, 255, 255 );	//Set color for images drawing

		//Original decimated image 左上
		imageDecimated.draw(0, 0, w, h);

		//Difference 右上
		diff.draw( w+10, 0, w, h );

		//Thresholded 左下
		//mask.draw( 0, h+10, w, h );
		background.draw(0, h + 10, w, h);

		//Draw found contours 左下
		contourFinder.draw( 0, h+10, w, h );


		//Draw centers over decimated image 右下
		imageDecimated.draw(w+10, h+10, w, h);
		ofSetColor( 255, 255, 0 );
		ofSetLineWidth( 2 );
		
		//为其描绘十字线
		for (int i=0; i<obj.size(); i++) {
			ofPoint p = obj[i] + ofPoint( w+10, h+10 );
			ofLine( p.x - 20, p.y, p.x + 20, p.y );
			ofLine( p.x, p.y - 20, p.x, p.y + 20 );
		}
		ofSetLineWidth( 1 );
	}
	else {
		//渲染
		//Additional rendering - draw generated image over original video
		ofEnableAlphaBlending();
		ofSetColor( 255, 255, 255 );
		video.draw( 0, 0 );
		img.draw( 0, 0, video.getWidth(), video.getHeight() );
	}
}

//--------------------------------------------------------------
//渲染效果的实现（已斑点为中心绘制同心圆）
//Additional rendering
void ofApp::generateImg( int w, int h )
{
	if ( !img.bAllocated() ) {
		img.allocate( w, h, OF_IMAGE_COLOR_ALPHA );
	}

	ofPixels &pixels = img.getPixelsRef();

	//Generate image using object's centers
	for ( int y=0; y<h; y++) {
		for ( int x=0; x<w; x++ ) {
			float step = 15.0; //15;	
			float f = 0.;
			//obj 存储斑点像素中心
			for (int i=0; i<obj.size(); i++) {
				ofPoint p = obj[i];
				float rad = 60;
				float val = 0;	
				//计算两点间的二维距离ofDist
				float dist = ofDist( p.x, p.y, x, y );	

				//Circle function
				val = max( rad - abs( dist-rad ), 0.0f ); 		//2-peaks

				//Uncomment to try other shape:
				//Square function
				//val = max(abs( x - p.x ), abs( y - p.y ));

				f += val;
			}

			float val = fmodf( f, step * 2 ) / step;	
			val = 1-abs( 1-val );
			val = ofMap( val, 0.9, 1.0, 0, 255, true );
			pixels.setColor( x, y, ofColor( 255, 255, 255, val ) );
		}	
	}
	img.update();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if ( key == '1' ) {
		drawMode = 0;
	}
	if ( key == '2' ) {
		drawMode = 1;
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
