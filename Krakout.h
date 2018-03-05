#ifndef KRAKOUT_H
#define KRAKOUT_H

#include "Graph.h"
using namespace TwoDimensions;
#include <vector>

namespace krakout {
	
	
	class Slipper;
	class Ball:public Circle {
	public:
		using Circle::Circle;
		virtual void draw()const;
		void update();
		void get_collison(bool result);
		void get_slipper(const Slipper& slider);
	private:
		bool is_collison=false;//观察者：依赖Ball
		const Slipper* p_slipper = nullptr;//底层const表明指向的对象为const
	};

	class Block:public Rectangle {
	public:
		using Rectangle::Rectangle;
		virtual void update();	
		virtual void draw()const;
	};

	class Slipper :public Rectangle {
	public:
		Slipper(int a, int b, int l1, int w1,int s);
		virtual void update();
		virtual void draw()const;
	private:
		int speed=0;
	};

	class Level {
	public:
		Level(int Block_h, int Block_w);		
		void get_ball(const Ball& ball);
		void draw()const;
		//删除碰撞的砖块
		void update();		
		//从任意关卡开始
		void load(int Stage_number);
	protected:
		void edit();
	private:
		int Block_high = 0;
		int Block_width = 0;
		vector<vector<Block>> Block_editor;
		int level_number=0;//当前的关卡层数
		const Ball* p_ball = nullptr;//观察者：依赖Ball
		vector<Block> Block_t;
	};		

}//namespace krakout

#endif
#pragma once
