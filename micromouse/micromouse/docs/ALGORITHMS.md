# Maze Solving Algorithms

## 1. Flood Fill Algorithm

### Concept

Flood fill is the primary algorithm for Micromouse maze solving. It assigns a distance value to each cell representing the minimum number of steps to reach the goal.

### Initialization

```
Goal cells (center): distance = 0
All other cells: distance = Manhattan distance to nearest goal
```

### Algorithm

```python
def flood_fill(maze, goal):
    queue = PriorityQueue()
    
    # Initialize goal cells
    for cell in goal_cells:
        maze[cell].distance = 0
        queue.put(cell, 0)
    
    while not queue.empty():
        current = queue.get()
        
        for direction in [NORTH, SOUTH, EAST, WEST]:
            if not maze.has_wall(current, direction):
                neighbor = maze.get_neighbor(current, direction)
                
                if maze[neighbor].distance > maze[current].distance + 1:
                    maze[neighbor].distance = maze[current].distance + 1
                    queue.put(neighbor, maze[neighbor].distance)
```

### Flood Fill Update

When a new wall is discovered:

```python
def update_flood(maze, cell):
    # Check if current cell's distance is correct
    min_neighbor = min(
        maze[d].distance 
        for d in [NORTH, SOUTH, EAST, WEST]
        if not maze.has_wall(cell, d)
    )
    
    if maze[cell].distance != min_neighbor + 1:
        # Propagate changes
        queue = [cell]
        while queue:
            current = queue.pop(0)
            for direction in [NORTH, SOUTH, EAST, WEST]:
                if not maze.has_wall(current, direction):
                    neighbor = maze.get_neighbor(current, direction)
                    if maze[neighbor].distance > maze[current].distance + 1:
                        maze[neighbor].distance = maze[current].distance + 1
                        queue.append(neighbor)
```

## 2. A* Pathfinding

### Concept

A* finds the shortest path from start to goal using heuristics for efficiency.

### Heuristic Function

```python
def heuristic(a, b):
    # Manhattan distance
    return abs(a.x - b.x) + abs(a.y - b.y)
```

### Algorithm

```python
def astar(maze, start, goal):
    open_set = PriorityQueue()
    open_set.put(start, 0)
    came_from = {}
    g_score = {start: 0}
    f_score = {start: heuristic(start, goal)}
    
    while not open_set.empty():
        current = open_set.get()
        
        if current == goal:
            return reconstruct_path(came_from, current)
        
        for direction in get_available_moves(maze, current):
            neighbor = get_neighbor(current, direction)
            tentative_g = g_score[current] + 1
            
            if tentative_g < g_score.get(neighbor, float('inf')):
                came_from[neighbor] = current
                g_score[neighbor] = tentative_g
                f_score[neighbor] = tentative_g + heuristic(neighbor, goal)
                open_set.put(neighbor, f_score[neighbor])
    
    return None  # No path found
```

## 3. Wall Following

### Left-Hand Rule

```python
def left_hand_rule(maze, position, direction):
    # Priority: Left, Straight, Right, Back
    left = turn_left(direction)
    if can_move(maze, position, left):
        return left
    
    if can_move(maze, position, direction):
        return direction
    
    right = turn_right(direction)
    if can_move(maze, position, right):
        return right
    
    return turn_around(direction)
```

### Right-Hand Rule

```python
def right_hand_rule(maze, position, direction):
    # Priority: Right, Straight, Left, Back
    right = turn_right(direction)
    if can_move(maze, position, right):
        return right
    
    if can_move(maze, position, direction):
        return direction
    
    left = turn_left(direction)
    if can_move(maze, position, left):
        return left
    
    return turn_around(direction)
```

## 4. Path Optimization

### Straight Line Merging

```python
def merge_straights(path):
    optimized = []
    i = 0
    
    while i < len(path):
        # Count consecutive same-direction moves
        count = 1
        while (i + count < len(path) and 
               path[i + count] == path[i]):
            count += 1
        
        optimized.append((path[i], count))
        i += count
    
    return optimized
```

### Diagonal Movement

```python
def optimize_diagonals(path):
    # Convert sequences to diagonal moves
    # Example: (FRONT, RIGHT, FRONT) -> (DIAGONAL_RIGHT)
    pass
```

## 5. Algorithm Comparison

| Algorithm | Pros | Cons | Best For |
|-----------|------|------|----------|
| Flood Fill | Memory efficient, guaranteed optimal | Requires full exploration | Primary solver |
| A* | Fast pathfinding | More memory | Path recalculation |
| Wall Following | Simple implementation | May not find shortest path | Backup/emergency |

## 6. Implementation Notes

### Distance Array Optimization

Use uint8_t for distances (max 255 cells):

```c
typedef struct {
    uint8_t distance;
    uint8_t walls;      // Bit field: N=1, E=2, S=4, W=8
    uint8_t visited;
} Cell;
```

### Flood Fill Propagation Limit

To prevent excessive computation:

```c
#define MAX_PROPAGATION_DEPTH 32

void flood_fill_bounded(Maze* maze, Cell start) {
    int depth = 0;
    Queue queue;
    
    queue_push(&queue, start);
    
    while (!queue_empty(&queue) && depth < MAX_PROPAGATION_DEPTH) {
        Cell current = queue_pop(&queue);
        depth++;
        
        // Process neighbors...
    }
}
```

## References

1. "Micromouse Maze Solving" - IEEE Robotics
2. "Flood Fill Algorithm for Maze Solving" - ACM Digital Library
3. Official Micromouse Competition Rules
