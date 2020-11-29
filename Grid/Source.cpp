#include <iostream>
#include <fstream>
#include <vector>
#include <functional>

class Point {
public: 
	Point() = default;
	Point(double _x, double _y) : x(_x), y(_y) {};

	double x;
	double y;
};

class Area {
public: 
	Area() = default;
	Area(int _area, int _left, int _right, int _bottom, int _top):
		area(_area), left(_left), right(_right), bottom(_bottom), top(_top) {};

	int area;
	int left;
	int right;
	int bottom;
	int top;
};

class Division {
public:
	Division() = default;
	Division(int _amount, double _coef) : amount(_amount), coef(_coef) {};
	int amount;
	double coef;
};

class Grid {
public: 
	int NWx;
	int NWy;
	int sizeWithDivisionX;
	int sizeWithDivisionY;
	std::vector<Point> pointsMain;
	std::vector<Point> points;

	std::vector<Area> areas;
	std::vector<Division> divisionsX;
	std::vector<Division> divisionsY;


	void input() {
		std::fstream fin(R"(grid.txt)");
		fin >> NWx >> NWy;
		pointsMain.resize(NWx * NWy);
		for (int i = 0; i < pointsMain.size(); i++) {
			double x, y;
			fin >> x >> y;
			pointsMain[i] = Point(x, y);
		}
		

		divisionsX.resize(NWx-1);
		sizeWithDivisionX = 0;
		for (int i = 0; i < divisionsX.size(); i++) {
			int amount;
			double coef;
			fin >> amount >> coef;
			divisionsX[i] = Division(amount, coef);
			sizeWithDivisionX += amount;
		}
		sizeWithDivisionX += 1;

		divisionsY.resize(NWy - 1);
		sizeWithDivisionY = 0;
		for (int i = 0; i < divisionsY.size(); i++) {
			int amount;
			double coef;
			fin >> amount >> coef;
			divisionsY[i] = Division(amount, coef);
			sizeWithDivisionY += amount;
		}
		sizeWithDivisionY += 1;

		int areaNum = 0;
		fin >> areaNum;
		areas.resize(areaNum);
		for (int i = 0; i < areas.size(); i++) {
			int area, bottom, top, left, right;
			fin >> area >> left>> right>> bottom>> top;
			areas[i] = Area(area, left, right, bottom, top);
		}
		std::cout << areas.size();

		// forming grid with division:
		points.resize(sizeWithDivisionX * sizeWithDivisionY);
		// consider main Points:
		int yLvl = 0;
		for (int j = 0; j < NWy; j++) {
			int xPos = 0;
			for (int i = 0; i < NWx; i++) {
				points[yLvl * sizeWithDivisionX + xPos] = pointsMain[j* NWx + i];
				// calculate next xPos:
				if(i != NWx - 1) xPos += divisionsX[i].amount;
			}
			// calculate next yLvl:
			if (j != NWy - 1) yLvl += divisionsY[j].amount;
		}

		auto caluculateInternalPoints = [](Point A, Point B, int amount, double coef) {
			double xLength = B.x - A.x;
			double yLength = B.y - A.y;

			if (coef < 0) coef = -1.0 / coef;

			double xStep = coef == 1 ? xLength / amount : xLength * (coef - 1) / (pow(coef, amount) - 1);
			double yStep = coef == 1 ? yLength / amount : yLength * (coef - 1) / (pow(coef, amount) - 1);

			Point temp = Point(A.x, A.y);
			std::vector<Point> internalPoints(amount - 1);
			for (int i = 0; i < internalPoints.size(); i++) {
				temp.x += xStep;
				temp.y += yStep;
				xStep *= coef;
				yStep *= coef;
				internalPoints[i] = Point(temp.x, temp.y);
			}
			return internalPoints;
		};

		//auto temp = caluculateInternalPoints(Point(0, 0), Point(0, 14), 3, 2);

		// froming main horizontal lines:
		yLvl = 0;
		for (int j = 0; j < NWy; j++) {
			int xPos = 0;
			int xNextPos = 0;
			for (int i = 0; i < NWx - 1; i++) {
				xNextPos += divisionsX[i].amount;
				auto internalPoints = caluculateInternalPoints(pointsMain[j * NWx + i], pointsMain[j * NWx + i + 1], divisionsX[i].amount, divisionsX[i].coef);
				int pointIndex = xPos + 1;
				for (int k = 0; k < internalPoints.size(); k++) {
					points[yLvl * sizeWithDivisionX + pointIndex++] = internalPoints[k];
				}
				// calculate next xPos:
				xPos = xNextPos;
			}
			// calculate next yLvl:
			if (j != NWy - 1) yLvl += divisionsY[j].amount;
		}


		// froming all verticals lines:
		yLvl = 0;
		int yNextLvl = 0;
		for (int j = 0; j < divisionsY.size(); j++) {
			yNextLvl += divisionsY[j].amount;
			for (int i = 0; i < sizeWithDivisionX; i++) {
				auto internalPoints = caluculateInternalPoints(points[yLvl * sizeWithDivisionX + i], points[yNextLvl * sizeWithDivisionX + i], divisionsY[j].amount, divisionsY[j].coef);
				int yLvlIncreemnt = 1;
				for (int k = 0; k < internalPoints.size(); k++) {
					points[(yLvl + yLvlIncreemnt++) * sizeWithDivisionX + i] = internalPoints[k];
				}
			}
			// calculate next yLvl:
			yLvl = yNextLvl;
		}

		std::cout << "hell";
	}
	void output() {
		std::fstream fout("output.txt");
		fout << sizeWithDivisionX << " " << sizeWithDivisionY << std::endl;
		for (auto p: points) {
			fout << p.x << " " << p.y << std::endl;
		}
	}
};


int main() {
	Grid g;
	g.input();
	g.output();
}