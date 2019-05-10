            // Adamou Moutari Souwaiba

// Main Code to get the file of our Pset
#include <cs50.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "wav.h"

#define WAV_SAMPLES_PER_SECOND 44100
#define BEAT_LEN (WAV_SAMPLES_PER_SECOND / 4)
#define DECAY_DURATION BEAT_LEN / 2
#define DECAY_FACTOR -5
#define SILENCE_DURATION 5
#define VOLUME 32000

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_E
#define M_E 2.71828182845904523536
#endif

// Representing a WAV header
struct wav_header
{
    char riff_tag[4];         
    int32_t riff_length;    
    char wave_tag[4];        
    char fmt_tag[4];          
    int32_t fmt_length;       
    int16_t audio_format;     
    int16_t num_channels;     
    int32_t sample_rate;      
    int32_t byte_rate;       
    int16_t block_align;      
    int16_t bits_per_sample;  
    char data_tag[4];       
    int32_t data_length;    
};

static FILE *wav_open(const char *filename)
{
    struct wav_header header;
    int samples_per_second = WAV_SAMPLES_PER_SECOND;
    int bits_per_sample = 16;
    strncpy(header.riff_tag, "RIFF", 4);
    strncpy(header.wave_tag, "WAVE", 4);
    strncpy(header.fmt_tag, "fmt ", 4);
    strncpy(header.data_tag, "data", 4);
    header.riff_length = 0;
    header.fmt_length = 16;
    header.audio_format = 1;
    header.num_channels = 1;
    header.sample_rate = samples_per_second;
    header.byte_rate = samples_per_second * (bits_per_sample / 8);
    header.block_align = bits_per_sample / 8;
    header.bits_per_sample = bits_per_sample;
    header.data_length = 0;
    FILE *file = fopen(filename, "w+");
    if (!file)
    {
        return NULL;
    }
    fwrite(&header, sizeof(header), 1, file);
    fflush(file);
    return file;
}


static bool wav_write(FILE *file, short data[], size_t length)
{
    size_t items = fwrite(data, sizeof(short), length, file);
    return items == length;
}


static bool wav_close(FILE *file)
{
    int file_length = ftell(file);
    int data_length = file_length - sizeof(struct wav_header);
    fseek(file, sizeof(struct wav_header) - sizeof(int), SEEK_SET);
    fwrite(&data_length, sizeof(data_length), 1, file);
    int riff_length = file_length - 8;
    fseek(file, 4, SEEK_SET);
    size_t items = fwrite(&riff_length, sizeof(riff_length), 1, file);
    fclose(file);
    return items == 1;
}

song song_open(string filename)
{
    song s = calloc(1, sizeof(struct song));
    if (!s)
    {
        return NULL;
    }

    s->filename = filename;
    s->capacity = 16;
    s->notes = calloc(s->capacity, sizeof(*s->notes));
    if (!s->notes)
    {
        free(s);
        return NULL;
    }
    s->size = 0;
    return s;
}


bool note_write(song s, int frequency, int duration)
{
    
    if (s->size == s->capacity)
    {
        if (s->capacity > SIZE_MAX / sizeof(*s->notes) / 2)
        {
            return false;
        }
        s->capacity *= 2;
        note **temp = realloc(s->notes, sizeof(*s->notes) * s->capacity);
        if (!temp)
        {
            return false;
        }
        s->notes = temp;
    }

    note *n = calloc(1, sizeof(note));
    if (!n)
    {
        return false;
    }
    n->frequency = frequency;
    n->duration = duration;

    s->notes[s->size] = n;
    s->duration += duration;
    s->size++;

    return true;
}


bool rest_write(song s, int duration)
{
    return note_write(s, 0, duration);
}

bool song_close(song s)
{
    if (!s || !s->size)
    {
        return false;
    }
    FILE *f = wav_open(s->filename);
    if (!f)
    {
        return false;
    }
    short *waveform = calloc(s->duration * BEAT_LEN, sizeof(short));
    if (!waveform)
    {
        return false;
    }
    short *current_sample = waveform;

    for (size_t i = 0; i < s->size; i++)
    {
      
        note *n = s->notes[i];
        double phase = 0.0;
        double phase_step = n->frequency * 2.0 * M_PI / WAV_SAMPLES_PER_SECOND;
        short *note_end = current_sample + n->duration * BEAT_LEN - SILENCE_DURATION;
        short *decay_start = note_end - DECAY_DURATION;
        for (; current_sample != decay_start; phase += phase_step)
        {
            *current_sample++ = round(VOLUME * sin(phase));
        }
        for (; current_sample != note_end; phase += phase_step)
        {
            double t = (double) (current_sample - decay_start) / BEAT_LEN;
            *current_sample++ = round(VOLUME * pow(M_E, t * DECAY_FACTOR) * sin(phase));
        }

        
        current_sample += SILENCE_DURATION;

        free(n);
    }


    bool success = wav_write(f, waveform, s->duration * BEAT_LEN);
    wav_close(f);

    free(s->notes);
    free(s);
    free(waveform);
    return success;
}
