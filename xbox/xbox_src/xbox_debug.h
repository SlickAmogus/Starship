// Xbox debug logging - writes to D:\starship.log
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void xbox_log(const char* fmt, ...);

#ifdef __cplusplus
}
#endif
