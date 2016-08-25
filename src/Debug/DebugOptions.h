#ifndef DEBUG_OPTIONS_H
#define DEBUG_OPTIONS_H

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
