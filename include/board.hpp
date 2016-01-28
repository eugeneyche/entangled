#pragma once

#include <string>
#include <vector>

struct Tile
{
    enum class Type
    {
        NONE,
        BLOCK,
        NORMAL
    };

    bool is_focus;
    float rotation_offset; // this should be between -60 and 60 in radians

    Type type;
    int rotation;
    int link [12];
    float weights [12];
};

class Board
{
public:
    Board();

    void load(const std::string& path);

    int getWidth() const;
    int getHeight() const;
    Tile& getTile(int x, int y);

private:
    int width_;
    int height_;
    std::vector<Tile> tiles_;

    void randomize(Tile& tile);
};
