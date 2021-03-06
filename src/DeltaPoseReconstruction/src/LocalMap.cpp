/* This file is part of the Visual-Odometry-Cpp project.
* It is subject to the license terms in the LICENSE file
* found in the top-level directory of this distribution.
*
* Copyright (C) 2020 Manuel Kraus
*/

#include <LocalMap.h>

namespace VOCPP
{
namespace DeltaPoseReconstruction
{

LocalMap::~LocalMap()
{
}

void LocalMap::Initialize(const std::vector<cv::KeyPoint>& in_keypoints, const cv::Mat& in_projectionMat, const int in_currentFrameId)
{
    if (m_status == Status::TrackingLost)
    {
       // m_landmarks.clear();
        
       // for (auto keypoint : in_keypoints)
       // {
       //     m_landmarks.push_back(Landmark(in_currentFrameId, ))
      //  }
    }
}

/*
void LocalMap::InsertLandmark(const LandmarkPosition& in_position, const cv::DMatch& in_match, int in_currentFrameId)
{
    // Check whether this landmark has been observed before
    bool found = false;
    for (auto& mark : m_landmarks)
    {
        if (mark.IsPresentInFrame(in_match.imgIdx, in_match.trainIdx))
        {
            mark.UpdateLandmark(in_position, in_currentFrameId, in_match.queryIdx);
            found = true;
        }
    }

    // If Landmark has not been found --> create new one
    if (!found)
    {
        m_landmarks.push_back(Landmark(in_position, in_currentFrameId, in_match.queryIdx, in_match.imgIdx, in_match.trainIdx));
    }
}*/

} //namespace DeltaPoseReconstruction
} //namespace VOCPP