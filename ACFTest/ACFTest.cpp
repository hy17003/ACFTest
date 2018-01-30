// ACFTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gradientUtil.h"
#include "wrappers.h"
#include "CellArray.h"
#include "ACFDetector.h"

#include <opencv2/opencv.hpp>

int main() {
	try {
		CellArray I, E, O;
		ACFDetector acfDetector;
		acfDetector.loadModel("model/acfmodel_caltech.bin");

		std::vector<cv::String> filenames;
		cv::String folder = "E:/dataset/person";
		cv::glob(folder, filenames);
		for (size_t i = 0; i < filenames.size(); ++i) {
			cv::Mat src = cv::imread(filenames[i]), dst;
			std::cerr << filenames[i] << std::endl;
			cv::cvtColor(src, src, CV_BGR2RGB);
		//	cv::resize(dst, src, cv::Size(640, 480));
			int h = src.rows, w = src.cols, d = 3;
			uint8_t* I = (uint8_t*)wrCalloc(h * w * d, sizeof(uint8_t));
			for (int k = 0; k < d; ++k) {
				for (int c = 0; c < w; ++c) {
					for (int r = 0; r < h; ++r) {
						I[k * w * h + c * h + r] = ((uint8_t*)src.data)[r * w * d + c * d + k];
					}
				}
			}
			Boxes res = acfDetector.acfDetect(I, h, w, d);
			printf("%d\n", (int)res.size());
			cv::cvtColor(src, src, CV_RGB2BGR);
			for (size_t i = 0; i < res.size(); ++i) {
				printf("%d %d %d %d %.4f\n", res[i].c + 1, res[i].r + 1, res[i].w, 
					res[i].h, res[i].s);
				cv::rectangle(src, cv::Rect(res[i].c, res[i].r, res[i].w, res[i].h),
					cv::Scalar(0, 0, 255), 1);
			}
			cv::imshow("result", src);
			cv::waitKey(0);
			wrFree(I);
		}
	}
	catch (const std::string &e) {
		std::cerr << e << std::endl;
	}
	return 0;
}