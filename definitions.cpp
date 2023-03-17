#define NOMINMAX

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <windows.h>
#include <fstream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <math.h>

#include "Project_0.h"

struct person_t
{
    int days;
    char symbol;
};

double random()
{
    return 1 + (double)(rand()) / ((double)(RAND_MAX / 99));
}

void nextFrame()
{
    COORD cursorPosition;
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cursorPosition.X = 0;
    cursorPosition.Y = 0;
    SetConsoleCursorPosition(hOut, cursorPosition);
}

void print_arr(int* arr, int size)
{
    for (int i = 0; i < size; ++i)
    {
        cout << arr[i];
    }
    cout << endl;
}

int* data_to_legend(int counter, int size)
{
    int* arr = new int[size];
    int rmndr;
    int dvdr = (int)pow(10, size-1);
    for (int i = 0; i < size; i++)
    {
        arr[i] = counter / dvdr;
        rmndr = counter % dvdr;
        counter = rmndr;
        dvdr = dvdr / 10;
    }
    return arr;
}

int read_from()
{
    system("cls");
    fstream file;
    file.open("simulation.txt", std::ios::in);
    if (!file)
    {
        std::cout << "No file with simulation found" << std::endl;
        return 1;
    }
    string line;
    FRAME:
    while (getline(file, line))
    {
        if (line[0] == 'e')
        {
            sleep_for(300ms);
            nextFrame();
            goto FRAME;
        }
        cout << line << endl;
    }
    file.close();
    return 0;
}

