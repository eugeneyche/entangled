#include <fstream>
#include <cstdlib>
#include <algorithm>
#include "board.hpp"
#include "error.hpp"

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
    for (int i = 0; i < height_; i++)
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
    // TODO: Weights for bezier curve
}
