#pragma once

struct Tile
{
    enum class Type
    {
        NONE,
        BLOCK,
        NORMAL
    };
    
    Type type;
    int rotation;
    int link [12];
    float weights [12];
};

class Board
{
public:
    Board(const std::string& path);

    int getWidth() const;
    int getHeight() const;
    Tile& getTile(int x, int y);

private:
    int width_;
    int height_;
    std::vector<Tile> tiles_;

    void randomize(Tile& tile);
};
