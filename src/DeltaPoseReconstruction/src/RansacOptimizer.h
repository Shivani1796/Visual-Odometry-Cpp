/* This file is part of the Visual-Odometry-Cpp project.
* It is subject to the license terms in the LICENSE file 
* found in the top-level directory of this distribution.
*
* Copyright (C) 2020 Manuel Kraus
*/

#ifndef VOCPP_RANSAC_OPTIMIZER_H
#define VOCPP_RANSAC_OPTIMIZER_H

#include <EpipolarModel.h>

#include <opencv2/core/types.hpp>
#include <opencv2/core/core.hpp>


namespace VOCPP
{
namespace DeltaPoseReconstruction
{

/**
  * /brief Tries to minimize the error of each EpipolarModel given a set of corresponding image points
  * and additionally selects the model which fits best to the observed correspondences
  */
class RansacOptimizer
{
public:
    
    /**
      * /brief Constructor
      */
    RansacOptimizer(const float in_initialOutlierRatio, const float in_smoothingFactor) : 
        m_outlierRatio(in_initialOutlierRatio),
        m_smoothingFactor(in_smoothingFactor)
    {
    }
    
    /**
      * /brief Run the RANSAC optimizer given a set of models to be tested
      */
    int Run(const std::vector<EpipolarModel*>& in_testedModels, const std::vector<cv::Point2f>& in_correspondFirst, const std::vector<cv::Point2f>& in_correspondSecond,
        std::vector<cv::Point2f>& out_inliersFirst, std::vector<cv::Point2f>& out_inliersSecond, cv::Mat& out_bestSolution);

    /**
      * /brief Calculate the number of necessary model iterations
      *
      * \param[in] in_prob probability of having a pure inlier draw (this is what we want)
      * \param[in] in_outlierRatio expect ratio of outliers / correspondences
      * \param[in] in_numCorrespondences degrees of freedom of the model --> necessary correspondences
      */
    static int CalculateNecessaryIterations(const float in_prob, const float in_outlierRatio, const int in_numCorrespondences);

private:

    /**
      * /brief Update the outlier ratio estimate using an exponential average of the measured outlier ratio
      */
    void UpdateOutlierRatio(const float in_measuredRatio)
    {
        m_outlierRatio = (static_cast<float>(1.0) - m_smoothingFactor) * m_outlierRatio + m_smoothingFactor * in_measuredRatio;
    }

    float m_outlierRatio; ///< ratio of outliers expected in frames, controls number of trials for models
    float m_smoothingFactor; ///< smoothing factor for the outlier ratio exponential average
};

} //namespace DeltaPoseReconstruction
} //namespace VOCPP

#endif /* VOCPP_RANSAC_OPTIMIZER_H */