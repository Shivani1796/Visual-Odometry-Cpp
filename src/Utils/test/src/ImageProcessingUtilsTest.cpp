/* This file is part of the Visual-Odometry-Cpp project.
* It is subject to the license terms in the LICENSE file
* found in the top-level directory of this distribution.
*
* Copyright (C) 2020 Manuel Kraus
*/

#include <Vocpp_Utils/ImageProcessingUtils.h>
#include <Vocpp_Utils/NumericalUtilities.h>
#include <Vocpp_Utils/FrameRotations.h>
#include <Vocpp_Utils/ConversionUtils.h>

#include <gtest/gtest.h>

using namespace VOCPP::Utils;

TEST(ExtractLocalMaximaTest, BlockMatrix)
{
    // Construct frame filled with ones
    cv::Mat ones = GetWindowKernel(5);

    std::vector<cv::Point2f> max;
    ExtractLocalMaxima(ones, 2U, max, 0U);
    
    // We expect 5 * 5 maxima since all values are equal
    EXPECT_EQ(max.size(), 25);
}

TEST(ExtractLocalMaximaTest, OneMax)
{
    // Construct image with one center maxima
    cv::Mat ones = cv::Mat::zeros(5, 5, CV_32F);
    ones.at<float>(2, 2) = 1.0;

    std::vector<cv::Point2f> max;
    ExtractLocalMaxima(ones, 2U, max, 0U);

    // We expect one single maximum
    EXPECT_EQ(max.size(), 1);
    EXPECT_FLOAT_EQ(max[0].x, 2.0);
    EXPECT_FLOAT_EQ(max[0].y, 2.0);
}

TEST(ExtractLocalMaximaTest, TestDistanceCheck)
{
    // Construct image with three close-by maxima
    cv::Mat ones = cv::Mat::zeros(5, 5, CV_32F);
    ones.at<float>(2, 2) = 1.0;
    ones.at<float>(2, 3) = 3.0;
    ones.at<float>(3, 2) = 4.0;

    std::vector<cv::Point2f> max;
    ExtractLocalMaxima(ones, 0U, max, 0U);

    // Without any distance check we expect three maxima
    EXPECT_EQ(max.size(), 3);
    EXPECT_FLOAT_EQ(max[0].x, 2.0);
    EXPECT_FLOAT_EQ(max[0].y, 2.0);
    EXPECT_FLOAT_EQ(max[1].x, 2.0);
    EXPECT_FLOAT_EQ(max[1].y, 3.0);
    EXPECT_FLOAT_EQ(max[2].x, 3.0);
    EXPECT_FLOAT_EQ(max[2].y, 2.0);

    max.clear();
    // Do it again, this time with a minimum distance
    ExtractLocalMaxima(ones, 1U, max, 0U);

    // Only one maximum survives
    EXPECT_EQ(max.size(), 1);
    EXPECT_FLOAT_EQ(max[0].x, 2.0);
    EXPECT_FLOAT_EQ(max[0].y, 3.0);
}

TEST(ExtractLocalMaximaTest, SubPixPrecisionTest)
{
    // Construct image with two maxima
    cv::Mat ones = cv::Mat::zeros(5, 5, CV_32F);
    ones.at<float>(2, 2) = 2.0;
    ones.at<float>(2, 3) = 1.0;

    std::vector<cv::Point2f> max;
    //Average of a pixel distance of 1
    ExtractLocalMaxima(ones, 1U, max, 1U);

    // Test averaged pixel position
    EXPECT_EQ(max.size(), 1);
    EXPECT_NEAR(max[0].x, 2.33, 1e-2);
    EXPECT_FLOAT_EQ(max[0].y, 2.0);

    max.clear();
    // Now test what happens when the averaging distance
    // exceeds the image range, we expect to have one maximum
    ExtractLocalMaxima(ones, 1U, max, 3U);
    EXPECT_EQ(max.size(), 1);
    EXPECT_FLOAT_EQ(max[0].y, 2.0);
    EXPECT_FLOAT_EQ(max[0].y, 2.0);
}

