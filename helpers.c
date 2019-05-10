        //Adamou Moutari Souwaiba

    //Pset's Functions
#include <cs50.h>
#include "helpers.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
 
int duration(string fraction)
{
    int numer = atoi(&fraction[0]);
    int denom = atoi(&fraction[2]);
    int frac = (8/denom) * numer;
    return frac;
}
    
int frequency(string note)
{

    char letter;
    char accidental = 0;
    float octave = 0.0;

    if (strlen(note) == 3)
    {
        letter = note[0];
        accidental = note[1];
        octave = atoi(&note[2]);
    }
    if (strlen(note) == 2)
    {
        letter = note[0];
        octave = atoi(&note[1]);
    }
    int octcount = 0;
    float hz = 0.0;
    if (octave == 4)
    {
        hz = 440.00;
    }
       

    else if (octave > 4)
    {
        octcount = (octave - 4);
        hz = 440 * pow(2, octcount);
    }
    else if (octave < 4)
    {
        octcount = 4 - octave;
        hz = 440 / pow(2, octcount);
        
    }

        
    float hertz = 0.0;
    float numerator = 0.0;
    float themath = pow(2.0, (numerator/12));

    switch (letter)
    {

        case 'C':

        {
            float power = pow(2, (9.0/12));
            hertz = hz / power;
            break;
        }
        case 'D':
       
        {
            float power = pow(2, (7.0/12));
            hertz = hz / power;
            break;
        }
        case 'E':
        
        {
            float power = pow(2, (5.0/12));
            hertz = hz / power;
            break;
        }
        case 'F':
        
        {
            float power = pow(2, (4.0/12));
            hertz = hz / power;
            break;
        }
        case 'G':
        
        {
            float power = pow(2, (2.0/12));
            hertz = hz / power;
            break;
        }
        case 'A':
        
            {
                numerator = 12.0;
                hertz = hz * themath;
                break;
            }
        case 'B':
       
        {
            float power = pow(2, (2.0/12));
            hertz = hz * power;
            break;
        }
    }
    switch (accidental)
    {
        case '#':
        
            {
                float power = pow(2, (1.0/12));
                hertz = hertz * power;
                 printf("A octcount after = %.2d, hertzz = %.2f\n", octcount, hertz);
                 printf("A case A hertz = %.2f\n", hz);
                break;
            }
        case 'b':
        
        {
            float power = pow(2, (1.0/12));
            hertz = (hertz / power);
            printf("B octcount after = %.2d, hertz = %.2f\n", octcount, hertz);
            break;
        }
        default:
        
        {
            break;
        }
    }
    int finalhertz = round(hertz);
    return finalhertz;
}


bool is_rest(string s)
{
    if  (strcmp(s, "") == 0)
    
    {
        return true;
    }
    else
    {
        return false;
    }
}