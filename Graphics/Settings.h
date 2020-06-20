#ifndef SETTINGS_H
#define SETTINGS_H

static int X_RESOLUTION;                     //Default: 960       (Range 960-3840)
static int Y_RESOLUTION;                     //Default: 540       (Range 540-2160)

static unsigned short TEXTURE_QUALITY;       //Default: 2         (Range 1-4)       256x256 512x512 1024x1024 2048x2048

static bool ENABLE_MULTISAMPLING;            //Default: false
static unsigned int MULTISAMPLING_LEVEL;     //Default: 4         (Range 4-8-16)

static bool UNLOCK_FRAMERATE;                //Default: false

static float GAMMA_CORRECTION;               //Default: 2.2       (Range 1-2.2)?

static float CAMERA_SENSIBILITY;             //Default: 0.5f      (Range 0.1f-2f)

#endif //SETTINGS_H
