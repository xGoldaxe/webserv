#pragma once

#include <signal.h>
#include <iostream>

extern int exit_code;

bool shouldQuit(void);
void handleExit(void);
