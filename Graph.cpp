#include<iostream>
#include<stdlib.h>
using namespace std;

#include "Graph.h"

namespace TwoDimensions {
	
	bool intersect(int z1, int z2, int r, int d)
	{
		int a = z1 - r;
		int c = z2 - d / 2;
		if (a == c) return true;
		else if (a < c) {
			int b = z1 + r;
			if (b >= c) return true;
		}
		else {
			int f = z2 + d / 2;
			if (f >= a) return true;
		}
		return false;
	}
	//碰撞检测
	bool collison(const Circle& C, const Rectangle& R)
	{
		//通过两个坐标轴的投影是否重叠判断相交
		//每个坐标轴分2种情况讨论
		if (intersect(C.y + C.speedY, R.y, C.r, R.high))
			return intersect(C.x + C.speedX, R.x, C.r, R.width);
		else return false;
	}



	Circle::Circle(int a, int b, int c, int d, int f) :
		x(a), y(b), r(c), speedX(d), speedY(f)
	{}

	void Circle::check()const
	{
		cout << "x= " << x << ", " << "y= " << y << endl;
	}

	void Circle::draw()const
	{
		//圆心(100, 100)
		fillcircle(x, y, r);
	}

	void Circle::clear()
	{
		//绘制黑色填充的圆
		setcolor(BLACK);
		setfillcolor(BLACK);
		fillcircle(x, y, r);
		/*
		clearcircle(x, y, r);
		可用于轨迹追踪
		*/
	}

	Rectangle::Rectangle(int a, int b, int l1, int w1) :
		x(a), y(b), high(l1), width(w1)
	{}

	void Rectangle::update()
	{}

	void Rectangle::change_width(int d)
	{
		width += d;
		if(width<0) width = 0;
	}

	//通过宽高画出中心距离矩形的点
	void Rectangle::draw()const
	{
		int bar_left = x - width / 2;
		int bar_right = x + width / 2;
		int bar_top = y - high / 2;
		int bar_bottom = y + high / 2;
		//cout<< bar_left<< bar_top<< bar_righ		
		bar(bar_left, bar_top, bar_right, bar_bottom);
	}

	void Rectangle::clear()
	{
		//绘制黑色填充的圆
		setcolor(BLACK);
		setfillcolor(BLACK);
		Rectangle::draw();
	}

	void Rectangle::change(int d1, int d2)
	{
		x += d1;
		y += d2;
	}

	void Rectangle::change_to(int x1, int y1)
	{
		x = x1;
		y = y1;
	}

	int Rectangle::visit_longth()const
	{
		return high;
	}

	int Rectangle::visit_width()const
	{
		return width;
	}

}//namespace TwoDimensions