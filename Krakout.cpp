#include<iostream>
#include<stdlib.h>
using namespace std;

#include "krakout.h"

namespace krakout {
	//�߿���
	static int interface_longth = 400;
	static int interface_width = 500;
	//��Ҫ��ש����ڻ�������
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
		//�ж��Ƿ�Խ���߿� 
		//TO-DO:��ɾ���߿�����Ľ��㣬������Ҫÿ��ѭ�����»��Ʊ߿�
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
		//�ж��Ƿ�ͻ����ཻ
		if (is_collison) {			
			//��û������ǽ��
			if (mark == false) {
				//�´���ͱ���Ӵ�
				//��speedY�жϽӴ����ǻ������һ����
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
		MOUSEMSG m;		// ���������Ϣ
		vector<Block> a;
		while (1){
			// �ж�����Ƿ��е��
			if (MouseHit()) {				
				m = GetMouseMsg();// ��ȡһ�������Ϣ
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					// ����������ʱ�����λ�û�һ������
					Block b(m.x, m.y, Block_high, Block_width);
					b.draw();
					a.push_back(b);
				}
			}			
			else if (kbhit())  // �жϼ����Ƿ�������
			{
				char input = getch();		
				if (input == 'e') {//�����ؿ�����
					Block_editor.push_back(a);
					clearcliprgn();
					return;
				}
				else if (input == 'n') {//������һ�ؿ�����
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

	//��������ؿ�
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
		//TO-DO:������һ��
		if (Block_t.size() == 0)
			if(level_number < Block_editor.size())
				load(level_number + 2);
			else return;
		//ÿ��ѭ���������δ����ײ��ש��
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
		if (kbhit())  // �ж��Ƿ�������
		{
			input = getch();  // �����û��Ĳ�ͬ�������ƶ�����������س�		
			//TO-DO:�����ƶ�ʱ�ᴥ�����߽磬������Ҫ��ѭ�������»��Ʊ߿�					
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
	//����ʹ��ͼ�ο�����ƣ�����Ĳ���ֻ��Ϊ��������
	//����Ҫ�����ʱ�����������Բ������Ϊ����
	initgraph(640, 600);// ��ʼ��640x480�Ļ�ͼ��Ļ
	//�߿�
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

	int fps = 60;//����Ϸ�е�ѭ������������Ҫ��С��ʾʱ��

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
		
	getch();// �����������
	closegraph();// �ر�ͼ�ν���
		
	return 0;
}






