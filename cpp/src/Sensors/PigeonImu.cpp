/*
 *  Software License Agreement
 *
 * Copyright (C) Cross The Road Electronics.  All rights
 * reserved.
 * 
 * Cross The Road Electronics (CTRE) licenses to you the right to 
 * use, publish, and distribute copies of CRF (Cross The Road) firmware files (*.crf) and Software
 * API Libraries ONLY when in use with Cross The Road Electronics hardware products.
 * 
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * CROSS THE ROAD ELECTRONICS BE LIABLE FOR ANY INCIDENTAL, SPECIAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE
 */

#include <memory>

#ifndef CTR_EXCLUDE_WPILIB_CLASSES
#include "ctre/phoenix/Sensors/PigeonIMU.h"
#include "ctre/phoenix/CTRLogger.h"
#include "ctre/phoenix/CCI/Logger_CCI.h"
#include "ctre/phoenix/CCI/PigeonIMU_CCI.h"
#include "ctre/phoenix/MotorControl/CAN/TalonSRX.h"

#include "FRC_NetworkCommunication/CANSessionMux.h"

#include "Utility.h"
#include "HAL/HAL.h"

using namespace CTRE::MotorControl::CAN;

namespace CTRE {
/**
 * Create a Pigeon object that communicates with Pigeon on CAN Bus.
 * @param deviceNumber CAN Device Id of Pigeon [0,62]
 */
PigeonIMU::PigeonIMU(int deviceNumber) : CANBusAddressable(deviceNumber)
{
	m_handle = c_PigeonIMU_Create1(deviceNumber);
	_deviceNumber = deviceNumber;
	
	PigeonIMU::ApplyUsageStats(UsageFlags::ConnectCAN);
}

/**
 * Create a Pigeon object that communciates with Pigeon through the Gadgeteer ribbon
 * @param talonSrx cable connected to a Talon on CAN Bus.
 */
PigeonIMU::PigeonIMU(CTRE::MotorControl::CAN::TalonSRX * talonSrx) : CANBusAddressable(0)
{
	m_handle = c_PigeonIMU_Create2(talonSrx->GetDeviceID());
	_deviceNumber = talonSrx->GetDeviceID();
	PigeonIMU::ApplyUsageStats(UsageFlags::ConnectTalonSRX);
}

//----------------------- Control Param routines -----------------------//
int PigeonIMU::ConfigSetParameter(ParamEnum paramEnum, double paramValue)
{
	return c_PigeonIMU_ConfigSetParameter(m_handle, (int)paramEnum, paramValue);
}

/**
 * Change the periodMs of a TALON's status frame.  See kStatusFrame_* enums for
 * what's available.
 */
void PigeonIMU::SetStatusFrameRateMs(StatusFrameRate statusFrameRate, int periodMs) {
	c_PigeonIMU_SetStatusFrameRateMs(m_handle, statusFrameRate, periodMs);
}
int PigeonIMU::SetYaw(double angleDeg)
{
	int errCode = c_PigeonIMU_SetYaw(m_handle, angleDeg);
	return errCode;
}
/**
 * Atomically add to the Yaw register.
 */
int PigeonIMU::AddYaw(double angleDeg)
{
	int errCode = c_PigeonIMU_AddYaw(m_handle, angleDeg);
	return errCode;
}
int PigeonIMU::SetYawToCompass()
{
	int errCode = c_PigeonIMU_SetYawToCompass(m_handle);
	return errCode;
}
int PigeonIMU::SetFusedHeading(double angleDeg)
{
	int errCode = c_PigeonIMU_SetFusedHeading(m_handle, angleDeg);
	return errCode;
}
int PigeonIMU::SetAccumZAngle(double angleDeg)
{
	int errCode = c_PigeonIMU_SetAccumZAngle(m_handle, angleDeg);
	return errCode;
}
/**
 * Enable/Disable Temp compensation.  Pigeon defaults with this on at boot.
 * @param tempCompEnable
 * @return nonzero for error, zero for success.
 */
int PigeonIMU::EnableTemperatureCompensation(bool bTempCompEnable)
{
	int errCode = c_PigeonIMU_EnableTemperatureCompensation(m_handle, bTempCompEnable);
	PigeonIMU::ApplyUsageStats(UsageFlags::TempComp);
	return errCode;
}
/**
 * Atomically add to the Fused Heading register.
 */
int PigeonIMU::AddFusedHeading(double angleDeg)
{
	int errCode = c_PigeonIMU_AddFusedHeading(m_handle, angleDeg);
	return errCode;
}
int PigeonIMU::SetFusedHeadingToCompass()
{
	int errCode = c_PigeonIMU_SetFusedHeadingToCompass(m_handle);
	return errCode;
}
/**
 * Set the declination for compass.
 * Declination is the difference between Earth Magnetic north, and the geographic "True North".
 */
int PigeonIMU::SetCompassDeclination(double angleDegOffset)
{
	int errCode = c_PigeonIMU_SetCompassDeclination(m_handle, angleDegOffset);
	return errCode;
}
/**
 * Sets the compass angle.
 * Although compass is absolute [0,360) degrees, the continuous compass
 * register holds the wrap-arounds.
 */
int PigeonIMU::SetCompassAngle(double angleDeg)
{
	int errCode = c_PigeonIMU_SetCompassAngle(m_handle, angleDeg);
	return errCode;
}
//----------------------- Calibration routines -----------------------//
int PigeonIMU::EnterCalibrationMode(CalibrationMode calMode)
{
	return c_PigeonIMU_EnterCalibrationMode(m_handle, calMode);
}
/**
 * Get the status of the current (or previousley complete) calibration.
 * @param statusToFill
 */
int PigeonIMU::GetGeneralStatus(PigeonIMU::GeneralStatus & statusToFill)
{
	int state;
	int currentMode;
	int calibrationError;
	int bCalIsBooting;
	double tempC;
	int upTimeSec;
	int noMotionBiasCount;
	int tempCompensationCount;
	int lastError;
	
	int errCode = c_PigeonIMU_GetGeneralStatus(m_handle, &state, &currentMode, &calibrationError, &bCalIsBooting, &tempC, &upTimeSec, &noMotionBiasCount, &tempCompensationCount, &lastError);

	statusToFill.currentMode = (PigeonIMU::CalibrationMode)currentMode;
	statusToFill.calibrationError = calibrationError;
	statusToFill.bCalIsBooting = bCalIsBooting;
	statusToFill.state = (PigeonIMU::PigeonState)state;
	statusToFill.tempC = tempC;
	statusToFill.noMotionBiasCount = noMotionBiasCount;
	statusToFill.tempCompensationCount = tempCompensationCount;
	statusToFill.upTimeSec = upTimeSec;
	statusToFill.lastError = errCode;

	/* build description string */
	if (errCode != 0) { // same as NoComm
		statusToFill.description = "Status frame was not received, check wired connections and web-based config.";
	} else if(statusToFill.bCalIsBooting) {
		statusToFill.description = "Pigeon is boot-caling to properly bias accel and gyro.  Do not move Pigeon.  When finished biasing, calibration mode will start.";
	} else if(statusToFill.state == UserCalibration) {
		/* mode specific descriptions */
		switch(currentMode) {
			case BootTareGyroAccel:
				statusToFill.description = "Boot-Calibration: Gyro and Accelerometer are being biased.";
				break;
			case Temperature:
				statusToFill.description = "Temperature-Calibration: Pigeon is collecting temp data and will finish when temp range is reached.  "
				"Do not moved Pigeon.";
				break;
			case Magnetometer12Pt:
				statusToFill.description = "Magnetometer Level 1 calibration: Orient the Pigeon PCB in the 12 positions documented in the User's Manual.";
				break;
			case Magnetometer360:
				statusToFill.description = "Magnetometer Level 2 calibration: Spin robot slowly in 360' fashion.  ";
				break;
			case Accelerometer:
				statusToFill.description = "Accelerometer Calibration: Pigeon PCB must be placed on a level source.  Follow User's Guide for how to level surfacee.  ";
				break;
		}
	} else if (statusToFill.state == Ready){
		/* definitely not doing anything cal-related.  So just instrument the motion driver state */
		statusToFill.description = "Pigeon is running normally.  Last CAL error code was ";
		statusToFill.description += std::to_string(calibrationError);
		statusToFill.description += ".";
	} else if (statusToFill.state == Initializing){
		/* definitely not doing anything cal-related.  So just instrument the motion driver state */
		statusToFill.description = "Pigeon is boot-caling to properly bias accel and gyro.  Do not move Pigeon.";
	} else {
		statusToFill.description = "Not enough data to determine status.";
	}

	return errCode;
}
//----------------------- General Error status  -----------------------//
int PigeonIMU::GetLastError()
{
	return c_PigeonIMU_GetLastError(m_handle);
}

int PigeonIMU::HandleError(int errorCode)
{
	/* error handler */
	if (errorCode != 0) {
		/* what should we do here? */
	    //wpi_setErrorWithContext(errorCode, HAL_GetErrorMessage(errorCode));
	}
	/* mirror last status */
	_lastError = errorCode;
	return _lastError;
}

//----------------------- Strongly typed Signal decoders  -----------------------//
int PigeonIMU::Get6dQuaternion(double wxyz[4])
{
	int errCode = c_PigeonIMU_Get6dQuaternion(m_handle, wxyz);
	return errCode;
}
int PigeonIMU::GetYawPitchRoll(double ypr[3])
{
	int errCode = c_PigeonIMU_GetYawPitchRoll(m_handle, ypr);
	PigeonIMU::ApplyUsageStats(UsageFlags::GetYPR);
	return errCode;
}
int PigeonIMU::GetAccumGyro(double xyz_deg[3])
{
	int errCode = c_PigeonIMU_GetAccumGyro(m_handle, xyz_deg);
	return errCode;
}
/**
 *  @return compass heading [0,360) degrees.
 */
double PigeonIMU::GetAbsoluteCompassHeading()
{
	double retval;
	c_PigeonIMU_GetAbsoluteCompassHeading(m_handle, &retval);
	return retval;
}
/**
 *  @return continuous compass heading [-23040, 23040) degrees.
 *  Use SetCompassHeading to modify the wrap-around portion.
 */
double PigeonIMU::GetCompassHeading()
{
	double retval;
	c_PigeonIMU_GetCompassHeading(m_handle, &retval);
	PigeonIMU::ApplyUsageStats(UsageFlags::GetCompass);
	return retval;
}
/**
 * @return field strength in Microteslas (uT).
 */
double PigeonIMU::GetCompassFieldStrength()
{
	double retval;
	c_PigeonIMU_GetCompassFieldStrength(m_handle, &retval);
	return retval;
}
double PigeonIMU::GetTemp()
{
	double tempC;
	c_PigeonIMU_GetTemp(m_handle, &tempC);
	return tempC;
}
PigeonIMU::PigeonState PigeonIMU::GetState()
{
	int retval;
	c_PigeonIMU_GetState(m_handle, &retval);
	return (PigeonIMU::PigeonState)retval;
}
/// <summary>
/// How long has Pigeon been running
/// </summary>
/// <param name="timeSec"></param>
/// <returns></returns>
uint32_t PigeonIMU::GetUpTime()
{
	int timeSec;
	c_PigeonIMU_GetUpTime(m_handle, &timeSec);
	return timeSec;
}

int PigeonIMU::GetRawMagnetometer(int16_t rm_xyz[3])
{
	int errCode = c_PigeonIMU_GetRawMagnetometer(m_handle, rm_xyz);
	return errCode;
}
int PigeonIMU::GetBiasedMagnetometer(int16_t bm_xyz[3])
{
	int errCode = c_PigeonIMU_GetBiasedMagnetometer(m_handle, bm_xyz);
	return errCode;
}
int PigeonIMU::GetBiasedAccelerometer(int16_t ba_xyz[3])
{
	int errCode = c_PigeonIMU_GetBiasedAccelerometer(m_handle, ba_xyz);
	return errCode;
}
int PigeonIMU::GetRawGyro(double xyz_dps[3])
{
	int errCode = c_PigeonIMU_GetRawGyro(m_handle, xyz_dps);
	return errCode;
}

int PigeonIMU::GetAccelerometerAngles(double tiltAngles[3])
{
	int errCode = c_PigeonIMU_GetAccelerometerAngles(m_handle, tiltAngles);
	return errCode;
}
/**
 * @param status 	object reference to fill with fusion status flags.  
 *					Caller may omit this parameter if flags are not needed.
 * @return fused heading in degrees.
 */
double PigeonIMU::GetFusedHeading(FusionStatus & status)
{
	int bIsFusing, bIsValid;
	double fusedHeading;
	int lastError;

	int errCode = c_PigeonIMU_GetFusedHeading2(m_handle, &bIsFusing, &bIsValid, &fusedHeading, &lastError);

	std::string description;

	if (errCode != 0) {
		bIsFusing = false;
		bIsValid = false;
		description = "Could not receive status frame.  Check wiring and web-config.";
	} else {
		if(bIsValid == false) {
			description = "Fused Heading is not valid.";
		}else if(bIsFusing == false){
			description = "Fused Heading is valid.";
		} else {
			description = "Fused Heading is valid and is fusing compass.";
		}
	}

	/* fill caller's struct */
	status.heading = fusedHeading;
	status.bIsFusing = (bool)bIsFusing;
	status.bIsValid = (bool)bIsValid;
	status.description = description;
	status.lastError = errCode;

	PigeonIMU::ApplyUsageStats(UsageFlags::GetFused);
	return fusedHeading;
}
double PigeonIMU::GetFusedHeading()
{
	double value;
	PigeonIMU::ApplyUsageStats(UsageFlags::GetFused);
	return c_PigeonIMU_GetFusedHeading1(m_handle, &value);
}
//----------------------- Startup/Reset status -----------------------//
uint32_t PigeonIMU::GetResetCount()
{
	int retval;
	c_PigeonIMU_GetResetCount(m_handle, &retval);
	return retval;
}
uint32_t PigeonIMU::GetResetFlags()
{
	int retval;
	c_PigeonIMU_GetResetCount(m_handle, &retval);
	return (uint32_t)retval;
}
/**
 * @param param holds the version of the Talon.  Talon must be powered cycled at least once.
 */
uint32_t PigeonIMU::GetFirmVers()
{
	int retval;
	c_PigeonIMU_GetFirmVers(m_handle, &retval);
	return retval;
}
/**
 * @return true iff a reset has occured since last call.
 */
bool PigeonIMU::HasResetOccured()
{
	bool retval;
	c_PigeonIMU_HasResetOccured(m_handle, &retval);
	return retval;
}

/* static */ std::string PigeonIMU::ToString(PigeonState state)
{
	std::string retval = "Unknown";
	switch (state) {
	case Initializing:
		return "Initializing";
	case Ready:
		return "Ready";
	case UserCalibration:
		return "UserCalibration";
	case NoComm:
		return "NoComm";
	}
	return retval;
}
/* static */ std::string PigeonIMU::ToString(CalibrationMode cm)
{
	std::string retval = "Unknown";
	switch (cm) {
	case BootTareGyroAccel:
		return "BootTareGyroAccel";
	case Temperature:
		return "Temperature";
	case Magnetometer12Pt:
		return "Magnetometer12Pt";
	case Magnetometer360:
		return "Magnetometer360";
	case Accelerometer:
		return "Accelerometer";
	}
	return retval;
}

void PigeonIMU::ApplyUsageStats(UsageFlags Usage)
{
	if((Usage & _usageHist) == 0)
	{  
		_usageHist |= Usage;

		HAL_Report(61, _deviceNumber + 1, _usageHist);
	}
}
}
#endif // CTR_EXCLUDE_WPILIB_CLASSES
