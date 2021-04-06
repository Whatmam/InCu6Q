#include <stdio.h>
#include <fcntl.h>
#include <alsa/asoundlib.h>

#include <pthread.h>

#define PCM_DEVICE "default"

#define WAVE_FORMAT_UNKNOWN 0X0000
#define WAVE_FORMAT_PCM 0X0001
#define WAVE_FORMAT_MS_ADPCM 0X0002
#define WAVE_FORMAT_IEEE_FLOAT 0X0003
#define WAVE_FORMAT_ALAW 0X0006
#define WAVE_FORMAT_MULAW 0X0007
#define WAVE_FORMAT_IMA_ADPCM 0X0011
#define WAVE_FORMAT_YAMAHA_ADPCM 0X0016
#define WAVE_FORMAT_GSM 0X0031
#define WAVE_FORMAT_ITU_ADPCM 0X0040
#define WAVE_FORMAT_MPEG 0X0050
#define WAVE_FORMAT_EXTENSIBLE 0XFFFE
typedef struct { 
	unsigned char ChunkID[4]; // Contains the letters "RIFF" in ASCII form 
	unsigned int ChunkSize; // This is the size of the rest of the chunk following this number 
	unsigned char Format[4]; // Contains the letters "WAVE" in ASCII form 
} RIFF;

//------------------------------------------- 
// [Channel] 
// - streo : [left][right] 
// - 3 channel : [left][right][center] 
// - quad : [front left][front right][rear left][reat right] 
// - 4 channel : [left][center][right][surround] 
// - 6 channel : [left center][left][center][right center][right][surround] 
//-------------------------------------------

typedef struct { 
	unsigned char ChunkID[4]; // Contains the letters "fmt " in ASCII form 
	unsigned int ChunkSize; // 16 for PCM. This is the size of the rest of the Subchunk which follows this number. 
	unsigned short AudioFormat; // PCM = 1 
	unsigned short NumChannels; // Mono = 1, Stereo = 2, etc. 
	unsigned int SampleRate; // 8000, 44100, etc. 
	unsigned int AvgByteRate; // SampleRate * NumChannels * BitsPerSample/8 
	unsigned short BlockAlign; // NumChannels * BitsPerSample/8 
	unsigned short BitPerSample; // 8 bits = 8, 16 bits = 16, etc 
} FMT;

typedef struct { 
	char ChunkID[4]; // Contains the letters "data" in ASCII form 
	unsigned int ChunkSize; // NumSamples * NumChannels * BitsPerSample/8 
} DATA;

typedef struct { 
	RIFF Riff; 
	FMT	Fmt; 
	DATA Data; 
} WAVE_HEADER;

int fd;
unsigned int pcm, tmp, dir;
snd_pcm_t *pcm_handle;
snd_pcm_hw_params_t *params;
snd_pcm_uframes_t frames;
char *buff;
int buff_size, loops;
WAVE_HEADER header;
unsigned long low_file_size;



void *sound_thread_function(void * data)
{
	/* Resume information */
	printf("PCM name: '%s'\n", snd_pcm_name(pcm_handle));

	printf("PCM state: %s\n", snd_pcm_state_name(snd_pcm_state(pcm_handle)));

	snd_pcm_hw_params_get_channels(params, &tmp);
	printf("channels: %i ", tmp);

	if (tmp == 1)
		printf("(mono)\n");
	else if (tmp == 2)
		printf("(stereo)\n");

	snd_pcm_hw_params_get_rate(params, &tmp, 0);
	printf("rate: %d bps\n", tmp);
	int seconds = 15;
	printf("seconds: %d\n", seconds);

	/* Allocate buffer to hold single period */
	snd_pcm_hw_params_get_period_size(params, &frames, 0);
	printf("period size = %d frames\n", tmp);

	//buff_size = frames * header.Fmt.NumChannels * 2 /* 2 -> sample size */;
	buff_size = frames * 4; /* 2 bytes/sample, 2 channels */
	buff = (char *) malloc(buff_size);


	snd_pcm_hw_params_get_buffer_time(params,
			&tmp, 0);
	printf("buffer time = %d us\n", tmp);

	snd_pcm_hw_params_get_buffer_size(params,
			(snd_pcm_uframes_t *) &tmp);
	printf("buffer size = %d frames\n", tmp);


	snd_pcm_hw_params_get_period_time(params, &tmp, NULL);

	printf("period time = %d us\n", tmp);

	//for (loops = (seconds * 1000000) / tmp; loops > 0; loops--) {
	
	loops = (seconds * 1000000) / tmp;
	while(loops > 0) {
		loops --;

		pcm = read(fd, buff, buff_size);
		if( pcm == 0) {
			printf("Early end of file.\n");
			break;
		} else if (pcm != buff_size) {
			printf("short read: read %d bytes\n", pcm);
		}

		pcm = snd_pcm_writei(pcm_handle, buff, frames); 
		if (pcm == -EPIPE) {
			/* EPIPE means underrun */
			printf("XRUN.\n");
			snd_pcm_prepare(pcm_handle);
		} else if (pcm < 0) {
			printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(pcm));
		} else if (pcm != (int)frames) {
			printf("short write, write %d frames\n", pcm);
		}

	}

	snd_pcm_drain(pcm_handle);
	snd_pcm_close(pcm_handle);
	free(buff);
}

