#include "ofApp.h"
/*关于ofmesh 一段机翻 供参考
你可能会想：我设置八个顶点，瞧：一个立方体。不那么快。这里有一个问题，
问题是，OpenGL渲染器有不同的连接点，你通过它，没有方法只通过八个顶点来创建多维数据集。
一般来说，你必须创建你的点，以适应你选择的绘图模式。一个顶点被连接到另一个顶点，在秩序的模式做它的曲折，
这意味着你可能需要在一个给定的位置的多个顶点创建你想要的形状。
例如，多维数据集需要18个顶点，而不是你所期望的8个顶点。
输入网格，这实际上只是一个抽象的顶点和绘图模式，我们开始，但有额外的奖金管理的绘制顺序为你。*/
//定义网格heigh wide
int W = 100;		//Grid size
int H = 100;


//--------------------------------------------------------------
void ofApp::setup() {
	image.loadImage("5.png");
	//设置顶点和颜色，
	//Set up vertices and colors
	for (int y = 0; y<H; y++) {
		for (int x = 0; x<W; x++) {
			//已x0 y0为对称轴设置点
			mesh.addVertex(ofPoint((x - W / 2) * 6, (y - H / 2) * 6, 0));
			mesh.addColor(ofColor(0, 0, 0));
		}
	}
	//设置三角形，连接索引index
	//重要的是要确保每个顶点都正确地连接到它周围的其他顶点。这是使用索引（标准写法）
	//通过将索引（即点）addtriangel已达到连接各个索引点的效果
	//Set up triangles' indices
	for (int y = 0; y<H - 1; y++) {
		for (int x = 0; x<W - 1; x++) {
			int i1 = x + W * y;
			int i2 = x + 1 + W * y;
			int i3 = x + W * (y + 1);
			int i4 = x + 1 + W * (y + 1);
			mesh.addTriangle(i1, i2, i3);
			mesh.addTriangle(i2, i4, i3);
		}
	}
	setNormals(mesh);		//Set normals
	light.enable();			//Enable lighting
}

//--------------------------------------------------------------
void ofApp::update() {
	//获取时间作为递增指标
	float time = ofGetElapsedTimef();	//Get time
	
	//实时的改变顶点
	//Change vertices
	for (int y = 0; y<H; y++) {
		for (int x = 0; x<W; x++) {
			//获取索引（只改变部分索引点的坐标）
			int i = x + W * y;			//Vertex index
			//获取索引点的坐标
			ofPoint p = mesh.getVertex(i);

			//获取随机噪声
			//Get Perlin noise value
			float value = ofNoise(x * 0.05, y * 0.05, time * 0.5);

			//改变索引点的z 坐标，并覆盖原有项
			//Change z-coordinate of vertex
			p.z = value * 100;
			mesh.setVertex(i, p);

			//该表索引点颜色
			//Change color of vertex
			mesh.setColor(i,ofColor(value * 255, value * 255, 255));
		}
	}
	setNormals(mesh);	//Update the normals
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
	ofRotate(angle, 0, 0, 1);

	ofSetColor(255, 255, 255);		//Set color
	//绘制纹理
	image.bind();						//Use image's texture for drawing
	mesh.draw();						//Draw mesh
	image.unbind();						//End using image's texture
    
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