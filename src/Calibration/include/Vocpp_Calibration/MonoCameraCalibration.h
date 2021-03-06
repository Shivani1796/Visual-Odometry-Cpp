/* This file is part of the Visual-Odometry-Cpp project.
* It is subject to the license terms in the LICENSE file
* found in the top-level directory of this distribution.
*
* Copyright (C) 2020 Manuel Kraus
*/

#ifndef VOCPP_MONO_CAMERA_CALIBRATION_H
#define VOCPP_MONO_CAMERA_CALIBRATION_H

#include <opencv2/core/types.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

namespace VOCPP
{
namespace Calibration
{
   
/**
  * /brief This class saves a camera calibration for a monocular camera
  */
class MonoCameraCalibration
{

public:
    
    /**
      * /brief Default constructor for an invalid camera calibration
      */
    MonoCameraCalibration() : m_calibrationMatrix(cv::Mat1f::eye(3,3)),
        m_validCalib(false)
    {
    }
    
    /**
      * /brief Constructor specifying parameters for calibration matrix
      */
    MonoCameraCalibration(const float& in_focLength, const float& in_cameraCentX
        ,
        const float& in_cameraCentY, const float& in_skew) : m_validCalib(false)
    {
        m_calibrationMatrix = cv::Mat1f::zeros(3, 3);
        m_calibrationMatrix(0, 0) = in_focLength;
        m_calibrationMatrix(0, 1) = in_skew;
        m_calibrationMatrix(1, 1) = in_focLength;
        m_calibrationMatrix(0, 2) = in_cameraCentX;
        m_calibrationMatrix(1, 2) = in_cameraCentY;
        m_calibrationMatrix(2, 2) = 1.0F;

        m_validCalib = true;
    }

    cv::Mat1f GetCalibrationMatrix() const
    {
        return m_calibrationMatrix;
    }

    /**
      * /brief Specifies whether the camera calibration is valid
      */
    bool IsValid() const
    {
        return m_validCalib;
    }

private:

    cv::Mat1f m_calibrationMatrix;
    bool m_validCalib;

};

} //namespace Calibration
} //namespace VOCPP

#endif /* VOCPP_MONO_CAMERA_CALIBRATION_H */