// Test essential matrix decomposition with translation only
TEST(DecomposeEssentialMatrixTest, PureTranslationTest)
{
    // Construct set of 3D points in world coordinate system
    std::vector<cv::Point3f> realWorldPoints;
    const float minDist = 150.0;
    for (int it = 0; it < 1000; it++)
    {
        realWorldPoints.push_back(cv::Point3f(DrawFloatInRange(-minDist, minDist), DrawFloatInRange(-minDist, minDist), DrawFloatInRange(50.0, 150.0)));
    }

    for (auto coord : realWorldPoints)
    {
        // Projection matrix for left image, the translation is measured in the system of the left frame
        cv::Mat projMatLeft;
        cv::Mat translationLeft = (cv::Mat_<float>(3, 1) << DrawFloatInRange(-1.0, 1.0), DrawFloatInRange(-1.0, 1.0), DrawFloatInRange(-5.0, 5.0));
        EXPECT_TRUE(GetProjectionMatrix(cv::Mat::eye(3, 3, CV_32F), translationLeft, projMatLeft));

        // Get projected points in both camera frames, the right ones uses a trivial projection matrix
        cv::Mat projPointLeft = (projMatLeft * VOCPP::Utils::Point3fToMatHomCoordinates(coord));
        cv::Point2f imgPointsLeft = cv::Point2f(projPointLeft.at<float>(0, 0) / projPointLeft.at<float>(2, 0), projPointLeft.at<float>(1, 0) / projPointLeft.at<float>(2, 0));
        cv::Point2f imgPointsRight = cv::Point2f(coord.x / coord.z, coord.y / coord.z);

        // Compute true essential matrix (which is only given by the crossproduct matrix of the translation
        cv::Mat translatCross;
        GetCrossProductMatrix(translationLeft, translatCross);
        cv::Mat essentialMat = translatCross;

        // Decompose the essential matrix
        cv::Vec3f decompTranslat;
        cv::Mat decompRotMat;
        DecomposeEssentialMatrix(essentialMat, imgPointsLeft, imgPointsRight, decompTranslat, decompRotMat);
        // The true translation and the extracted one may differ by a global scale
        decompTranslat = decompTranslat * (translationLeft.at<float>(0, 0) / decompTranslat[0]);

        // And check both extracted translation and rotation
        cv::Mat eye = cv::Mat::eye(3, 3, CV_32F);
        for (int rowIt = 0; rowIt < 3; rowIt++)
        {
            EXPECT_NEAR(decompTranslat[rowIt], translationLeft.at<float>(rowIt, 0), 1e-2);
            for (int colIt = 0; colIt < 3; colIt++)
            {
                EXPECT_NEAR(decompRotMat.at<float>(rowIt, colIt), eye.at<float>(rowIt, colIt), 1e-2);
            }
        }
    }
  
}

// Test essential matrix decomposition with translation and rotation
TEST(DecomposeEssentialMatrixTest, TranslationAndRotationTest)
{
    // Construct set of 3D points in world coordinate system
    std::vector<cv::Point3f> realWorldPoints;
    const float minDist = 150.0;
    for (int it = 0; it < 150; it++)
    {
        realWorldPoints.push_back(cv::Point3f(DrawFloatInRange(-minDist, minDist), DrawFloatInRange(-minDist, minDist), DrawFloatInRange(50.0, 150.0)));
    }

    for (auto coord : realWorldPoints)
    {
        // Projection matrix for left image, the translation is measured in the system of the left frame
        cv::Mat projMatLeft;
        cv::Mat translationLeft = (cv::Mat_<float>(3, 1) << DrawFloatInRange(-1.0, 1.0), DrawFloatInRange(-1.0, 1.0), DrawFloatInRange(-5.0, 5.0));
        cv::Mat rotMat = GetFrameRotationX(DrawFloatInRange(-0.3, 0.3)) * GetFrameRotationY(DrawFloatInRange(-0.3, 0.3)) * GetFrameRotationZ(DrawFloatInRange(-0.3, 0.3));
        EXPECT_TRUE(GetProjectionMatrix(rotMat, translationLeft, projMatLeft));

        // Get projected points in both camera frames, the right ones uses a trivial projection matrix
        cv::Mat projPointLeft = (projMatLeft * VOCPP::Utils::Point3fToMatHomCoordinates(coord));
        cv::Point2f imgPointsLeft = cv::Point2f(projPointLeft.at<float>(0, 0) / projPointLeft.at<float>(2, 0), projPointLeft.at<float>(1, 0) / projPointLeft.at<float>(2, 0));
        cv::Point2f imgPointsRight = cv::Point2f(coord.x / coord.z, coord.y / coord.z);

        // Compute true essential matrix (which is only given by the crossproduct matrix of the translation
        cv::Mat translatCross;
        GetCrossProductMatrix(translationLeft, translatCross);
        cv::Mat essentialMat = translatCross * rotMat;

        // Decompose the essential matrix
        cv::Vec3f decompTranslat;
        cv::Mat decompRotMat;
        DecomposeEssentialMatrix(essentialMat, imgPointsLeft, imgPointsRight, decompTranslat, decompRotMat);
        // The true translation and the extracted one may differ by a global scale
        decompTranslat = decompTranslat * (translationLeft.at<float>(0, 0) / decompTranslat[0]);

        // And check both extracted translation and rotation
        cv::Mat eye = cv::Mat::eye(3, 3, CV_32F);
        for (int rowIt = 0; rowIt < 3; rowIt++)
        {
            EXPECT_NEAR(decompTranslat[rowIt], translationLeft.at<float>(rowIt, 0), 1e-2);
            for (int colIt = 0; colIt < 3; colIt++)
            {
                EXPECT_NEAR(decompRotMat.at<float>(rowIt, colIt), rotMat.at<float>(rowIt, colIt), 1e-2);
            }
        }
    }

}

