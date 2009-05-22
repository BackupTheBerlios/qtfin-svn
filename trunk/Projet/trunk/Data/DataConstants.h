#ifndef DATACONSTANTS_H
#define DATACONSTANTS_H

namespace Data{
    const int MONOFIN_SURFACE_NO_CONTROL_POINT = -1;
    const int MONOFIN_SURFACE_NOT_CREATED_SEGMENT = -2;
    const int MONOFIN_SURFACE_NOT_CREATED_POINT = -3;
    const int MONOFIN_SURFACE_NO_SEGMENT_ERASED = -4;
    const float MONOFIN_PROFIL_BAD_LAYER = -1.0f;
    const float MONOFIN_CONFIGFILE_BAD_LAYER = -2.0f;
    enum Modification {MonofinSurface,MonofinLayer,MonofinLayerConfig};
}

#endif // DATACONSTANTS_H
