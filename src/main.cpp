#include "../includes/mod1.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int MAPX = 400;
const int MAPY = 400;

void split(const string &s, char delim, vector <string> &elems) {
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

vector <string> split(const string &s, char delim) {
    vector <string> elems;
    split(s, delim, elems);
    return elems;
}

string repace(string str, char replace) {
    string::iterator end_pos = remove(str.begin(), str.end(), replace);
    str.erase(end_pos, str.end());
    return str;
}

t_point make_point(string point) {
    t_point re;
    int count = 0;
    bool set_num = false;

    point = repace(point, ' ');
    point = repace(point, '(');
    point = repace(point, ')');
    vector <string> data = split(point, ',');

    for (vector<string>::iterator iter = data.begin(); iter != data.end(); iter++) {
        string::size_type sz;
        switch (count) {
            case 0:
                re.x = stod(*iter, &sz);
                break;
            case 1:
                re.y = stod(*iter, &sz);
                break;
            case 2:
                re.z = stod(*iter, &sz);
                break;
        }
        count++;
    }
    cout << "point: " << re.x << " " << re.y << " " << re.z << endl;
    return re;
}

void read_file(char *file_name, map *file_map) {

    puts("called readfile");
    ifstream infile;
    string line;
    vector <t_point> points;

    infile.open(file_name);
    if (infile.is_open()) {
        puts("opend file");
        while (getline(infile, line)) {
            try {
                regex re("\\(\\s*(\\d{1,}\\s*,\\s*\\d{1,}\\s*,\\s*\\d{1,})\\s*\\)");
                auto begin =
                        sregex_iterator(line.begin(), line.end(), re);
                auto end = sregex_iterator();
                for (sregex_iterator i = begin; i != end; ++i) {
                    smatch match = *i;
                    string point = match.str();
                    points.push_back(make_point(point));
                    puts("pushed point");
                }
            } catch (regex_error &e) {
                // Syntax error in the regular expression
            }
        }
        infile.close();

        *file_map = map(points, MAPX, MAPY);
    }
}

void iso_map(uint32_t *px, map *filemap) {
    double x;
    double y;
    int ix = -1;
    int iy;

    while (++ix < MAPX) {
        iy = -1;
        while (++iy < MAPY) {
            t_point temp = filemap->get_point(ix, iy);
            /*double rand;
            rand = temp.z;
            temp.z = temp.y;
            temp.y = rand;*/
            //t_point temp = (t_point){(double)ix, (double )iy,0,0,false,0};
            x = (1 / sqrt(6)) * ((temp.x * sqrt(3)) + (-(sqrt(3) * temp.z)));
            y = (1 / sqrt(6)) * (temp.x + (2 * temp.y) + temp.z);

            x /= filemap->get_scalex();
            y /= filemap->get_scaley();


            px[((int) (x) * SCREEN_WIDTH) + (int) (y)] = filemap->get_color(ix, iy);
        }
    }

}

void color_map(map *filemap, uint32_t *pixels) {
    uint32_t *px;

    int x = -1;
    while (++x < MAPX) {
        int y = -1;
        px = &pixels[((x + 4) * SCREEN_WIDTH) + 8];
        while (++y < MAPY) {
            *px = filemap->get_color(x, y);
            px++;
        }
    }
}

void SDL_loop(map filemap, t_sdl sdl, int type) {
    bool quit = false;
    SDL_Event event;
    int h = 0;
    uint32_t *pixels = (uint32_t *) malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint32_t));

    memset(pixels, 155, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint32_t));
    color_map(&filemap, pixels);
    while (!quit) {
        SDL_UpdateTexture(sdl.texture, NULL, pixels, SCREEN_WIDTH * sizeof(uint32_t));

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
            }
        }
        SDL_RenderClear(sdl.render);
        SDL_RenderCopy(sdl.render, sdl.texture, NULL, NULL);
        SDL_RenderPresent(sdl.render);

        switch (type)
        {
            case 1:
                if (h < 5000)
                    filemap.flood(h += 10);
                break;
            case 2:
                filemap.wave(5000);
                break;
            case 3:
                filemap.rain(ceil(50));
                break;
        }

        filemap.flow();

        color_map(&filemap, pixels);
    }
    delete[] pixels;
}

int main(int ac, char **av) {
    map filemap;
    int type = 0;
    char *file = NULL;

    for (int i = 1; i < ac; ++i) {
        if (type == 0 && strcmp(av[i], "-f") == 0) {
            type = 1;
            cout << "type flood: " << type << " com " << strcmp(av[i], "-f")<<endl;
        }
        else if (type == 0 && strcmp(av[i], "-w") == 0) {
            type = 2;
            cout << "type wave: " << type << endl;
        }
        else if (type == 0 && strcmp(av[i], "-r") == 0) {
            type = 3;
            cout << "type rain: " << type << endl;
        }
        else if (file == NULL)
            file = av[i];
        else
            cout << "argument not recognised use ./mod1 [-r | -f | -w] 'file name'" << endl;
    }

    if (type == 0) {
        cout << "using flooding pattern" << endl;
        type = 1;
    }
    cout << "type: " << type << endl;
    if (file && (regex_match(file, regex(".*([.]mod1)$")))) {
        read_file(file, &filemap);
        t_sdl sdl;

        SDL_Init(SDL_INIT_VIDEO);
        sdl.window = SDL_CreateWindow("mod1",
                                      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

        sdl.render = SDL_CreateRenderer(sdl.window, -1, 0);
        sdl.texture = SDL_CreateTexture(sdl.render,
                                        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH,
                                        SCREEN_HEIGHT);
        SDL_loop(filemap, sdl, type);
        SDL_DestroyTexture(sdl.texture);
        SDL_DestroyRenderer(sdl.render);

        SDL_DestroyWindow(sdl.window);
        SDL_Quit();
    } else
        cout << "the file you passed is not recognised" << endl;
    return 0;
}