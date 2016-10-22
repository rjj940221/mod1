//
// Created by Robert JONES on 2016/10/21.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>
#include <limits>

#ifndef MOD1_MAP_H
#define MOD1_MAP_H
#define GRAVITY 9.89
#define WATER_LIQUID_DENCITY 999.9
#define MAPX 100
#define MAPY 100


using namespace std;

typedef struct  s_point
{
    double x;
    double y;
    double z;
    double h;
    bool   water;
    double dist;
}               t_point;

class map {
private:
    double          minhight;
    double          maxhight;
    vector<t_point> points;
    t_point         map_data[MAPX][MAPY];
    double  infer_height(int x, int y);



public:
    map(vector<t_point> points);// constructor
    map();

    double  get_presure(int x, int y);
    t_point *nerest_points(double x, double y);
    int     getMapX(){return MAPX;};
    int     getMapY(){return MAPY;};
    void    toString();
};


#endif
