#include "carte.hpp"

const int WIDTH = 50;
const int HEIGHT = 30;
const int ITERATIONS = 5;
const int FILL_PROBABILITY = 45; // Pourcentage de cellules initialement remplies


void initializeGrid(std::vector<std::vector<int>>& grid);
int countAliveNeighbors(const std::vector<std::vector<int>>& grid, int x, int y);
void simulateStep(std::vector<std::vector<int>>& grid);

// Fonction pour initialiser la grille avec des cellules aléatoires
void initializeGrid(std::vector<std::vector<int>>& grid) {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            grid[y][x] = (rand() % 100 < FILL_PROBABILITY) ? 1 : 0;
        }
    }
}

// Compte les cellules voisines remplies autour d'une cellule donnée
int countAliveNeighbors(const std::vector<std::vector<int>>& grid, int x, int y) {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue; // Ignore la cellule elle-même
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                count += grid[ny][nx];
            } else {
                count++; // Considère les bords comme remplis
            }
        }
    }
    return count;
}

// Applique une étape de l'automate cellulaire
void simulateStep(std::vector<std::vector<int>>& grid) {
    std::vector<std::vector<int>> newGrid = grid;
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int aliveNeighbors = countAliveNeighbors(grid, x, y);
            if (grid[y][x] == 1) {
                newGrid[y][x] = (aliveNeighbors >= 4) ? 1 : 0; // Survie
            } else {
                newGrid[y][x] = (aliveNeighbors >= 5) ? 1 : 0; // Naissance
            }
        }
    }
    grid = newGrid;
}

// Affiche la grille dans la console
void printGrid(const std::vector<std::vector<int>>& grid) {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            std::cout << (grid[y][x] ? '#' : '.');
        }
        std::cout << std::endl;
    }
}

// Génère la carte complète en appliquant les étapes de l'automate cellulaire
// et en retournant la grille finale
std::vector<std::vector<int>> createMap(){

    
    srand(static_cast<unsigned>(time(0)));
    
    std::vector<std::vector<int>> grid(HEIGHT, std::vector<int>(WIDTH, 0));
    initializeGrid(grid);
    
    
    for (int i = 0; i < ITERATIONS; ++i) {
        simulateStep(grid);
    }

    //Blocs minables
    int yellowProbability = 5; 
    for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
        if (grid[y][x] == 0) {
            bool isEdge = false;
            if ((y > 0 && grid[y-1][x] == 1) || // haut
                (y < HEIGHT-1 && grid[y+1][x] == 1) || // bas
                (x > 0 && grid[y][x-1] == 1) || // gauche
                (x < WIDTH-1 && grid[y][x+1] == 1)) { // droite
                isEdge = true;
            }
            if (isEdge && (rand() % 100 < yellowProbability)) {
                grid[y][x] = 2; 
            }
        }
    }
    }

    int blueProbability = 1; 
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (grid[y][x] == 0 && (rand() % 100 < blueProbability)) {
                grid[y][x] = 3; 
            }
        }
    }
    return grid;
}