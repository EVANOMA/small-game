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
	//��ײ���
	bool collison(const Circle& C, const Rectangle& R)
	{
		//ͨ�������������ͶӰ�Ƿ��ص��ж��ཻ
		//ÿ���������2���������
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
		//Բ��(100, 100)
		fillcircle(x, y, r);
	}

	void Circle::clear()
	{
		//���ƺ�ɫ����Բ
		setcolor(BLACK);
		setfillcolor(BLACK);
		fillcircle(x, y, r);
		/*
		clearcircle(x, y, r);
		�����ڹ켣׷��
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

	//ͨ����߻������ľ�����εĵ�
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
		//���ƺ�ɫ����Բ
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