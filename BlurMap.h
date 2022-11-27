#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Eigen>
#include <iostream>
#include <string>
#ifndef OPENCVTEST_BLURMAP_H
#define OPENCVTEST_BLURMAP_H

using namespace std;
using namespace cv;

class BlurMap {

public:
    Mat original;
    Mat originalColor;
    Mat maskedImage;
    string filePathG;
    string filePathM;
    int blocksize;
    int svdNum;
    int originalWidth;
    int originalHeight;
    vector<vector<float>> blurMap;

    BlurMap(string inputColor, int blocksize, int svdNum, string projectName);
    void test();
    void getBlurMap();
    void analyzeSVD();
    vector<vector<float>> fillBlock(vector<vector<float>> &img, int y, int x);


};


#endif //OPENCVTEST_BLURMAP_H
