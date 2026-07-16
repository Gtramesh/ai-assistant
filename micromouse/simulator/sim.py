#!/usr/bin/env python3
"""
Micromouse Maze Simulator
A simple simulator for testing maze solving algorithms.
"""

import pygame
import sys
import json
from collections import deque

# Constants
CELL_SIZE = 40
MAZE_SIZE = 16
WINDOW_SIZE = CELL_SIZE * MAZE_SIZE + 200
FPS = 60

# Colors
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
YELLOW = (255, 255, 0)
GRAY = (128, 128, 128)

class Maze:
    def __init__(self, size=16):
        self.size = size
        self.cells = [[{'walls': {'N': True, 'E': True, 'S': True, 'W': True},
                        'visited': False, 'distance': 0}
                       for _ in range(size)] for _ in range(size)]
        self.start = (0, 0)
        self.goal = [(size//2 - 1, size//2 - 1), (size//2 - 1, size//2),
                     (size//2, size//2 - 1), (size//2, size//2)]
        
        # Initialize distances
        self._init_distances()
    
    def _init_distances(self):
        for x in range(self.size):
            for y in range(self.size):
                min_dist = float('inf')
                for gx, gy in self.goal:
                    dist = abs(x - gx) + abs(y - gy)
                    min_dist = min(min_dist, dist)
                self.cells[x][y]['distance'] = min_dist
    
    def has_wall(self, x, y, direction):
        if 0 <= x < self.size and 0 <= y < self.size:
            return self.cells[x][y]['walls'][direction]
        return True
    
    def set_wall(self, x, y, direction, present):
        if 0 <= x < self.size and 0 <= y < self.size:
            self.cells[x][y]['walls'][direction] = present
            
            # Set opposite wall on neighbor
            opposites = {'N': 'S', 'S': 'N', 'E': 'W', 'W': 'E'}
            dx, dy = {'N': (0, 1), 'S': (0, -1), 'E': (1, 0), 'W': (-1, 0)}[direction]
            nx, ny = x + dx, y + dy
            if 0 <= nx < self.size and 0 <= ny < self.size:
                self.cells[nx][ny]['walls'][opposites[direction]] = present
    
    def get_neighbor(self, x, y, direction):
        dx, dy = {'N': (0, 1), 'S': (0, -1), 'E': (1, 0), 'W': (-1, 0)}[direction]
        return (x + dx, y + dy)
    
    def is_valid(self, x, y):
        return 0 <= x < self.size and 0 <= y < self.size
    
    def flood_fill(self):
        queue = deque()
        
        # Reset distances
        for x in range(self.size):
            for y in range(self.size):
                self.cells[x][y]['distance'] = float('inf')
        
        # Initialize goals
        for gx, gy in self.goal:
            self.cells[gx][gy]['distance'] = 0
            queue.append((gx, gy))
        
        # Propagate
        while queue:
            x, y = queue.popleft()
            current_dist = self.cells[x][y]['distance']
            
            for direction in ['N', 'E', 'S', 'W']:
                if not self.has_wall(x, y, direction):
                    nx, ny = self.get_neighbor(x, y, direction)
                    if self.is_valid(nx, ny):
                        new_dist = current_dist + 1
                        if new_dist < self.cells[nx][ny]['distance']:
                            self.cells[nx][ny]['distance'] = new_dist
                            queue.append((nx, ny))
    
    def generate_random(self, seed=None):
        if seed is not None:
            import random
            random.seed(seed)
        else:
            import random
        
        # Reset walls
        for x in range(self.size):
            for y in range(self.size):
                self.cells[x][y]['walls'] = {'N': True, 'E': True, 'S': True, 'W': True}
        
        # Generate maze using recursive backtracker
        stack = [(0, 0)]
        visited = {(0, 0)}
        
        while stack:
            x, y = stack[-1]
            
            # Get unvisited neighbors
            neighbors = []
            for direction in ['N', 'E', 'S', 'W']:
                nx, ny = self.get_neighbor(x, y, direction)
                if self.is_valid(nx, ny) and (nx, ny) not in visited:
                    neighbors.append((nx, ny, direction))
            
            if neighbors:
                nx, ny, direction = random.choice(neighbors)
                self.set_wall(x, y, direction, False)
                visited.add((nx, ny))
                stack.append((nx, ny))
            else:
                stack.pop()
        
        self._init_distances()
    
    def flood_fill_update(self, x, y):
        queue = deque([(x, y)])
        
        while queue:
            cx, cy = queue.popleft()
            current_dist = self.cells[cx][cy]['distance']
            
            # Find minimum neighbor distance
            min_neighbor = float('inf')
            for direction in ['N', 'E', 'S', 'W']:
                if not self.has_wall(cx, cy, direction):
                    nx, ny = self.get_neighbor(cx, cy, direction)
                    if self.is_valid(nx, ny):
                        min_neighbor = min(min_neighbor, self.cells[nx][ny]['distance'])
            
            # Update if needed
            if current_dist != min_neighbor + 1:
                self.cells[cx][cy]['distance'] = min_neighbor + 1
                for direction in ['N', 'E', 'S', 'W']:
                    if not self.has_wall(cx, cy, direction):
                        nx, ny = self.get_neighbor(cx, cy, direction)
                        if self.is_valid(nx, ny):
                            queue.append((nx, ny))


class Robot:
    def __init__(self, maze):
        self.maze = maze
        self.x, self.y = maze.start
        self.direction = 'N'
        self.path = []
        self.visited = set()
        self.solved = False
    
    def get_direction_priority(self):
        """Get directions to try based on flood fill (lowest distance first)"""
        directions = []
        for d in ['N', 'E', 'S', 'W']:
            if not self.maze.has_wall(self.x, self.y, d):
                nx, ny = self.maze.get_neighbor(self.x, self.y, d)
                if self.maze.is_valid(nx, ny):
                    dist = self.maze.cells[nx][ny]['distance']
                    directions.append((dist, d))
        
        directions.sort()
        return [d for _, d in directions]
    
    def move(self):
        if self.solved:
            return False
        
        # Check if at goal
        if (self.x, self.y) in self.maze.goal:
            self.solved = True
            return False
        
        # Get best direction
        directions = self.get_direction_priority()
        
        if not directions:
            # Dead end, backtrack
            if self.path:
                prev_x, prev_y = self.path.pop()
                self.x, self.y = prev_x, prev_y
                return True
            return False
        
        # Move to lowest distance neighbor
        best_dir = directions[0]
        
        # Record current position
        self.path.append((self.x, self.y))
        self.visited.add((self.x, self.y))
        
        # Move
        self.direction = best_dir
        self.x, self.y = self.maze.get_neighbor(self.x, self.y, best_dir)
        
        # Update flood fill
        self.maze.flood_fill_update(self.x, self.y)
        
        return True
    
    def get_speed_path(self):
        """Get optimized path for speed run"""
        if not self.solved:
            return []
        
        path = []
        x, y = self.maze.start
        
        while (x, y) not in self.maze.goal:
            path.append((x, y))
            
            # Get best direction
            best_dist = float('inf')
            best_dir = None
            
            for d in ['N', 'E', 'S', 'W']:
                if not self.maze.has_wall(x, y, d):
                    nx, ny = self.maze.get_neighbor(x, y, d)
                    if self.maze.is_valid(nx, ny):
                        dist = self.maze.cells[nx][ny]['distance']
                        if dist < best_dist:
                            best_dist = dist
                            best_dir = d
            
            if best_dir:
                x, y = self.maze.get_neighbor(x, y, best_dir)
            else:
                break
        
        path.append((x, y))
        return path


class Simulator:
    def __init__(self):
        pygame.init()
        self.screen = pygame.display.set_mode((WINDOW_SIZE, WINDOW_SIZE))
        pygame.display.set_caption("Micromouse Simulator")
        self.clock = pygame.time.Clock()
        
        self.maze = Maze()
        self.robot = Robot(self.maze)
        
        self.auto_mode = False
        self.speed = 10  # moves per second
        self.last_move_time = 0
    
    def draw_maze(self):
        self.screen.fill(WHITE)
        
        for x in range(self.maze.size):
            for y in range(self.maze.size):
                px = x * CELL_SIZE + 10
                py = (self.maze.size - 1 - y) * CELL_SIZE + 10
                
                # Draw cell background
                if (x, y) in self.maze.goal:
                    pygame.draw.rect(self.screen, GREEN, (px, py, CELL_SIZE, CELL_SIZE))
                elif (x, y) in self.robot.visited:
                    pygame.draw.rect(self.screen, (200, 200, 255), (px, py, CELL_SIZE, CELL_SIZE))
                
                # Draw walls
                if self.maze.has_wall(x, y, 'N'):
                    pygame.draw.line(self.screen, BLACK, (px, py), (px + CELL_SIZE, py), 2)
                if self.maze.has_wall(x, y, 'S'):
                    pygame.draw.line(self.screen, BLACK, (px, py + CELL_SIZE), (px + CELL_SIZE, py + CELL_SIZE), 2)
                if self.maze.has_wall(x, y, 'W'):
                    pygame.draw.line(self.screen, BLACK, (px, py), (px, py + CELL_SIZE), 2)
                if self.maze.has_wall(x, y, 'E'):
                    pygame.draw.line(self.screen, BLACK, (px + CELL_SIZE, py), (px + CELL_SIZE, py + CELL_SIZE), 2)
                
                # Draw distance value
                dist = self.maze.cells[x][y]['distance']
                font = pygame.font.Font(None, 16)
                text = font.render(str(dist), True, GRAY)
                self.screen.blit(text, (px + 5, py + 5))
        
        # Draw robot
        px = self.robot.x * CELL_SIZE + 10 + CELL_SIZE // 2
        py = (self.maze.size - 1 - self.robot.y) * CELL_SIZE + 10 + CELL_SIZE // 2
        pygame.draw.circle(self.screen, RED, (px, py), 8)
        
        # Draw direction indicator
        dx, dy = {'N': (0, -15), 'S': (0, 15), 'E': (15, 0), 'W': (-15, 0)}[self.robot.direction]
        pygame.draw.line(self.screen, RED, (px, py), (px + dx, py + dy), 3)
        
        # Draw info panel
        info_x = self.maze.size * CELL_SIZE + 20
        font = pygame.font.Font(None, 24)
        
        info_texts = [
            f"Position: ({self.robot.x}, {self.robot.y})",
            f"Direction: {self.robot.direction}",
            f"Visited: {len(self.robot.visited)}",
            f"Path length: {len(self.robot.path)}",
            f"Solved: {self.robot.solved}",
            f"Auto: {self.auto_mode}",
            f"Speed: {self.speed} moves/s",
            "",
            "Controls:",
            "SPACE: Toggle auto",
            "UP/DOWN: Speed",
            "R: Reset",
            "G: Generate new maze",
            "F: Flood fill",
            "Arrow keys: Move"
        ]
        
        for i, text in enumerate(info_texts):
            surface = font.render(text, True, BLACK)
            self.screen.blit(surface, (info_x, 20 + i * 25))
        
        pygame.display.flip()
    
    def handle_input(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                return False
            
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    return False
                
                elif event.key == pygame.K_SPACE:
                    self.auto_mode = not self.auto_mode
                
                elif event.key == pygame.K_UP:
                    self.speed = min(100, self.speed + 5)
                
                elif event.key == pygame.K_DOWN:
                    self.speed = max(1, self.speed - 5)
                
                elif event.key == pygame.K_r:
                    self.maze = Maze()
                    self.robot = Robot(self.maze)
                
                elif event.key == pygame.K_g:
                    import random
                    self.maze.generate_random()
                    self.robot = Robot(self.maze)
                    self.maze.flood_fill()
                
                elif event.key == pygame.K_f:
                    self.maze.flood_fill()
                
                elif event.key == pygame.K_LEFT:
                    if not self.robot.solved:
                        if not self.maze.has_wall(self.robot.x, self.robot.y, 'W'):
                            self.robot.path.append((self.robot.x, self.robot.y))
                            self.robot.visited.add((self.robot.x, self.robot.y))
                            self.robot.x, self.robot.y = self.maze.get_neighbor(self.robot.x, self.robot.y, 'W')
                            self.robot.direction = 'W'
                            self.maze.flood_fill_update(self.robot.x, self.robot.y)
                            if (self.robot.x, self.robot.y) in self.maze.goal:
                                self.robot.solved = True
                
                elif event.key == pygame.K_RIGHT:
                    if not self.robot.solved:
                        if not self.maze.has_wall(self.robot.x, self.robot.y, 'E'):
                            self.robot.path.append((self.robot.x, self.robot.y))
                            self.robot.visited.add((self.robot.x, self.robot.y))
                            self.robot.x, self.robot.y = self.maze.get_neighbor(self.robot.x, self.robot.y, 'E')
                            self.robot.direction = 'E'
                            self.maze.flood_fill_update(self.robot.x, self.robot.y)
                            if (self.robot.x, self.robot.y) in self.maze.goal:
                                self.robot.solved = True
                
                elif event.key == pygame.K_UP:
                    if not self.robot.solved:
                        if not self.maze.has_wall(self.robot.x, self.robot.y, 'N'):
                            self.robot.path.append((self.robot.x, self.robot.y))
                            self.robot.visited.add((self.robot.x, self.robot.y))
                            self.robot.x, self.robot.y = self.maze.get_neighbor(self.robot.x, self.robot.y, 'N')
                            self.robot.direction = 'N'
                            self.maze.flood_fill_update(self.robot.x, self.robot.y)
                            if (self.robot.x, self.robot.y) in self.maze.goal:
                                self.robot.solved = True
                
                elif event.key == pygame.K_DOWN:
                    if not self.robot.solved:
                        if not self.maze.has_wall(self.robot.x, self.robot.y, 'S'):
                            self.robot.path.append((self.robot.x, self.robot.y))
                            self.robot.visited.add((self.robot.x, self.robot.y))
                            self.robot.x, self.robot.y = self.maze.get_neighbor(self.robot.x, self.robot.y, 'S')
                            self.robot.direction = 'S'
                            self.maze.flood_fill_update(self.robot.x, self.robot.y)
                            if (self.robot.x, self.robot.y) in self.maze.goal:
                                self.robot.solved = True
        
        return True
    
    def update(self):
        if self.auto_mode and not self.robot.solved:
            current_time = pygame.time.get_ticks()
            if current_time - self.last_move_time > 1000 / self.speed:
                self.robot.move()
                self.last_move_time = current_time
    
    def run(self):
        running = True
        while running:
            running = self.handle_input()
            self.update()
            self.draw_maze()
            self.clock.tick(FPS)
        
        pygame.quit()
        sys.exit()


if __name__ == "__main__":
    sim = Simulator()
    sim.run()
