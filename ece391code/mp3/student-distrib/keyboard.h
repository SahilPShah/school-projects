#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "i8259.h"
#include "lib.h"
#include "x86_desc.h"

void kbd_init();
void kbd_handler();

#endif
