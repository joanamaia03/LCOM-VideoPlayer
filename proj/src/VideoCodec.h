#include <lcom/lcf.h>
#include <stdio.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include "graphic.h"
#include <lcom/timer.h>
#include "keyboard.h"

/// @brief This is the video codec it decodes the video files into seperate frames to then send them to the display following the FPS dictated by the timer
/// @return 
int videoCodec();
