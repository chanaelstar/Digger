#include "carte.hpp"

const int WIDTH = 50;
const int HEIGHT = 30;
const int ITERATIONS = 5;
const int FILL_PROBABILITY = 45; // Pourcentage de cellules initialement remplies

void initializeGrid(std::vector<std::vector<int>> &grid);
int countAliveNeighbors(const std::vector<std::vector<int>> &grid, int x, int y);
void simulateStep(std::vector<std::vector<int>> &grid);

// Fonction pour initialiser la grille avec des cellules aléatoires
void initializeGrid(std::vector<std::vector<int>> &grid)
{
    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            grid[y][x] = (rand() % 100 < FILL_PROBABILITY) ? 1 : 0;
        }
    }
}

// Compte les cellules voisines remplies autour d'une cellule donnée
int countAliveNeighbors(const std::vector<std::vector<int>> &grid, int x, int y)
{
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            if (dx == 0 && dy == 0)
                continue;
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT)
            {
                count += grid[ny][nx];
            }
            else
            {
                count++;
            }
        }
    }
    return count;
}

// Applique une étape de l'automate cellulaire
void simulateStep(std::vector<std::vector<int>> &grid)
{
    std::vector<std::vector<int>> newGrid = grid;
    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            int aliveNeighbors = countAliveNeighbors(grid, x, y);
            if (grid[y][x] == 1)
            {
                newGrid[y][x] = (aliveNeighbors >= 4) ? 1 : 0;
            }
            else
            {
                newGrid[y][x] = (aliveNeighbors >= 5) ? 1 : 0;
            }
        }
    }
    grid = newGrid;
}

const int DX[4] = {1, -1, 0, 0};
const int DY[4] = {0, 0, 1, -1};

// Marque toutes les cases connectées à partir de (x, y)
void floodFill(std::vector<std::vector<int>> &grid, std::vector<std::vector<int>> &componentMap, int x, int y, int compId, Component &comp)
{
    int h = grid.size();
    int w = grid[0].size();
    std::queue<std::pair<int, int>> q;
    q.push({x, y});
    componentMap[y][x] = compId;
    comp.tiles.push_back({x, y});

    while (!q.empty())
    {
        auto [cx, cy] = q.front();
        q.pop();
        for (int d = 0; d < 4; ++d)
        {
            int nx = cx + DX[d], ny = cy + DY[d];
            if (nx >= 0 && nx < w && ny >= 0 && ny < h &&
                componentMap[ny][nx] == -1 &&
                (grid[ny][nx] == 0 || grid[ny][nx] == 2 || grid[ny][nx] == 3))
            {
                componentMap[ny][nx] = compId;
                comp.tiles.push_back({nx, ny});
                q.push({nx, ny});
            }
        }
    }

    // Calcul du centre
    int sumX = 0, sumY = 0;
    for (auto &p : comp.tiles)
    {
        sumX += p.first;
        sumY += p.second;
    }
    comp.centerX = sumX / comp.tiles.size();
    comp.centerY = sumY / comp.tiles.size();
}

// Creuse un chemin rectiligne entre deux points
void connectPoints(std::vector<std::vector<int>> &grid, int x1, int y1, int x2, int y2)
{
    float t = 0.0f;
    float dt = 0.01f;
    while (t <= 1.0f)
    {
        float xt = (1 - t) * x1 + t * x2;
        float yt = (1 - t) * y1 + t * y2;

        float offsetX = sin(t * 10.0f) * 2.0f;
        float offsetY = cos(t * 10.0f) * 2.0f;

        int xi = int(xt + offsetX);
        int yi = int(yt + offsetY);

        // creuse un tunnel
        for (int dy = -2; dy <= 2; ++dy)
        {
            for (int dx = -2; dx <= 2; ++dx)
            {
                int nx = xi + dx;
                int ny = yi + dy;
                if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT)
                {
                    grid[ny][nx] = 0;
                }
            }
        }

        t += dt;
    }
}

// Affiche la grille dans la console
void printGrid(const std::vector<std::vector<int>> &grid)
{
    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            std::cout << (grid[y][x] ? '#' : '.');
        }
        std::cout << std::endl;
    }
}

// Génère la carte complète en appliquant les étapes de l'automate cellulaire et en retournant la grille finale
std::vector<std::vector<int>> createMap()
{
    srand(static_cast<unsigned>(time(0)));

    // Initialisation de la grille
    std::vector<std::vector<int>> grid(HEIGHT, std::vector<int>(WIDTH, 0));
    initializeGrid(grid);

    for (int i = 0; i < ITERATIONS; ++i)
    {
        simulateStep(grid);
    }

    // Détection des composantes connexes
    std::vector<std::vector<int>> componentMap(HEIGHT, std::vector<int>(WIDTH, -1));
    std::vector<Component> components;
    int compId = 0;

    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            if ((grid[y][x] == 0 || grid[y][x] == 2 || grid[y][x] == 3) && componentMap[y][x] == -1)
            {
                Component comp;
                comp.id = compId;
                floodFill(grid, componentMap, x, y, compId, comp);
                components.push_back(comp);
                ++compId;
            }
        }
    }

    // Connexion de toutes les composantes à la première
    for (size_t i = 1; i < components.size(); ++i)
    {
        int x1 = components[i].centerX;
        int y1 = components[i].centerY;
        int x2 = components[0].centerX;
        int y2 = components[0].centerY;
        connectPoints(grid, x1, y1, x2, y2);

        // Placement des blocs spéciaux et minables
        int yellowProbability = 5;
        int blueProbability = 1;

        for (int y = 0; y < HEIGHT; ++y)
        {
            for (int x = 0; x < WIDTH; ++x)
            {
                if (grid[y][x] == 0)
                {
                    bool isEdge = false;
                    if ((y > 0 && grid[y - 1][x] == 1) ||
                        (y < HEIGHT - 1 && grid[y + 1][x] == 1) ||
                        (x > 0 && grid[y][x - 1] == 1) ||
                        (x < WIDTH - 1 && grid[y][x + 1] == 1))
                    {
                        isEdge = true;
                    }
                    if (isEdge && (rand() % 100 < yellowProbability))
                    {
                        grid[y][x] = 2;
                    }
                    else if (rand() % 100 < blueProbability)
                    {
                        grid[y][x] = 3;
                    }
                }
            }
        }
        int greenProbability = 1;
        for (int y = 0; y < HEIGHT; ++y)
        {
            for (int x = 0; x < WIDTH; ++x)
            {
                if (grid[y][x] == 0 && (rand() % 100 < greenProbability))
                {
                    grid[y][x] = 4;
                }
            }
        }
    }

    return grid;
}