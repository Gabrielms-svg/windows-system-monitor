#ifndef LOGGER_H
#define LOGGER_H

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) void log_message(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif
