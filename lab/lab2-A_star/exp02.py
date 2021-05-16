import math
from utils import Point,paintMap, paintMap2
from queue import PriorityQueue

direction = [[0, 1], [0, -1], [1, 0], [-1, 0], [1, 1], [-1, -1], [-1, 1], [1, -1]]

start_point = None
end_point = None

def getF(point, target):
    #dist = (math.fabs(target.x - point.x) + math.fabs(target.y - point.y))
    dist = int(math.sqrt(math.pow(math.fabs(target.x - point.x), 2) + math.pow(math.fabs(target.y - point.y), 2)))
    return point.g  * 5 + dist

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
    res = []
    startp = Point(start.parent, start.weight, start.g, start.x, start.y)
    endp = Point(end.parent, end.weight, end.g, end.x, end.y)
    max_row = len(map) - 1
    max_col = len(map[0]) - 1
    open = PriorityQueue()
    close = set()
    open2 = PriorityQueue()
    open.put(start)
    open2.put(end)
    while not open.empty() or open2.empty():
        """
        前向
        """
        curr = open.get()
        print("a", curr.x, curr.y)
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
            if move_x == endp.x and move_y == endp.y:
                curr = next
                break

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
            next.f =getF(next, endp)
            if result is None:
                open.put(next)
            else:
                if next.g < result.g:
                    open.put(next)
                else:
                    open.put(result)
        close.add(curr.get_coordinate())
        startp.x = curr.x
        startp.y = curr.y


        """
        后向
        """
        curr2 = open2.get()
        print("b", curr2.x, curr2.y)
        for direct in direction:
            move_x = curr2.x + direct[0]
            move_y = curr2.y + direct[1]
            if (move_x, move_y) in close or isBound(move_x, move_y, max_row, max_col):
                continue
            if map[move_x][move_y] == 6:
                continue

            move_cost = 1
            if math.fabs(direct[0]) == 1 and math.fabs(direct[1]) == 1:
                move_cost *= math.sqrt(2)
            land_cost = map[move_x][move_y]

            next2 = Point(curr2, land_cost, curr2.g + land_cost + move_cost, move_x, move_y)
            if move_x == startp.x and move_y == startp.y:
                curr2 = next2
                break

            result2 = None
            temp_queue2 = []
            while not open2.empty():
                tmp_block2 = open2.get()
                if tmp_block2.get_coordinate() == next2.get_coordinate():
                    result2 = tmp_block2
                    break
                temp_queue2.append(tmp_block2)
            while len(temp_queue2) != 0:
                open2.put(temp_queue2.pop())
            open2.task_done()
            next2.f = getF(next2, startp)
            if result2 is None:
                open2.put(next2)
            else:
                if next2.g < result2.g:
                    open2.put(next2)
                else:
                    open2.put(result2)
        close.add(curr2.get_coordinate())
        endp.x = curr2.x
        endp.y = curr2.y

        print(startp.x, startp.y, endp.x, endp.y)
        if startp.x == endp.x and startp.y == endp.y:
            return [curr, curr2]
        # curr2 = open2.get()
        # print("b", curr2.x, curr2.y)
        # for direct in direction:
        #     move_x = curr2.x + direct[0]
        #     move_y = curr2.y + direct[1]
        #     if (move_x, move_y) in close2 or isBound(move_x, move_y, max_row, max_col):
        #         continue
        #     if map[move_x][move_y] == 6:
        #         continue
        #
        #     move_cost = 1
        #     if math.fabs(direct[0]) == 1 and math.fabs(direct[1]) == 1:
        #         move_cost *= math.sqrt(2)
        #     land_cost = map[move_x][move_y]
        #     next2 = Point(curr2, land_cost, curr2.g + land_cost + move_cost, move_x, move_y)
        #
        #     if move_x == start.x and move_y == start.y:
        #         res.append([start, next2])
        #         return res
        #     result = None
        #     temp_queue = []
        #     while not open2.empty():
        #         tmp_block = open2.get()
        #         if tmp_block.get_coordinate() == next2.get_coordinate():
        #             result = tmp_block
        #             break
        #         temp_queue.append(tmp_block)
        #     while len(temp_queue) != 0:
        #         open2.put(temp_queue.pop())
        #     open2.task_done()
        #     # 检查该点是否在open表中, 如果不在则加入, 同时计算启发值 F = G + H
        #     next2.f = getH(next2, end)
        #     if result is None:
        #         open2.put(next2)
        #     else:
        #         if next2.g < result.g:
        #             open2.put(next2)
        #         else:
        #             open2.put(result)
        # close2.add(curr2.get_coordinate())
        # end = curr2

    return None


if __name__ == '__main__':
    map = [
        [0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
         2, 2, 2],
        [0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2,
         2, 2, 2],
        [6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2,
         2, 2, 2],
        [0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2,
         0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 0,
         0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 4, 4, 0, 0,
         0, 0, 0],
        [0, 0, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 4, 4, 0, 0,
         0, 0, 0],
        [0, 0, 6, 0, 0, 6, 0, 6, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 4, 4, 4, 6,
         0, 0, 0],
        [0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0,
         0, 0, 0],
        [0, 0, 0, 0, 0, 6, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 6,
         0, 0, 0],
        [0, 0, 6, 0, 0, 6, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 4, 4, 0, 4, 4,
         0, 0, 0],
        [0, 0, 6, 6, 6, 6, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 4, 0, 4, 4, 0,
         0, 0, 0],
        [0, 0, 0, 6, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0,
         0, 0, 0],
        [0, 0, 0, 6, 0, 0, 0, 0, 6, 6, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 4, 4, 4, 0, 0,
         0, 0, 0],
        [0, 0, 0, 6, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0,
         0, 0, 0],
        [0, 0, 0, 6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0,
         0, 0, 0],
        [0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0,
         0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0,
         0, 0, 0],
        [0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0,
         0, 0, 0],
        [0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0,
         0, 0, 0]]

    for i in range(len(map)):
        for j in range(len(map[0])):
            if map[i][j] == 4:
                map[i][j] = 2
            elif map[i][j] == 2:
                map[i][j] = 4

    start = Point(None, 0, 0, 10, 4)
    end = Point(None, 0, 2, 0, 35)
    start_point = start
    end_point = end
    route = aStar(map, start, end)
    a = getPath(route[0])
    b = getPath(route[1])
    for i in a:
        map[i[0]][i[1]] = -2
    for j in b:
        map[j[0]][j[1]] = -2
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
    map2[8][3] = -6
    map2[9][14] = -6
    start2 = Point(None, 0, 0, 8, 3)
    end2 = Point(None, 0, 0, 9, 14)
    print("test")
    route2 = aStar(map2, start2, end2)
    a = getPath(route2[0])
    b = getPath(route2[1])
    for i in a:
        map2[i[0]][i[1]] = -6
    for j in b:
        map2[j[0]][j[1]] = -6
    paintMap2(map2)