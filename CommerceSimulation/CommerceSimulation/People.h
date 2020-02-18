#pragma once
#include<random>
#include<time.h>
#include"N_distribution.h"

class People {
private:
	int id;
	double money;		// �ֽ�
	int food;			// ʳ��

	int credit;			// ������
	int trick;			// ��թֵ
	int judge;			// �ж�����
	int caution;		// ������

public:
	People() {
		money = 100;
		food = 100;
		credit = 100;
	}

	void setId(int id) {
		this->id = id;
	}
	void setTrick(int trick) {
		this->trick = trick;
	}
	void setJudge(int judge) {
		this->judge = judge;
	}
	void setCaution(int caution) {
		this->caution = caution;
	}

	// �Ƿ񿴴�
	bool isSeeThrough(int _trick) {
		int s = (10 + judge - _trick) * 5;
		if ((rand() % 100) < s)
			return true;
		else return false;
	}

	// �Ƿ���ƭ
	bool isGointTrick(int _judge) {
		N_Distribution N(0, 1);
		srand((int)time(NULL));

		int s = 100 - (10 + _judge - trick) * 5;
		int prob = (int)((double)s * N.probability(-caution, caution));
		if (rand() % 100 < s)
			return true;
		else return false;
	}

	// ���ü۸�
	double setSalePrice(double standard) {
		srand((int)time(NULL));

		return standard + exp(trick) * (rand() % 100) * 0.01;
	}

	// �ж����� true ����false ��
	bool isSell() {
		if (food == 0) return false;

		N_Distribution N(0, 1);
		double p = 1 - exp(-abs(money - food)) * N.probability(-caution, caution);
		srand((int)time(NULL));
		int x = rand() % 100;
		int _p = p * 100;
		if (money < food) {
			if (x < _p) return true;
			else return false;
		}
		else if (money > food){
			if (x < _p) return false;
			else return true;
		}
		else {
			if (rand() % 2 == 0) return true;
			else return false;
		}
	}

	void updateMoney(double difference) {
		money += difference;
	}
	void updateFood(int difference) {
		food += difference;
		if (food < 0) food = 0;
	}
	void updateCredit(int difference) {
		credit += difference;
		if (credit < 0) credit = 0;
		if (credit > 100) credit = 100;
	}

	int getTrick() {
		return trick;
	}
	int getJudge() {
		return judge;
	}
	int getCredit() {
		return credit;
	}
	int getCaution() {
		return caution;
	}
	double getMoney() {
		return money;
	}
	int getId() {
		return id;
	}
	int getFood() {
		return food;
	}

	void clear() {
		money = 0;
		food = 0;
	}
};