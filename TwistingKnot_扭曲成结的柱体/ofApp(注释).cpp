#include "ofApp.h"
#include <fstream>

//设置圆的参数
//The circle parameters
float Rad = 25; 			//Raduis of circle
float circleStep = 3;		//Step size for circle motion 圆移动的步数
int circleN = 40; 			//Number of points on the circle 一个圆绘制多少个点

//增加文件流，导出过程中变量的值
ofstream outfile("out.txt");
 
//记录圆的当前状态
//Current circle state
ofPoint pos;				//Circle center 当前圆心
ofPoint axeX, axyY, axyZ;	//Circle's coordinate system 当前坐标系

//--------------------------------------------------------------
void ofApp::setup() {
	//设置圆心和初始坐标
	pos = ofPoint(0, 0, 0);	//Start from center of coordinate
	axeX = ofPoint(1, 0, 0);	//Set initial coordinate system
	axyY = ofPoint(0, 1, 0);
	axyZ = ofPoint(0, 0, 1);

	light.enable();				//Enable lighting

	//设置屏幕重绘率,刷新速率
	ofSetFrameRate(60);       //Set the rate of screen redrawing
}

//--------------------------------------------------------------
void ofApp::update() {
	addRandomCircle(mesh);
}

//--------------------------------------------------------------
void ofApp::draw() {
	//Z-buffering是在为物件进行着色时，执行“隐藏面消除”工作的一项技术，所以隐藏物件背后的部分就不会被显示出来
	ofEnableDepthTest();//Enable z-buffering

	//设置一个渐变背景，从白色到灰色									
	ofBackgroundGradient(ofColor(255), ofColor(128));//Set a background

	//存储坐标系(标准写法)，push入栈
	ofPushMatrix();						//Store the coordinate system

	//设置画面中心为坐标系原点
	//Move coordinate center to screen's center
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);

	//为图形设置旋转效果，设置每时的旋转角度
	//Rotate the coordinate system
	//获取时间，作为递增指标(也可取其他)
	float time = ofGetElapsedTimef();	//Get time in seconds
	//设置当前角度的取值，因time递增，所以angle为顺时针递增
	float angle = time * 20;			//Rotate angle
	//旋转
	ofRotate(angle, 0, 1, 0);//along y-axe

	//将坐标中心移动，以便网格将被绘制在屏幕中心
	//Shift the coordinate center so the mesh
	//will be drawn in the screen center
	ofTranslate(-mesh.getCentroid());

	mesh.draw();						//Draw mesh
    
	ofPopMatrix();						//Restore the coordinate system
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

//Universal function which sets normals for the triangle mesh
void setNormals(ofMesh &mesh) {

	//获取定点个数
	//The number of the vertices
	int nV = mesh.getNumVertices();

	//获取三角形个数
	//The number of the triangles
	int nT = mesh.getNumIndices() / 3;

	//定义一个向量用于存储法线 法向量
	vector<ofPoint> norm(nV); //Array for the normals
							  //扫描所有的三角形。为每一个三角形添加法线，并存入法线的向量
							  //Scan all the triangles. For each triangle add its normal to norm's vectors of triangle's vertices
	for (int t = 0; t<nT; t++) {

		//取得三角形各个点的索引值index
		//Get indices of the triangle t
		int i1 = mesh.getIndex(3 * t);
		int i2 = mesh.getIndex(3 * t + 1);
		int i3 = mesh.getIndex(3 * t + 2);

		//取得三角形各个点的坐标值
		//Get vertices of the triangle
		const ofPoint &v1 = mesh.getVertex(i1);
		const ofPoint &v2 = mesh.getVertex(i2);
		const ofPoint &v3 = mesh.getVertex(i3);

		//计算三角形的法向量，叉乘，并调用归一函数normalized，归一化
		//Compute the triangle's normal
		ofPoint dir = ((v2 - v1).crossed(v3 - v1)).normalized();

		//将索引作为下标，把每个三角形的法向量，存入与其相关的索引点的法线向量中
		//因为每个索引点可能被多个三角形复用，所以向量累加
		//Accumulate it to norm array for i1, i2, i3
		norm[i1] += dir;
		norm[i2] += dir;
		norm[i3] += dir;
	}

	//遍历法线向量，并做归一处理
	//Normalize the normal's length
	for (int i = 0; i<nV; i++) {
		norm[i].normalize();
	}

	//清除mesh各个索引点法向量，set新的
	//Set the normals to mesh
	mesh.clearNormals();
	mesh.addNormals(norm);
}

