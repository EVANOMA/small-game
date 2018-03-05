#include<iostream>
#include<stdlib.h>
using namespace std;

#include "planecombat.h"

namespace FlySimulator {
	
	static int interface_longth = 700;
	static int interface_width = 600;
	static int Plane_x = 10;//300
	static int Plane_y = 400;//400



	InputHandler::InputHandler(Product& p):
		product(p)
	{}

	void InputHandler::update()
	{
		for (auto& fly : product.air_force) {
			if ((GetAsyncKeyState(VK_UP) & 0x8000))     // 上方向键
				fly.move_up();
			else if ((GetAsyncKeyState(VK_DOWN) & 0x8000))  // 下方向键
				fly.move_down();
			if ((GetAsyncKeyState(VK_LEFT) & 0x8000))  // 下方向键
				fly.move_left();
			else if ((GetAsyncKeyState(VK_RIGHT) & 0x8000))  // 下方向键
				fly.move_right();
			if ((GetAsyncKeyState(0x51) & 0x8000))  //Q
				fly.open_fire();
		}
		
		/*
		if ((GetAsyncKeyState(0x57) & 0x8000))     // w
			fly_b->move_left();
		else if ((GetAsyncKeyState(0x53) & 0x8000))  // s
			fly_b->move_right();
        //调试：通过空格暂停
		if ((GetAsyncKeyState(VK_SPACE) & 0x8000))  
			getch();
		*/		
	}

	AI::AI(Product& p) :
		product(p)
	{}

	void AI::update()
	{
		static bernoulli_distribution b;
		static default_random_engine e;
		for (auto& fly : product.air_force) {
			if (b(e)) fly.move_left();
			if (b(e)) fly.move_right();

			if (b(e)) fly.open_fire();
		}	
	}

	void Life::get_damage(int damage)
	{
		Rectangle::change_width(-damage);
	}

	bool Life::is_crash()const
	{
		return (Rectangle::visit_width() == 0);
	}

	void Life::draw()const
	{
		setcolor(RED);
		setfillcolor(RED);
		Rectangle::draw();
	}

	Plane::Plane(int x1, int y1, int s1, int s2,int r, IMAGE img1, IMAGE img2, Life& l):
	x(x1),y(y1), speedX(s1), speedY(s2),rate(r), life(l)
	{	
		img_plane.push_back(img1);		
		img_plane.push_back(img2);
	}

	void Plane::get_damage(int damage)
	{
		life.get_damage(damage);
	}

	bool Plane::is_crash()const
	{
		return life.is_crash();
	}

	void Plane::move_up()
	{
		y -= speedY;
		if (y <= 0) y = 0;
	}
	void Plane::move_down() 
	{
		y += speedY;
		if (y+161 >= interface_width) y = interface_width-161;
	}
	void Plane::move_left()
	{
		x -= speedX;
		if (x <= 0) x = 0;
	}
	void Plane::move_right()
	{
		x += speedX;
		if (x+107 >= interface_longth) x = interface_longth-107;
	}
	void Plane::open_fire()
	{	
		if (cool_down == 0) {
			cool_down = rate;
			fire = true;
		}
	}

	void Plane::update()
	{
		fire = false;
		if (cool_down>0)  --cool_down;
		life.change_to(x,y);
	}

	void Plane::draw()const
	{		
		putimage(x, y, &img_plane[1], NOTSRCERASE);
		putimage(x, y, &img_plane[0], SRCINVERT);
		life.draw();
	}

	int Plane::get_x()const
	{
		return x;
	}

	int Plane::get_y()const
	{
		return y;
	}

	bool Plane::is_fire()const
	{
		return fire;
	}

	void Plane::set_position(int x1, int y1)
	{
		x = x1;
		y = y1;
		life.change_to(x, y);
	}

	Product::Product(const Plane& plane,int max, int y1):
		porototype(plane), max_num(max), plane_y(y1)
	{
		update();
	}

	void Product::draw()const
	{
		for (auto &r : air_force) 
			r.draw();			
	}

	Plane& Product::get_control(int i)
	{
		auto iter = air_force.begin();
		if (i >= air_force.size()) {
			cout << "Error:" << __FILE__ << ": in function "
				<< __func__ << " at line " << __LINE__ << endl;
			cout << "subsript is out of the range" << endl;
		}
		while (i--) ++iter;
		return *iter;
	}

	void Product::update() {
		//先判断是否需要增加
		int t = max_num - air_force.size();
		static uniform_int_distribution<int> u(10,interface_longth-150);
		static default_random_engine e;
		while (t--) {		
			Plane p=porototype;
			p.set_position(u(e), plane_y);
			air_force.push_back(p);
		}
		for (auto &r : air_force)
			r.update();
	}

