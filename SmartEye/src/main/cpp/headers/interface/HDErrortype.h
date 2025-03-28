
#pragma once
#include <stdint.h>
#include <sys/cdefs.h>
#include <functional>

namespace autonavi {
namespace HDCOLLECT {
    
    enum {
    HD_OK = 0,
	HD_ERROR_BASE = -100,
	HD_MEMORY_FAIL = HD_ERROR_BASE - 1,
	HD_SERVICE_CONNECTFAIL = HD_ERROR_BASE - 2,
	HD_SERVICE_NOTCONNECT = HD_ERROR_BASE -3,
	HD_CALL_REPEATED = HD_ERROR_BASE -4,
	HD_LISTENER_NOTFOUND = HD_ERROR_BASE -5,
	HD_SERVER_NOTSTART = HD_ERROR_BASE -6,
	HD_INVALID_PARAMETER = HD_ERROR_BASE - 7,
    };
};
};

