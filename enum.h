#ifndef ENUM_H
#define ENUM_H

/**
 * @brief The block_t enum
 * NOTE: 4 also indicates 3, and 4 -> 3, only 3 -> 4
 */
enum block_t {
    EMPTY = 1,
    PLAYER = 2,
    OCCUPIED = 3,
    ACTIVATED = 4,
    PROPS = 5
};

/**
 * @brief The direction_t enum
 */
enum direction_t {
    DIR_UP = 1,
    DIR_DOWN = 2,
    DIR_LEFT = 3,
    DIR_RIGHT = 4,
    UNDEF = 5
};

/**
 * @brief The moving action enum
 */
enum moving_action_t {
    NORMAL = 1,
    ACTIVATE = 2,
    REPLACE = 3,
    ELIMATE = 4,
    EATPROPS = 5,
    PLAYEROCCUPIED = 6
};

/**
 * @brief The gameStatus_t enum
 * Status of game
 */
enum game_status_t {
    unstarted,
    running,
    gameOver,
    paused
};

/**
 * @brief The prop_t enum
 * type of props
 */
enum prop_t {
    plusSec = 1,
    reshuffle = 2,
    hint = 3
};


enum role_status_t {
    normal,
    hasHint
};

enum game_mode_t {
    onePlayer,
    twoPlayer
};


#endif // ENUM_H
