#include <iostream>
#include <opencv2/opencv.hpp>
#include "BlurMap.h"

int main() {
    cv::Mat srcImage = cv::imread("/Users/stephenswetonic/Documents/FocusStackCLI/out/artifacts/FocusStackCLI_jar/clitest2/result.jpg"); // Use full path!
    BlurMap bm("/Users/stephenswetonic/Desktop/568.png", 10, 3, "test");
    bm.getBlurMap();
}
