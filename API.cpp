#include <iostream>
#include "WordRecord.h"
#include "API.h"
using namespace std;

//记录所有2字词组，没有记录则添加，有记录权重++
bool search2Table(Word2Record *tb, char target[5], int &times) {
	int i;
	for (i = 0; i < times; i++) {
		if (target[0] == (tb + i)->doubleWord[0] && target[1] == (tb + i)->doubleWord[1] &&
			target[2] == (tb + i)->doubleWord[2] && target[3] == (tb + i)->doubleWord[3]) {
			(tb + i)->weight++;
			if ((tb + i)->weight == 2)
				(tb + i)->isAvailable = true;
			return true;
		}
	}
	times++;
	(tb + times)->doubleWord[0] = target[0]; (tb + times)->doubleWord[1] = target[1];
	(tb + times)->doubleWord[2] = target[2]; (tb + times)->doubleWord[3] = target[3];
	(tb + times)->doubleWord[4] = '\0';
	(tb + times)->weight++;
	return false;
}

//查找3字词组，没有记录则添加，有记录权重++
bool search3Table(Word3Record *tb, char target[7], int &times) {
	int i;
	for (i = 0; i < times; i++) {
		if (target[0] == (tb + i)->tripleWord[0] && target[1] == (tb + i)->tripleWord[1] &&
			target[2] == (tb + i)->tripleWord[2] && target[3] == (tb + i)->tripleWord[3] &&
			target[4] == (tb + i)->tripleWord[4] && target[4] == (tb + i)->tripleWord[4]) {
			(tb + i)->weight++;
			if ((tb + i)->weight == 2)
				(tb + i)->isAvailable = true;
			return true;
		}
	}
	times++;
	(tb + times)->tripleWord[0] = target[0]; (tb + times)->tripleWord[1] = target[1];
	(tb + times)->tripleWord[2] = target[2]; (tb + times)->tripleWord[3] = target[3];
	(tb + times)->tripleWord[4] = target[4]; (tb + times)->tripleWord[5] = target[5];
	(tb + times)->tripleWord[6] = '\0';
	(tb + times)->weight++;
	return false;
}

//处理文本串，将文本串存入表
void AutoDevide(Word2Record *tb2, Word3Record *tb3, int &times2, int &times3, char *c) {
	int buf = 0;
	char word2Record[5];
	char word3Record[7];

	while (true) {
		if (*(c + buf) == '\0')
			break;
		else if (*(c + buf) >= 0) {
			buf++;
			goto next;
		}
		else if (*(c + buf) >= 0xA1-256 && *(c + buf) <= 0xA9-256) {
			buf += 2;
			goto next;
		}
		else if (*(c + buf + 2) >= 0) {
			buf += 2;
			goto next;
		}
		else if (*(c + buf + 2) >= 0xA1-256 && *(c + buf + 2) <= 0xA9-256) {
			buf += 4;
			goto next;
		}
		//else if (*(c + buf + 1) == '\0')
		//	break;
		//if (*(c + buf + 4) != '\0') {
		for (int i = 0; i < 4; i++) 
			word2Record[i] = *(c + buf + i);
		word2Record[4] = '\0';
		search2Table(tb2, word2Record, times2);
		//}
		//if (*(c + buf + 4) >= 0) {
		//	buf += 5;
		//	goto next;
		//}
		if (*(c + buf + 4) < 0 && (*(c + buf + 4) < 0xA1 - 256 || *(c + buf + 2) > 0xA9 - 256)) {
			for (int i = 0; i < 6; i++)
				word3Record[i] = *(c + buf + i);
			word3Record[6] = '\0';
			search3Table(tb3, word3Record, times3);
		}
		//if (*(c + buf + 5) >= 0xA1-256 && *(c + buf + 5) <= 0xA9-256) {
		//	buf += 6;
		//	goto next;
		//}
		//if (*(c + buf + 7) != '\0') {
		//	for (int i = 0; i < 6; i++)
		//		word3Record[i] = *(c + buf + i);
		//	word3Record[6] = '\0';
		//	search3Table(tb3, word3Record, times3);
		//}
		buf += 2;
	next:;
	}
	ReScan(tb2, tb3, times2, times3, c);
	//1;
}

