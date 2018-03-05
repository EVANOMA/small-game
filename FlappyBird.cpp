#include<iostream>
#include<stdlib.h>
using namespace std;

#include "Flappybird.h"

namespace FlappyBird {

	static int interface_longth = 700;
	static int interface_width = 600;
	static int Bird_x = 50;
	static int Bird_y = 100;
	//static int Bird_r = 44;
	static double PI = 3.1415926;

	void test(int t)
	{
		static int d = 0;		
		TCHAR s[5];
		_stprintf(s, _T("%d"), t);
		outtextxy(d, interface_width, s);
		d += 20;
	}

	Pillar::Pillar(int a, int b, int l1, int w1, int s) :
		Rectangle(a, b, l1, w1)
	{
		speed = s;
	}

	void Pillar::change_speed(int x1, int r)
	{
		speed = x-width/2-x1-r;
	}

	void Pillar::change_speed(int s)
	{
		speed = s;
	}

	int Pillar::visit_speed()
	{
		return speed;
	}

	void Pillar::draw()const
	{
		setfillcolor(RED);
		Rectangle::draw();
	}

	void Pillar::update()
	{
		if (x+ visit_width()/2 > 0) x -= speed;
		else clear();
		//判断是否和鸟相撞 
		
	}

	Level::Level(int w,int s)
	{
		IMAGE img1,img2;
		loadimage(&img1, _T("D:\\code/test6/test6/edtor.jpg"));
		putimage(0, 0, &img1);
		getch();
		clearcliprgn();

		Pillar_width = w;
		game_speed = s;
		Pillar p(150, 130, 260, Pillar_width, game_speed);
		p.draw();
		Pillar_editor.push_back(130);
		//TO-DO:考虑移动速度和距离的关系
		Pillar_distance.push_back(game_speed);
		Pillar_updown.push_back(true);
		edit();	

		loadimage(&img2, _T("D:\\code/test6/test6/finish.jpg"));
		putimage(0, 0, &img2);
		getch();
		clearcliprgn();
	}
	//设置的障碍位置不能超过第一个
	//TO-DO：只能按先后顺序进行设置，可以考虑使用map进行存储
	void Level::edit()
	{
		MOUSEMSG m;// 定义鼠标消息
		int t=150;//第一个障碍物的横坐标
		while (1) {
			// 判断鼠标是否有点击
			if (MouseHit()) {
				m = GetMouseMsg();// 获取一条鼠标消息
				// 鼠标左键按下时到顶部的长度为高
				if (m.uMsg == WM_LBUTTONDOWN){
					Pillar p(m.x, m.y, 2*m.y, Pillar_width, game_speed);
					p.draw();
					Pillar_updown.push_back(true);
					Pillar_editor.push_back(m.y);
					Pillar_distance.push_back(m.x - t);
					t = m.x;
					
				}
				//鼠标右键按下时到底部的长度为高
				else if (m.uMsg == WM_RBUTTONUP)
				{
					Pillar p(m.x, m.y, 2*(interface_width-m.y), Pillar_width, game_speed);
					p.draw();	
					Pillar_updown.push_back(false);
					Pillar_editor.push_back(interface_width-m.y);
					Pillar_distance.push_back(m.x - t);
					t = m.x;
				}				
			}
			// 判断键盘是否有输入
			else if (kbhit()){  
				char input = getch();
				//结束关卡设置
				if (input == 'e') {
					clearcliprgn();//清屏
					return;
				}
				else if (input == 'n') {//进入下一关卡设置
					clearcliprgn();//清屏
					Pillar p(150, 130, 260, Pillar_width, game_speed);
					p.draw();
					t = 150;
				}				
			}

		}
	}

	void Level::get_bird(const Bird& bird)
	{
		p_bird = &bird;
	}
	//判断是否显示新的障碍物
	void Level::show()
	{				
		t_distance += game_speed;
		if (start < Pillar_distance.size()) {
			//当有新障碍物出来时显示的宽度
			int x = t_distance - Pillar_distance[start];
			if (x >= 0) {
				double t = interface_longth - x;
				int y;
				//确定纵坐标的位置
				if (Pillar_updown[start]) y = Pillar_editor[start];
				else  y = interface_width - Pillar_editor[start];

				Pillar p(t + Pillar_width / 2, y, 2 * Pillar_editor[start], Pillar_width, game_speed);
				Pillar_storage.push_back(p);
				Pillar_count.push_back(ceil((t + Pillar_width) / game_speed));				
				t_distance = x;
				++start;
			}
		}
	}
	//删除需要移出屏幕的障碍物
	void Level::cut_off()
	{		
		int t = 0;
		for (auto &r : Pillar_count) {
			--r;
			if (r == 0) ++t;
		}
		while (t--) {
			Pillar_storage.pop_front();
			Pillar_count.pop_front();
		}
	}

