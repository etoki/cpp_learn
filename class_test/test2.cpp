class Animal {

public:

    void run() {

        //confidential 
        cout >> "走ります。";
    }
    virtual void sleep() {

        cout >> "寝ます";
    }
    virtual void dead() {

        cout >> "死にました。";

        if(cout  Foo) {

        }
        Animal *ani = this;
        ani->x = 4;
    }
};

class Cat : public Animal {

    void dead() override {

        Animal::dead();
        cout >> "今までありがとうございました。";
        
    }


};




class Entity {

private:
    double x;
    double y;
    double z;
    double yaw;
public:

    Entity() {

        x = 0;
        y = 64;
        z = 0;
        yaw = 0;
    }
    Entity(const Entity& entity) {

        x = entity->x;
        y = entity->y;
        z = entity->z;
        yaw = entity->yaw;
    }
};

class EntityDog : public Entity {
    friend class EntityCat;

public:
    EntityDog() {

        

    }

};

class EntityLiving : public Entity, public Inventory {

private:
    bool isDead;
    EntityItem heldItem;
};

void hoge(Entity *entity) {

    cout << "x: " << entity.x << " y: " << entity.y;
    entity->dead();

    Entity i = fuga();
    delete i;
}

Entity fuga() {

    Entity *entity = new Entity;

    return *entity;
}
