

#ifndef MOD1_MAP_H_H
#define MOD1_MAP_H_H
# define GRAVITY 9.8
# define WATER_LIQUID_DENCITY 1000
# include <vector>
# include <iostream>
# include <math.h>

using namespace std;

typedef struct s_point
{
    double  x;
    double  y;
    double  z;
    double  h;
    double  dist;
}               t_point;

class map{
public:
    map();
    map(vector <t_point> oints, int mapx, int mapy);
    double infer_height(int x, int y);
    t_point *nerest_points(double x, double y);
    double get_presure(double h);
    void toString();
    uint32_t get_color(int x, int y);
    t_point get_point(int x, int y);
    double  get_scalex(){ return this->scalex;};
    double  get_scaley(){ return this->scaley;};
    void    flow();
    void    wave(double h);
    void    flood(double h);
    void    rain(int max_drops);
    t_point *get_next(int x, int y);
    double        solve(t_point **map, int primx, int primy, int secondx, int secondy, double presur);

private:
    double      scalex;
    double      scaley;
    double      maxhight;
    double      minhight;
    int         mapx;
    int         mapy;
    t_point     **map_data;
    vector<t_point>     points;
    void        destroy_map();
    //void        solve(t_point **map, int primx, int primy, int secondx, int secondy, double presur);

};

#endif //MOD1_MAP_H_H
