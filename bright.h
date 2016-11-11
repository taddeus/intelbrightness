#ifndef _BRIGHT_H
#define _BRIGHT_H

long current_brightness();
long max_brightness();

void set_brightness(long level);
void inc_brightness(long amount);

#endif // _BRIGHT_H
