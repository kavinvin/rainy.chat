/**
 * File: room.c
 * ----------------------------
 *   room structure organization
 */

#include "room.h"

/**
 * Function: getRoom
 * ----------------------------
 *      recursively create rooms from the given dot seperated string
 *   e.g. "compro.kmitl.rainy.chat" with diffent stack for each level
 *   return room created
 */
List *getRoom(List *global, char *last) {
    char *name, prefix[256];
    strcpy(prefix, last);
    name = strtok_r(NULL, ".", &last);
    if (strcmp(name, "rainy") == 0) {
        return global;
    }
    List *rooms = getRoom(global, last);
    Node *room = rooms->head;
    if (room != NULL) {
        room = get(room, name);
    }
    if (room == NULL) {
        room = create(NULL);
        strcpy(room->name, name);
        strcpy(room->prefix, prefix);
        room->superlist = rooms;
        room->sublist = newList();
        room->sublist->from = room;
        room->users = newList();
        room->sublist->level = rooms->level + 1;
        // room->sublist->from = room;
        append(rooms, room);
    }
    return room->sublist;
}

