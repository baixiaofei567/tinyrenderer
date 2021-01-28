﻿#include<iostream>
#include<vector>
#include<cmath>
#include"tgaimage.h"
#include"model.h"
#include"geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
Model* model = NULL;
const int width = 800;
const int height = 800;

/// <summary>
/// bresenham
/// </summary>
/// <param name="x0">起点x</param>
/// <param name="y0">起点y</param>
/// <param name="x1">终点x</param>
/// <param name="y1">终点y</param>
/// <param name="image">在image上操作</param>
/// <param name="color">像素点的颜色</param>
void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {//如果x步长小于y，就让y每次走1步
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) {//make it left to right
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	float derror = std::abs(dy / float(dx));
	float error = 0;
	for (int x = x0; x <= x1; ++x) {
		float t = (x - x0) / float(x1 - x0);
		int y = y0 * (1. - t) + y1 * t;
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
		error += derror;
		if (errno > .5) {
			y += (y1 > y0 ? 1 : -1);
			errno -= 1;
		}
	}
}

int main(int argc, char** argv) {
	if (2 == argc) {
		model = new Model (argv[1]);
	}
	else {
		model = new Model ("obj/african_head.obj");
	}
	TGAImage image(width, height, TGAImage::RGB);

	for (int i = 0; i < model->nfaces(); ++i) {
		std::vector<int> face = model->face(i);
		for (int j = 0; j < 3; ++j) {
			Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j + 1) % 3]);
			int x0 = (v0.x + 1.) * width / 2;
			int y0 = (v0.y + 1.) * height / 2;
			int x1 = (v1.x + 1.) * width / 2;
			int y1 = (v1.y + 1.) * height / 2;
			line(x0, y0, x1, y1, image, white);
		}
	}

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	delete model;
	return 0;
}
