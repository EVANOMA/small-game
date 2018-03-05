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
		virtual void update();//���������ƶ�
		virtual void draw()const;
		void change_speed(int x,int r);
		void change_speed(int s);
		int visit_speed();
	private:
		int speed = 0;//ͨ���ƶ�������ʵ����ķ���Ч��		
	};

	class Level {
	public:
		Level(int w,int s);
		void get_bird(const Bird& ball);
		//ɾ����ײ��ש��
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
		int t_distance = 0;//�ж��Ƿ���ʾ�µ��ϰ���
		int start=0;//��¼��һ��δ��������Ļ�е��±�
		//����ÿ��block���ȳ�������Ļ�����ұߣ���Ҫ�ı����꣬���Բ��ܴ洢Pillar
		vector<int> Pillar_editor;//�洢�߶ȵ�һ��
		vector<int> Pillar_distance;//ÿ2���ϰ�������ľ���	
		vector<bool> Pillar_updown;//trueΪ��
		list<Pillar> Pillar_storage;
		list<int> Pillar_count;//���ٺ�Pillar����϶�				
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
		int v = 0;//���ٶ�
		int speedY = 0;
		bool up = false;
		bool stop = false;
		vector<IMAGE> img_bird;
	};
	

}//namespace FlappyBird

#endif

