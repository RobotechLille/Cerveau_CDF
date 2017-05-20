#ifndef _CHEF
#define _CHEF

 typedef struct coor{
    float x;
    float y;
}coor;

enum etatZone {LIBRE,INTERDITE,DANGER};

void init_Terrain();
#endif
