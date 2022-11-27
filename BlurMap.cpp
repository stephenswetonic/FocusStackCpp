//
// Created by Stephen Swetonic on 11/23/22.
//

#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <algorithm>
#include "BlurMap.h"

BlurMap::BlurMap(string inputColor, int blocksize, int svdNum, string projectName) {
    srand((unsigned)time(NULL));
    int random = rand();

    this->filePathG = projectName + to_string(rand()) + "g" + ".jpg";
    this->filePathM = projectName + to_string(rand()) + "m" + ".png";
    this->originalColor = imread(inputColor, IMREAD_COLOR);
    this->original = imread(inputColor, IMREAD_GRAYSCALE);
    this->maskedImage = imread(inputColor,  IMREAD_UNCHANGED);
    cvtColor(this->maskedImage, this->maskedImage, COLOR_RGB2BGRA);
    this->blocksize = blocksize;
    this->svdNum = svdNum;
    this->originalHeight = this->originalColor.rows;
    this->originalWidth = this->originalColor.cols;


}

void BlurMap::test() {}

void BlurMap::getBlurMap() {
    auto start = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    //Convert grayscale mat to 2d vector
    vector<vector<float>> image(this->original.rows);
    for (int i = 0; i < this->original.rows; i++) {
        this->original.row(i).reshape(1, 1).copyTo(image[i]);
    }


    int newImgWidth = this->originalWidth + (blocksize * 2);
    int newImgHeight = this->originalHeight + (blocksize * 2);

    vector<vector<float>> newImg(newImgWidth, vector<float>(newImgHeight, 0.0));

    // Filling newImg with image data and handling edges
    for (int i = 0; i < newImgHeight; i++) {
        for (int j = 0; j < newImgWidth; j++) {
            int p = 0;
            int q = 0;
            if (i < blocksize) {
                p = blocksize - i;
            } else if (i > (originalHeight + blocksize - 1)) {
                p = originalHeight * 2 - i;
            } else {
                p = i - blocksize;
            }
            if (j < blocksize) {
                q = blocksize - j;
            } else if (j > (originalWidth + blocksize - 1)) {
                q = originalWidth * 2 - j;
            } else {
                q = j - blocksize;
            }

            // q and p might be swapped
            float c = image[p][q];
            newImg[j][i] = c;
        }
    }

    vector<vector<float>> blurMap(originalWidth, vector<float>(originalHeight, 0.0));
    float maxSV = 0;
    float minSV = 1;
    for (int i = 0; i < originalHeight; i++) {
        for (int j = 0; j < originalWidth; j++) {
            vector<vector<float>> block = fillBlock(newImg, i, j);
            Eigen::MatrixXf mat(block.size(), block[0].size());
            for (int i = 0; i < block.size(); i++) {
                mat.row(i) = Eigen::VectorXf::Map(&block[i][0], block[i].size());
            }
            Eigen::JacobiSVD<Eigen::MatrixXf> svd(mat, Eigen::ComputeThinU | Eigen::ComputeThinV);
            Eigen::VectorXf S = svd.singularValues();

            float topSV = 0.0;
            float totalSV = 0.0;
            for (int k = 0; k < svdNum; k++) {
                topSV += S[k];
            }
            for (float v : S) {
                totalSV += v;
            }
            float svDegree = topSV / totalSV;
            maxSV = max(maxSV, svDegree);
            minSV = min(minSV, svDegree);
            blurMap[j][i] = svDegree;
        }
    }
    for (int i = 0; i < originalHeight; i++) {
        for (int j = 0; j < originalWidth; j++) {
            blurMap[j][i] = (blurMap[j][i] - minSV) / (maxSV - minSV);
        }
    }
    auto end = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    cout << (end - start) << endl;

}

void BlurMap::analyzeSVD() {}

vector<vector<float>> BlurMap::fillBlock(vector<vector<float>> &img, int y, int x) {
    vector<vector<float>> block(blocksize * 2, vector<float>(blocksize * 2, 0.0));
    for (int i = 0; i < blocksize*2; i++) {
        for (int j = 0; j < blocksize*2; j++) {
            block.at(j).at(i) = img[j+x][i+y];
        }
    }
    return block;
}
