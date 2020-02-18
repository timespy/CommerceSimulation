#include<iostream>
using namespace std;
#include<math.h>
#include<Windows.h>

#include"People.h"

double standard = 1;			// ��۱�׼����ǰ�ֽ��ܶ�/��ǰʳ���ܶ�
int loops = 0;					// ѭ����ǰ����
int Loop = 100;					// �ܹ�ѭ������

int N = 10;						// �ܹ�����ĳ�ʼ����
int trade_account = 5;			// ÿ�ν��׵�ʳ����
int credit_decrease = 10;		// ÿ�θ�ծ�������½��ٶ�
People* exprimenters;			

int N_sell = 0;					// ��������
int* id_sell;

int N_buy = 0;					// �������
int* id_buy;

int* hungry_days;				// ͳ��ÿ���˵ļ���״̬����
bool* is_alive;					// ��¼�Ƿ���
bool* is_matched;				// ��¼�Ƿ���ƥ��

void trade(int id_seller, int id_buyer);

void init() {
	srand((int)time(NULL));
	exprimenters = new People[N];
	hungry_days = new int[N];
	id_sell = new int[N];
	id_buy = new int[N];
	is_alive = new bool[N];
	is_matched = new bool[N];
	for (int i = 0; i < N; i++) {
		exprimenters[i].setId(i);
		exprimenters[i].setJudge(rand() % 10);
		exprimenters[i].setCaution(rand() % 10);
		exprimenters[i].setTrick(rand() % 10);
		hungry_days[i] = 0;
		id_sell[i] = -1;
		id_buy[i] = -1;
		is_alive[i] = true;
		is_matched[i] = false;
	}
}

void reset() {
	for (int i = 0; i < N; i++) {
		is_matched[i] = false;
	}
	N_sell = 0;
	N_buy = 0;
}

void update() {
	// ����ȷ������
	for (int i = 0; i < N; i++) {
		if (is_alive[i]) {
			if (exprimenters[i].isSell())
				id_sell[N_sell++] = exprimenters[i].getId();
			else
				id_buy[N_buy++] = exprimenters[i].getId();
		}
	}

	cout << loops << "��:�����" << N_buy << "��,������" << N_sell << "��" << endl;

	// ƥ�佻��
	srand((int)time(NULL));
	if (N_sell <= N_buy) {
		// �����г���������ѡ�����
		for (int i = 0; i < N_sell; i++) {
			int id_seller = id_sell[i];
			int id_buyer;
			while (true) {
				int x = rand() % N_buy;
				if (!is_matched[x]) {
					id_buyer = id_buy[x];
					break;
				}
			}

			is_matched[id_seller] = true;
			is_matched[id_buyer] = true;
			
			trade(id_seller, id_buyer);
		}
	}
	else {
		// ����г������ѡ������
		for (int i = 0; i < N_buy; i++) {
			int id_buyer = id_buy[i];
			int id_seller;
			while (true) {
				int x = rand() % N_sell;
				if (!is_matched[x]) {
					id_seller = id_sell[x];
					break;
				}
			}

			is_matched[id_seller] = true;
			is_matched[id_buyer] = true;

			trade(id_seller, id_buyer);
		}
	}
}

void settlement() {
	for (int i = 0; i < N; i++) {
		if (exprimenters[i].getFood() == 0 && is_alive[i]) {
			cout << endl << "idΪ" << exprimenters[i].getId() << "��ʵ�������ڰ�����" << endl << endl;
			hungry_days[i] ++;
		}

		if (exprimenters[i].getMoney() <= 0 && is_alive[i]) {
			cout << endl << "idΪ" << exprimenters[i].getId() << "��ʵ�������ڸ�ծ��" << endl << endl;
			exprimenters[i].updateCredit(credit_decrease);
		}

		exprimenters[i].updateFood(-1);
	}

	for (int i = 0; i < N; i++) {
		if (is_alive[i] && hungry_days[i] >= 5) {
			is_alive[i] = false;
			cout << endl << "idΪ" << exprimenters[i].getId() << "��ʵ�����Ѷ�����" << endl << endl;
		}
		if (is_alive[i] && exprimenters[i].getMoney() <= -3000) {
			is_alive[i] = false;
			cout << endl << "idΪ" << exprimenters[i].getId() << "��ʵ�������Ʋ���" << endl << endl;
		}
	}

	int food_sum = 0;
	double money_sum = 0;
	for (int i = 0; i < N; i++) {
		if (is_alive[i]) {
			food_sum += exprimenters[i].getFood();
			money_sum += exprimenters[i].getMoney();
		}
	}

	standard = money_sum / food_sum;

	srand((int)time(NULL));
	for (int i = 0; i < N / 2; i++) {
		int luck = rand() % N;
		if (is_alive[luck]) {
			exprimenters[luck].updateFood(2);
		}
	}

	cout << loops << "���꣡" << endl;
	loops++;
}

void show() {
	for (int i = 0; i < N; i++) {
		if (is_alive[i]) {
			cout << "id:" << exprimenters[i].getId() << ",��թֵ:" << exprimenters[i].getTrick()
				<< ",�ж�����:" << exprimenters[i].getJudge() << ",������:" << exprimenters[i].getCaution() << endl;
			cout << "���:" << exprimenters[i].getMoney() << ",ʳ��:" << exprimenters[i].getFood()
				<< ",����:" << exprimenters[i].getCredit() << endl << endl;
		}
	}
}

int main(void) {
	init();

	for (int i = 0; i < Loop; i ++) {
		update();
//		cout << loops << "�ָ�����ɣ�" << endl;
		reset();
//		cout << loops << "��������ɣ�" << endl;
		settlement();
//		cout << loops << "�ֽ�����ɣ�" << endl;
//		Sleep(1000);
	}

	show();
}

void trade(int id_seller, int id_buyer) {
	bool seller_trick = exprimenters[id_seller].isGointTrick(exprimenters[id_buyer].getJudge());
	bool buyer_see_through = exprimenters[id_buyer].isSeeThrough(exprimenters[id_seller].getTrick());

	double m;
	int difference_credit;
	if (seller_trick) {
		m = exprimenters[id_seller].setSalePrice(standard);
		difference_credit = (m - standard) / exp(exprimenters[id_seller].getTrick()) * 100;
	}
	else {
		m = standard;
		difference_credit = 10;
	}

	// �жϽ����Ƿ�ɹ�
	if (seller_trick && buyer_see_through) {
		// ������ƭ����ҿ���������ʧ�� 
		exprimenters[id_buyer].updateCredit(difference_credit);
		exprimenters[id_seller].updateCredit(-difference_credit);
	}
	else if (!seller_trick && buyer_see_through) {
		// ���ҳ�ʵ�������Ϊ��ƭ������ʧ��
		exprimenters[id_buyer].updateCredit(-difference_credit);
		exprimenters[id_seller].updateCredit(difference_credit);
	}
	else {
		// ������ƭ�����δ�ܿ��������׳ɹ� or ���ҳ�ʵ������Ͽɣ����׳ɹ�
		exprimenters[id_buyer].updateFood(trade_account);
		exprimenters[id_buyer].updateMoney(-(trade_account * m));
		exprimenters[id_seller].updateFood(-trade_account);
		exprimenters[id_seller].updateMoney(trade_account * m);
	}

}
