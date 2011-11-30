#ifndef MATCHACTORS_H
#define	MATCHACTORS_H


#include "../GNAFramework/Geom.h"
#include "Sphere.h"
#include "Cube.h"
#include "Plane.h"
#include "MatchDescriptors.h"
#include <cfloat>


#define PLAYER_SHOOT_SPEED 20.f
class Player{
public:
    int       life;
    Player_d *descriptor;
    BoundingSphere    area[3];
    float     time_to_shoot;
    
    bool ia_activated;
    
    static Color color(int index) {
        switch (index){
            case 0: return Color::Blue;
            case 1: return Color::Red;
            case 2: return Color::Green;
            default: return Color(1.f, 0.5f, 0.f, 1.f); //Orange
        }
    }
    
    Player() {};
    
    Player(Player_d *descriptor, int life = 3){
        // <editor-fold defaultstate="collapsed" desc="Code">
        this->descriptor = descriptor;
        this->life = life;
        t = 0.0;
        index = 0;

        time_to_shoot = descriptor->shoot;
        ia_activated = true;

        shoot_desc.requested = false;

        float radio = descriptor->height / 2.f;
        area[0] = BoundingSphere(Vector3(0.f, radio, 0.f), radio);
        area[2] = area[1] = area[0]; 
        // </editor-fold>
    };
    
    void hit(int damage) {
        life -= damage;
        if(life < 0) life = 0;
    }
    
    bool alive() const { return life > 0; }
    
    bool haveDisc() const { return time_to_shoot <= 0.f; }
    
    Vector3 position(float a=0.f) const {
        // <editor-fold defaultstate="collapsed" desc="code">
        Vector3 *pw = descriptor->pathway.points;
        Vector3 pos;
        if(descriptor->pathway.length == 1) {
            pos = pw[0];
        } else {
            Vector3 v0 = pw[index],
                    v1 = pw[(index + 1) % descriptor->pathway.length];
            float tt = t + a;
            pos = v0 * (1 - tt) + v1*tt;
        }
        pos.Y += descriptor->height / 2.f;
        return pos;
        // </editor-fold>
    }
    
    Vector3 shoot_position() const {
        Vector3 pos = position();
        pos.Y += descriptor->height / 2.f;
        return pos;
    }
    
    Vector3 size() const {
        return Vector3(descriptor->height, descriptor->height, 2*descriptor->height);
    }
    
    void update(float time) {
        advance(descriptor->pathway.speed * time);
        time_to_shoot -= time;
    }
    
    void ia_update(float time, const Player *players, int num_players) {
        // <editor-fold defaultstate="collapsed" desc="code">
        if (ia_activated) {
            if (haveDisc()) {
                const Player *closest = NULL;
                float dist_closest = FLT_MAX;
                float dist_test;
                Vector3 pos = shoot_position();
                for (int i = 0; i < num_players; i++)
                    if (this != &players[i] && players[i].alive()) {
                        dist_test = (players[i].position() - pos).LengthSquared();
                        if (dist_test < dist_closest) {
                            dist_closest = dist_test;
                            closest = players + i;
                        }
                    }
                if (closest != NULL) {
                    shoot_desc.requested = true;
                    shoot_desc.velocity = Vector3::Normalize(closest->position() - pos) * PLAYER_SHOOT_SPEED;
                    shoot_desc.position = pos;
                }
            }
        }// </editor-fold>
    }
    
    bool get_shoot_request(Vector3 *position, Vector3 *velocity) {
        // <editor-fold defaultstate="collapsed" desc="Code">
        if (shoot_desc.requested) {
            *velocity = shoot_desc.velocity;
            *position = shoot_desc.position;

            shoot_desc.requested = false;
            time_to_shoot = descriptor->shoot;
            return true;
        }
        return false;
        // </editor-fold>
    }
    
    void advance(float d) {
        // <editor-fold defaultstate="collapsed" desc="code">
        if (descriptor->pathway.length > 1) {
            Vector3 *pw = descriptor->pathway.points,
                    v0 = pw[index],
                    v1 = pw[(index + 1) % descriptor->pathway.length];

            Vector3 seg = v1 - v0;
            float td = 4 * d / seg.Length();
            float tf = t + td;

            if (tf >= 1.f) {
                t = tf - 1.f;
                index++;
                if (index >= descriptor->pathway.length) index = 0;
            } else if (tf < 0.f) {
                t = tf + 1.f;
                index--;
                if (index < 0) index = descriptor->pathway.length - 1;
            } else {
                t = tf;
            }
        }
        // </editor-fold>
    }
    
