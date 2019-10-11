//https://www.geeksforgeeks.org/a-search-algorithm/
//https://dev.to/jansonsa/a-star-a-path-finding-c-4a4h


#include <GL/glut.h>
#include "Graphics.h"
#include <vector>
#include <array>
#include <cfloat>
#include <stack>
#include <iostream>
#include <chrono>
#include <cmath>

using namespace std;

#define X_MAX WINDOW_WIDTH
#define Y_MAX WINDOW_HEIGHT
#define X_STEP 40
#define Y_STEP 40

// map size
const int m = X_MAX / X_STEP;
const int n = Y_MAX / Y_STEP;

struct Node {
	int x;
	int y;
	int parentX;
	int parentY;
	float fcost;
	float gcost;
	float hcost;
};

// Globals, for visuals of course
int VisualMap[m][n] = {};
//Empty = 0
//obstacle = 1
//closed list = 2
//path = 3
//agent = 4
//destination 5

static bool isValid(int x, int y) {
	//	int id = x + y * m;
	//	if ( obstacle[id] == 0 ) {
	if(x == 16 && y > 4) return false;
	return !(x < 0 || y < 0 || x >= m || y >= n); // temp just check oob
	//	}
	//	return false;
}

inline static bool isDestination(int x1, int y1, int x2, int y2) {
	return x1 == x2 && y1 == y2;
}

//three possible distance formulas here
// Manhattan for 4 square movement ??????????????????
// diagonal for 8 square movement ??????????????????
// euclidean for any side movement
// also, just return 0 if Dijkstra
//TODO: write a heuristic that takes the higher disgonal cost into account
inline static float calculateH(int x1, int y1, int x2, int y2) {
	float H = (sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)));
	//float absX = fabs(x1 - x2);
	//float absY = fabs(y1 - y2);
	//float H = (absX>=absY) ? absX : absY;
	//float H = absX+absY;
	return H;
}

	static vector<Node> makePath(array<array<Node, n>, m> map, int x, int y) {
		try {
			cout << "Found a path" << endl;
			stack<Node> path;
			vector<Node> usablePath;

			while (!(map[x][y].parentX == x && map[x][y].parentY == y)
				&& map[x][y].x != -1 && map[x][y].y != -1)
			{
				path.push(map[x][y]);
				int tempX = map[x][y].parentX;
				int tempY = map[x][y].parentY;
				x = tempX;
				y = tempY;

			}
			path.push(map[x][y]);

			while (!path.empty()) {
				Node top = path.top();
				path.pop();
				usablePath.emplace_back(top);
			}
			return usablePath;
		}
		catch(const exception& e){
			cout << e.what() << endl;
		}
	}



static vector<Node> aStar(Node agent, Node dest, bool (&TestList)[m][n]) {
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
	array<array<Node, n>, m> Map{};
	// use simple array, unless i figure out why the fuck this guy used the std array

	for (int x = 0; x < m; ++x) {
		for (int y = 0; y < n; ++y) {
			Map[x][y] = {x,y,-1,-1,FLT_MAX,FLT_MAX,FLT_MAX};
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

	while (!openList.empty() && openList.size() < m * n) {
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

		for (int newX = -1; newX <= 1; newX++) {
			for (int newY = -1; newY <= 1; newY++) { // break when newX == 0 and newY == 0, possibly split this
				if (newX == 0 && newY == 0) continue;
				//if (newX == newY) continue;
				float newG, newF, newH;
				if (isValid(x + newX, y + newY)) {
					TestList[x + newX][y + newY] = true;
					if (isDestination(x + newX, y + newY, dest.x, dest.y)) {
						//Destination found
						Map[x + newX][y + newY].parentX = x;
						Map[x + newX][y + newY].parentY = y;
						destinationFound = true;
						return makePath(Map, dest.x, dest.y);
					} else if (!closedList[x + newX][y + newY]) {
						//if diagonal, higher g cost
						if(newX == newY) newG = node.gcost + 1.4;
						else newG = node.gcost + 1;
						newH = calculateH(x + newX, y + newY, dest.x, dest.y);
						newF = newG + newH;

						// compare if this path is better than the previous one
						if (Map[x + newX][y + newY].fcost == FLT_MAX || Map[x + newX][y + newY].fcost > newF) {
							// update details of this neighbour node
							Map[x + newX][y + newY].fcost = newF;
							Map[x + newX][y + newY].gcost = newG;
							Map[x + newX][y + newY].hcost = newH;
							Map[x + newX][y + newY].parentX = x;
							Map[x + newX][y + newY].parentY = y;
							openList.emplace_back(Map[x + newX][y + newY]);
						}
					}
				}
			}
		}
	}
	cout << "Destination not found." << endl;
	return empty;
}



void drawScene() {
	clearWindow();

	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; j++){
			switch (VisualMap[i][j]) {
				case 0: setColor(DARKZERO);     break;
				case 1: setColor(DARKONE);      break;
				case 2: setColor(DARKTWO);      break;
				case 3: setColor(DARKTHREE);    break;
				case 4: setColor(DARKFOUR);     break;
				case 5: setColor(DARKFIVE);     break;
			}
			drawFilledBox(i * X_STEP + 2, j*Y_STEP + 2, i * X_STEP + X_STEP - 2, j*Y_STEP + Y_STEP - 2);
		}
	}

	glEnd();
	glutSwapBuffers();
}

int main(int argc, char **argv) {

	Node agent{};
	agent.x = 3;
	agent.y = 6;

	Node destination{};
	destination.x = m-2;
	destination.y = n-2;

	auto start = chrono::high_resolution_clock::now();

	//make the closed list here
	bool TestList[m][n] = {};
	auto path = aStar(agent, destination, TestList);

	//build map

	for (int x = 0; x < m; ++x) {
		for (int y = 0; y < n; ++y) {
			if (TestList[x][y])
				VisualMap[x][y] = 2;
			if (x==16&&y>4) VisualMap[x][y] = 1;
		}
	}

	for (Node node : path) {
		VisualMap[node.x][node.y] = 3;
	}

	VisualMap[agent.x][agent.y] = 4;
	VisualMap[destination.x][destination.y] = 5;

	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

	for (Node node : path) {
		cout << node.x << " " << node.y << endl;
	}
	cout << "Time taken (microseconds) -> " << duration.count() << endl;
	graphicsSetup(argc, argv);
	glutDisplayFunc(drawScene);
	glutMainLoop();
}