	void Weapon::draw()
	{}

	void Weapon::update()
	{}

	Bullet::Bullet(int x1, int y1, int s, int h, IMAGE img1) :
		x(x1), y(y1), speedY(s),hurt(h), img(img1) {}

	void Bullet::draw()const
	{
		putimage(x, y, &img);
	}

	void Bullet::update()
	{
		y += speedY;
	}

	bool Bullet::disappear()
	{	
		if (y<0 || y>interface_width) 
			return true;
		else return false;			
	}

	int Bullet::damage()const
	{
		return hurt;
	}

	int Bullet::get_x()const
	{
		return x;
	}

	int Bullet::get_y()const
	{
		return y;
	}

	void Bullet::change_to(int x1, int y1)
	{
		x = x1;
		y = y1;
	}

	ImpactWave::ImpactWave(int a, int b, int r, int s1, int s2) :
		x(a), y(b), radius(r), speedX(s1), speedY(s2)
	{}

	void ImpactWave::draw()const
	{
		if (end) return;
		setcolor(WHITE);
		setfillcolor(WHITE);
		fillcircle(x, y, rt);
		bar(x - rt, y, x + rt, y + l);
	}

	void ImpactWave::update()
	{
		if (s == State::state1) {
			rt += speedX;
			if (rt >= radius) {
				rt = radius;
				s = State::state2;
			}
		}
		else if (s == State::state2) {
			l += speedY;
			if ((y + l) >= interface_longth)
				s = State::state3;
		}
		else {
			rt -= speedX;
			if (rt <= 0) {
				rt = 0;
				end = true;
			}
		}
	}

	bool ImpactWave::disappear()
	{
		return end;
	}

	void ImpactWave::change_to(int x1, int y1)
	{
		x = x1;
		y = y1;
	}

	int ImpactWave::damage()const
	{
		return hurt;
	}

	int ImpactWave::get_x()const
	{
		return x;
	}

	int ImpactWave::get_y()const
	{
		return y+l;
	}

	Collision::Collision(int b_longth, int b_width, int p_longth, int p_width):
		Bullet_longth(b_longth),Bullet_width(b_width), Plane_longth(p_longth), Plane_width(p_width)
	{}

	bool Collision::assistent(int z1, int z2, int d1, int d2)const
	{
		if (z1 == z2) return true;
		else if (z1 < z2) {
			if ((z1 + d1) >= z2) return true;
		}
		else {
			if ((z2 + d2) >= z1) return true;
		}
		return false;
	}

	bool Collision::collison(const Bullet& b, const Plane& p)const
	{		
		if (assistent(b.get_x(), p.get_x(), Bullet_longth, Plane_longth))
			return assistent(b.get_y(), p.get_y(), Bullet_width, Plane_width);
		else return false;
	}

	bool Collision::collison(const ImpactWave& w, const Plane& p)const
	{
		if (w.get_y() <= p.get_y()) return false;
		if ((w.get_x() > p.get_x())&& (w.get_x() < p.get_x() + Plane_longth))		
			return true;
		return false;
	}

	Explode::Explode()
	{
		IMAGE img, img1, img2, img3, img4, img5, img6, img7, img8, img9;

		loadimage(&img, _T("D:\\code/test6/test6/boum/b (1).jpg"));
		process.push_back(img);
		loadimage(&img1, _T("D:\\code/test6/test6/boum/b (2).jpg"));
		process.push_back(img1);
		loadimage(&img2, _T("D:\\code/test6/test6/boum/b (3).jpg"));
		process.push_back(img2);
		loadimage(&img3, _T("D:\\code/test6/test6/boum/b (4).jpg"));
		process.push_back(img3);
		loadimage(&img4, _T("D:\\code/test6/test6/boum/b (5).jpg"));
		process.push_back(img4);
		loadimage(&img5, _T("D:\\code/test6/test6/boum/b (6).jpg"));
		process.push_back(img5);
		loadimage(&img6, _T("D:\\code/test6/test6/boum/b (7).jpg"));
		process.push_back(img6);
		loadimage(&img7, _T("D:\\code/test6/test6/boum/b (8).jpg"));
		process.push_back(img7);
		loadimage(&img8, _T("D:\\code/test6/test6/boum/b (9).jpg"));
		process.push_back(img8);
		loadimage(&img9, _T("D:\\code/test6/test6/boum/b (10).jpg"));
		process.push_back(img9);

	}

	void Explode::add_position(int x, int y)
	{
		position.push_back(x);
		position.push_back(y);
		position.push_back(0);
	}