    Pathway_d path() const { return descriptor->pathway; }
    
    
    
    
    virtual ~Player(){};
private:
    float t;
    int index;
    
    struct {
        bool requested;
        Vector3 velocity;
        Vector3 position;
    } shoot_desc;
    
};


#define DISC_HITPOINTS 15
#define DISC_SIZE      0.5f
class Disc {
public:
    Vector3 position;
    Vector3 velocity;
    int     player_index;
    
    int     hitPoints;
    
    Disc() {
        hitPoints = 0;
    };
    
    void activate(Vector3 position, Vector3 velocity, int player_index) {
        this->position = position;
        this->velocity = velocity;
        hitPoints = DISC_HITPOINTS;
        this->player_index = player_index;
    }
    
    bool active() const { return hitPoints > 0; }
    
    void update(float time) {
        position += velocity * time;
    }
    
    float size() const { return DISC_SIZE; }
    
    BoundingSphere area()const {
        return BoundingSphere(Vector3(), size() / 2.f);
    }
    
    void bounce(Cube cube) {
        bounce( BoundingSphere(cube.Center(), abs(cube.p1.X - cube.p0.X)) );
    }
    
    void bounce(BoundingSphere sphere) {
        Vector3 norm = Vector3::Normalize(position - sphere.Center());
        velocity = Vector3::Reflect(norm, velocity);
        position = sphere.Center() + norm * (sphere.Radio() + size());
    }
    
    bool bounce(Plane plane) {
        float t = -Vector3::Dot(plane.Position - position, plane.Normal) / Vector3::Dot(plane.Normal, plane.Normal);
        if(t - size()/2.f <= 0) {
            velocity = Vector3::Reflect(plane.Normal, velocity);
            position -= plane.Normal * (t - size()/2.f);
            return true;
        }
        return false;
    }
    
    bool hitTest(Player *player) const {
        // <editor-fold defaultstate="collapsed" desc="code">
        BoundingSphere sph(position - player->position(), size()/2.f);
        for (int i = 0; i < 3; i++) {
            if (sph.Intersects(player->area[i])) return true;
        }
        return false;
        // </editor-fold>
    }
    
    bool detect_bounce_alPlane() {
        
    }
    
    virtual ~Disc(){}; 
private:
};




class Obstacle {
public:
    int life;
    Obstacle_d *descriptor;
    
    Obstacle(){}
    
    Obstacle(Obstacle_d *descriptor, int life = 10) {
        this->descriptor = descriptor;
        this->life = life;
    };
    
    void hit(int damage) {
        life -= damage;
        if(life < 0) life = 0;
    }
    
    float size() const { return descriptor->size; }
    
    Vector3 position() const { return descriptor->position; }
    
    bool alive() const { return life > 0; }
    
    Obstacle_d::Type type() const { return descriptor->type; }
    
    bool hitTest(Player *player) const {
        // <editor-fold defaultstate="collapsed" desc="code">
        switch (type()) {
            case Obstacle_d::Cube:
            {
                Cube cube(size());
                cube.Move(position() - player->position());
                for (int i = 0; i < 3; i++) {
                    if (cube.Intersects(player->area[i])) return true;
                }
                return false;
            }
            case Obstacle_d::Sphere:
            {
                BoundingSphere sph(position() - player->position(), size()/2.f);
                for (int i = 0; i < 3; i++) {
                    if (sph.Intersects(player->area[i])) return true;
                }
                return false;
            }
        }
        return false;
        // </editor-fold>
    }
    
    bool hitTest(Disc *disc) const {
        switch (type()) {
            case Obstacle_d::Cube: {
                Cube cube(size());
                cube.Move(position() - disc->position);
                
                return cube.Intersects(disc->area());
            }
            case Obstacle_d::Sphere: {
                BoundingSphere sph(position() - disc->position, size()/2.f);
                
                return sph.Intersects(disc->area());
            }
        }
        return false;
    }
    
    bool cube_area(Cube *cube) const {
        if(type() == Obstacle_d::Cube) {
            *cube = Cube(size());
            cube->Move(position());
            return true;
        }
        return false;
    }
    
    bool sphere_area(BoundingSphere *sph) const {
        if(type() == Obstacle_d::Sphere) {
            *sph = BoundingSphere(position(), size() / 2.f);
            return true;
        }
        return false;
    }
    
    virtual ~Obstacle(){ };
private:
};



#endif	/* MATCHACTORS_H */