void ofApp::addRandomCircle(ofMesh &mesh) {
	//获取当前时间
	float time = ofGetElapsedTimef();	//Time

	//设置扭曲的角度、旋转的角度、和颜色
	//Parameters :twisting and rotating angles and color
	float twistAngle = 5.0 * ofSignedNoise(time * 0.3 + 332.4);
	float rotateAngle = 1.5;
	ofFloatColor color(ofNoise(time * 0.05),ofNoise(time * 0.1),ofNoise(time * 0.15));

	//设置色彩饱和度
	color.setSaturation(1.0);			//Make the color maximally colorful

	//文件流输出，便于确定过程中变量的值
	outfile << "旋转前  ";
	outfile << "axeX:" << axeX;
	outfile << ",axyY:" << axyY;
	outfile << ",axyZ:" << axyZ;
	outfile << ",pos:" << pos << endl;

	//旋转圆的坐标轴，根据twistAngle，rotateAngle合成一个最终的旋转角度
	//均为坐标变量，在圆的坐标轴旋转后，其起始点的值也会发生改变
	//将(1,0,0)沿着Z轴旋转30度，则会得到一个新的坐标(X,X,X)(懒得计算)
	//Rotate the coordinate system of the circle
	//axeX沿着Z轴扭曲，axyY沿着Y轴扭曲
	axeX.rotate(twistAngle, axyZ); 
	axyY.rotate(twistAngle, axyZ);
	//axeX沿着Y轴旋转，axyZ沿着Y轴旋转
	axeX.rotate(rotateAngle, axyY);
	axyZ.rotate(rotateAngle, axyY);

	//使圆心在Z轴上处于递增的情况,axyZ也处于变化中，所以pos的变化不是等量的
	//Move the circle on a step
	ofPoint move = axyZ * circleStep;
	pos += move;

	//文件流输出，便于确定过程中变量的值
	outfile << "旋转后  ";
	outfile << "axeX:" << axeX;
	outfile << ",axyY:" << axyY;
	outfile << ",axyZ:" << axyZ;
	outfile << ",pos:" << pos << endl;

	//根据上述新得到的点，计算圆上的其他点
	//Add vertices
	for (int i = 0; i<circleN; i++) {
		//计算其他圆上的点的坐标
		float angle = float(i) / circleN * TWO_PI;
		float x = Rad * cos(angle);
		float y = Rad * sin(angle);

		
		//使每个圆不那么圆润,ofMap的作用暂时还不知道，得到一个变化的系数distort
		//We would like to distort this point
		//to make the knot's surface embossed
		float distort = ofNoise(x * 0.2, y * 0.2, time * 0.2 + 30);
		//outfile << "distort:"<< distort;
		distort = ofMap(distort, 0.2, 0.8, 0.8, 1.2);
		//outfile << ",distort ofMap:" <<distort<<endl;
		
		x *= distort;
		y *= distort;

		//计算的新的点，加入mesh
		ofPoint p = axeX * x + axyY * y + pos;
		mesh.addVertex(p);
		mesh.addColor(color);
	}
	
	//如下这段代码的作用可以概括成：为mesh表面平滑的填上色彩
	//不再新增点，而是为这些索引点建立连接
	//Add the triangles
	int base = mesh.getNumVertices() - 2 * circleN; //确保绘制的不是第一个圆
	if (base >= 0) {	//Check if it is not the first step
						//and we really need to add the triangles
		//连接各个点，写法不止如下一种
		for (int i = 0; i<circleN; i++) {
			int a = base + i;
			int b = base + (i + 1) % circleN;
			int c = circleN + a;
			int d = circleN + b;
			mesh.addTriangle(a, b, d);	//Clock-wise
			mesh.addTriangle(a, d, c);
		}
		//Update the normals
		setNormals(mesh);
	}

}