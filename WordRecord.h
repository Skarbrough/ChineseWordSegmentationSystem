#pragma once

struct Word2Record {
	char doubleWord[5];
	int weight = 0;
	bool isAvailable = false;
};
struct Word3Record {
	char tripleWord[7];
	int weight = 0;
	bool isAvailable = false;
};