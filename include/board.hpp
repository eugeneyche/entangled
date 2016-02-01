#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

struct BezierCurve
{
    glm::vec2 begin;
    glm::vec2 begin_control;
    glm::vec2 end_control;
    glm::vec2 end;

    std::vector<glm::vec2> positions;

    void update();
};

class Tile
{
public:
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
    BezierCurve curves [12]; 

    void updateCurves();
};

class Board
{
public:
    Board();

    void load(const std::string& path);

    int getWidth() const;
    int getHeight() const;
    Tile& getTile(int x, int y);
    const Tile& getTile(int x, int y) const;

private:
    int width_;
    int height_;
    std::vector<Tile> tiles_;

    void randomize(Tile& tile);
};
