#include <iostream>
#include "WordRecord.h"
#include "API.h"

#define MAX_SIZE 10000

using namespace std;

//全局变量，用于记录单词信息
char textDevide[MAX_SIZE];
Word2Record table2Words[MAX_SIZE] = { 0 };
Word3Record table3Words[MAX_SIZE] = { 0 };
int time2Words = -1;
int time3Words = -1;

int main() {

	cin >> textDevide;
	AutoDevide(table2Words, table3Words, time2Words, time3Words, textDevide);
	DevideResult(table2Words, table3Words, time2Words, time3Words, textDevide);

	system("pause");
	return 0;
}
