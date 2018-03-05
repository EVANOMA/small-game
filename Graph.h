#ifndef GRAPH_H
#define GRAPH_H

#include <graphics.h>// ���� EasyX ͼ�ο�
#include <conio.h>// ����̨�������ͷ�ļ���getch()�����Ҫ

namespace TwoDimensions{
	
	//ͳһ�涨��άͼ�ε�X�᷽������Y�᷽�����
	bool intersect(int y1, int y2, int r, int high);
	class Rectangle;
	class Circle {
		friend bool collison(const Circle& C, const Rectangle& R);
	public:
		Circle(int a, int b, int c, int d, int f);
		//�������
		void check()const;
		virtual void update() = 0;
		virtual void draw()const;
		void clear();
	protected:
		int x = 0;
		int y = 0;
		int r = 0;
		int speedX = 0;
		int speedY = 0;
	};

	class Rectangle {
		friend bool collison(const Circle& C, const Rectangle& R);
	public:
		Rectangle(int a, int b, int l1, int w1);
		//�������
		virtual void update();
		//�������������ɫ
		virtual void draw()const;
		void clear();
		void change(int d1, int d2);
		void change_to(int x1, int y1);
		int visit_longth()const;
		int visit_width()const;
		void change_width(int d);
	protected:
		int width = 0;
		int high = 0;
		//��������
		int x = 0;
		int y = 0;
	};
}//namespace TwoDimensions

#endif
#pragma once
