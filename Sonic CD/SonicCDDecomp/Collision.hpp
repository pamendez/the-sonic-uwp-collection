#ifndef COLLISION_H
#define COLLISION_H

enum CollisionSidess {
    CSIDE_FLOOR = 0,
    CSIDE_LWALL = 1,
    CSIDE_RWALL = 2,
    CSIDE_ROOF  = 3,
};

enum CollisionModes {
    CMODE_FLOOR = 0,
    CMODE_LWALL = 1,
    CMODE_ROOF  = 2,
    CMODE_RWALL = 3,
};

enum CollisionSolidity {
    SOLID_ALL  = 0,
    SOLID_TOP  = 1,
    SOLID_LRB  = 2,
    SOLID_NONE = 3,
};

enum ObjectCollisionTypes {
    C_TOUCH       = 0,
    C_BOX         = 1,
    C_BOX2        = 2,
    C_PLATFORM    = 3,
};

struct CollisionSensor
{
    int XPos;
    int YPos;
    int angle;
    bool collided;
};

extern int collisionLeft;
extern int collisionTop;
extern int collisionRight;
extern int collisionBottom;

extern CollisionSensor sensors[6];

void FindFloorPosition(Player *player, CollisionSensor *sensor, int startYPos);
void FindLWallPosition(Player *player, CollisionSensor *sensor, int startXPos);
void FindRoofPosition(Player *player, CollisionSensor *sensor, int startYPos);
void FindRWallPosition(Player *player, CollisionSensor *sensor, int startXPos);
void FloorCollision(Player *player, CollisionSensor *sensor);
void LWallCollision(Player *player, CollisionSensor *sensor);
void RoofCollision(Player *player, CollisionSensor *sensor);
void RWallCollision(Player *player, CollisionSensor *sensor);
void SetPathGripSensors(Player *player);

void ProcessPathGrip(Player *player);
void ProcessAirCollision(Player *player);

void ProcessPlayerTileCollisions(Player *player);

void TouchCollision(int left, int top, int right, int bottom);
void BoxCollision(int left, int top, int right, int bottom);  // Standard
void BoxCollision2(int left, int top, int right, int bottom); // Updated (?)
void PlatformCollision(int left, int top, int right, int bottom);

void ObjectFloorCollision(int xOffset, int yOffset, int cPath);
void ObjectLWallCollision(int xOffset, int yOffset, int cPath);
void ObjectRoofCollision(int xOffset, int yOffset, int cPath);
void ObjectRWallCollision(int xOffset, int yOffset, int cPath);

void ObjectFloorGrip(int xOffset, int yOffset, int cPath);
void ObjectLWallGrip(int xOffset, int yOffset, int cPath);
void ObjectRoofGrip(int xOffset, int yOffset, int cPath);
void ObjectRWallGrip(int xOffset, int yOffset, int cPath);

#endif // !COLLISION_H
