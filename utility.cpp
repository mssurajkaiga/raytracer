#include "utility.h"

Film::Film(int x, int y)
{
	size_x = x;
	size_y = y;
	data = new Vector3f[size_x*size_y]; /* row major storage */
}

Film::Film()
{
	Film(800, 600);
}

void Film::commit(Sample& sample, Vector3f& color)
{
	*(data + (int)((sample.y-1)*size_y + sample.x)) = color;
}

void Film::output(std::string filename)
{
	return;
}