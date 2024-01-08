from enum import IntEnum

class EntityType(IntEnum):
    MAZE_ENTITY_NONE = 0,
    MAZE_ENTITY_WALL = 1,
    MAZE_ENTITY_KEY = 2,
    MAZE_ENTITY_TRAP = 3,
    MAZE_ENTITY_PLEASURE_PLATE = 4,
    MAZE_ENTITY_BTN = 5,
    MAZE_ENTITY_BOX = 6,

entities = {
    EntityType.MAZE_ENTITY_KEY:[
         (45, 6), (4, 22), (41, 61)
    ],
    EntityType.MAZE_ENTITY_TRAP: [
        (30, 26), (25, 30), (52, 30), (14, 41), (13, 42)
    ],
    EntityType.MAZE_ENTITY_PLEASURE_PLATE: [
        (37, 26), (60, 39)
    ],
    EntityType.MAZE_ENTITY_BTN: [
        (31, 30, 26), (12, 38, 34), (55, 39, 32)
    ],
    EntityType.MAZE_ENTITY_BOX: [
        (8, 13, 1), (29, 26, 0), (51, 29, 1), (12, 40, 0), (8, 61, 0), (34, 61, 0) 
    ]
}

keynum = 0