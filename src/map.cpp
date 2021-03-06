//
// Created by Robert JONES on 2016/10/21.
//

#include "../includes/map.h"

t_point **ft_malloc_mapdata(int mapx, int mapy) {
    t_point **map_data;
    map_data = (t_point **) malloc(sizeof(t_point *) * mapx);
    for (int i = 0; i < mapx; i++) {
        map_data[i] = (t_point *) malloc(sizeof(t_point) * mapy);
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

double map::dit_to_edge(int x, int y) {
    double dist;
    double tx;
    double ty;

    tx = (x < (mapx - 1) / 2) ? 0 : mapx - 1;
    ty = (y < (mapy - 1) / 2) ? 0 : mapy - 1;

    x = x * scalex;
    tx = tx * scalex;
    y = y * scaley;
    ty = ty * scaley;

    double ab_x = ABS(tx - x);
    double ab_y = ABS(ty - y);

    if (ab_x < ab_y) {
        dist = sqrt(pow((x - tx), 2) + 0);
    }
    else {
        dist = sqrt(0 + pow((y - ty), 2));
    }
    return dist;
}

map::map() {

}

map::map(vector <t_point> points, int mapx, int mapy) {
    int x = 0;
    this->points = points;
    this->mapx = mapx;
    this->mapy = mapy;
    double centerx = 0;
    double centery = 0;

    this->map_data = ft_malloc_mapdata(mapx, mapy);

    int count = 0;
    std::vector<t_point>::iterator iter = points.begin();


    // if there are points passed find the center point
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

        cout << "centerx " << centerx << " centery " << centery << " from " << count << " points" << endl;
    } else {
        // if there are no points passed center is the center of the array;
        centerx = ((this->mapx - 1) / 2);
        centery = ((this->mapy - 1) / 2);
    }

    //find x and y scale factors
    scalex = centerx / ((this->mapx) / 2);
    scaley = centery / ((this->mapy) / 2);

    cout << "scalex " << scalex << " scaley " << scaley << endl;

    // add points to pull the corners to 0
    /*this->points.push_back(t_point{0, 0, 0, 0, 0});
    this->points.push_back(t_point{0, (this->mapy - 1) * scaley, 0, 0, 0});
    this->points.push_back(t_point{(this->mapx - 1) * scalex, 0, 0, 0, 0});
    this->points.push_back(t_point{(this->mapx - 1) * scalex, (this->mapy - 1) * scaley, 0, 0, 0});*/


    //place min and max heights on 0 to make color scale e.g. min = 3 max = 9 moves to min = 0 max = 6
    this->minhight = (this->minhight > 0) ? 0 : this->minhight;
    this->maxhight -= this->minhight;
    cout << "max height: " << this->maxhight << " MIN height: " << this->minhight << endl << endl;

    // loop through map[][] find relertive x by x * scale of x and the same for y
    while (x < this->mapx) {
        int y = 0;
        while (y < this->mapy) {
            this->map_data[x][y].x = x * scalex;
            this->map_data[x][y].y = y * scaley;
            //this->map_data[x][y].water = false;
            this->map_data[x][y].h = 0;
            //IDW interprolation function call
            this->map_data[x][y].z = (x == 0 || y == 0 || x == mapx - 1 || y == mapy - 1) ? 0 :
                                     infer_height(this->map_data[x][y].x, this->map_data[x][y].y, dit_to_edge(x, y));
            y++;
        }
        x++;
    }
}

void map::destroy_map() {
    int x = -1;

    while (++x < this->mapx) {
        delete map_data[x];
    }
    delete map_data;
}

double map::infer_height(int x, int y, double dist) {
    double numer = 0;
    double denom = 0;
    t_point temp;

    // assume the only deminishing feature is distence therfor all points given are considered
    for (vector<t_point>::iterator iter = this->points.begin(); iter != this->points.end(); iter++) {
        temp = *iter;
        temp.dist = sqrt(((x - temp.x) * (x - temp.x)) + ((y - temp.y) * (y - temp.y)));
        // if the point we are trying to find has a distance of 0 from a referance point the referincepoints height is returnd emediatly
        if (temp.dist == 0)
            return (temp.z);
        numer += (temp.z / pow(temp.dist, 2));
        denom += (1 / pow(temp.dist, 2));
    }
    numer += (0 / pow(dist, 2));
    denom += (1 / pow(dist, 2));
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

double map::get_presure(double h) {
    if (h > 0)
        return (round((WATER_LIQUID_DENCITY * h) * 1000.0) / 1000.0);
    else
        return (0);
}

double map::get_height(double presure) {
    return (round((presure / (WATER_LIQUID_DENCITY)) * 1000.0) / 1000.0);
}

void map::toString() {
    int x = -1;
    int y;
    double water = 0;

    cout << endl << "water height" << endl;
    while (++x < this->mapx) {
        y = -1;
        while (++y < this->mapy) {
            cout.width(6);
            cout.precision(4);
            cout << this->map_data[x][y].h << " ";
            water += this->map_data[x][y].h;
        }
        cout << endl;
    }
    cout << "Total water in system: " << water << endl;
    x = -1;
    cout << endl << "terain height" << endl;
    while (++x < this->mapx) {
        y = -1;
        while (++y < this->mapy) {
            cout.width(6);
            cout.precision(4);
            cout << this->map_data[x][y].z << " ";
        }
        cout << endl;
    }

}

uint32_t map::get_color(int x, int y) {
    if (x < this->mapx && y < this->mapy) {
        if (this->map_data[x][y].h > 0) {
            //cout << "has height: "<< this->map_data[x][y].h << "  ";
            return (255);
        } else {
            double scale = (this->map_data[x][y].z - this->minhight) / this->maxhight;
            return ((int) (255 * scale) << 8);
        }
    } else
        return (0);
}

t_point map::get_point(int x, int y) {
    if (x < this->mapx && y < this->mapy) {
        return (this->map_data[x][y]);
    } else
        return (t_point{0, 0, 0, 0, 0});
}

t_point *map::get_next(int x, int y) {
    if (x < this->mapx && x > -1 && y < this->mapy && y > -1)
        return (&this->map_data[x][y]);
    else
        return (NULL);
}

double map::solve(t_point **map, int primx, int primy, int secondx, int secondy, double presur) {
    t_point *temp;
    double presur_in = 0;
    double water_to_move;


    temp = get_next(secondx, secondy);
    if (temp) {
        //return (get_presure(temp->h + (temp->z - map[primx][primy].z)));
        /*if ((temp->h + (temp->z - map[primx][primy].z)) > 0)
            return (temp->h + (temp->z - map[primx][primy].z));
        else
            return (0);*/
        return (temp->h + temp->z);
    }
    return (numeric_limits<double>::max());
}

bool sort_desc(int i, int j) { return (i > j); }

double get_move_volume(double *external_presur, double internal_presure, int end) {
    double total = internal_presure;
    double unit = 1;
    double ave;

    sort(external_presur, external_presur + end, sort_desc);
    for (int i = 0; i < end; i++) {
        total += external_presur[i];
        unit++;

    }
    ave = total / unit;
    int i = 0;
    while (i < end) {
        if (external_presur[i] > ave) {
            total -= external_presur[i];
            unit--;
            ave = total / unit;
        }
        i++;
    }
    return (ave);
}

double split_move_volume(double *external_presur, double internal_presure, int end) {
    double total = internal_presure;
    double unit = 0;
    double ave;

    sort(external_presur, external_presur + end, sort_desc);
    for (int i = 0; i < end; i++) {
        total += external_presur[i];
        unit++;

    }
    ave = total / unit;
    int i = 0;
    while (i < end) {
        if (external_presur[i] > ave) {
            total -= external_presur[i];
            unit--;
            ave = total / unit;
        }
        i++;
    }
    return (ave);
}

double map::get_ave_alt(int x, int y) {
    double ave = 0;
    int count = 0;
    t_point *temp;

    for (int i = -1; i < 2; i++) {
        temp = get_next(x - 1, y + i);
        if (temp) {
            count++;
            ave += temp->z;
        }
        temp = get_next(x + 1, y + i);
        if (temp) {
            count++;
            ave += temp->z;
        }
    }
    temp = get_next(x, y + 1);
    if (temp) {
        count++;
        ave += temp->z;
    }
    temp = get_next(x + 1, y - 1);
    if (temp) {
        count++;
        ave += temp->z;
    }
    return (ave / count);
}

void map::flow() {
    t_point **new_map_data = ft_malloc_mapdata(this->mapx, this->mapy);
    int x = -1;
    int y;
    t_point *temp;
    double presure;
    bool re = true;
    double presures[8];
    double move;
    double deduct;
    int count;

    while (++x < this->mapx) {
        y = -1;
        while (++y < this->mapy) {
            memcpy(&new_map_data[x][y], &this->map_data[x][y], sizeof(t_point));
        }
    }

    x = -1;
    while (++x < this->mapx) {
        y = -1;
        while (++y < this->mapy) {


            deduct = 0;
            move = 0;
            count = 0;
            presure = map_data[x][y].h + map_data[x][y].z;
            //cout<< endl << "checking  " << x << " " << y << " " << presure << endl;
            if (map_data[x][y].h > 0) {
                presures[0] = solve(map_data, x, y, x - 1, y - 1, presure);
                presures[1] = solve(map_data, x, y, x - 1, y, presure);
                presures[2] = solve(map_data, x, y, x - 1, y + 1, presure);

                presures[3] = solve(map_data, x, y, x, y - 1, presure);
                presures[4] = solve(map_data, x, y, x, y + 1, presure);

                presures[5] = solve(map_data, x, y, x + 1, y - 1, presure);
                presures[6] = solve(map_data, x, y, x + 1, y, presure);
                presures[7] = solve(map_data, x, y, x + 1, y + 1, presure);


                /*cout << "next " << x-1 << " " << y-1 << " "<< presures[0] << endl;
                cout << "next " << x-1 << " " << y<< " " << presures[1] << endl;
                cout << "next " << x-1 << " " << y+1<< " " << presures[2] << endl;
                cout << "next " << x << " " << y-1<< " " << presures[3] << endl;
                cout << "next " << x << " " << y+1<< " " << presures[4] << endl;
                cout << "next " << x+1 << " " << y-1 << " "<< presures[5] << endl;
                cout << "next " << x+1 << " " << y<< " " << presures[6] << endl;
                cout << "next " << x+1 << " " << y+1 << " "<< presures[7] << endl;*/

                double external_preures[8];
                int end = 0;
                for (int i = 0; i < 8; i++) {
                    if (presures[i] < presure) {
                        external_preures[end] = presures[i];
                        //cout << "use height " << presures[i] << endl;
                        end++;
                    }
                }
                move = get_move_volume(external_preures, presure, end);
                if (move < map_data[x][y].z) {
                    presure = map_data[x][y].h;
                    move = split_move_volume(external_preures, presure, end);
                }
                //move = round (move * 1000.0) / 1000.0;
                //cout << "av for " << x << " " << y << " is " << move << endl;
                for (int i = 0; i < 8; i++) {
                    double moveheight = 0;
                    if (presures[i] < move) {
                        moveheight = (move - presures[i]);

                        switch (i) {
                            case 0:
                                //              cout << "moving " << moveheight << " to " << x-1 << " " << y-1 << endl;
                                new_map_data[x - 1][y - 1].h += moveheight;
                                deduct += moveheight;
                                break;
                            case 1:
                                //            cout << "moving " << moveheight << " to " << x-1 << " " << y << endl;
                                new_map_data[x - 1][y].h += moveheight;
                                deduct += moveheight;
                                break;
                            case 2:
                                //            cout << "moving " << moveheight << " to " << x-1 << " " << y+1 << endl;
                                new_map_data[x - 1][y + 1].h += moveheight;
                                deduct += moveheight;
                                break;
                            case 3:
                                //            cout << "moving " << moveheight << " to " << x << " " << y-1 << endl;
                                new_map_data[x][y - 1].h += moveheight;
                                deduct += moveheight;
                                break;
                            case 4:
                                //            cout << "moving " << moveheight << " to " << x << " " << y+1 << endl;
                                new_map_data[x][y + 1].h += moveheight;
                                deduct += moveheight;
                                break;
                            case 5:
                                //            cout << "moving " << moveheight << " to " << x+1 << " " << y-1 << endl;
                                new_map_data[x + 1][y - 1].h += moveheight;
                                deduct += moveheight;
                                break;
                            case 6:
                                //            cout << "moving " << moveheight << " to " << x+1 << " " << y << endl;
                                new_map_data[x + 1][y].h += moveheight;
                                deduct += moveheight;
                                break;
                            case 7:
                                //            cout << "moving " << moveheight << " to " << x+1 << " " << y+1 << endl;
                                new_map_data[x + 1][y + 1].h += moveheight;
                                deduct += moveheight;
                                break;
                        }
                    }
                }
                new_map_data[x][y].h -= deduct;
                if (new_map_data[x][y].h < 0)
                    new_map_data[x][y].h = 0;

                /*double water = 0;
                for(int j = 0; j < mapx; j++){
                    for (int k = 0; k < mapy ; ++k) {
                        cout.width(8);
                        cout.precision(4);
                        cout << new_map_data[j][k].h << "   ";
                        water += new_map_data[j][k].h;
                    }
                    cout << endl;
                }
                cout << "total water: " << water << endl;*/
            }
        }
    }
    //delete this->map_data;
    destroy_map();
    this->map_data = new_map_data;
}

void map::flood(double h) {
    int x = -1;
    while (++x < this->mapx) {
        this->map_data[x][0].h = h - this->map_data[x][0].z;
        if (this->map_data[x][0].h < 0)
            this->map_data[x][0].h = 0;

        this->map_data[x][mapy - 1].h = h - this->map_data[x][mapy - 1].z;
        if (this->map_data[x][mapy - 1].h < 0)
            this->map_data[x][mapy - 1].h = 0;
    }
    int y = 0;
    while (++y < this->mapy - 1) {
        this->map_data[0][y].h = h - this->map_data[0][y].z;
        if (this->map_data[0][y].h < 0)
            this->map_data[0][y].h = 0;

        this->map_data[mapx - 1][y].h = h - this->map_data[0][y].z;
        if (this->map_data[mapx - 1][y].h < 0)
            this->map_data[mapx - 1][y].h = 0;
    }
}

void map::rain(int drops) {
    double h = 0;
    int num_drop = rand() % drops + 1;
    int i = -1;

    //this->map_data[mapx / 2][mapy / 2].h += 10;

    while (++i < num_drop) {
        int x = rand() % this->mapx;
        int y = rand() % this->mapy;
        this->map_data[x][y].h += 1;
        //   cout << "rain at point x " << x << " y " << y << endl;
    }
}

void map::wave(double h) {
    for (int x = 0; x < mapx; ++x) {
        this->map_data[x][0].h = h - this->map_data[x][0].z;
        if (this->map_data[x][0].h < 0)
            this->map_data[x][0].h = 0;
    }
}
