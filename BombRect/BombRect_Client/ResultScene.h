#pragma once
class ResultScene
{
	int ranking[4];
	int index;

public:
	int SetRanking(int id) {
		ranking[index++] = id;
		return index;
	}
};
