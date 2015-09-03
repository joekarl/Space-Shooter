//
//  Entity.hpp
//  Space Shooter
//
//  Created by Karl Kirch on 8/26/15.
//  Copyright © 2015 Karl Kirch. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <unordered_map>
#include <boost/variant.hpp>

/**
 * Each component type should implement the getTypeId method
 * Each component Id should be unique
 */
template <typename ComponentType>
int getTypeId();

template <typename... ComponentTypes>
class Entity {
    
    class InitVisitor : public boost::static_visitor<void> {
    private:
        Entity<ComponentTypes...> *entity;
        
    public:
        
        InitVisitor(Entity<ComponentTypes...> *entity) {
            this->entity = entity;
        }
        
        template <typename T>
        void operator()(T &component)
        {
            component.init(*entity);
        }
    };
    
private:
    std::unordered_map<int, boost::variant<ComponentTypes...>> components;
    int componentMask = 0;
    bool dead = true;
    bool dying = false;
    bool creating = true;
    size_t id;
    
public:
    
    const size_t getId() {
        return id;
    }
    
    void setId(size_t id) {
        this->id = id;
    }
    
    const bool isDead() {
        return dead;
    }
    
    void kill() {
        dying = true;
    }
    
    void setDead(bool dead) {
        this->dead = dead;
    }
    
    const bool isDying() {
        return dying;
    }
    
    void setDying(bool dying) {
        this->dying = dying;
    }
    
    const bool isCreating() {
        return creating;
    }
    
    void setCreating(bool creating) {
        this->creating = creating;
    }
    
    const int getComponentMask() {
        return componentMask;
    }
    
    template <typename SpecificComponentType>
    SpecificComponentType& getComponent() {
        auto &componentVariant = components[getTypeId<SpecificComponentType>()];
        return boost::get<SpecificComponentType>(componentVariant);
    }
    
    template <typename SpecificComponentType>
    void addComponent(SpecificComponentType &component) {
        int typeId = getTypeId<SpecificComponentType>();
        componentMask |= 1 << typeId;
        auto &existingComponent = components[typeId];
        existingComponent = component;
    }
    
    template <typename SpecificComponentType>
    void removeComponent() {
        int typeId = getTypeId<SpecificComponentType>();
        componentMask &= ~(1 << typeId);
        // don't worry about actually removing the component as it's masked off
    }
    
    bool matchesMask(int specifiedMask) {
        return (specifiedMask & componentMask) == specifiedMask;
    }
    
    void init() {
        dead = false;
        dying = false;
        creating = false;
        for (auto &kv : components) {
            InitVisitor initVisitor(this);
            boost::apply_visitor(initVisitor, kv.second);
        }
    }
    
    void reset() {
        dead = true;
        dying = false;
        creating = false;
        componentMask = 0;
        components.clear();
    }
};

#endif /* Entity_hpp */
