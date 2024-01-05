from random import randint
from modules.globalparam import EntityType

class User:
    def __init__(self):
        self.token = None
        self.pos_x = 1
        self.pos_y = 1
        self.life = 5
        self.unsync_queue = []
    def getToken(self):
        return self.token
    def genToken(self):
        self.token = bytes([randint(1, 255) for _ in range(8)])
    def moveto(self, x, y):
        self.pos_x = x
        self.pos_y = y
    def getpos(self):
        return (self.pos_x, self.pos_y)
    def setlife(self, life):
        self.life = life
    def getlife(self):
        return self.life
    def enque_unsyncqueue(self, x, y, newentitytype: EntityType):
        self.unsync_queue.append((x, y, newentitytype))
    def deque_unsyncqueue(self):
        return self.unsync_queue.pop(0)
    
users = [User(), User()]