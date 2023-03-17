#pragma once
using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::vector;
using std::pair;
using std::fstream;
using std::srand;
using std::time;
using std::rand;
using namespace std::this_thread;
using namespace std::chrono_literals;
using std::chrono::system_clock;

struct person_t;
double random();
void nextFrame();
int read_from();
double check_input();

struct buffer
{
private:
    person_t** person = nullptr;
    int height;
    int witdh;
    //methods
public:
    buffer(const int height, const int width);
    void display();
    void display_legend(int day_of_ep, int counter_healthy, int counter_ill, int counter_rec, int counter_died);
    void infect_p0();
    void infection(const int max_encounter, const double t_rate, vector<pair<int, int>>& coor);
    void recovery_timer();
    void outcome(const int recovery_time, const double f_rate);
    int count(char status);
    ~buffer();
    int write_to(int, int recovery_time, int max_encounter, double t_rate, double f_rate);
};