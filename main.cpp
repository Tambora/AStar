//https://www.geeksforgeeks.org/a-search-algorithm/
//https://dev.to/jansonsa/a-star-a-path-finding-c-4a4h


#include <GL/glut.h>
#include "Graphics.h"
#include <vector>
#include <array>
#include <cfloat>
#include <iostream>
#include <stack>

using namespace std;

#define X_MAX WINDOW_WIDTH
#define Y_MAX WINDOW_HEIGHT
#define X_STEP 80
#define Y_STEP 80

// map size
const int n = X_MAX/X_STEP;
const int m = Y_MAX/Y_STEP;

struct Node {
	int x;
	int y;
	int parentX;
	int parentY;
	float fcost;
	float gcost;
	float hcost;
};


static bool isValid(int x, int y) {
//	int id = x + y * m;
//	if ( obstacle[id] == 0 ) {
		return !(x < 0 || y < 0 || x >= m || y >= n); // temp just check oob
//	}
//	return false;
}

inline static bool isDestination(int x1, int y1, int x2, int y2) {
	return x1 == x2 && y1 == y2;
}

//three possible distance formulas here
// Manhattan for 4 square movement
// diagonal for 8 square movement
// euclidean for any side movement
// also, just return 0 if Dijkstra
inline static float calculateH(int x1, int y1, int x2, int y2) {
	return 0;
}

static vector<Node> makePath(array<array<Node, n>, m> map, int x, int y) {
	try {
		cout << "Path found." << endl;
		stack<Node> path;
		vector<Node> usablePath;

		while (!(map[x][y].parentX == x && map[x][y].parentY == y) && map[x][y].x != -1 && map[x][y].y != -1) {
			path.push(map[x][y]);
			int tempX = map[x][y].parentX;
			int tempY = map[x][y].parentY;
			x = tempX;
			y = tempY;
		}
		path.push(map[x][y]);

		while(!path.empty()) {
			Node top = path.top();
			path.pop();
			usablePath.emplace_back(top);
		}
		return usablePath;
	} catch (const exception& e) {
		cout << e.what() << endl;
	}

}

static vector<Node> aStar (Node agent, Node dest) {
	vector<Node> empty;

	// check if destination is an obstacle or oob
	if (!isValid(dest.x, dest.y)) {
		cout << "Destination is an obstacle" << endl;
		return empty;
		//Destination is invalid
	}
	// check if the agent and destination and player are in the same spot
	if (isDestination(agent.x, agent.y, dest.x, dest.y)) {
		cout << "You are the destination" << endl;
		return empty;
		//You clicked on yourself
	}

	bool closedList[m][n];
	//initialize the map
	// Node map[n][m]

	array<array <Node, n>, m> Map{};
	// use simple array, unless i figure out why the fuck this guy used the std array

	for (int x = 0; x < m; ++x) {
		for (int y = 0; y < n; ++y) {
			Map[x][y].x = x;
			Map[x][y].y = y;
			Map[x][y].parentX = -1;
			Map[x][y].parentY = -1;
			Map[x][y].fcost = FLT_MAX;
			Map[x][y].gcost = FLT_MAX;
			Map[x][y].hcost = FLT_MAX;
	        //Map[x][y] = {x,y,-1,-1,FLT_MAX,FLT_MAX,FLT_MAX};
			closedList[x][y] = false;
		}
	}

	//Initialize starting list

	// source
	int x = agent.x;
	int y = agent.y;
	Map[x][y].parentX = x;
	Map[x][y].parentY = y;
	Map[x][y].fcost = 0.0;
	Map[x][y].gcost = 0.0;
	Map[x][y].hcost = 0.0;


	vector<Node> openList;
	openList.emplace_back(Map[x][y]);
	bool destinationFound = false;

	while(!openList.empty() && openList.size() < m * n) {
		Node node{};
		do {
			// see the first one
			float temp = FLT_MAX;
			vector<Node>::iterator itNode; //TODO:review iterator
			for (auto it = openList.begin(); it != openList.end(); it = next(it)) {
				Node nit = *it;
				if (nit.fcost < temp) {
					temp = nit.fcost;
					itNode = it;
				}
			}
			node = *itNode;
			openList.erase(itNode);
		} while (!isValid(node.x, node.y));

		x = node.x;
		y = node.y;
		closedList[x][y] = true;

		//all the directions

		for (int newX = -1; newX <= 1; ++newX) {
			for (int newY = -1; newY <= 1; ++newY) { // break when newX == 0 and newY == 0, possibly split this
				 float newG, newF, newH;
				 if (isValid(x+newX, y+newY)) {
				 	if (isDestination(x+newX, y+newY, dest.x, dest.y)){
				 		//Destination found
				 		Map[x+newX][y+newY].parentX = x;
				 		Map[x+newX][y+newY].parentY = y;
				 		destinationFound = true;
				 		return makePath(Map, dest.x, dest.y);
				 	} else if(!closedList[x + newX][y + newY]) {
				 		newG = node.gcost + 1.0;
				 		newH = calculateH(x+newX, y+newY, dest.x, dest.y);
				 		newF = newG + newH;

				 		// compare if this path is better than the previous one
				 		if (Map[x+newX][y+newY].fcost == FLT_MAX || Map[x+newX][y+newY].fcost > newF) {
				 			// update details of this neighbour node
							Map[x + newX][y + newY].fcost = newG;
							Map[x + newX][y + newY].gcost = newH;
							Map[x + newX][y + newY].hcost = newF;
							Map[x + newX][y + newY].parentX = x;
							Map[x + newX][y + newY].parentY = y;
							openList.emplace_back(Map[x + newX][y + newY]);
				 		}
				 	}
				 }
			}
		}
	}
	if(!destinationFound) {
		cout << "Destination not found." << endl;
		return empty;
	}
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

	Node player{};
    player.x = 7;
    player.y = 7;

    Node destination{};
    destination.x = 1;
    destination.y = 1;

    for (Node node : aStar(player, destination)) {
    	cout << node.x << " " << node.y << endl;
    }
     graphicsSetup(argc, argv);
     glutDisplayFunc(drawScene);
     glutMainLoop();
}