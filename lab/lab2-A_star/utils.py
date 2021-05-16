import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap

class Point:
    def __init__(self, parent, weight, g, x, y):
        self.parent = parent
        self.weight = weight
        self.x = x
        self.y = y
        self.g = g
        self.f = 0.0

    def get_coordinate(self):
        return self.x, self.y

    def __lt__(self, other):
        return self.f < other.f

def paintMap(map):
    cmap = ListedColormap(['r', 'w', 'b', 'y', 'k'])
    plt.matshow(map, cmap = cmap)
    plt.show()
    # plt.savefig('fig.png', bbox_inches='tight')

def paintMap2(map):
    cmap = ListedColormap(['r', 'w', 'k'])
    plt.matshow(map, cmap = cmap)
    plt.show()
    # plt.savefig('fig.png', bbox_inches='tight')