package com.ctre.phoenix;

import java.util.HashMap;

public enum ErrorCode {
	//OK(0),

	//CAN Error Codes
	//CAN_MSG_STALE(1),
	//CAN_TX_FULL(-1),
	//CAN_INVALID_PARAM(-2),
	//CAN_MSG_NOT_FOUND(-3),
	//CAN_NO_MORE_TX_JOBS(-4),

	//CAN_NO_SESSIONS_AVAIL(-5),
	//CAN_OVERFLOW(-6),

	//GENERAL_ERROR(-100),

	//SIG_NOT_UPDATED(-200),

	//Gadgeteer Port Error Codes
	//These include errors between ports and modules
	//GEN_PORT_ERROR(-300),
	//PORT_MODULE_TYPE_MISMATCH(-301),

	//Gadgeteer Module Error Codes
	//These apply only to the module units themselves
	//GEN_MODULE_ERROR(-400),
	//MODULE_NOT_INIT_SET_ERROR(-401),
	//MODULE_NOT_INIT_GET_ERROR(-402),

	//FeatureNotSupported(101),




	OKAY(0),					//!< No Error - Function executed as expected

	//CAN-Related
	TxFailed(-1),				//!< Could not transmit the CAN frame.
	InvalidParamValue(-2), 	//!< Caller passed an invalid param
	RxTimeout(-3),				//!< CAN frame has not been received within specified period of time.
	TxTimeout(-4),				//!< Not used.
	UnexpectedArbId(-5),		//!< Specified CAN Id is invalid.
	BufferFull(-6),			//!< Caller attempted to insert data into a buffer that is full.
	SensorNotPresent(-7),		//!< Sensor is not present

	//General
	GeneralError(-100),		//!< User Specified General Error

	//??
	SigNotUpdated(-200),			//!< Have not received an value response for signal.
	NotAllPIDValuesUpdated(-201),

	//API
	WheelRadiusTooSmall(-500),
	TicksPerRevZero(-501),
	DistanceBetweenWheelsTooSmall(-502),
	GainsAreNotSet(-503),

	//Higher Level
	IncompatibleMode(-600),
	InvalidHandle(-601),		//!< Handle does not match stored map of handles


	//CAN Related
	//PulseWidthSensorNotPresent(1),	//!< Special Code for "isSensorPresent"

	//General
	GeneralWarning(100),
	FeatureNotSupported(101),
	NotImplemented(102);
	
	//---------------------- Integral To Enum operators -----------//
    private int _value; //!< Hold the integral value of an enum instance.
    /** Keep singleton map to quickly lookup enum via int */
    private static HashMap<Integer, ErrorCode> _map = null;
    /** private c'tor for above declarations */
	private ErrorCode(int value) {_value = value;	}
	/** static c'tor, prepare the map */
    static {
    	_map = new HashMap<Integer, ErrorCode>();
		for (ErrorCode type : ErrorCode.values()) {
			_map.put(type._value, type);
		}
    }
    /** public lookup to convert int to enum */
	public static ErrorCode valueOf(int value) {
		ErrorCode retval = _map.get(value);
		if (retval != null)
			return retval;
		return GeneralError;
    }
	/** @return integral value of enum instance */
    public int getValue() {
        return _value;
    }
};