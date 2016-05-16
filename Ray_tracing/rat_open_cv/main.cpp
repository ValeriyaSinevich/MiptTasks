#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <thread>
#include "View.h"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\core\core.hpp"
#include "opencv\highgui.h"
#include "opencv\cv.h"
#include "parser.h"



	
using namespace cv;
using namespace std;


void scale(Mat & image, Mat & scaled_image,  int scale) {
	int h = image.size().height;
	int w = image.size().width;
	Size size(w * scale, h * scale);
	resize(image, scaled_image, size);
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			for (int k = 0; k < scale; ++k) {
				for (int l = 0; l < scale; ++l)
					scaled_image.at<Vec3b>(Point(i + k, j + l)) = scaled_image.at<Vec3b>(Point(i, j));
			}	
		}
	}
}

int main(int argc, char** argv)
{
	Viewer viewer;
	screen scr;
	kd_tree *objects = nullptr;
	vector<torch> torches;
	vector<Object*> objs;
	vector<vector<color> > img;
	//point location = Parser::parse_stl_input(scr, objects, torches, "gnome.stl", objs);
	point location = Parser::parse_input(scr, objects, torches, objs);
	cout << "width " <<  scr.width << endl << "height " << scr.height << endl;
	cout << "check mult" << vect(1, 1, 1) * 2 << endl;
	viewer = Viewer(location);

	img.resize(scr.height);
	for (int i = 0; i < scr.height; ++i) {
		img[i].resize(scr.width);
	}

	scr.calc_screen_points();

	int scr_size = scr.scr_points.size();
	unsigned number_of_threads = std::thread::hardware_concurrency() / 2;
	//cout << "concurrency " << number_of_threads << endl;
	std::vector<std::thread> threads;
	//cout << "size " << scr_size << ' ' << scr_size / number_of_threads << endl;
	for (int i = 0; i < number_of_threads; ++i) {
		//cout << scr_size / number_of_threads * i << ' ' << scr_size / number_of_threads * (i + 1) << endl;
		threads.push_back(std::thread([&viewer, &scr, scr_size, number_of_threads, i, &objects, &img, &torches]
		{viewer.view(scr, scr_size / number_of_threads * i, scr_size / number_of_threads * (i + 1), objects, img, torches);}));
		//threads.push_back(std::thread(&Viewer::view, std::ref(viewer), std::ref(scr), scr_size / number_of_threads * i,
			//scr_size / number_of_threads * (i + 1), objects, std::ref(img), std::ref(torches)));
	}
	for (std::vector<std::thread>::iterator i = threads.begin(); i < threads.end(); ++i) {
		(*i).join();
	}
	delete objects;

//	viewer.view(scr, 0, scr_size, objects, img, torches);
	for (unsigned int i = 0; i < objs.size(); ++i) {
		delete objs[i];
	}

	//cout << endl << img.size() << endl;
	//cout << img[19][19].rgb[0] << endl;
	Mat image(scr.height, scr.width, CV_8UC3, Scalar(0, 0, 0));
	for (int i = 0; i < scr.height; ++i) {
		for (int j = 0; j < scr.width; ++j) {
			Vec3b col = Vec3b(img[i][j].rgb[0], img[i][j].rgb[1], img[i][j].rgb[2]);
			image.at<Vec3b>(i, j) = col;	
		}
	}
	Mat scaled_image;
	scale(image, scaled_image, 1);
	namedWindow("Camera", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Camera", scaled_image); // Show our image inside it.

	waitKey(0); // Wait for a keystroke in the window
	return 0;
}