	void Explode::draw()
	{
		int size = process.size();
		auto iter = position.begin();
		while (iter != position.end()) {
			auto iter1 = iter;
			int a1 = *iter, a2 = *(++iter), a3 = *(++iter);
			putimage(a1, a2, &process[a3]);
			*iter = (*iter) + 1;
			if (*iter == size) {
				iter = position.erase(iter1, ++iter);
			}
			else ++iter;
		}
	}

	Fire::Fire(Bullet b):
		bullet(b){}

	void Fire::get_Plane(Product& fly1, Product& fly2)
	{
		fly_enemy = &fly1;
		fly_friend = &fly2;
	}

	void Fire::get_Collision(const Collision& c)
	{
		p_collision = &c;
	}

	void Fire::get_Explode(Explode& e)
	{
		p_explode = &e;
	}

	void Fire::update()
	{
		auto iter = missile.begin();		
		while (iter != missile.end()) {
			iter->update();
			bool mark = false;
			//每个子弹对每架敌机进行碰撞检测
			auto iter1 = fly_enemy->air_force.begin();
			while (iter1 != fly_enemy->air_force.end()) {
				mark = p_collision->collison(*iter, *iter1);
				//击中敌机						
				if (mark) {
					iter1->get_damage(bullet.damage());
					if (iter1->is_crash()) {
						//爆炸
						p_explode->add_position(iter1->get_x(), iter1->get_y());
						//坠毁
						fly_enemy->air_force.erase(iter1);
					}
					break;//发生碰撞后子弹不用再与后面的飞机进行判断
				}
				else ++iter1;
			}						
			if (iter->disappear()||mark)
				iter = missile.erase(iter);		
			else ++iter;
		}
		//增加
		auto iter1 = fly_friend->air_force.begin();
		while (iter1 != fly_friend->air_force.end()) {
			if (iter1->is_fire()) {
				Bullet b = bullet;
				b.change_to(iter1->get_x() + 20, iter1->get_y() + 90);
				missile.push_back(b);
				b.change_to(iter1->get_x() + 75, iter1->get_y() + 90);					
				missile.push_back(b);
			}
			++iter1;
		}
				
	}

	void Fire::draw()const
	{
		for (auto& r : missile)
			r.draw();
		p_explode->draw();
	}

	Fire1::Fire1(ImpactWave w) :
		wave(w) {}

	void Fire1::get_Plane(Product& fly1, Product& fly2)
	{
		fly_enemy = &fly1;
		fly_friend = &fly2;
	}

	void Fire1::get_Collision(const Collision& c)
	{
		p_collision = &c;
	}

	void Fire1::get_Explode(Explode& e)
	{
		p_explode = &e;
	}

	void Fire1::update()
	{
		auto iter = lightsaber.begin();
		while (iter != lightsaber.end()) {
			iter->update();
			//每个子弹对每架敌机进行碰撞检测
			auto iter1 = fly_enemy->air_force.begin();
			while (iter1 != fly_enemy->air_force.end()) {
				//击中敌机						
				if (p_collision->collison(*iter, *iter1)) {
					iter1->get_damage(wave.damage());
					if (iter1->is_crash()) {
						//爆炸
						p_explode->add_position(iter1->get_x(), iter1->get_y());
						//坠毁
						fly_enemy->air_force.erase(iter1);
					}
					break;//发生碰撞后子弹不用再与后面的飞机进行判断
				}
				else ++iter1;
			}
			if (iter->disappear())
				iter = lightsaber.erase(iter);
			else ++iter;
		}
		//增加
		auto iter1 = fly_friend->air_force.begin();
		while (iter1 != fly_friend->air_force.end()) {
			if (iter1->is_fire()) {
				ImpactWave w = wave;
				w.change_to(iter1->get_x() + 20, iter1->get_y() + 90);
				lightsaber.push_back(w);
				w.change_to(iter1->get_x() + 75, iter1->get_y() + 90);	
				lightsaber.push_back(w);
			}
			++iter1;
		}

	}

	void Fire1::draw()const
	{
		for (auto& r : lightsaber)
			r.draw();
		p_explode->draw();
	}

	BackGround::BackGround()
	{
		position.push_back(0);
		position.push_back(175);
		position.push_back(350);
		position.push_back(525);
		IMAGE img;
		surface s;

		loadimage(&img, _T("D:\\code/test6/test6/FlySimulator/back_01.jpg"));
		s.img = img; s.num = 0;
		ground.push_back(s);
		loadimage(&img, _T("D:\\code/test6/test6/FlySimulator/back_02.jpg"));
		s.img = img; s.num = 1;
		ground.push_back(s);
		loadimage(&img, _T("D:\\code/test6/test6/FlySimulator/back_03.jpg"));
		s.img = img; s.num = 2;
		ground.push_back(s);
		loadimage(&img, _T("D:\\code/test6/test6/FlySimulator/back_04.jpg"));
		s.img = img; s.num = 3;
		ground.push_back(s);
	}

