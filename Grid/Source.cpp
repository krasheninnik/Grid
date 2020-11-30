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
	Area(int _area, int _x0, int _x1, int _y0, int _y1, int _z0, int _z1):
		area(_area), x0(_x0), x1(_x1), y0(_y0), y1(_y1), z0(_z0), z1(_z1) {};

	int area;
	int x0;
	int x1;
	int y0;
	int y1;
	int z0;
	int z1;
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
	int NWz;
	int sizeWithDivisionX;
	int sizeWithDivisionY;
	int sizeWithDivisionZ;

	std::vector<int> IX;
	std::vector<int> IY;
	std::vector<int> IZ;


	std::vector<Point> pointsMain;
	std::vector<Point> points;
	std::vector<double> zAxis;

	std::vector<Area> areas;
	std::vector<Division> divisionsX;
	std::vector<Division> divisionsY;
	std::vector<Division> divisionsZ;

	void input() {
		std::fstream fin(R"(grid.txt)");
		// Read information about XY main plane
		fin >> NWx >> NWy;
		pointsMain.resize(NWx * NWy);
		for (int i = 0; i < pointsMain.size(); i++) {
			double x, y;
			fin >> x >> y;
			pointsMain[i] = Point(x, y);
		}
		
		// Information about X divisions:
		IX.resize(NWx);	
		divisionsX.resize(NWx-1);
		sizeWithDivisionX = 0;
		for (int i = 0; i < divisionsX.size(); i++) {
			int amount;
			double coef;
			fin >> amount >> coef;
			divisionsX[i] = Division(amount, coef);
			IX[i] = sizeWithDivisionX;
			sizeWithDivisionX += amount;
		}
		IX[IX.size() - 1] = sizeWithDivisionX;
		sizeWithDivisionX += 1;
		
		// Information about Y divisions:
		IY.resize(NWy);
		divisionsY.resize(NWy - 1);
		sizeWithDivisionY = 0;
		for (int i = 0; i < divisionsY.size(); i++) {
			int amount;
			double coef;
			fin >> amount >> coef;
			divisionsY[i] = Division(amount, coef);
			IY[i] = sizeWithDivisionY;
			sizeWithDivisionY += amount;
		}
		IY[IY.size() - 1] = sizeWithDivisionY;
		sizeWithDivisionY += 1;

		// Information about Z axis
		std::vector<double> zAxisMain;
		fin >> NWz;
		zAxisMain.resize(NWz);
		for (int i = 0; i < zAxisMain.size(); i++) {
			double z;
			fin >> z;
			zAxisMain[i] = z;
		}

		// Information about Z divisions:
		IZ.resize(NWz);
		divisionsZ.resize(NWz - 1);
		sizeWithDivisionZ = 0;
		for (int i = 0; i < divisionsZ.size(); i++) {
			int amount;
			double coef;
			fin >> amount >> coef;
			divisionsZ[i] = Division(amount, coef);
			IZ[i] = sizeWithDivisionZ;
			sizeWithDivisionZ += amount;
		}
		IZ[IZ.size() - 1] = sizeWithDivisionZ;
		sizeWithDivisionZ += 1;

		// Information about grid areas;
		int areaNum = 0;
		fin >> areaNum;
		areas.resize(areaNum);
		for (int i = 0; i < areas.size(); i++) {
			int area, x0, x1, y0, y1, z0, z1;
			fin >> area >> x0 >> x1 >> y0 >> y1 >> z0 >> z1;
			areas[i] = Area(area, x0, x1, y0, y1, z0, z1);
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

		// froming main horizontal XY lines:
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


		// froming all verticals XY lines:
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

		// Forming Z axis:
		zAxis.resize(sizeWithDivisionZ);
		int zPos = 0;
		for (int i = 0; i < divisionsZ.size(); i++) {
			zAxis[zPos] = zAxisMain[i];
			auto internalPoints = caluculateInternalPoints(Point(zAxisMain[i], 0), Point(zAxisMain[i+1], 0), divisionsZ[i].amount, divisionsZ[i].coef);
			for (int k = 0; k < internalPoints.size(); k++) {
				zAxis[zPos + k + 1] = internalPoints[k].x;
			}
			zPos += divisionsZ[i].amount;
		}
		// set last Z axis point;
		zAxis[zAxis.size() - 1] = zAxisMain[zAxisMain.size() - 1];

		// Find's elems area:
		for (int zi = 0; zi < zAxis.size() - 1; zi++) {
			for (int yi = 0; yi < sizeWithDivisionY - 1; yi++) {
				for (int xi = 0; xi < sizeWithDivisionX - 1; xi++) {
					int areaNum = 0;
					auto included = [](int inner, int out0, int out1) {return out0 <= inner && inner + 1 <= out1; };
					for (int ai = 0; ai < areas.size(); ai++) {
						auto area = areas[ai];
						bool inX = included(xi, IX[area.x0 - 1], IX[area.x1 - 1]);
						bool inY = included(yi, IY[area.y0 - 1], IY[area.y1 - 1]);
						bool inZ = included(zi, IZ[area.z0 - 1], IZ[area.z1 - 1]);

						if (inX && inY && inZ) {
							areaNum = area.area;
							break;
						}
					}
					// do some calculations for finite elem 
					std::cout << "area: " << areaNum << std::endl;
				}
			}
		}
	}


	void output() {
		// Output XY main plane
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