int main(int argc, char* argv[])
{
	if(argc<2)
	{
		return -1;
	}

	fd = open(argv[1], O_RDONLY);

	if(fd < 0)
	{
		printf("open error : %d!!\n", argv[1]);
		return -1;
	}

	read(fd, &header, sizeof(header));
	header.Fmt.ChunkID[3] = '\0';
	low_file_size = header.Riff.ChunkSize - 36; // Header Size(44 Bytes), RIFF.ChunkSize = WholeSize - 8( RIFF Header ChunckID(4Bytes) + ChunkSize(4Bytes) )
	printf("Sound File : %s\n", argv[1]);
	printf("===========================================\n");
	printf("Chunk ID : %s\n", header.Fmt.ChunkID);
	printf("Chunk Size : %d\n", header.Fmt.ChunkSize);
	printf("Audio Format : %s\n", header.Fmt.AudioFormat == 1 ? "PCM" : "Unknown");
	printf("Audio Channels : %s\n", header.Fmt.NumChannels == 1 ? "Mono" : "Stereo");
	printf("Sample rate : %d\n", header.Fmt.SampleRate);
	printf("Low Data Size : %d\n", low_file_size);
	printf("BitPerSample : %d\n", header.Fmt.BitPerSample);
	printf("Whole Play time : %.2f\n", 
			(double) (low_file_size / (double) (header.Fmt.SampleRate * header.Fmt.NumChannels * header.Fmt.BitPerSample / 8)) );
	printf("===========================================\n");
	/* Open the PCM device in playback mode */
	if (pcm = snd_pcm_open(&pcm_handle, PCM_DEVICE,
				SND_PCM_STREAM_PLAYBACK, 0) < 0)
	{
		printf("ERROR: Can't open \"%s\" PCM device. %s\n",
				PCM_DEVICE, snd_strerror(pcm));
	}

	/* Allocate parameters object and fill it with default values*/
	snd_pcm_hw_params_alloca(&params);

	snd_pcm_hw_params_any(pcm_handle, params);

	/* Set parameters */
	if (pcm = snd_pcm_hw_params_set_access(pcm_handle, params,
				SND_PCM_ACCESS_RW_INTERLEAVED) < 0)
		printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(pcm));

	if (pcm = snd_pcm_hw_params_set_format(pcm_handle, params,
				SND_PCM_FORMAT_S16_LE) < 0)
		printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));

	if (pcm = snd_pcm_hw_params_set_channels(pcm_handle, params, header.Fmt.NumChannels) < 0)
		printf("ERROR: Can't set channels number. %s\n", snd_strerror(pcm));

	if (pcm = snd_pcm_hw_params_set_rate_near(pcm_handle, params, &header.Fmt.SampleRate, 0) < 0)
		printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm));

	/* Write parameters */
	if (pcm = snd_pcm_hw_params(pcm_handle, params) < 0)
		printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(pcm));


	pthread_t pthread;
	int thr_id;
	int status;
	thr_id = pthread_create(&pthread, NULL, sound_thread_function, (void *)"sound_thread");

	if(thr_id < 0)
	{
		printf("thread create error \n"); 
		return -1;
	}


	pthread_join(pthread, (void **)&status);

	return 0;
}
