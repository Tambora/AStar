#include <GL/glut.h>
#include "Graphics.h"
#include <vector>
#include <array>
#include <cfloat>

using namespace std;

#define X_MAX WINDOW_WIDTH
#define Y_MAX WINDOW_HEIGHT
#define X_STEP 80
#define Y_STEP 80

// map size
const int n = X_MAX/X_STEP;
const int m = Y_MAX/Y_STEP;



struct vec2 {
	int x,y;
	//explicit vec2 (int x = 0, int y = 0) {this->x = x; this->y = y;} // learn explicit, also is it necessary?
};

struct Node {
	int x;
	int y;
	int parentX;
	int parentY;
	float gcost;
	float fcost;
	float hcost;
};


static vector<Node> aStar (Node agent, Node dest) {
	vector<Node> empty;

	// check if destination is an obstacle or oob

	// check if the agent and destination and player are in the same spot

	bool closedList[n][m];
	//initialize the map
	// Node map[n][m]

	array<array <Node, m>, n> Map;
	// use simple array, unless i figure out why the fuck this guy used the std array

	for (int x = 0; x < n; ++x) {
		for (int y = 0; y < m; ++y) {
			Map[x][y].fcost = FLT_MAX;
			Map[x][y].gcost = FLT_MAX;
			Map[x][y].hcost = FLT_MAX;
			Map[x][y].parentX = -1;
			Map[x][y].parentY = -1;
			Map[x][y].x = x;
			Map[x][y].y = y;

			closedList[x][y] = false;
		}
	}

	//Initialize starting list
	int x = agent.x;
	int y = agent.y;
	Map[x][y].fcost = 0.0;
	Map[x][y].gcost = 0.0;
	Map[x][y].hcost = 0.0;
	Map[x][y].parentX = x;
	Map[x][y].parentY = y;

	


}

















void drawScene() {
     clearWindow();
     setColor(DARKTWO);
	for (int i = 0; i < X_MAX; i += X_STEP) {
		for (int j = 0; j < Y_MAX; j += Y_STEP) {
            drawFilledBox(i+2,j+2,i + X_STEP - 2,j + Y_STEP - 2);
		}
	}

     glEnd();
     glutSwapBuffers();
   }

int main(int argc, char ** argv) {
     graphicsSetup(argc, argv);
     glutDisplayFunc(drawScene);
     glutMainLoop();
}