	void Level::update()
	{
		//判断是否发生碰撞
		if (!Pillar_storage.empty()) {
			for (auto r : Pillar_storage)					
				if (collison(*p_bird, r)) {
					stop = true;
					//在碰撞时为了接触但不重叠，需要改变game_speed
					//TO-DO：要重新游戏时需要保存最初的game_speed
					//TO-DO：此处没有按照原有的距离放置和删除障碍物
					
					/*
					r.change_speed(Bird_x, Bird_r);
					game_speed = r.visit_speed();
					for (auto &r : Pillar_storage) {
						r.change_speed(game_speed);
						r.update();
					}
					*/											
					return;
				}								
		}		
		show();
		cut_off();
		for (auto &r : Pillar_storage) 
			r.update();			
	}

	void Level::draw()
	{
		if (Pillar_storage.empty()) return;
		for (auto &r : Pillar_storage)
			r.draw();
	}

	void Level::clear()
	{
		if (Pillar_storage.empty()) return;
		for (auto r : Pillar_storage)
			r.clear();
	}

	bool Level::quit()
	{
		return stop;
	}

	Bird::Bird(int x1, int y1, int a1, int v1, IMAGE& img1) :
		x(x1), y(y1), a(a1), v(v1)
	{
		IMAGE img_bird1, img_bird2;
		//上升
		rotateimage(&img_bird1, &img1, PI / 6);
		img_bird.push_back(img_bird1);
		//下降
		rotateimage(&img_bird1, &img1, -PI / 6);
		img_bird.push_back(img_bird1);
	}

	void Bird::draw()const
	{
		//改变方向
		if (up == true) 
			putimage(x, y, &img_bird[0]);//xy为右上角坐标
		else 
			putimage(x, y, &img_bird[1]);				
	}

	void Bird::update()
	{
		char input;
		if (kbhit()) {  // 判断是否有输入
			input = getch(); 
			if (input == 'w') {
				speedY = -v;
				up = true;
				//cout << "up" << endl;
			}
		}
		//没有按键加速下落
		else {
			speedY += a;
			up = false;
			//cout << "down" << endl;
		}
		y += speedY;
		//cout << "y= " <<y<< endl;
		if ((y + 2 * r) >= interface_width ) {
			stop = true;		
			y = interface_width - 2 * r;		
		}
		else if (y <= 0){			
			stop = true;
			y = 0;
			//cout << "stop" << endl;
		}
	}

	bool Bird::quit()
	{
		return stop;
	}

	bool collison(const Bird& C, const Pillar& R)
	{				
		if (intersect(C.x+C.r, R.x + R.speed, C.r, R.width))
			return intersect(C.y + C.r, R.y, C.r, R.high);
		else return false;		
	}
	
}//namespace FlappyBird

/*
using namespace FlappyBird;

int main()
{
	
	initgraph(interface_longth, interface_width);
	
	int w = 40, s = 10;
	Level level(w, s);

	IMAGE img1, img2;
	loadimage(&img1, _T("D:\\code/test6/test6/angrybird.jpg"));		
	int a1=10,v1=30;
	Bird bird(Bird_x, Bird_y, a1, v1, img1);

	loadimage(&img2, _T("D:\\code/test6/test6/start.jpg"));
	putimage(0, 0, &img2);
	getch();
	clearcliprgn();
	//图像绘制到缓存区，只限制此语句之后的put
	BeginBatchDraw();

	int fps = 100;
	while (1) {		
		bird.draw();
		level.draw();
		FlushBatchDraw();//从缓存统一绘制,加快GPU的处理速度
		
		if (level.quit()|| bird.quit()) {			
			getch();
			break;
		}
		Sleep(fps);
		//getch();
		clearcliprgn();
		
		bird.update();
		level.get_bird(bird);
		level.update();
		
	}
	clearcliprgn();
	loadimage(&img2, _T("D:\\code/test6/test6/end.jpg"));
	putimage(0, 0, &img2);
	EndBatchDraw();
	getch();				// 按任意键继续
	closegraph();
	
	return 0;
}
*/
	
	
	