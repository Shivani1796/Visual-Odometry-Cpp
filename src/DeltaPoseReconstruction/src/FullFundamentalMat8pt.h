/* This file is part of the Visual-Odometry-Cpp project.
* It is subject to the license terms in the LICENSE file 
* found in the top-level directory of this distribution.
*
* Copyright (C) 2020 Manuel Kraus
*/

#ifndef VOCPP_FULL_FUNDAMENTAL_8PT_H
#define VOCPP_FULL_FUNDAMENTAL_8PT_H

#include <EpipolarModel.h>

namespace VOCPP
{
namespace DeltaPoseReconstruction
{

/**
  * /brief Tries to reconstruct a full fundamental matrix from provided point matches. 8 correspondences are needed.
  */
class FullFundamentalMat8pt : public EpipolarModel
{
public:

    /**
      * /brief Constructor of model, needed correspondences: 8, dimension = 3, 7 DOF
      */
    FullFundamentalMat8pt() : EpipolarModel(8, 3,  7)
    {
        m_type = EpipolarModel::Types::FullFundMat8pt;
    }
    
    /**
      * /brief Compute fundamental matrix F such that x_left.T * F * x_right = 0
      */
    virtual bool Compute(const std::vector<cv::Point2f>& in_pointCorrLeft, const std::vector<cv::Point2f>& in_pointCorrRight,
        std::vector<cv::Mat>& out_solutions) override;

    /**
      * /brief Test model solution given a set of image correspondences
      */
    virtual void Test(const std::vector<cv::Point2f>& in_pointCorrLeft, const std::vector<cv::Point2f>& in_pointCorrRight,
        cv::Mat& in_solution, const float in_errorTresh, std::vector<int>& out_inliers) override;
};

} //namespace DeltaPoseReconstruction
} //namespace VOCPP

#endif /* VOCPP_FULL_FUNDAMENTAL_8PT_H */
