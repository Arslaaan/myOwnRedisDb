#pragma once

#include <grpc/support/time.h>

namespace GprTime {
inline gpr_timespec timeoutSecondsToDeadline(int64_t time_s) {
    return gpr_time_add(
        gpr_now(GPR_CLOCK_MONOTONIC),
        gpr_time_from_millis(static_cast<int64_t>(1e3) * time_s, GPR_TIMESPAN));
}
};  // namespace GprTime