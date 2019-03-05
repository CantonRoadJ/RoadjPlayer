#ifndef UTILS_H
#define UTILS_H

const int REAL = 0;
const int IMAG = 1;

struct wavfile_header {
    char	riff_tag[4];
    int	riff_length;
    char	wave_tag[4];
    char	fmt_tag[4];
    int	fmt_length;
    short	audio_format;
    short	num_channels;
    int	sample_rate;
    int	byte_rate;
    short	block_align;
    short	bits_per_sample;
    char	data_tag[4];
    int	data_length;
};

#endif // UTILS_H
