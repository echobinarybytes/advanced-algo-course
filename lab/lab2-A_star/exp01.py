import math
from utils import Point,paintMap, paintMap2
from queue import PriorityQueue

direction = [[0, 1], [0, -1], [1, 0], [-1, 0], [1, 1], [-1, -1], [-1, 1], [1, -1]]

def getF(point, target):
    #dist = (math.fabs(target.x - point.x) + math.fabs(target.y - point.y))
    dist = math.sqrt(math.pow(math.fabs(target.x - point.x), 2) + math.pow(math.fabs(target.y - point.y), 2))
    return point.g + dist

def isBound(row, col, max_row, max_col):
    if row < 0 or row > max_row:
        return True
    if col < 0 or col > max_col:
        return True
    return False

def getPath(target_area_block):
    stack = [target_area_block.get_coordinate()]
    father_area_block = target_area_block.parent
    while father_area_block is not None:
        stack.append(father_area_block.get_coordinate())
        father_area_block = father_area_block.parent
    return stack

def aStar(map, start, end):
    max_row = len(map) - 1
    max_col = len(map[0]) - 1
    open = PriorityQueue()
    close = set()
    open.put(start)
    while not open.empty():
        curr = open.get()
        for direct in direction:
            move_x = curr.x + direct[0]
            move_y = curr.y + direct[1]
            if (move_x, move_y) in close or isBound(move_x, move_y, max_row, max_col):
                continue
            if map[move_x][move_y] == 6:
                continue

            move_cost = 1
            if math.fabs(direct[0]) == 1 and math.fabs(direct[1]) == 1:
                move_cost *= math.sqrt(2)
            land_cost = map[move_x][move_y]
            next = Point(curr, land_cost, curr.g + land_cost + move_cost, move_x, move_y)

            if move_x == end.x and move_y == end.y:
                return getPath(next)
            result = None
            temp_queue = []
            while not open.empty():
                tmp_block = open.get()
                if tmp_block.get_coordinate() == next.get_coordinate():
                    result = tmp_block
                    break
                temp_queue.append(tmp_block)
            while len(temp_queue) != 0:
                open.put(temp_queue.pop())
            open.task_done()
            next.f =getF(next, end)
            if result is None:
                open.put(next)
            else:
                if next.g < result.g:
                    open.put(next)
                else:
                    open.put(result)
        close.add(curr.get_coordinate())
    return None




if __name__ == '__main__':
    map = [[0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2],
           [0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2],
           [6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2],
           [0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 0, 0, 0],
           [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 0, 0, 0, 0],
           [0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 4, 4, 0, 0, 0, 0, 0],
           [0, 0, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 4, 4, 0, 0, 0, 0, 0],
           [0, 0, 6, 0, 0, 6, 0, 6, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 4, 4, 4, 6, 0, 0, 0],
           [0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0],
           [0, 0, 0, 0, 0, 6, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 6, 0, 0, 0],
           [0, 0, 6, 0, 0, 6, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 4, 4, 0, 4, 4, 0, 0, 0],
           [0, 0, 6, 6, 6, 6, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 4, 0, 4, 4, 0, 0, 0, 0],
           [0, 0, 0, 6, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0],
           [0, 0, 0, 6, 0, 0, 0, 0, 6, 6, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 4, 4, 4, 0, 0, 0, 0, 0],
           [0, 0, 0, 6, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0],
           [0, 0, 0, 6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0],
           [0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0],
           [0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0],
           [0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0],
           [0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0]]

    for i in range(len(map)):
        for j in range(len(map[0])):
            if map[i][j] == 4:
                map[i][j] = 2
            elif map[i][j] == 2:
                map[i][j] = 4

    # for i in map:
    #     print(i)
    # paintMap(map)
    start = Point(None, 0, 0, 10, 4)
    end = Point(None,0, 2, 0, 35)
    route = aStar(map, start, end)
    for p in route:
        map[p[0]][p[1]] = -2
    for i in map:
        print(i)
    paintMap(map)

    map2 = [[0 for x in range(17)] for y in range(14)]
    map2[5][6] = 6
    map2[6][6] = 6
    map2[7][7] = 6
    map2[8][7] = 6
    map2[9][7] = 6
    map2[9][8] = 6
    map2[10][8] = 6
    map2[11][8] = 6
    start2 = Point(None, 0, 0, 8, 3)
    end2 = Point(None,0, 0, 9, 14)
    route2 = aStar(map2, start2, end2)
    for p in route2:
        map2[p[0]][p[1]] = -6
    for i in map2:
        print(i)
    paintMap2(map2)