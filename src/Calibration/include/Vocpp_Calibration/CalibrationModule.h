/* This file is part of the Visual-Odometry-Cpp project.
* It is subject to the license terms in the LICENSE file
* found in the top-level directory of this distribution.
*
* Copyright (C) 2020 Manuel Kraus
*/

#ifndef VOCPP_CALIBRATION_MODULE_H
#define VOCPP_CALIBRATION_MODULE_H

#include<opencv2/core/types.hpp>
#include<opencv2/core/core.hpp>
#include<Vocpp_Calibration/MonoCameraCalibration.h>

namespace VOCPP
{
namespace Calibration
{
   
/**
  * /brief This class takes care of saving, loading and computing a camera calibration
  *
  * Until now it has no computing functionality, it just offers an interface to provide a calibration matrix
  */
class CalibrationModule
{

public:
    
    /**
      * /brief Constructor
      */
    CalibrationModule();

    /**
      * /brief Destructor
      */
    ~CalibrationModule();

    /**
      * /brief Load a mono camera calibration and save it (if the camera calibration is valid)
      */
    bool LoadCalibration(const MonoCameraCalibration& in_monoCalib)
    {
        bool ret = false;

        if (in_monoCalib.IsValid())
        {
            m_monoCalib = in_monoCalib;
            ret = true;
        }

        return ret;
    }

    /**
      * /brief Get the saved mono camera calibration (if it is valid)
      */
    bool GetSavedMonoCalib(MonoCameraCalibration& out_monocalib) const
    {
        bool ret = false;

        if (m_monoCalib.IsValid())
        {
            out_monocalib = m_monoCalib;
            ret = true;
        }

        return ret;
    }

private:

    MonoCameraCalibration m_monoCalib;
};

} //namespace Calibration
} //namespace VOCPP

#endif /* VOCPP_CALIBRATION_MODULE_H */
