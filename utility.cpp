#include "utility.h"

Film::film(int x, int y)
{
	size_x = x;
	size_y = y;
	data = new (std::nothrow) Vector3f[size_x * size_y]; /* row major storage */
}

Film::film()
{
	film(800, 600);
}

void Film::commit(Sample& sample, Vector3f& color)
{
	for (int i = 0; i<3; i++)
		data[(sample.y-1) * size_y + sample.x][i] = color[i];
}

void Film::output(string filename)
{
	return;
}