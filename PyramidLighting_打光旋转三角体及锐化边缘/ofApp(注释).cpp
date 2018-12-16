#include "ofApp.h"

/*一个网格代表一组在三维空间的顶点，并在这些点的法线，
在这些点的颜色和纹理坐标的点。
这些不同的属性中的每一个都存储在一个向量中。
顶点被传递给你的图形卡和你的图形卡填充在它们之间的空间在一个处理通常被称为渲染管道。*/

//--------------------------------------------------------------
void ofApp::setup(){
	//方法一：
	//为mesh添加固定值顶点，并确保他们相互连接，建立索引
	//索引记为index 默认存在，下标由创建顺序确定
	//Pyramid's base vertices with indices 0, 1, 2
	mesh.addVertex(ofPoint(-200, -100, -50));
	mesh.addVertex(ofPoint(200, -100, -50));
	mesh.addVertex(ofPoint(0, 200, 0));

	//设置顶点
	//Pyramid's top vertex with index 3
	mesh.addVertex(ofPoint(0, 0, 50));

	//设置三角形，直接取用index的下标
	//Vertices with indices 3, 2, 0
	mesh.addTriangle(3, 2, 0);
	//Vertices with indices 3, 1, 2
	mesh.addTriangle(3, 1, 2);
	//Vertices with indices 3, 0, 1
	mesh.addTriangle(3, 0, 1);

	//为mesh设立法线，详见函数内部(自定义函数)
	setNormals(mesh);			//Set normals to the surface
	
	//Enabling light source
	light.enable();
	
	/*
	//方法二：
	//设置三角体的底面点
	//Pyramid's base vertices
	ofPoint v0 = ofPoint(-200, -100, 0);
	ofPoint v1 = ofPoint(200, -100, 0);
	ofPoint v2 = ofPoint(0, 200, 0);
	//设置三角体的定点
	//Pyramid's top vertex
	ofPoint v3 = ofPoint(0, 0, 100);

	//通过定点在mesh中加入三角体
	//Add triangles by its vertices
	mesh.addVertex(v3); mesh.addVertex(v2); mesh.addVertex(v0);
	mesh.addVertex(v3); mesh.addVertex(v1); mesh.addVertex(v2);
	mesh.addVertex(v3); mesh.addVertex(v0); mesh.addVertex(v1);
	//自动设置网格上点的索引index
	mesh.setupIndicesAuto();	//Set up indices

	setNormals(mesh);			//Set normals to the surface

	//Enabling light source
	light.enable();
  */
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	//固定写法

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
	float angle = time * 30;			//Rotate angle
	ofRotate(angle, 0, 1, 1);

	ofSetColor(0, 128, 0);			//Set a dark green color
	//绘制网线
	mesh.draw();						//Draw the mesh

	ofPopMatrix();						//Restore the coordinate system
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