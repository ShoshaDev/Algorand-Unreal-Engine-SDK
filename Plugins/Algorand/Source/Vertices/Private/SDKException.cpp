// Copyright 2022, Wisdom Labs. All Rights Reserved

#include "SDKException.h"

SDKException::SDKException()
{
	msg = "Exception happened";
}

// VTC error cases supporting from vertices_errors.h
SDKException::SDKException(ret_code_t& err_code) {
	switch(err_code)
	{
		case VTC_ERROR_INTERNAL: msg = "ERROR: INTERNAL"; break;
		case VTC_ERROR_NO_MEM: msg = "ERROR: NO MEMBER"; break;
		case VTC_ERROR_NOT_FOUND: msg = "ERROR: NOT FOUND"; break;
		case VTC_ERROR_INVALID_PARAM: msg = "ERROR: INVALID PARAM"; break;
		case VTC_ERROR_INVALID_STATE: msg = "ERROR: INVALID STATE"; break;
		case VTC_ERROR_INVALID_ADDR: msg = "ERROR: INVALID ADDR"; break;
		case VTC_ERROR_OFFLINE: msg = "ERROR: OFFLINE"; break;
		case VTC_ERROR_ASSERT_FAILS: msg = "ERROR: ASSERT FAILS"; break;
		case VTC_ERROR_HTTP_BASE: msg = "ERROR: HTTP_BASE"; break;
		case VTC_SAME_MEM_EXIST: msg = "ERROR: SAME MEMBER EXISTS"; break;
		default: msg = "ERROR: OCCURED"; break;
	}
}

SDKException::SDKException(char* msg_, ret_code_t& err_code)
{
	switch(err_code)
	{
		case VTC_ERROR_INTERNAL: msg = msg_; break;
		case VTC_ERROR_NO_MEM: msg = msg_; break;
		case VTC_ERROR_NOT_FOUND: msg = msg_; break;
		case VTC_ERROR_INVALID_PARAM: msg = msg_; break;
		case VTC_ERROR_INVALID_STATE: msg = msg_; break;
		case VTC_ERROR_INVALID_ADDR: msg = msg_; break;
		case VTC_ERROR_OFFLINE: msg = msg_; break;
		case VTC_ERROR_ASSERT_FAILS: msg = msg_; break;
		case VTC_ERROR_HTTP_BASE: msg = msg_; break;
		default: break;
	}
}

const char* SDKException::what() const
{
	return msg;
}
