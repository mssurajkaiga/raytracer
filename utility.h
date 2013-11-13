#ifndef UTILITY_H
#define UTILITY_H

#include "core.h"
#include <string>

class Film
{
public:
	int size_x, size_y;
	Vector3f* data;
	Film::Film();
	Film::Film(int x, int y);
	void commit(Sample& sample, Vector3f& color);
	void output(string filename);
};

#endif