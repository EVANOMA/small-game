#ifndef FLAPPYBIRD_H
#define FLAPPYBIRD_H

#include "Graph.h"
#include <Math.h>
using namespace TwoDimensions;
#include <vector>
#include <list>

namespace FlappyBird {

	void test(int t);

	class Bird;
	class Pillar :public Rectangle {
		friend bool collison(const Bird& C, const Pillar& R);
	public:
		Pillar(int a, int b, int l1, int w1, int s);
		virtual void update();//从右向左移动
		virtual void draw()const;
		void change_speed(int x,int r);
		void change_speed(int s);
		int visit_speed();
	private:
		int speed = 0;//通过移动柱子来实现鸟的飞行效果		
	};

	class Level {
	public:
		Level(int w,int s);
		void get_bird(const Bird& ball);
		//删除碰撞的砖块
		void update();		
		void clear();
		void draw();
		bool quit();		
	protected:
		void edit();
		void show();
		void cut_off();
	private:
		int Pillar_width=0;
		int game_speed=0;
		const Bird* p_bird = nullptr;
		bool stop = false;
		int t_distance = 0;//判断是否显示新的障碍物
		int start=0;//记录第一个未出现在屏幕中的下标
		//由于每个block首先出现在屏幕的最右边，需要改变坐标，所以不能存储Pillar
		vector<int> Pillar_editor;//存储高度的一半
		vector<int> Pillar_distance;//每2个障碍物相隔的距离	
		vector<bool> Pillar_updown;//true为上
		list<Pillar> Pillar_storage;
		list<int> Pillar_count;//减少和Pillar的耦合度				
	};	

	static int Bird_r = 44;
	class Bird {
		friend bool collison(const Bird& C, const Pillar& R);
	public:
		Bird(int x1, int y1, int a1, int v1, IMAGE& img);
		void update();
		void draw()const;
		bool quit();
	private:
		int x = 0;
		int y = 0;		
		int a = 10;
		int r = Bird_r;
		int v = 0;//初速度
		int speedY = 0;
		bool up = false;
		bool stop = false;
		vector<IMAGE> img_bird;
	};
	

}//namespace FlappyBird

#endif

