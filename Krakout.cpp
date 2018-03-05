#include<iostream>
#include<stdlib.h>
using namespace std;

#include "krakout.h"

namespace krakout {
	//边框宽高
	static int interface_longth = 400;
	static int interface_width = 500;
	//不要将砖块放在滑块下面
	static int forbiden_width = 440;

	void Ball::draw()const
	{
		setfillcolor(WHITE);
		Circle::draw();
	}

	void Ball::get_slipper(const Slipper& slider)
	{
		p_slipper = &slider;
	}

	void Ball::get_collison(bool result)
	{
		is_collison = result;
	}

	void Ball::update()
	{
		//判断是否越出边框 
		//TO-DO:会删除边框与球的交点，可能需要每次循环重新绘制边框
		x += speedX;
		if (x+r > interface_longth|| x-r < 0) {
			x -= speedX;
			speedX *= -1; 
			//cout << "x=" << x << endl;
		}
		y += speedY;
		bool mark = false;
		if (y + r > interface_width || y - r < 0) {
			y -= speedY;
			speedY *= -1;
			mark = true;
		}			
		//判断是否和滑块相交
		if (is_collison) {			
			//还没有碰到墙壁
			if (mark == false) {
				//下次球和表面接触
				//从speedY判断接触的是滑块的哪一个面
				if (speedY > 0) y = forbiden_width-r;
				else y = forbiden_width+ p_slipper->visit_longth()+r;
			}
			speedY *= -1;			
		}
	}	
	
	void Block::update()
	{}

	void Block::draw()const
	{
		setfillcolor(RED);
		Rectangle::draw();
	}

	Level::Level(int Block_h, int Block_w)
	{
		Block_high = Block_h;
		Block_width = Block_w;
		
	    edit();			
	}

	void Level::edit()
	{
		MOUSEMSG m;		// 定义鼠标消息
		vector<Block> a;
		while (1){
			// 判断鼠标是否有点击
			if (MouseHit()) {				
				m = GetMouseMsg();// 获取一条鼠标消息
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					// 鼠标左键按下时在鼠标位置画一个方块
					Block b(m.x, m.y, Block_high, Block_width);
					b.draw();
					a.push_back(b);
				}
			}			
			else if (kbhit())  // 判断键盘是否有输入
			{
				char input = getch();		
				if (input == 'e') {//结束关卡设置
					Block_editor.push_back(a);
					clearcliprgn();
					return;
				}
				else if (input == 'n') {//进入下一关卡设置
					Block_editor.push_back(a);
					clearcliprgn();
					a.clear();
					line(0, interface_width, interface_longth, interface_width);
					line(interface_longth, 0, interface_longth, interface_width);
				}
			}			
		}
	}

	void Level::get_ball(const Ball& ball)
	{
		p_ball = &ball;
	}

	//加载任意关卡
	void Level::load(int Stage_number)
	{
		if (Stage_number <= 0 || Stage_number > Block_editor.size()) {
			cout << "Error:" << __FILE__ << ": in function "
				<< __func__ << " at line " << __LINE__ << endl
				<< "Stage_number is out of the right range" << endl;
			return;
		}
		Stage_number -= 1;
		level_number= Stage_number;
		Block_t= Block_editor[level_number];		
	}

	void Level::draw()const
	{
		for (auto& r : Block_t)
			r.draw();
	}

	void Level::update()
	{
		//TO-DO:载入下一关
		if (Block_t.size() == 0)
			if(level_number < Block_editor.size())
				load(level_number + 2);
			else return;
		//每次循环检测所有未被碰撞的砖块
		auto iter = Block_t.begin();
		while (iter != Block_t.end()) {
			if (collison(*p_ball, *iter)) 
				iter = Block_t.erase(iter);
			else  ++iter;
		}					
	}
	

	Slipper::Slipper(int a, int b, int l1, int w1,int s):
		Rectangle(a, b, l1, w1)
	{
		speed = s;
	}

	void Slipper::draw()const
	{
		setfillcolor(WHITE);
		Rectangle::draw();
	}

	void Slipper::update()
	{
		char input;
		if (kbhit())  // 判断是否有输入
		{
			input = getch();  // 根据用户的不同输入来移动，不必输入回车		
			//TO-DO:滑块移动时会触碰到边界，可能需要在循环中重新绘制边框					
			if (input == 'a' ) {
				int x1 = x - width / 2;
				if (x1 >= speed) change(-speed, 0);
				else change(-x1, 0);
			}
			else if (input == 'd') {
				int x2 = x + width / 2;
				if ( (x2 + speed) <= interface_longth ) 
					change(speed, 0);
				else change(interface_longth-x2, 0);
			}			
		}
	}

}//namespace krakout



using namespace krakout;
int main()
{
	//由于使用图形库的限制，传入的参数只能为整数，在
	//当需要计算的时候会出现误差，所以参数最好为整数
	initgraph(640, 600);// 初始化640x480的绘图屏幕
	//边框
	line(0, interface_width, interface_longth, interface_width);//x1,y1-x2,y2 
	line(interface_longth, 0, interface_longth, interface_width);

	int l = 20, w1 = 60;
	Level level(l, w1);
	level.load(1);
	
	int a=10, b=10, c=10, d=10,f=10;
	Ball ball(a, b, c, d,f);
	level.get_ball(ball);

	int w = 100,s=30,p1=300,p2=450;
	Slipper slider(p1, p2, l, w, s);
	ball.get_slipper(slider);

	int fps = 60;//当游戏中的循环内容增加需要减小显示时间

	BeginBatchDraw();

	while (1) {	
		line(0, interface_width, interface_longth, interface_width);
		line(interface_longth, 0, interface_longth, interface_width);
		ball.draw();
		slider.draw();
		level.draw();
		FlushBatchDraw();

		Sleep(fps);
		//getch();
		
		clearcliprgn();

		slider.update();
		ball.get_collison(collison(ball, slider));		
		ball.update();			
		level.update();	
	}	
		
	getch();// 按任意键继续
	closegraph();// 关闭图形界面
		
	return 0;
}






