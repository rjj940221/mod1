//
// Created by Robert JONES on 2016/10/21.
//

#include "../includes/map.h"

void fill_space(double x) {
    double base =  1000;
    do {
        if (!(x /= 10))
            cout << " ";
        base /= 10;
    } while (base);
    cout << " ";
}

map::map() {

}

map::map(vector<t_point> points) {
    int x = 0;
    this->points = points;
    double centerx = 0;
    double centery = 0;
    double scalex;
    double scaley;
    int count = 0;
    std::vector<t_point>::iterator iter = points.begin();

    do {
        this->minhight = (count > 0 ) ? ((this->minhight < (*iter).z) ? this->minhight : (*iter).z) : (*iter).z;
        this->maxhight = (count > 0 ) ? ((this->maxhight > (*iter).z) ? this->maxhight : (*iter).z) : (*iter).z;
        cout << "point in constructer : " << (*iter).x << (*iter).y << (*iter).z << endl;
        centerx += (*iter).x;
        centery += (*iter).y;
        count++;
        iter++;
    } while (iter != points.end());

    centerx /= count;
    centery /= count;

    cout << "centerx " << centerx << " centery " << centery <<endl;

    scalex =  centerx / (MAPX / 2);
    scaley =  centery / (MAPY / 2);

    cout << "scalex " << scalex << " scaley " << scaley << endl;

    this->points.push_back(t_point{0, 0, 0, 0, false, 0});
    this->points.push_back(t_point{0, (MAPY - 1) * scaley, 0, 0, false, 0});
    this->points.push_back(t_point{(MAPX - 1) * scalex, 0, 0, 0, false, 0});
    this->points.push_back(t_point{(MAPX - 1) * scalex, (MAPY - 1) * scaley, 0, 0, false, 0});

    while (x < MAPX) {
        int y = 0;
        while (y < MAPY) {
            this->map_data[x][y].x = x * scalex;
            this->map_data[x][y].y = y * scaley;
            this->map_data[x][y].water = false;
            this->map_data[x][y].h = 0;

            this->map_data[x][y].z = infer_height(x * scalex, y * scaley);
          //  cout << "new map point: x" << this->map_data[x][y].x << " y " << this->map_data[x][y].y << " z " << this->map_data[x][y].z << endl;
            y++;
        }
        x++;
    }
}

double map::infer_height(int x, int y) {
    t_point *close = nerest_points(x, y);

    return (((close[0].z / close[0].dist) + (close[1].z / close[1].dist) + (close[2].z / close[2].dist)) /
            ((1 / close[0].dist) + (1 / close[1].dist) + (1 / close[2].dist)));
}

t_point *map::nerest_points(double x, double y) {
    t_point *re;
    double new_dist;
    t_point temp;

    if (!(re = (t_point *)(malloc(3 * sizeof(t_point)))))
        return (NULL);

    for (int i  = 0; i < 3; i++)
        re[i].dist = std::numeric_limits<double>::max();

    for (std::vector<t_point>::size_type i = 0; i != this->points.size(); i++) {
        /* std::cout << someVector[i]; ... */
        temp = this->points.at(i);
        temp.dist = -1;
        new_dist = sqrt(((x - temp.x) * (x - temp.x)) + ((y - temp.y) * (y - temp.y)));
        for (int i = 0; i < 3; i++) {
            if (new_dist <= re[i].dist) {
                temp.dist = new_dist;
                re[i] = temp;
                break;
            }
        }
    }
    return (re);
}

double map::get_presure(int x, int y) {
    if (x < MAPX && y < MAPY) {
        if (this->map_data[x][y].water) {
            return (WATER_LIQUID_DENCITY * GRAVITY * this->map_data[x][y].h);
        } else
            return (0);
    } else
        return (0);
}

void map::toString() {
    int x = -1;
    int y;

    while (++x < MAPX) {
        y = -1;
        while (++y < MAPY) {
            cout << this->map_data[x][y].z << " ";
            //fill_space(this->map_data[x][y].z);
        }
        cout << endl;
    }
}

