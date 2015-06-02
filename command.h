#ifndef COMMAND_H_Record_and_Replay
#define COMMAND_H_Record_and_Replay
#include <linux/ioctl.h>
//chose by run find_valid_ioctl_num.c
//_IO _IOW is not the same 
//but not differ  by modules
#define RECORDANDREPLAY_IO		111
#define RECORDANDREPLAY_IOW		66
#define RECORDANDREPLAY_IOR		67
//only continuous from 1 to 9
//careful when adding more commands
#define IOCTL_RESET		_IO(RECORDANDREPLAY_IO, 0x1)
#define IOCTL_SET_PID_RECORD	_IOW(RECORDANDREPLAY_IOW, 200, int)
#define IOCTL_GET_PID_RECORD	_IOR(RECORDANDREPLAY_IOR, 200, int)
#define IOCTL_START_RECORD	_IO(RECORDANDREPLAY_IO, 0x4)
#define IOCTL_STOP_RECORD	_IO(RECORDANDREPLAY_IO, 0x5)
#define IOCTL_START_REPLAY	_IO(RECORDANDREPLAY_IO, 0x6)
#define IOCTL_STOP_REPLAY	_IO(RECORDANDREPLAY_IO, 0x7)
#define IOCTL_SET_PID_REPLAY	_IOW(RECORDANDREPLAY_IOW, 201, int)
#define IOCTL_GET_PID_REPLAY	_IOR(RECORDANDREPLAY_IOR, 201, int)
#endif
