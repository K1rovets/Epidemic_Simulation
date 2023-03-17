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
#include <conio.h>
#include "Project_0.h"

#define QUIT 113

int main()
{
    
    bool SIMULATION;

    string command; //store commands;
    int prmtrI;      //check int parameters;
    double prmtrD;  // check double parameters
    int keypad;

    cout << "Press any button to start or 'Q' to quit the simulation";
    keypad = _getch();
    cout << endl;
    if (keypad == QUIT)
        return 0;
    else 
    {
        do
        {
            system("cls");
            //Counters:
            int counter_ill = 0;
            int counter_rec = 0;
            int counter_died = 0;
            int counter_healthy = 0;
            int day_of_ep = 0;
            bool EPIDEMIC = true;

            vector<pair<int, int>> coordinates; // list of coordinates;

            //Part 0: Use default values
            fstream file;
            file.open("default.txt", std::ios::in);
            if (!file)
            {
                std::cout << "No file with default parameters found" << std::endl;
                return 1;
            }
            double arrPar[7]{};
            int element = 0;
            while (!file.eof())
            {
                file >> arrPar[element];
                element += 1;
            }
            file.close();

            //Parameters:
            int height = (int)arrPar[0];        //default 40
            int witdh = (int)arrPar[1];         //default 40
            int recovery_time = (int)arrPar[2]; //default 7
            int max_encounter = (int)arrPar[3]; //default 4
            double t_rate = arrPar[4];          //default 75%
            double f_rate = arrPar[5];          //default 3%
            bool SAVE_TO = arrPar[6];           //true

            //Part 1: "Ask parameters"
            cout << "Choose parameters for the epidemic simulation within the provided boundaries." << endl << "In case of out of bounds input default values will be used" << endl;
            cout << "Choose population's size: small(625), medium(1600), large(3025) --> ";
            cin >> command;
            if (command == "large" || command == "Large")
            {
                height = 55;
                witdh = 55;
            }
            else if (command == "medium" || command == "Medium")
            {
                height = 40;
                witdh = 40;
            }
            else if (command == "small" || command == "Small")
            {
                height = 25;
                witdh = 25;
            }
            //Recovery time
            cout << "Enter incubation period (from 1 to 14) --> ";
            prmtrI = check_input();
            if (prmtrI <= 14 && 1 <= prmtrI)
                recovery_time = prmtrI;
            //Max number of encounter per day
            cout << "Enter maximum number of encounters per day (from 1 to 8) --> ";
            prmtrI = check_input();
            if (prmtrI <= 8 && 1 <= prmtrI)
                max_encounter = prmtrI;
            //Transmission rate
            cout << "Enter transmission rate of a disease (from 1.0% to 100.0%) --> ";
            prmtrD = check_input();
            if (prmtrD <= 100 && 1 <= prmtrD)
                t_rate = prmtrD;
            //Fatality rate
            cout << "Enter fatality rate of a disease (from 1.0% to 100.0%) --> ";
            prmtrD = check_input();
            if (prmtrD <= 100 && 1 <= prmtrD)
                f_rate = prmtrD;
            //Saving simulation into the simulation.txt file
            cout << "Do you want to save simulation in txt file? Y/N --> ";
            cin >> command;
            if (command == "y" || command == "Y")
            {
                SAVE_TO = true;
            }
            else if (command == "n" || command == "N")
            {
                SAVE_TO = false;
            }

            //Part 2: Create population
            system("cls");
            srand(time(nullptr));

            buffer population(height, witdh);
            population.display();
            if (SAVE_TO)
            {
                population.write_to(1, recovery_time, max_encounter, t_rate, f_rate);
            }
            population.display_legend(day_of_ep, counter_healthy, counter_ill, counter_rec, counter_died);

            //infect patient zero
            population.infect_p0();
            population.display();
            if (SAVE_TO)
            {
                population.write_to(0, recovery_time, max_encounter, t_rate, f_rate);
            }
            day_of_ep += 1;
            counter_healthy = population.count('H');
            counter_ill = population.count('I');
            counter_rec = population.count('R');
            counter_died = population.count('D');
            population.display_legend(day_of_ep, counter_healthy, counter_ill, counter_rec, counter_died);

            //Part 3: Update population
            while (EPIDEMIC)
            {
                population.infection(max_encounter, t_rate, coordinates);
                population.recovery_timer();
                population.outcome(recovery_time, f_rate);
                population.display();
                if (SAVE_TO)
                {
                    population.write_to(0, recovery_time, max_encounter, t_rate, f_rate);
                }
                day_of_ep += 1;
                counter_healthy = population.count('H');
                counter_ill = population.count('I');
                counter_rec = population.count('R');
                counter_died = population.count('D');
                population.display_legend(day_of_ep, counter_healthy, counter_ill, counter_rec, counter_died);
                if (counter_ill == 0)
                {
                    EPIDEMIC = false;
                }
                counter_ill = 0;
                counter_rec = 0;
                counter_died = 0;
                counter_healthy = 0;
            }

            //Part 4: Repeat last saved simulation
        REPEAT:
            cout << "Repeat last saved simulation 'Y', quit 'Q' or start a new one 'N' --> ";
            cin >> command;
            if (command == "y" || command == "Y")
            {
                read_from();
                sleep_for(1000ms);
                system("cls");
                goto REPEAT;
            }
            else if (command == "q" || command == "Q")
            {
                SIMULATION = false;
            }
            else if (command == "n" || command == "N")
            {
                SIMULATION = true;
            }
            else
            {
                cout << "Unknown command. Repeat ..." << endl;
                goto REPEAT;
            }
        } while (SIMULATION);
        return 0;
    }
}