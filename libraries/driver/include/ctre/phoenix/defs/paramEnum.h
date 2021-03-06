#pragma once
#include <stdint.h>

/**
 * Signal enumeration for generic signal access.
 */
enum ParamEnum
	: uint32_t
	{
	eOnBoot_BrakeMode = 31,
	eQuadFilterEn = 91,

	eStatusFramePeriod = 300,
	eOpenloopRamp = 301,
	eClosedloopRamp = 302,
	eNeutralDeadband = 303,
	//eClosedloopDeadband = 304,
	ePeakPosOutput = 305,
	eNominalPosOutput = 306,
	ePeakNegOutput = 307,
	eNominalNegOutput = 308,

	eProfileParamSlot_P = 310,
	eProfileParamSlot_I = 311,
	eProfileParamSlot_D = 312,
	eProfileParamSlot_F = 313,
	eProfileParamSlot_IZone = 314,
	eProfileParamSlot_AllowableErr = 315,
	eProfileParamSlot_MaxIAccum = 316,

	eClearPositionOnLimitF = 320,
	eClearPositionOnLimitR = 321,
	eClearPositionOnQuadIdx = 322,

	eSampleVelocityPeriod = 325,
	eSampleVelocityWindow = 326,

	eFeedbackSensorType = 330,
	eSelectedSensorPosition = 331,
	eFeedbackNotContinuous = 332,

	eForwardSoftLimitThreshold = 340,
	eReverseSoftLimitThreshold = 341,
	eForwardSoftLimitEnable = 342,
	eReverseSoftLimitEnable = 343,

	eNominalBatteryVoltage = 350,
	eBatteryVoltageFilterSize = 351,

	eContinuousCurrentLimitAmps = 360,
	eContinuousCurrentLimitMs = 361,
	ePeakCurrentLimitAmps = 362,

	eClosedLoopIAccum = 370,

	eCustomParam = 380,

	eStickyFaults = 390,

	eAnalogPosition = 400,
	eQuadraturePosition = 401,
	ePulseWidthPosition = 402,

	eMotMag_Accel = 410,
	eMotMag_VelCruise = 411,

	eLimitSwitchSelect = 421, // ordinal (fwd=0,reverse=1), value=0(local),value=1(use remote filter)
	eLimitSwitchNormClosed = 422, // ordinal (fwd=0,reverse=1)
	eLimitSwitchDisabled = 423, // ordinal (fwd=0,reverse=1)
	eLimitSwitchDisableNeutralOnLOS = 424, // ordinal (fwd=0,reverse=1)

	eLimitRemoteFilter_IDValue = 430, // ordinal (fwd=0,reverse=1),
	eLimitRemoteFilter_IDMask = 431, // ordinal (fwd=0,reverse=1),
	eLimitRemoteFilter_BitPos = 432, // ordinal (fwd=0,reverse=1),
	eLimitRemoteFilter_TimeoutMs = 433, // ordinal (fwd=0,reverse=1),

};
