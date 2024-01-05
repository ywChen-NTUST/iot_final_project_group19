from pwn import *
from enum import IntEnum

class Command(IntEnum):
    PING = 1
    CREATEUSER = 2
    GETUSERATTR = 3
    SETUSERPOS = 4
    SETKEYNUM = 6

class Response(IntEnum):
    OK = 1
    ERROR = 2

IP = "192.168.222.67"
PORT = 12345

def ping():
    conn = remote(IP, PORT)
    conn.send(bytes([Command.PING]))
    recv = conn.recvuntil(b"\x00")
    conn.close()
    if recv[0] == Response.OK:
        return True
    else:
        return False

def createUser() -> bytes:
    conn = remote(IP, PORT)
    conn.send(bytes([Command.CREATEUSER]))
    recv = conn.recvuntil(b"\x00")
    conn.close()
    if recv[0] == Response.OK:
        uid = recv[1]
        token = recv[2:10]
        return uid,token
    else:
        return None

def getUserAttr(uid: int) -> tuple:
    conn = remote(IP, PORT)
    conn.send(bytes([Command.GETUSERATTR]) + bytes([uid]))
    recv = conn.recvuntil(b"\x00")
    conn.close()
    if recv[0] == Response.OK:
        pos_x = recv[1]
        pos_y = recv[2]
        life = recv[3]
        return (pos_x, pos_y, life)
    else:
        return None

def setUserAttr(token: bytes, pos_x: int = None, pos_y: int = None, keynum: int = None) -> bool:
    result = True
    if pos_x != None and pos_y != None:
        # set pos
        conn = remote(IP, PORT)
        conn.send(bytes([Command.SETUSERPOS]) + token + bytes([pos_x, pos_y]))
        recv = conn.recvuntil(b"\x00")
        conn.close()
        if recv[0] == Response.OK:
            result = result and True
        else:
            result = result and False
    if keynum != None:
        # set life
        conn = remote(IP, PORT)
        conn.send(bytes([Command.SETKEYNUM]) + bytes([keynum]))
        recv = conn.recvuntil(b"\x00")
        conn.close()
        if recv[0] == Response.OK:
            result = result and True
        else:
            result = result and False
    return result
    
res = ping()
if res:
    print("Ping OK")
else:
    print("Ping failed")
    exit(1)

res = createUser()
if res:
    myuid, token = res
    print("Create user OK")
    print("UID:", myuid)
    print("Token:", token)
else:
    print("Create user failed")
    exit(1)

while True:
    res = getUserAttr(1)
    if not res:
        print("Get user 0 attr failed")
        exit(1)
    pos_x, pos_y, life = res
    if(myuid == 1):
        print("(me)", end=" ")
    print("User 0:")
    print("Pos:", pos_x, pos_y)
    print("Life:", life)

    res = getUserAttr(2)
    if not res:
        print("No user 1 data")
    else:
        pos_x, pos_y, life = res
        if(myuid == 2):
            print("(me)", end=" ")
        print("User 1:")
        print("Pos:", pos_x, pos_y)
        print("Life:", life)

    x,y,keynum = None,None,None

    try:
        properties = tuple(map(int, input("Enter new pos and keynum: ").split()))
        if len(properties) == 3:
            x, y, life = properties
        elif len(properties) == 2:
            x, y = properties
        elif len(properties) == 1:
            keynum = properties[0]
        else:
            print("Invalid input")
            continue
    except KeyboardInterrupt:
        print("\nClosing client...")
        break

    res = setUserAttr(token, x, y, keynum)
    if res:
        print("Set user attr OK")
    else:
        print("Set user attr failed")
        exit(1)