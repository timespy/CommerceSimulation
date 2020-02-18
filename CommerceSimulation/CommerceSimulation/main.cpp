#include<iostream>
using namespace std;
#include<math.h>
#include<Windows.h>

#include"People.h"

double standard = 1;			// 物价标准，当前现金总额/当前食物总额
int loops = 0;					// 循环当前轮数
int Loop = 100;					// 总共循环轮数

int N = 10;						// 总共参与的初始人数
int trade_account = 5;			// 每次交易的食物量
int credit_decrease = 10;		// 每次负债的信誉下降速度
People* exprimenters;			

int N_sell = 0;					// 卖家数量
int* id_sell;

int N_buy = 0;					// 买家数量
int* id_buy;

int* hungry_days;				// 统计每个人的饥饿状态天数
bool* is_alive;					// 记录是否存活
bool* is_matched;				// 记录是否已匹配

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
	// 个人确定买卖
	for (int i = 0; i < N; i++) {
		if (is_alive[i]) {
			if (exprimenters[i].isSell())
				id_sell[N_sell++] = exprimenters[i].getId();
			else
				id_buy[N_buy++] = exprimenters[i].getId();
		}
	}

	cout << loops << "轮:买家有" << N_buy << "家,卖家有" << N_sell << "家" << endl;

	// 匹配交易
	srand((int)time(NULL));
	if (N_sell <= N_buy) {
		// 卖家市场，由卖家选择买家
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
		// 买家市场，买家选择卖家
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
			cout << endl << "id为" << exprimenters[i].getId() << "的实验者正在挨饿！" << endl << endl;
			hungry_days[i] ++;
		}

		if (exprimenters[i].getMoney() <= 0 && is_alive[i]) {
			cout << endl << "id为" << exprimenters[i].getId() << "的实验者正在负债！" << endl << endl;
			exprimenters[i].updateCredit(credit_decrease);
		}

		exprimenters[i].updateFood(-1);
	}

	for (int i = 0; i < N; i++) {
		if (is_alive[i] && hungry_days[i] >= 5) {
			is_alive[i] = false;
			cout << endl << "id为" << exprimenters[i].getId() << "的实验者已饿死！" << endl << endl;
		}
		if (is_alive[i] && exprimenters[i].getMoney() <= -3000) {
			is_alive[i] = false;
			cout << endl << "id为" << exprimenters[i].getId() << "的实验者已破产！" << endl << endl;
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

	cout << loops << "轮完！" << endl;
	loops++;
}

void show() {
	for (int i = 0; i < N; i++) {
		if (is_alive[i]) {
			cout << "id:" << exprimenters[i].getId() << ",狡诈值:" << exprimenters[i].getTrick()
				<< ",判断能力:" << exprimenters[i].getJudge() << ",谨慎度:" << exprimenters[i].getCaution() << endl;
			cout << "存款:" << exprimenters[i].getMoney() << ",食物:" << exprimenters[i].getFood()
				<< ",信誉:" << exprimenters[i].getCredit() << endl << endl;
		}
	}
}

int main(void) {
	init();

	for (int i = 0; i < Loop; i ++) {
		update();
//		cout << loops << "轮更新完成！" << endl;
		reset();
//		cout << loops << "轮重置完成！" << endl;
		settlement();
//		cout << loops << "轮结算完成！" << endl;
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

	// 判断交易是否成功
	if (seller_trick && buyer_see_through) {
		// 卖家欺骗，买家看穿，交易失败 
		exprimenters[id_buyer].updateCredit(difference_credit);
		exprimenters[id_seller].updateCredit(-difference_credit);
	}
	else if (!seller_trick && buyer_see_through) {
		// 卖家诚实，买家认为欺骗，交易失败
		exprimenters[id_buyer].updateCredit(-difference_credit);
		exprimenters[id_seller].updateCredit(difference_credit);
	}
	else {
		// 卖家欺骗，买家未能看穿，交易成功 or 卖家诚实，买家认可，交易成功
		exprimenters[id_buyer].updateFood(trade_account);
		exprimenters[id_buyer].updateMoney(-(trade_account * m));
		exprimenters[id_seller].updateFood(-trade_account);
		exprimenters[id_seller].updateMoney(trade_account * m);
	}

}
