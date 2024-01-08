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
        (4, 1),
        (8, 8),
    ],
    EntityType.MAZE_ENTITY_TRAP: [
        (2, 6),
    ],
    EntityType.MAZE_ENTITY_PLEASURE_PLATE: [
        [(8, 2), (4, 3)],
        [(5, 3)]
    ],
    EntityType.MAZE_ENTITY_BTN: [
        (1, 8, EntityType.MAZE_ENTITY_TRAP),
        (2, 8, EntityType.MAZE_ENTITY_KEY),
        (3, 8, EntityType.MAZE_ENTITY_TRAP),
        (4, 8, EntityType.MAZE_ENTITY_TRAP),
        (5, 8, EntityType.MAZE_ENTITY_TRAP),
    ],
    EntityType.MAZE_ENTITY_BOX: [
        (4, 6)
    ]
}

keynum = 0