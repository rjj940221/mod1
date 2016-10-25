//
// Created by Robert JONES on 2016/10/21.
//

#include "../includes/map.h"

t_point    **ft_malloc_mapdata(int mapx, int mapy)
{
    t_point ** map_data;
    map_data = (t_point**)malloc(sizeof(t_point*) * mapx);
    for (int i = 0; i < mapx; i++)
    {
        map_data[i] = (t_point*)malloc(sizeof(t_point) * mapy);
    }
    return (map_data);
}

void move_scale(double *min, double *max) {
    if (*min < 0) {
        *max += *min;
    } else {
        *max -= *min;
    }
    cout << "max height: " << *max << " MIN height: " << *min;
}

void fill_space(double x) {
    double base = 1000;
    do {
        if (!(x /= 10))
            cout << " ";
        base /= 10;
    } while (base);
    cout << " ";
}

map::map() {

}

map::map(vector <t_point> points , int mapx, int mapy) {
    int x = 0;
    this->points = points;
    this->mapx = mapx;
    this->mapy = mapy;
    double centerx = 0;
    double centery = 0;

    this->map_data = ft_malloc_mapdata(mapx, mapy);

    int count = 0;
    std::vector<t_point>::iterator iter = points.begin();

    if (!points.empty()) {
        do {
            this->minhight = (count > 0) ? ((this->minhight < (*iter).z) ? this->minhight : (*iter).z) : (*iter).z;
            this->maxhight = (count > 0) ? ((this->maxhight > (*iter).z) ? this->maxhight : (*iter).z) : (*iter).z;
            cout << "point in constructer : " << (*iter).x << (*iter).y << (*iter).z << endl;
            centerx += (*iter).x;
            centery += (*iter).y;
            count++;
            iter++;
        } while (iter != points.end());


        centerx /= count;
        centery /= count;

        cout << "centerx " << centerx << " centery " << centery  << " from " << count << " points"<< endl;
    }
    else {
        centerx = ((this->mapx - 1) / 2);
        centery = ((this->mapy - 1) / 2);
    }
    scalex = centerx / ((this->mapx) / 2);
    scaley = centery / ((this->mapy) / 2);

    cout << "scalex " << scalex << " scaley " << scaley << endl;

    this->points.push_back(t_point{0, 0, 0, 0, 0});
    this->points.push_back(t_point{0, (this->mapy - 1) * scaley, 0, 0, 0});
    this->points.push_back(t_point{(this->mapx - 1) * scalex, 0, 0, 0, 0});
    this->points.push_back(t_point{(this->mapx - 1) * scalex, (this->mapy - 1) * scaley, 0, 0, 0});

    this->minhight = (this->minhight > 0) ? 0 : this->minhight;

    this->maxhight -= this->minhight;
    cout << "max height: " << this->maxhight << " MIN height: " << this->minhight<< endl << endl;

    while (x < this->mapx) {
        int y = 0;
        while (y < this->mapy) {
            this->map_data[x][y].x = x * scalex;
            this->map_data[x][y].y = y * scaley;
            //this->map_data[x][y].water = false;
            this->map_data[x][y].h = 0;

            this->map_data[x][y].z = infer_height(this->map_data[x][y].x, this->map_data[x][y].y);
            y++;
        }
        x++;
    }
}

double map::infer_height(int x, int y) {
    double numer = 0;
    double denom = 0;
    t_point temp;

    for (vector<t_point>::iterator iter = this->points.begin(); iter != this->points.end(); iter++) {
        temp = *iter;
        temp.dist = sqrt(((x - temp.x) * (x - temp.x)) + ((y - temp.y) * (y - temp.y)));
        if (temp.dist == 0)
            return (temp.z);
        numer += (temp.z / pow(temp.dist, 2));
        denom += (1 / pow(temp.dist, 2));
    }
    return (numer / denom);
}

t_point *map::nerest_points(double x, double y) {
    t_point *re;
    double new_dist;
    t_point temp;

    if (!(re = (t_point *) (malloc(3 * sizeof(t_point)))))
        return (NULL);

    for (int j = 0; j < 3; j++)
        re[j].dist = std::numeric_limits<double>::max();

    for (vector<t_point>::iterator iter = this->points.begin(); iter != this->points.end(); iter++) {
        temp = *iter;

        temp.dist = sqrt(((x - temp.x) * (x - temp.x)) + ((y - temp.y) * (y - temp.y)));
        if (temp.dist < re[0].dist)
            re[0] = temp;
        for (int i = 0; i < 2; i++) {

            if (re[i + 1].dist > re[i].dist) {
                t_point swap = re[i];
                re[i] = re[i + 1];
                re[i + 1] = swap;
            }
        }
    }
    return (re);
}

double map::get_presure(int x, int y) {
    if (x < this->mapx && y < this->mapy) {
        if (this->map_data[x][y].h > 0) {
            return (WATER_LIQUID_DENCITY * GRAVITY * this->map_data[x][y].h);
        } else
            return (0);
    } else
        return (0);
}

void map::toString() {
    int x = -1;
    int y;

    while (++x < this->mapx) {
        y = -1;
        while (++y < this->mapy) {
            cout << this->map_data[x][y].z << " ";
            //fill_space(this->map_data[x][y].z);
        }
        cout << endl;
    }
}

uint32_t map::get_color(int x, int y) {
    if (x < this->mapx && y < this->mapy) {
        double scale = (this->map_data[x][y].z - this->minhight) / this->maxhight;

        return ((int) (255 * scale) << 8);
    } else
        return (0);
}


t_point map::get_point(int x, int y) {
    if (x < this->mapx && y < this->mapy){
        return (this->map_data[x][y]);
    }
    else
        return (t_point{0,0,0,0,0});
}