	void BackGround::draw()
	{
		for (auto &r : ground) {
			putimage(0, position[r.num], &r.img);
			r.num = (r.num + 1) % 4;
		}
	}

}//namespace FlySimulator
/*
using namespace FlySimulator;

int main()
{
	initgraph(interface_longth, interface_width);

	//TO-DO：播放音乐
	int s1=30,s2=30,r=2;
	//生命条初始化
	Life life(Plane_x, Plane_y, 7, 60);
	//Life life2(0, 0, 7, 60);
	
	//飞机初始化
	IMAGE img, img1, img2;
	loadimage(&img1, _T("D:\\code/test6/test6/FlySimulator/fly.jpg"));
	loadimage(&img2, _T("D:\\code/test6/test6/FlySimulator/fly1.jpg"));	
	Plane J20(Plane_x, Plane_y, s1, s2, r, img1, img2, life);
	loadimage(&img1, _T("D:\\code/test6/test6/FlySimulator/F14.jpg"));
	loadimage(&img2, _T("D:\\code/test6/test6/FlySimulator/f141.jpg"));
	r = 30;
	Plane F14(10, 10, s1, s2, r, img1, img2, life);
	
	Product product_a(J20, 1, Plane_y);
	Product product_b(F14, 2,10);
	
	//输入设置
	InputHandler input(product_a);
	AI noma(product_b);
	
	//碰撞系统设置
	Collision collision(7, 20, 107, 161);
	
	//发射系统设置
	loadimage(&img, _T("D:\\code/test6/test6/FlySimulator/bullet.jpg"));
	Bullet b(0, 0, -30, 20, img);
	Fire fire_a(b);
	fire_a.get_Plane(product_b, product_a);
	fire_a.get_Collision(collision);
	Explode explode;
	fire_a.get_Explode(explode);
	
	ImpactWave w(300, 300, 10, 2, 50);
	Fire1 fire_b(w);
	fire_b.get_Plane(product_a, product_b);
	fire_b.get_Collision(collision);
	fire_b.get_Explode(explode);

	BackGround backGround;
	IMAGE img3;
	loadimage(&img3, _T("D:\\code/test6/test6/start.jpg"));
	putimage(0, 0, &img3);
	getch();
	clearcliprgn();

	BeginBatchDraw();
	
	int fps = 100;
	while (1) {		
		backGround.draw();
		fire_a.draw();
		fire_b.draw();
		product_a.draw();
		product_b.draw();
		FlushBatchDraw();
		
		Sleep(fps);
		//getch();
		clearcliprgn();

		input.update();	
		noma.update();
		fire_a.update();
		fire_b.update();
		product_a.update();
		product_b.update();
	}
	
	loadimage(&img2, _T("D:\\code/test6/test6/end.jpg"));
	putimage(0, 0, &img2);
	EndBatchDraw();
	getch();				// 按任意键继续
	closegraph();
	
	return 0;
}
*/

/*
int main()
{
	int s1 = 30, s2 = 30, r = 2;
	//生命条初始化
	Life life1(Plane_x, Plane_y, 7, 60);
	Life life2(0, 0, 7, 60);

	//飞机初始化
	IMAGE img, img1, img2;
	loadimage(&img1, _T("D:\\code/test6/test6/FlySimulator/fly.jpg"));
	loadimage(&img2, _T("D:\\code/test6/test6/FlySimulator/fly1.jpg"));
	Plane J20(Plane_x, Plane_y, s1, s2, r, img1, img2, life1);
	loadimage(&img1, _T("D:\\code/test6/test6/FlySimulator/F14.jpg"));
	loadimage(&img2, _T("D:\\code/test6/test6/FlySimulator/f141.jpg"));
	Plane F14(10, 10, s1, s2, r, img1, img2, life2);

	Product product_a(J20, 1, Plane_y);
	Product product_b(F14, 1, 10);

	//输入设置
	InputHandler input;
	input.get_Plane(product_a.get_control(0));
	//碰撞系统设置
	Collision collision(7, 20, 107, 161);

	//发射系统设置
	loadimage(&img, _T("D:\\code/test6/test6/FlySimulator/bullet.jpg"));
	Fire fire(img);
	fire.get_Plane(product_b, product_a);
	fire.get_Collision(collision);
	Explode explode;
	fire.get_Explode(explode);

	int fps = 100;
	int i = 0;
	while (1) {
		cout << "i=" << i << endl;
		++i;
		Sleep(fps);

		input.update();
		fire.update();
		product_a.update();
		product_b.update();
	}

	return 0;
}
*/