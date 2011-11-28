#include "../Match.h"
#include "../../GNAFramework/MathHelper.h"
#include "../../GNAFramework/GNAExeptions.h"
#include "../../DataProvider.h"
#include "../Plane.h"
#include "../Ray.h"

Match::Match(Game *game) {
    this->game = game;
    
    DataManager::loadLevels("Content/config2.xml");
    
    playerStats.level = 0;
    
    playerStats.cam.angL  = 0.75f;
    playerStats.cam.pos   = Vector3(-5.f, 40.f, -5.f);
    playerStats.cam.angV  = -0.6f;
    
    match_state = toLoad;
}

void Match::Update(GameTime gameTime) {
    float time = 2.f * gameTime.elapsedTime.toSecondsF();
    im.Update();

    switch (match_state) {
        case toLoad: return;
        case Loading:
        {
            // <editor-fold defaultstate="collapsed" desc="Loading Proccess">
            if (playerStats.level >= DataManager::levels.length)
                throw new InvalidOperationException("Se trato de cargar un nivel que no existe");

            int l = level()->obstacles.length;
            obstacles = new Obstacle[l];
            for (int i = 0; i < l; i++) {
                obstacles[i] = Obstacle(&level()->obstacles.list[i]);
            }

            l = level()->enemys.length;
            players = new Player[l + 1];
            players[0] = Player(&level()->user);
            for (int i = 1; i <= l; i++) {
                players[i] = Player(&level()->enemys.list[i-1]);
            }
            
            teams = new bool[l + 1];
            
            scenario[0] = Plane(Vector3::Zero                    , Vector3::Up   );
            scenario[1] = Plane(Vector3(0.f,level()->height,0.f) , Vector3::Down );
            scenario[2] = Plane(Vector3(level()->width,0.f,0.f)  , Vector3::Left );
            scenario[3] = Plane(Vector3::Zero                    , Vector3::Right);
            scenario[4] = Plane(Vector3::Zero                    , Vector3::Backward);
            scenario[5] = Plane(Vector3(0.f,0.f,level()->deep)   , Vector3::Forward );
            
            
            // </editor-fold>
            match_state = Runing;
            pause = true;
            break;
        }
        case Runing:
            if (im.KeyGetPressed(Keyboard::P)) { 
                pause = !pause;
            }
            if(!pause) {
                int l;

                // <editor-fold defaultstate="collapsed" desc="Camera Control">
                MouseState ms = Mouse::getState();

                ViewPort vp = game->graphicDevice->getViewPort();
                Vector2 cen = Vector2(vp.width / 2, vp.height / 2);
                Vector2 mov = (Vector2(ms.X(), ms.Y()) - cen) / 20.f;

                if(mov.LengthSquared() > 100.f) mov = Vector2::Zero;

                Mouse::setPosition(cen);

                float camAngH = playerStats.cam.angL;
                float camAngV = playerStats.cam.angV;
                Vector3 cam_d(cos(camAngH) * cos(camAngV), sin(camAngV), sin(camAngH) * cos(camAngV));
                Vector3 cam_o = Vector3(-cam_d.Z, 0, cam_d.X);

                if (im.KeyPressed(Keyboard::Up)) {
                    playerStats.cam.pos = playerStats.cam.pos + cam_d * 30.f * time;
                } else if (im.KeyPressed(Keyboard::Down)) {
                    playerStats.cam.pos = playerStats.cam.pos - cam_d * 30.f * time;
                }

                if (im.KeyPressed(Keyboard::Right)) {
                    playerStats.cam.pos = playerStats.cam.pos + cam_o * 30.f * time;
                } else if (im.KeyPressed(Keyboard::Left)) {
                    playerStats.cam.pos = playerStats.cam.pos - cam_o * 30.f * time;
                }

                playerStats.cam.angL = fmod(playerStats.cam.angL + mov.X * time, TwoPi);
                playerStats.cam.angV = MathHelper::Truncate(playerStats.cam.angV - mov.Y * time, -PiOver2*0.99f, PiOver2*0.99f);

                float cam_angXZ = playerStats.cam.angL;
                float cam_angY = playerStats.cam.angV;
                Vector3 dir(cos(cam_angXZ) * cos(cam_angY), sin(cam_angY), sin(cam_angXZ) * cos(cam_angY));

                playerStats.cam.ray = Ray(playerStats.cam.pos, dir);

                // </editor-fold>

                // <editor-fold defaultstate="collapsed" desc="Bikes Update">
                l = level()->enemys.length;
                for (int i = 0; i <= l; i++) {
                    if(players[i].alive()) {
                        teams[i] = true;

                        players[i].update(time);
                        players[i].ia_update(time, players, level()->enemys.length + 1);

                        Vector3 pos, vel;
                        if(players[i].get_shoot_request(&pos, &vel)) {
                            for (int j = 0; j < MAX_DISCS; j++) {
                                if(!discs[j].active()) {
                                    discs[j].activate(pos, vel, i);
                                    //printf("Disco %d generado por %d\n", j, i); fflush(stdout);
                                    break;
                                } 
                            }
                        }

                        int l2 = level()->obstacles.length;
                        for(int j = 0; j < l2; j++) {
                            if(obstacles[j].alive() && obstacles[j].hitTest(&players[i])) {
                                //printf("Obstacle %d been hited by %d\n", j, i);
                                obstacles[j].hit(1000);
                            }
                        }
                    }
                }
                if(teams[0]) {
                    int teams_alive = 0, last_alive = -1;
                    for(int i = 1; i <= l; i++) {
                        if(teams[i]) {
                            teams_alive++;
                            last_alive = i;
                        }
                    }
                    if(teams_alive < 1) winer(0);
                } else {
                     winer(-1);
                }

                // </editor-fold>

                // <editor-fold defaultstate="collapsed" desc="Discs Update">
                for (int i = 0; i <= MAX_DISCS; i++) {
                    if(discs[i].active()) {
                        discs[i].update(time);

                        //Walls
                        for(int j = 0; j < 6; j++) {
                            if(discs[i].bounce(scenario[j])) {
                                //printf("\n%d Hit against wall %d\n", i, j); 
                                discs[i].hitPoints--;
                            }
                        }

                        //Players
                        l = level()->enemys.length;
                        for(int j = 0; j <= l; j++) {
                            if(j != discs[i].player_index && 
                                    players[j].alive() && discs[i].hitTest(&players[j])) {
                                //printf("\n%d Hit player %d!!!\n", i, j); 
                                discs[i].hitPoints = -1;
                                players[j].hit(1);
                            }
                        } 

                        //Obstacles
                        int l = level()->obstacles.length;
                        for(int j = 0; j < l; j++) { 
                            if(obstacles[j].alive() && obstacles[j].hitTest(&discs[i])) {
                                //printf("\n%d Hit against %d\n", i, j);  
                                obstacles[j].hit(1);
                                discs[i].hitPoints--;

                                Cube cube;
                                Sphere sphere;
                                if(obstacles[j].cube_area(&cube)) {
                                    discs[i].bounce(cube);
                                } else if(obstacles[j].sphere_area(&sphere)){
                                    discs[i].bounce(sphere); 
                                }
                            }
                        }
                    }
                }
                // </editor-fold>
            }
                break;
            default:
            break;
    } 
    
}

void Match::Draw(GameTime gameTime) {
    switch (match_state) {
        case toLoad: 
            GraphicManager::get()->DrawMatchLoading();
            
            match_state = Loading;
            break;
        case Loading:
            GraphicManager::get()->DrawMatchLoading();
            break;
        case Runing:
            GraphicManager::get()->DrawMatch(this);
            break;
        case Victory:
            GraphicManager::get()->DrawMatch(this);
            GraphicManager::get()->DrawMatchVictory();
            break;
        case Defeat:
            GraphicManager::get()->DrawMatch(this);
            GraphicManager::get()->DrawMatchDefeat();
            break;
        default:
            break;
    }
}

Level_d *Match::level(){
    return &DataManager::levels.list[playerStats.level];
}

void Match::winer(int i) {
    switch(i) {
        case 0:
            match_state = Victory;
            break;
        default:
            match_state = Defeat;
            break;  
    }
}

Match::~Match() {
}
