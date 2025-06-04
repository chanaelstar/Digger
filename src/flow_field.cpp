#include "flow_field.hpp"

FlowField computeFlowField(const std::vector<std::vector<int>>& map, int targetX, int targetY) {
    int height = map.size();
    int width = map[0].size();

    FlowField field(height, std::vector<Direction>(width, {0, 0}));
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    
    std::queue<std::pair<int, int>> q;
    q.push({targetX, targetY});
    visited[targetY][targetX] = true;

    const std::vector<std::pair<int, int>> dirs = {
        {0, -1}, {1, 0}, {0, 1}, {-1, 0}
    };

    while (!q.empty()) {
        auto [cx, cy] = q.front(); q.pop();
        for (auto [dx, dy] : dirs) {
            int nx = cx + dx;
            int ny = cy + dy;

            if (nx >= 0 && nx < width && ny >= 0 && ny < height && map[ny][nx] == 0 && !visited[ny][nx]) {
                field[ny][nx] = {-dx, -dy}; // Direction pour aller vers la source
                visited[ny][nx] = true;
                q.push({nx, ny});
            }
        }
    }

    return field;
}
