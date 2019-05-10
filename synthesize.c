                AMADOU ARIFA Arifa Mamane

// Prompting for a sequence of notes
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include "helpers.h"
#include "wav.h"

int main(int argc, string argv[])
{
    // Checking command line arguments
    if (argc != 2)
    {
        fprintf(stderr, "You must use it like: (Your file name after put:FILE\n");
        return 1;
    }
    string filename = argv[1];

    // Open file for writing
    song s = song_open(filename);

    // Expecting notes from user until the end of file
    while (true)
    {
        // Expect note
        string line = get_string("Enter your line: ");

        // Checking whether our line is 'NULL'
        if (line == NULL)
        {
            break;
        }

        // Checking if line is rest
        if (is_rest(line))
        {
            rest_write(s, 1);
        }
        else
        {
            // Parsing line into note and duration
            string note = strtok(line, "@");
            string fraction = strtok(NULL, "@");

            // Writing note to song
            note_write(s, frequency(note), duration(fraction));
        }
    }

    // Now, closing file
    song_close(s);
}
