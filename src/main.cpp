#include "../includes/mod1.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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

        *file_map = map(points, 300, 300);
    }
}

void iso_map(uint32_t *px, map *filemap) {
    double x;
    double y;
    int ix = -1;
    int iy;

    while (++ix < 300) {
        iy = -1;
        while (++iy < 300) {
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


int main(int ac, char **av) {
    map filemap;
    bool quit = false;
    uint32_t *px;
    SDL_Event event;


    if (ac == 2 && regex_match(av[1], regex(".*([.]mod1)$"))) {
        cout << "arg passed test" << endl;
        read_file(av[1], &filemap);
        //  filemap.toString();


        t_sdl sdl;
        SDL_Init(SDL_INIT_VIDEO);

        sdl.window = SDL_CreateWindow("SDL2 Pixel Drawing",
                                      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

        sdl.render = SDL_CreateRenderer(sdl.window, -1, 0);
        sdl.texture = SDL_CreateTexture(sdl.render,
                                        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH,
                                        SCREEN_HEIGHT);
        uint32_t *pixels = (uint32_t *) malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint32_t));

        memset(pixels, 155, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint32_t));

        filemap.rain(500);
        int x = -1;
        while (++x < 300) {
            int y = -1;
            px = &pixels[x * SCREEN_WIDTH];
            while (++y < 300) {
                *px = filemap.get_color(x, y);
                px++;
            }
        }

        // iso_map(pixels, &filemap);


        while (!quit) {
            int h = 10;
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
            filemap.rain(300);
            int x = -1;
            while (++x < 300) {
                int y = -1;
                px = &pixels[x * SCREEN_WIDTH];
                while (++y < 300) {
                    *px = filemap.get_color(x, y);
                    px++;
                }
            }
        }

        delete[] pixels;
        SDL_DestroyTexture(sdl.texture);
        SDL_DestroyRenderer(sdl.render);

        SDL_DestroyWindow(sdl.window);
        SDL_Quit();
    } else
        cout << "the file you passed is not recognised" <<
             endl;
    return 0;
}