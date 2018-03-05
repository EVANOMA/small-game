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
		//�ж��Ƿ������ײ 
		
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
		//TO-DO:�����ƶ��ٶȺ;���Ĺ�ϵ
		Pillar_distance.push_back(game_speed);
		Pillar_updown.push_back(true);
		edit();	

		loadimage(&img2, _T("D:\\code/test6/test6/finish.jpg"));
		putimage(0, 0, &img2);
		getch();
		clearcliprgn();
	}
	//���õ��ϰ�λ�ò��ܳ�����һ��
	//TO-DO��ֻ�ܰ��Ⱥ�˳��������ã����Կ���ʹ��map���д洢
	void Level::edit()
	{
		MOUSEMSG m;// ���������Ϣ
		int t=150;//��һ���ϰ���ĺ�����
		while (1) {
			// �ж�����Ƿ��е��
			if (MouseHit()) {
				m = GetMouseMsg();// ��ȡһ�������Ϣ
				// ����������ʱ�������ĳ���Ϊ��
				if (m.uMsg == WM_LBUTTONDOWN){
					Pillar p(m.x, m.y, 2*m.y, Pillar_width, game_speed);
					p.draw();
					Pillar_updown.push_back(true);
					Pillar_editor.push_back(m.y);
					Pillar_distance.push_back(m.x - t);
					t = m.x;
					
				}
				//����Ҽ�����ʱ���ײ��ĳ���Ϊ��
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
			// �жϼ����Ƿ�������
			else if (kbhit()){  
				char input = getch();
				//�����ؿ�����
				if (input == 'e') {
					clearcliprgn();//����
					return;
				}
				else if (input == 'n') {//������һ�ؿ�����
					clearcliprgn();//����
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
	//�ж��Ƿ���ʾ�µ��ϰ���
	void Level::show()
	{				
		t_distance += game_speed;
		if (start < Pillar_distance.size()) {
			//�������ϰ������ʱ��ʾ�Ŀ��
			int x = t_distance - Pillar_distance[start];
			if (x >= 0) {
				double t = interface_longth - x;
				int y;
				//ȷ���������λ��
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
	//ɾ����Ҫ�Ƴ���Ļ���ϰ���
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
		//�ж��Ƿ�����ײ
		if (!Pillar_storage.empty()) {
			for (auto r : Pillar_storage)					
				if (collison(*p_bird, r)) {
					stop = true;
					//����ײʱΪ�˽Ӵ������ص�����Ҫ�ı�game_speed
					//TO-DO��Ҫ������Ϸʱ��Ҫ���������game_speed
					//TO-DO���˴�û�а���ԭ�еľ�����ú�ɾ���ϰ���
					
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
		//����
		rotateimage(&img_bird1, &img1, PI / 6);
		img_bird.push_back(img_bird1);
		//�½�
		rotateimage(&img_bird1, &img1, -PI / 6);
		img_bird.push_back(img_bird1);
	}

	void Bird::draw()const
	{
		//�ı䷽��
		if (up == true) 
			putimage(x, y, &img_bird[0]);//xyΪ���Ͻ�����
		else 
			putimage(x, y, &img_bird[1]);				
	}

	void Bird::update()
	{
		char input;
		if (kbhit()) {  // �ж��Ƿ�������
			input = getch(); 
			if (input == 'w') {
				speedY = -v;
				up = true;
				//cout << "up" << endl;
			}
		}
		//û�а�����������
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
	//ͼ����Ƶ���������ֻ���ƴ����֮���put
	BeginBatchDraw();

	int fps = 100;
	while (1) {		
		bird.draw();
		level.draw();
		FlushBatchDraw();//�ӻ���ͳһ����,�ӿ�GPU�Ĵ����ٶ�
		
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
	getch();				// �����������
	closegraph();
	
	return 0;
}
*/
	
	
	