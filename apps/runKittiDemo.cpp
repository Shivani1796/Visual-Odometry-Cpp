/* This file is part of the Visual-Odometry-Cpp project.
* It is subject to the license terms in the LICENSE file
* found in the top-level directory of this distribution.
*
* Copyright (C) 2020 Manuel Kraus
*/

#include <stdio.h>
#include <opencv2/opencv.hpp>

#include<Vocpp_Master/Master.h>
#include<Vocpp_Interface/Frame.h>
#include<Vocpp_Interface/DeltaPose.h>
#include<Vocpp_Calibration/MonoCameraCalibration.h>

using namespace cv;
using namespace VOCPP;

int main(int argc, char** argv)
{
    if (argc != 2)
   {
        printf("usage: main.exe <Image_Path> (e.g. ./data/*.jpg) \n");
        return -1;
    }

    // Construct path
    String path(argv[1]);
    std::vector<String> imageNames;
    glob(path, imageNames);

    // Instantiate master
    Master::Master voMaster;

    // Construct MonoCameraCalibration
    // Hardcoded for KITTI data set sequence 0, left image
    cv::Mat calibMat = cv::Mat::eye(3, 3, CV_32F);
    const float focLength = 7.18856e+02F;
    const float camCenterX = 6.071928000000e+02F;
    const float camCenterY = 1.852157000000e+02F;
    const float skew = 0.0F;
    VOCPP::Calibration::MonoCameraCalibration monoCalib(focLength, camCenterX, camCenterY, skew);

    // Load calibration, we know it is valid
    (void)voMaster.LoadCalibration(monoCalib);

    int frameId = 0U;

    cv::Mat currentPose = cv::Mat::eye(3, 3, CV_32F);
    cv::Mat currentCamCenter = cv::Mat::zeros(3, 1, CV_32F);
    for (auto imageName : imageNames)
    {
        // Convert the image to grayscale CV_32F, here from CV_8U to CV_32F
        cv::Mat grayScaleImg;
        cv::cvtColor(imread(imageName, 1), grayScaleImg, COLOR_BGR2GRAY);
        grayScaleImg.convertTo(grayScaleImg, CV_32F, 1.0 / 255.0);

        // Feed frame to Master
        Frame frame(std::move(grayScaleImg), frameId);
        voMaster.FeedNextFrame(frame);

        DeltaPose delta = voMaster.GetLastDeltaPose();
        
        // Concatenate delta poses to get absolute pose in world coordinate system
        currentPose = delta.GetDeltaOrientation() * currentPose;
        // The translation is given from last frame to current frame in the last frame coordinate system
        // --> Transform the translation vector back to world coordinate system using the inverse rotation of the last frame
        currentCamCenter = currentPose.t() * delta.GetCamCenterTranslation() + currentCamCenter;

        frameId++;
    }
   
    return 0;
}