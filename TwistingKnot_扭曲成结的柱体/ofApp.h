#pragma once

#include "ofMain.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		ofMesh mesh;
		ofLight light;
		
		//设置成员函数
		//实现移动圆和加入三角形
		void addRandomCircle(ofMesh &mesh); //Main function which
											//moves circle and adds triangles to the object

		

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
		
};


//Universal function which sets normals
//for the triangle mesh
void setNormals(ofMesh &mesh);
