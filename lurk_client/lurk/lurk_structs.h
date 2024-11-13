#pragma once

#ifndef LURK_STRUCTS
#define LURK_STRUCTS

#include <inttypes.h>

#pragma pack(push,1)

struct lurk_message
{
    uint8_t     type;
    uint16_t    message_length;
    uint8_t     recipient_name[32];
    uint8_t     sender_name[30];
    uint16_t    end_of_struct;
};

struct lurk_change_room
{
    uint8_t     type;
    uint16_t    number_of_room_to_change_to;
};

struct lurk_fight
{
    uint8_t     type;
};

struct lurk_pvp_fight
{
    uint8_t     type;
    uint8_t     name_of_target_player[32];
};

struct lurk_loot
{
    uint8_t     type;
    uint8_t     name_of_target_player[32];
};

struct lurk_start
{
    uint8_t     type; //SHOULD ALWAYS BE 6
};

struct lurk_error
{
    uint8_t     type;
    uint8_t     error_code;
    uint16_t    error_message_length;
};

struct lurk_accept
{
    uint8_t     type;
    uint8_t     type_of_action;
};

struct lurk_room
{
    uint8_t     type;
    uint16_t    room_number;
    uint8_t     room_name[32];
    uint16_t    room_description_length;
};

struct lurk_character
{
    uint8_t     type;
    uint8_t     name_of_player[32];
    uint8_t     flags;
    uint16_t    attack;
    uint16_t    defense;
    uint16_t    regen;
    int16_t     health;
    uint16_t    gold;
    uint16_t    current_room_number;
    uint16_t    description_length;
};

struct lurk_game
{
    uint8_t     type;
    uint16_t    initial_points;
    uint16_t    stat_limit;
    uint16_t    description_length;
};

struct lurk_leave
{
    uint8_t     type;
};

struct lurk_connection
{
    uint8_t     type;
    uint16_t    room_number;
    uint8_t     room_name[32];
    uint16_t    room_description_length;
};

struct lurk_version
{
    uint8_t     type;
    uint8_t     major;
    uint8_t     minor;
    uint16_t    extension_list;
};

enum character_flags : uint8_t
{
    alive = (uint8_t)(1 << 8),
    join_battle = (uint8_t)(1 << 7),
    monster = (uint8_t)(1 << 6),
    started = (uint8_t)(1 << 5),
    ready = (uint8_t)(1 << 4),
};

enum error_code : uint8_t
{
    other,
    bad_room,
    player_exists,
    bad_monster,
    stat_error,
    not_ready,
    no_target,
    no_fight,
    no_pvp
};

#pragma pack(pop)

#endif