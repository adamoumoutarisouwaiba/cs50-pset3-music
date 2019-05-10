            //Adamou Moutari Souwaiba

#include <cs50.h>
#include <stdio.h>
#include <inttypes.h>

typedef struct note
{
    int frequency;
    int duration;
}
note;


struct song
{
    string filename;
    note **notes;
    size_t capacity;
    size_t size;
    int duration;
};
typedef struct song *song;


bool note_write(song s, int frequency, int duration);


bool rest_write(song s, int duration);


bool song_close(song s);


song song_open(string filename);
