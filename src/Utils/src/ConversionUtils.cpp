/* This file is part of the Visual-Odometry-Cpp project.
* It is subject to the license terms in the LICENSE file
* found in the top-level directory of this distribution.
*
* Copyright (C) 2020 Manuel Kraus
*/

#include <Vocpp_Utils/ConversionUtils.h>

namespace VOCPP
{
namespace Utils
{
    
cv::Mat1f Point2fToMatHomCoordinates(const cv::Point2f& in_point)
{
    cv::Mat1f ret = (cv::Mat1f(3, 1) << in_point.x, in_point.y, 1.0);

    return ret;
}

cv::Mat1f Point3fToMatHomCoordinates(const cv::Point3f& in_point)
{
    cv::Mat1f ret = (cv::Mat1f(4, 1) << in_point.x, in_point.y, in_point.z, 1.0);

    return ret;
}

} //namespace Utils
} //namespace VOCPP
