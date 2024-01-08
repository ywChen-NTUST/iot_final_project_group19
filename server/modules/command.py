from enum import IntEnum
from modules.user import users
from modules.globalparam import keynum, entities, EntityType

class Command(IntEnum):
    PING = 1
    CREATEUSER = 2
    GETUSERATTR = 3
    SETUSERPOS = 4
    GETKEYNUM = 5
    DOENTITY = 6
    SYNCMAZE = 7

class Response(IntEnum):
    OK = 1
    ERROR = 2

def commandHandler(data: bytes) -> bytes:
    global keynum
    command = data[0]
    if command == Command.PING:
        print("ping~pong")
        return bytes([Response.OK])
    elif command == Command.CREATEUSER:
        for uid,user in enumerate(users):
            if user.getToken() == None:
                user.genToken()
                return bytes([Response.OK, uid+1]) + user.getToken()
        return bytes([Response.ERROR])
    elif command == Command.GETUSERATTR:
        if len(data) >= 2:
            uid = data[1] - 1
            if 0 <= uid < len(users) and users[uid].getToken() != None:
                pos_x, pos_y = users[uid].getpos()
                life = users[uid].getlife()
                return bytes([Response.OK, pos_x, pos_y, life])
        return bytes([Response.ERROR])
    elif command == Command.SETUSERPOS:
        if len(data) >= 11:
            token = data[1:9]
            pos_x = data[9]
            pos_y = data[10]

            if pos_x < 1:
                pos_x = 1
            if pos_x > 255:
                pos_x = 255
            if pos_y < 1:
                pos_y = 1
            if pos_y > 255:
                pos_y = 255

            for user in users:
                if user.getToken() == token:
                    user.moveto(pos_x, pos_y)
                    return bytes([Response.OK])
        return bytes([Response.ERROR])
    elif command == Command.GETKEYNUM:
        return bytes([Response.OK, keynum])
    elif command == Command.DOENTITY:
        if len(data) >= 12:
            token = data[1:9]
            entity_type = data[9]
            pos_x = data[10]
            pos_y = data[11]

            for user in users:
                if user.getToken() == token:
                    user.moveto(pos_x, pos_y) # force update

                    all_entity_data = entities[entity_type]
                    if entity_type == EntityType.MAZE_ENTITY_KEY:
                        for i,entity_data in enumerate(all_entity_data):
                            print(all_entity_data, i, (pos_x, pos_y))
                            if (pos_x, pos_y) == entity_data:
                                keynum += 1
                                all_entity_data.pop(i)
                                users[0].enque_unsyncqueue(pos_x, pos_y, EntityType.MAZE_ENTITY_NONE)
                                users[1].enque_unsyncqueue(pos_x, pos_y, EntityType.MAZE_ENTITY_NONE)
                                return bytes([Response.OK])
                    elif entity_type == EntityType.MAZE_ENTITY_TRAP:
                        for i,entity_data in enumerate(all_entity_data):
                            print(all_entity_data, i, (pos_x, pos_y))
                            if (pos_x, pos_y) == entity_data:
                                user.setlife(user.getlife() - 1)
                                all_entity_data.pop(i)
                                users[0].enque_unsyncqueue(pos_x, pos_y, EntityType.MAZE_ENTITY_NONE)
                                users[1].enque_unsyncqueue(pos_x, pos_y, EntityType.MAZE_ENTITY_NONE)
                                return bytes([Response.OK])
                    elif entity_type == EntityType.MAZE_ENTITY_PLEASURE_PLATE:
                        for i,entity_data in enumerate(all_entity_data):
                            print(all_entity_data, i, (pos_x, pos_y))
                            if (pos_x, pos_y) == entity_data and keynum == 0:
                                keynum += 1
                                return bytes([Response.OK])
                    elif entity_type == EntityType.MAZE_ENTITY_BTN:
                        for i,entity_data in enumerate(all_entity_data):
                            print(all_entity_data, i, (pos_x, pos_y))
                            if (pos_x, pos_y) == (entity_data[0], entity_data[1]):
                                endY = entity_data[2]
                                for y in range(pos_y, endY, -1):
                                    users[0].enque_unsyncqueue(pos_x, y, EntityType.MAZE_ENTITY_NONE)
                                    users[1].enque_unsyncqueue(pos_x, y, EntityType.MAZE_ENTITY_NONE)
                                all_entity_data.pop(i)
                                return bytes([Response.OK])
                    elif entity_type == EntityType.MAZE_ENTITY_BOX:
                        for i,entity_data in enumerate(all_entity_data):
                            print(all_entity_data, i, (pos_x, pos_y))
                            if (pos_x, pos_y) == (entity_data[0], entity_data[1]):
                                if(entity_data[2] == 0):
                                    user.setlife(user.getlife() - 1)
                                else:
                                    keynum += 1
                                all_entity_data.pop(i)
                                users[0].enque_unsyncqueue(pos_x, pos_y, EntityType.MAZE_ENTITY_NONE)
                                users[1].enque_unsyncqueue(pos_x, pos_y, EntityType.MAZE_ENTITY_NONE)
                                return bytes([Response.OK])
        return bytes([Response.ERROR])
    elif command == Command.SYNCMAZE:
        if len(data) >= 9:
            token = data[1:9]

            for user in users:
                if user.getToken() == token:
                    ret = []
                    # print(user.unsync_queue)
                    queuelength = len(user.unsync_queue)
                    for _ in range(queuelength):
                        x, y, newentitytype = user.deque_unsyncqueue()
                        ret.append(x)
                        ret.append(y)
                        ret.append(newentitytype)
                    return bytes([Response.OK, queuelength] + ret)
        return bytes([Response.ERROR])
    else:
        return bytes([Response.ERROR])