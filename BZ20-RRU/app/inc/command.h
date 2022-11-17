#ifndef _COMMAND_H_
#define _COMMAND_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

int32_t RegisterCommand(const char* cmd, const char* help, void *handler);
int32_t WaitForCommand(void);
void ShowCommands(void);

#ifdef __cplusplus
}
#endif
#endif