double check_input()
{
    double prmtr;
    while (true)
    {
        if (cin >> prmtr)
        {
            return prmtr;
        }
        else
        {
            cout << "Please enter a valid input" << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

buffer::buffer(const int height, const int width)
{
    this->height = height;
    this->witdh = width;

    this->person = new person_t * [height];
    person_t* pCell = new person_t[height * witdh];

    for (int r = 0; r < height; ++r)
    {
        person[r] = pCell + r * witdh;
    }

    for (int r = 0; r < height; ++r)
    {
        for (int c = 0; c < witdh; ++c)
        {
            person[r][c] = { 0, 'H' };
        }
    }
}

void buffer::display()
{
    sleep_for(300ms);
    nextFrame();
    cout << "Population: " << height * witdh << endl;
    for (int r = 0; r < height; ++r)
    {
        for (int c = 0; c < witdh; ++c)
        {
            cout << person[r][c].symbol;
        }
        cout << endl;
    }
    cout << endl;
}

void buffer::display_legend(int day_of_ep, int counter_healthy, int counter_ill, int counter_rec, int counter_died)
{
    int* healthy = data_to_legend(counter_healthy, 4);
    int* ill = data_to_legend(counter_ill, 4);
    int* rec = data_to_legend(counter_rec, 4);
    int* died = data_to_legend(counter_died, 4);

    cout << "Day: " << day_of_ep << endl;
    cout << "Number of healthy people: ";
    print_arr(healthy, 4);
    cout << "Number of infected:       ";
    print_arr(ill, 4);
    cout << "Number of recupirated:    ";
    print_arr(rec, 4);
    cout << "Number of deaths:         ";
    print_arr(died, 4);

    delete[] healthy;
    delete[] ill;
    delete[] rec;
    delete[] died;
}

void buffer::infect_p0()
{
    person[height / 2][witdh / 2].symbol = 'I';
}

void buffer::infection(const int max_encounter, const double t_rate, vector<pair<int, int>>& coord)
{
    int counter_enc = 0;
    for (int r = 1; r < height - 1; ++r)
    {
        for (int c = 1; c < witdh - 1; ++c)
        {
            counter_enc = 0;
            if (person[r][c].symbol == 'I')
            {
                while (counter_enc < max_encounter)
                {
                    double rand = random();
                    if (1 <= rand && rand <= 12.5 && counter_enc <= max_encounter)
                    {
                        coord.push_back({ r - 1,c });
                        counter_enc += 1;
                    }
                    if (12.5 < rand && rand <= 25 && counter_enc <= max_encounter)
                    {
                        coord.push_back({ r - 1,c + 1 });
                        counter_enc += 1;
                    }
                    if (25 < rand && rand <= 37.5 && counter_enc <= max_encounter)
                    {
                        coord.push_back({ r,c + 1 });
                        counter_enc += 1;
                    }
                    if (37.5 < rand && rand <= 50 && counter_enc <= max_encounter)
                    {
                        coord.push_back({ r + 1,c + 1 });
                        counter_enc += 1;
                    }
                    if (50 < rand && rand <= 62.5 && counter_enc <= max_encounter)
                    {
                        coord.push_back({ r + 1,c });
                        counter_enc += 1;
                    }
                    if (62.5 < rand && rand <= 75 && counter_enc <= max_encounter)
                    {
                        coord.push_back({ r + 1,c - 1 });
                        counter_enc += 1;
                    }
                    if (75 < rand && rand <= 87.5 && counter_enc <= max_encounter)
                    {
                        coord.push_back({ r,c - 1 });
                        counter_enc += 1;
                    }
                    if (87.5 < rand && rand <= 100 && counter_enc <= max_encounter)
                    {
                        coord.push_back({ r - 1,c - 1 });
                        counter_enc += 1;
                    }
                }
            }
        }
    }
    while (coord.size() != 0)
    {
        bool Infect = (random() <= t_rate);
        if (Infect && person[coord.back().first][coord.back().second].symbol == 'H')
        {
            person[coord.back().first][coord.back().second].symbol = 'I';
        }
        coord.pop_back();
    }
}

int buffer::count(char status)
{
    int counter = 0;
    for (int r = 0; r < height; ++r)
    {
        for (int c = 0; c < witdh; ++c)
        {
            if (person[r][c].symbol == status)
            {
                counter += 1;
            }
        }
    }
    return counter;
}

void buffer::recovery_timer()
{
    for (int r = 0; r < height; ++r)
    {
        for (int c = 0; c < witdh; ++c)
        {
            if (person[r][c].symbol == 'I')
            {
                person[r][c].days += 1;
            }
        }
    }
}

void buffer::outcome(const int recovery_time, const double f_rate)
{
    for (int r = 0; r < height; ++r)
    {
        for (int c = 0; c < witdh; ++c)
        {
            if (person[r][c].days == recovery_time && person[r][c].symbol == 'I')
            {
                if (random() <= f_rate)
                {
                    person[r][c].symbol = 'D';
                }
                else
                {
                    person[r][c].symbol = 'R';
                }
            }
        }
    }
}

int buffer::write_to(int save, int recovery_time, int max_encounter, double t_rate, double f_rate)
{
    bool SAVE = save;
    if (SAVE)
    {
        fstream file;
        file.open("simulation.txt", std::ios::out);
        if (!file.good())
        {
            std::cout << "Error" << std::endl;
            return 1;
        }
        file << "Last saved simulation:" << endl;
        file << "Population: " << height * witdh << endl;
        file << "Incubation period:    " << recovery_time << endl << "Number of encounters: " << max_encounter << endl;
        file << "Transmission rate:    " << t_rate << endl << "Fatality rate:        " << f_rate << endl << endl;
        for (int r = 0; r < height; ++r)
        {
            for (int c = 0; c < witdh; ++c)
            {
                file << person[r][c].symbol;
            }
            file << endl;
        }
        file << "end of the frame" << endl;
        file.close();
        return 1;
    }
        fstream file;
        file.open("simulation.txt", std::ios::app);
        if (!file.good())
        {
            std::cout << "Error" << std::endl;
            return 1;
        }
        file << "Last saved simulation:" << endl;
        file << "Population: " << height * witdh << endl;
        file << "Incubation period:    " << recovery_time << endl << "Number of encounters: " << max_encounter << endl;
        file << "Transmission rate:    " << t_rate << endl << "Fatality rate:        " << f_rate << endl << endl;
        for (int r = 0; r < height; ++r)
        {
            for (int c = 0; c < witdh; ++c)
            {
                file << person[r][c].symbol;
            }
            file << endl;
        }
        file << "end of the frame" << endl;
        file.close();
    return 1;
}

buffer::~buffer()
{
    delete[] this->person[0];
    delete[] this->person;
}