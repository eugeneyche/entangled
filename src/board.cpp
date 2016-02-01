#include <fstream>
#include <cstdlib>
#include <algorithm>
#include "board.hpp"
#include "error.hpp"

static glm::vec2 lerp_vec2(
        const glm::vec2& begin,
        const glm::vec2& end,
        float amount)
{
    return begin * (1 - amount) + end * amount;
}

static glm::vec2 getPathPosition(int index)
{
}

void BezierCurve::update()
{
    static int num_subdivides = 10;
    positions.clear();

    // Basic implementation, switch to recurive later
    for (int i = 0; i <= num_subdivides; i++)
    {
        float amount = float(i) / num_subdivides;
        glm::vec2 begin_mid = lerp_vec2(begin, end_control, amount);
        glm::vec2 end_mid = lerp_vec2(begin_control, end, amount);
        positions.push_back(lerp_vec2(begin_mid, end_mid, amount));
    }
}

void Tile::updateCurves()
{
    for (int i = 0; i < 12; i++)
    {
        curves[i].update();
    }
}

Board::Board() { }

void Board::load(const std::string& path)
{
    std::ifstream file (path);
    if (!file.is_open())
    {
        fatalError("Failed to load board '" + path + "'.");
    }

    std::string line;
    file >> width_ >> height_;
    getline(file, line);
    tiles_.resize(width_ * height_);
    for (int i = height_ - 1; i >= 0; i--)
    {
        getline(file, line);
        for (int j = 0; j < width_ && j < static_cast<int>(line.size()); j++)
        {
            switch (line[i])
            {
            case 'X':
                getTile(i, j).type = Tile::Type::BLOCK;
                break;
            case 'O':
                randomize(getTile(i, j));
                break;
            default:
                getTile(i, j).type = Tile::Type::NONE;
                break;
            }
        }
    }
    file.close();
}

int Board::getWidth() const
{
    return width_;
}

int Board::getHeight() const
{
    return height_;
}

Tile& Board::getTile(int x, int y)
{
    return tiles_[y * width_ + x];
}

const Tile& Board::getTile(int x, int y) const
{
    return tiles_[y * width_ + x];
}

void Board::randomize(Tile& tile)
{
    int available [12];
    for (int i = 0; i < 12; i++)
    {
        available[i] = i;
    }
    for (int i = 0; i < 12; i += 2)
    {
        int j = rand() % (12 - i - 1) + 1;
        std::swap(available[i], available[i + j]);
    }
    for (int i = 0; i < 12; i += 2)
    {
        tile.link[available[i]] = available[i + 1];
        tile.link[available[i + 1]] = available[i];
    }
}