void ReScan(Word2Record *tb2, Word3Record *tb3, int times2, int times3, char *c) {
	char text2Front[5], text2Back[5];
	int wgt2Front, wgt2Back;
	text2Front[4] = '\0';
	text2Back[4] = '\0';
	for (int i = 0; i < times3; i++) {
		wgt2Front = 0; wgt2Back = 0;
		text2Front[0] = (tb3 + i)->tripleWord[0];
		text2Front[1] = (tb3 + i)->tripleWord[1];
		text2Front[2] = (tb3 + i)->tripleWord[2];
		text2Front[3] = (tb3 + i)->tripleWord[3];

		text2Back[0] = (tb3 + i)->tripleWord[0];
		text2Back[1] = (tb3 + i)->tripleWord[1];
		text2Back[2] = (tb3 + i)->tripleWord[2];
		text2Back[3] = (tb3 + i)->tripleWord[3];

		for (int j = 0; j < times2; j++) {
			if (!strcmp(text2Front, (tb2 + j)->doubleWord))
				wgt2Front = (tb2 + j)->weight;
			if (!strcmp(text2Back, (tb2 + j)->doubleWord))
				wgt2Back = (tb2 + j)->weight;
			if (wgt2Front != 0 && wgt2Back != 0)break;
		}
		if (wgt2Front > (tb3 + i)->weight&&wgt2Back > (tb3 + i)->weight)
			(tb3 + i)->isAvailable = false;
	}
}

void DevideResult(Word2Record *tb2, Word3Record *tb3, int times2, int times3, char *c) {
	int buf = 0;
	int i, j;
	char text2[5];
	char text3[7];
	text2[4] = '\0';
	text3[6] = '\0';
	//cout << tb2->doubleWord << tb2->weight;
	//cout << (tb2 + 1)->doubleWord << (tb2 + 1)->weight;
	cout << "\n分词结果如下：\n\n";
	while (true) {
		//printf("testLoop");
		if (*(c + buf) == '\0')break;
		//else if ((*c + buf + 1) == '\0')break;
		else if (*(c + buf) >= 0) {
			//if (*(c + buf - 2) < 0)
			//	cout << ' ';
			cout << *(c + buf);
			if (*(c + buf + 1) < 0) 
				cout << ' ';
			buf++;
			//cout << "test1";
			continue;
		}
		else if (*(c + buf) >= 0xA1-256 && *(c + buf) <= 0xA9-256) {
			cout << *(c + buf) << *(c + buf + 1) << ' ';
			buf += 2;
			//cout << "test2";
			continue;
		}
		else {
			if (*(c + buf + 2) < 0 && (*(c + buf + 2) < 0xA1 - 256 || *(c + buf + 2) > 0xA9 - 256)&&
				*(c + buf + 4) < 0 && (*(c + buf + 4) < 0xA1 - 256 || *(c + buf + 4) > 0xA9 - 256)) {

				text3[0] = *(c + buf); text3[1] = *(c + buf + 1); text3[2] = *(c + buf + 2);
				text3[3] = *(c + buf + 3); text3[4] = *(c + buf + 4); text3[5] = *(c + buf + 5);

				text2[0] = *(c + buf); text2[1] = *(c + buf + 1);
				text2[2] = *(c + buf + 2); text2[3] = *(c + buf + 3);

				for (i = 0; i < times3; i++)
					if (!strcmp(text3, (tb3 + i)->tripleWord) && (tb3 + i)->isAvailable) {
						cout << text3 << ' ';
						buf += 6;
						goto nextRound;
					}
				for (i = 0; i < times2; i++)
					if (!strcmp(text2, (tb2 + i)->doubleWord) && (tb2 + i)->isAvailable) {
						cout << text2 << ' ';
						buf += 4;
						goto nextRound;
					}
				cout << *(c + buf) << *(c + buf + 1) << ' ';
				buf += 2;
			}
			else if (*(c + buf + 2) < 0 && (*(c + buf + 2) < 0xA1 - 256 || *(c + buf + 2) > 0xA9 - 256)) {

				text2[0] = *(c + buf); text2[1] = *(c + buf + 1);
				text2[2] = *(c + buf + 2); text2[3] = *(c + buf + 3);
				for (i = 0; i < times2; i++)
					if (!strcmp(text2, (tb2 + i)->doubleWord) && (tb2 + i)->isAvailable) {
						cout << text2 << ' ';
						buf += 4;
						goto nextRound;
					}
				cout << *(c + buf) << *(c + buf + 1) << ' ';
				buf += 2;
			}
			else {
				cout << *(c + buf) << *(c + buf + 1) << ' ';
				buf += 2;
			}
			//cout << *(c + buf) << *(c + buf + 1);
			//buf += 2;
			//cout << "test3";
		}
		//else if()
	nextRound:;
	}
}
