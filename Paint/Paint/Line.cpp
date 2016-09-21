#include "stdafx.h"
#include "Line.h"
#include "Point.h"
#include <windows.h>


Line::Line(Point start, Point finish){
	this->start = start;
	this->finish = finish;
	width = 5;
}

Line::Line(){
	update();
	width = 1;
}

void Line::draw(HDC hdc){
	if (start.x < 0){
		return;
	}	
	HPEN pen = CreatePen(0, width, RGB(0,15,0));
	SelectObject(hdc, pen);
	MoveToEx(hdc, start.x, start.y, NULL); //сделать текущими координаты x1, y1
    LineTo(hdc, finish.x, finish.y);
}

void Line::setFinishPoint(Point point){
	finish = point;
}

void Line::setStartPoint(Point point){
	start = point;
}

Point Line::getFinishPoint(){
	return finish;
}

Point Line::getStartPoint(){
	return start;
}

void Line::update(){
	start.x = -10;
	finish.x = -10;
}

void Line::setWidth(int width){
	this->width = width;
}