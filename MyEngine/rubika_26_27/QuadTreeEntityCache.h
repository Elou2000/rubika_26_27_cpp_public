#pragma once


class QuadTreeEntityCache
{
public:
	void SetMaxThreeDepth(int depth);
	void SetMaxEltLeaf(int depth);
private:

	int maxThreeDepth;
	int maxEltLeaft;
};

