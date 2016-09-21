#pragma once

#include "Point.h"
#include <windows.h>

class Line{
private:
	Point start;
	Point finish;
	int width;
public:
	Line(Point, Point);
	Line();	
	void draw(HDC);
	void setFinishPoint(Point);
	void setStartPoint(Point);
	Point getStartPoint();
	Point getFinishPoint();
	void update();
	void setWidth(int);
};