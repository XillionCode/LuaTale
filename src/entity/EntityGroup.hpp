#pragma once

#include "Entity.hpp"

#include <memory>
#include <vector>

class EntityGroup : public Entity {
public:
    EntityGroup() = default;
    ~EntityGroup() override;

    void add_child(std::shared_ptr<Entity> child);
    void clear_children();

    void step() final;
    void draw() final;

protected:
    virtual void self_step();
    virtual void self_draw();

    std::vector<std::shared_ptr<Entity>> children_;
};