TEST(PointTriangulationLinearTest, TwoCamerasRotationAndTranslation)
{
    // Construct set of 3D points in world coordinate system
    std::vector<cv::Point3f> realWorldPoints;
    const float minDist = 150.0;
    for (int it = 0; it < 12; it++)
    {
        realWorldPoints.push_back(cv::Point3f(DrawFloatInRange(-minDist, minDist), DrawFloatInRange(-minDist, minDist), DrawFloatInRange(50.0, 150.0)));
    }

    std::vector<cv::Point2f> imgPointsLeft;
    std::vector<cv::Point2f> imgPointsRight;

    // Projection matrix for left image
    cv::Mat projMatLeft;
    cv::Mat rotMatLeft = GetFrameRotationX(DrawFloatInRange(-0.5, 0.5)) * GetFrameRotationY(DrawFloatInRange(-0.5, 0.5)) * GetFrameRotationZ(DrawFloatInRange(-0.5, 0.5));
    cv::Mat translationLeft = (cv::Mat_<float>(3, 1) << DrawFloatInRange(-1.0, 5.0), DrawFloatInRange(-1.0, 5.0), DrawFloatInRange(-5.0, 5.0));
    EXPECT_TRUE(GetProjectionMatrix(rotMatLeft, translationLeft, projMatLeft));
    
    // Projection matrix for right image
    cv::Mat projMatRight;
    cv::Mat rotMatRight = GetFrameRotationX(DrawFloatInRange(-0.5, 0.5)) * GetFrameRotationY(DrawFloatInRange(-0.5, 0.5)) * GetFrameRotationZ(DrawFloatInRange(-0.5, 0.5));
    cv::Mat translationRight = (cv::Mat_<float>(3, 1) << DrawFloatInRange(-1.0, 5.0), DrawFloatInRange(-1.0, 5.0), DrawFloatInRange(-5.0, 5.0));
    EXPECT_TRUE(GetProjectionMatrix(rotMatRight, translationRight, projMatRight));


    // Get camera coordinates of real world points
    for (auto coord : realWorldPoints)
    {
        cv::Mat projPointLeft = (projMatLeft * VOCPP::Utils::Point3fToMatHomCoordinates(coord));
        imgPointsLeft.push_back(cv::Point2f(projPointLeft.at<float>(0, 0) / projPointLeft.at<float>(2, 0), projPointLeft.at<float>(1, 0) / projPointLeft.at<float>(2, 0)));

        cv::Mat projPointRight = (projMatRight * VOCPP::Utils::Point3fToMatHomCoordinates(coord));
        imgPointsRight.push_back(cv::Point2f(projPointRight.at<float>(0, 0) / projPointRight.at<float>(2, 0), projPointRight.at<float>(1, 0) / projPointRight.at<float>(2, 0)));
    }

    // Triangulate points and check
    for (int it = 0; it < static_cast<int>(imgPointsLeft.size()); it++)
    {
        cv::Point3f triangPoint;
        PointTriangulationLinear(projMatLeft, projMatRight, imgPointsLeft[it], imgPointsRight[it], triangPoint);
        EXPECT_NEAR(triangPoint.x, realWorldPoints[it].x, 1e-2);
        EXPECT_NEAR(triangPoint.y, realWorldPoints[it].y, 1e-2);
        EXPECT_NEAR(triangPoint.z, realWorldPoints[it].z, 1e-2);
    }
}