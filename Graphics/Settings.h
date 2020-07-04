#ifndef SETTINGS_H
#define SETTINGS_H

extern int X_RESOLUTION;                     //Default: 960       (Range 960-3840)
extern int Y_RESOLUTION;                     //Default: 540       (Range 540-2160)

extern unsigned int TEXTURE_QUALITY;         //Default: 2         (Range 1-4)       256x256 512x512 1024x1024 2048x2048
extern unsigned int SHADOW_QUALITY;          //Default: 2         (Range 1-4)       256x256 512x512 1024x1024 2048x2048

extern bool ENABLE_MULTISAMPLING;            //Default: false
extern unsigned int MULTISAMPLING_LEVEL;     //Default: 4         (Range 4-8-16)

extern bool UNLOCK_FRAMERATE;                //Default: false

extern float GAMMA_CORRECTION;               //Default: 2.2       (Range 1-2.2)?

extern float CAMERA_SENSIBILITY;             //Default: 0.5f      (Range 0.1f-2f)

extern unsigned int TABLE_TYPE;
extern unsigned int WORLD_TYPE;

#endif //SETTINGS_H
