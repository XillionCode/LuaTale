#pragma once

#include <memory>
#include <vector>

class Entity;

class Room : public std::enable_shared_from_this<Room> {
public:
    Room() = default;
    virtual ~Room();

    void add_entity(std::shared_ptr<Entity> entity);

    virtual void step();
    virtual void draw();

protected:
    std::vector<std::shared_ptr<Entity>> entities_;
};
