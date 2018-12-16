#pragma once

/*
This is motion detection example, which uses ofxOpenCv addon.
It calculates of absolute differences between successive frames of video,
accumulates them in the buffer, and finally computes motion areas
using thresholding.
Images on the screen:
[Original video         ] [Difference image]
[Accumulated differences]	[Motion areas    ]

--------------------------------------
How to make openFrameworks project:
- Use Project generator for creating an empty project with linked ofxOpenCv addon.
- Copy this sources to src folder of the project.
- Copy movie handsTrees.mov into bin/data folder of the project.
--------------------------------------

It's the example 01-MotionDetection from the book
"Mastering openFrameworks: Creative Coding Demystified",
Chapter 9 - Computer Vision with OpenCV
*/
/*关于涉及的类
ofxCvColorImage类代表三通道（红、绿、蓝）用unsigned char类型的颜色分量的彩色图像。
这些图像是从相机获得的。
ofxCvGrayscaleImage类代表一个通道的图像灰度，用unsigned char类型的像素值。
这些图像被用于内部处理，如阈值和轮廓的发现，作为一个类的工作与二进制图像，仅包含两个像素值（0和255）。
ofxCvFloatImage类代表一个通道的灰度，图像像素值的类型。
它被用于在需要精确的计算的情况下，例如，用于平滑和傅立叶变换的特殊滤波器。
ofxCvShortImage类表示灰度，一个通道图像的像素值类型unsigned short int
因此，像素值的范围是从0到65535。这样的图像是从深度相机获得的像素值意味着相应的距离在毫米。
ofxCvImage类是所有前面的图像类。
它包含了大部分图像处理功能的声明，所以你可以探索它们。
注意，你不应该直接声明这个类的对象，因为它有一个抽象的函数，即，声明的函数，但没有定义。
因此，与该类的对象调用这个类的原因ofxcvimage执行错误。
*/

#include "ofMain.h"
#include "ofxOpenCv.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		ofVideoPlayer video;		//Declare the video player object
		//ofxCvColorImage类代表三通道（红、绿、蓝）用unsigned char类型的颜色分量的彩色图像。这些图像是从相机获得的。
		ofxCvColorImage image;		//The current video frame

		//当前和以前的视频帧作为灰度图像
		//The current and the previous video frames as grayscale images
		//ofxCvGrayscaleImage类代表一个通道的图像灰度，阈值和轮廓的发现，仅包含两个像素值（0和255）
		ofxCvGrayscaleImage grayImage, grayImagePrev;
		ofxCvGrayscaleImage diff;		//Absolute difference of the frames
		//ofxCvFloatImage类代表一个通道的灰度，它被用于在需要精确的计算的情况下，例如，用于平滑和傅立叶变换的特殊滤波器。
		ofxCvFloatImage diffFloat;		//Amplified difference images
		ofxCvFloatImage bufferFloat;	//Buffer image

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);

		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
