#ifndef DEBUG_OPTIONS_H
#define DEBUG_OPTIONS_H

/*
 * A collection of options regarding displaying debug info on the screen
 * Each option is only accessable by function so that whether they are
 * displayed depends on whether any debug options are displayed at all
 * as well as settings for that specific option
 *
 * NOTE: These settings are completely seperate from the DEBUG flag in the
 * build script. These will be compiled regardless of if it is built in debug
 * mode.
 *
 * As for documentation of each option, I believe the names speak for themselves
 * as far as what they represent
 */

class DebugOptions {
    static bool _renderEntityHitboxes;
    static bool _renderPlayerHitbox;
    static bool _showFpsCounter;
    static bool _showDebug;

    public:
        static inline bool entityHitboxesRendered() { return _showDebug && _renderEntityHitboxes; }
        static inline bool playerHitboxRendered()   { return _showDebug && _renderPlayerHitbox;   }
        static inline bool showFpsCounter()         { return _showDebug && _showFpsCounter;       }
        static inline bool showDebugOptions()       { return _showDebug;                          }

        static inline void setEntityHitboxesRendered(bool x) { _renderEntityHitboxes = x; }
        static inline void setPlayerHitboxRendered  (bool x) { _renderPlayerHitbox   = x; }
        static inline void setShowFpsCounter        (bool x) { _showFpsCounter       = x; }
        static inline void setShowDebugOptions      (bool x) { _showDebug            = x; }
};

#endif
