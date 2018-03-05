#ifndef PLANECOMBAT_H
#define PLANECOMBAT_H

#include "Graph.h"
#include <vector>
#include <list>
#include <random>

namespace FlySimulator {
	
	class Weapon{
	public:
		virtual void draw();
		virtual void update();
		//virtual bool disappear();
		//virtual int damage()const;
		//virtual int get_x()const;
		//virtual int get_y()const;
	};
	
	class Bullet {
	public:
		Bullet(int x1, int y1, int s, int h,IMAGE img1);
		void draw()const;
		void update();
		bool disappear();
		void change_to(int x1, int y1);
		virtual int damage()const;
		int get_x()const;
		int get_y()const;
	private:
		int x;
		int y;
		int speedY;
		int hurt = 0;
		IMAGE img;
	};

	enum class State { state1, state2, state3 };

	class ImpactWave {
	public:
		ImpactWave(int a, int b, int r, int s1, int s2);
		void draw()const;
		void update();
		bool disappear();
		void change_to(int x1, int y1);
		int damage()const;
		int get_x()const;
		int get_y()const;
	private:
		int x;
		int y;
		int radius;
		int speedX;//半径扩张的速度
		int speedY;//前进的速度
		int hurt = 5;
		int rt = 0;
		int l = 0;
		State s = State::state1;
		bool end = false;
	};

	using TwoDimensions::Rectangle;
	class Life:public Rectangle {
		friend class Plane;
	public:
		//width为t的倍数
		using Rectangle::Rectangle;
		virtual void draw()const;
		void get_damage(int damage);
		bool is_crash()const;
	};

	class Product;
	class InputHandler {		
	public:
		InputHandler(Product& p);
		void update();		
	private:
		Product& product;
	};

	class AI {
	public:
		AI(Product& p);
		void update();	
	private:
		Product& product;
	};
	
	class Plane {
	public:
		Plane(int x1, int y1, int s1, int s2, int r,IMAGE img1, IMAGE img2, Life& l);
		void update();
		void draw()const;		
		int get_x()const;
		int get_y()const;
		bool is_fire()const;
		bool is_crash()const;
		void get_damage(int damage);
		//方便生产新的飞机
		void set_position(int x1,int y1);
		//键盘输入
		void move_up();
		void move_down();
		void move_left();
		void move_right();
		void open_fire();
	private:
		int x = 0;
		int y = 0;
		int speedX = 0;
		int speedY = 0;	
		int rate = 0;
		int cool_down = 0;//子弹的发射间隔
		bool fire = false;
		vector<IMAGE> img_plane;
		Life life;
	};

	class Product{
	friend class Fire;
	friend class Fire1;
	friend class InputHandler;
	friend class AI;
	public:
		Product(const Plane& plane,int max,int y);
		void update();
		void draw()const;
		//和InputHandler建立联系
		Plane& get_control(int i);
	private:
		int max_num = 1;
		int plane_y = 0;
		const Plane& porototype;//原型
		list<Plane> air_force;
	};

	//使用Graph.h的collision函数比较麻烦 
	class Collision {
	public:
		Collision(int b_longth, int b_width, int p_longth,int p_width);
		bool collison(const Bullet& b, const Plane& p)const;
		bool collison(const ImpactWave& w, const Plane& p)const;
	protected:
		bool assistent(int z1, int z2, int d1, int d2)const;
	private:
		int Bullet_longth;
		int Bullet_width;
		int Plane_longth;
		int Plane_width;
	};

	class Explode {
	public:
		Explode();
		void draw();
		void add_position(int x, int y);
	private:
		//存储坐标和下一张图片的下标
		list<int> position;
		vector<IMAGE> process;
	};

	class Fire {
	public:
		Fire(Bullet b);
		void update();
		void draw()const;
		void get_Plane(Product& fly1, Product& fly2);
		void get_Collision(const Collision& c);
		void get_Explode(Explode& e);
	private:
		const Bullet bullet;
		Product* fly_enemy = nullptr;
		Product* fly_friend = nullptr;
		const Collision* p_collision = nullptr;
		Explode* p_explode = nullptr;
		list<Bullet> missile;
	};

	class Fire1 {
	public:
		Fire1(ImpactWave w);
		void update();
		void draw()const;
		void get_Plane(Product& fly1, Product& fly2);
		void get_Collision(const Collision& c);
		void get_Explode(Explode& e);
	private:
		const ImpactWave wave;
		Product* fly_enemy = nullptr;
		Product* fly_friend = nullptr;
		const Collision* p_collision = nullptr;
		Explode* p_explode = nullptr;
		list<ImpactWave> lightsaber;
	};

	struct surface {
		IMAGE img;
		int num;
	};

	class BackGround {
	public:
		BackGround();
		void draw();
	private:
		vector<int> position;
		vector<surface> ground;
	};

}//namespace FlySimulator

#endif
#pragma once
