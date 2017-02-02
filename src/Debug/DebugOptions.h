#ifndef DEBUG_OPTIONS_H
#define DEBUG_OPTIONS_H

/*!
 * A collection of options regarding displaying debug info on the screen
 *
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
        /*! \callergraph
         * Returns whether or not entity hitboxes should be rendered
         */
        static inline bool entityHitboxesRendered() { return _showDebug && _renderEntityHitboxes; }

        /*! \callergraph
         * Returns whether or not the player's hitbox should be rendered
         */
        static inline bool playerHitboxRendered()   { return _showDebug && _renderPlayerHitbox;   }

        /*! \callergraph
         * Returns whether or not the fps counter should be shown
         */
        static inline bool showFpsCounter()         { return _showDebug && _showFpsCounter;       }

        /*! \callergraph
         * Returns whether or not the debug options are enabled
         */
        static inline bool showDebugOptions()       { return _showDebug;                          }

        /*! \callergraph
         * Sets whether entity hitboxes should be rendered in debug mode
         */
        static inline void setEntityHitboxesRendered(bool x) { _renderEntityHitboxes = x; }

        /*! \callergraph
         * Sets whether player hitboxes should be rendered in debug mode
         */
        static inline void setPlayerHitboxRendered  (bool x) { _renderPlayerHitbox   = x; }

        /*! \callergraph
         * Sets whether the fps counter should be displayed in debug mode
         */
        static inline void setShowFpsCounter        (bool x) { _showFpsCounter       = x; }

        /*! \callergraph
         * Turns debug mode on and off
         */
        static inline void setShowDebugOptions      (bool x) { _showDebug            = x; }
};

#endif
