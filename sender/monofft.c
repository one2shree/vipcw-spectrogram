/*

    Convert a Stereo file into a Mono wave file by concatenating the left channel data together.
    Passes determined values from wav file into FFT function.
    Normalizes values based on the range, outputted values are from 0-1.
    Writes output to text file for use with the spectrogram display code.

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>    
#include "fft.h"
#include "fft_sender.h"


void genfft(CircularBuffer *cb, int N)
{
    printf("IN: monofft:genFFT(): sizeof(cb->fbuffer)=%d\n", sizeof(cb->fbuffer));

    /* Check size of buffer */
    if(sizeof(cb->fbuffer)/sizeof(float) != N)
        err("cb->fbuffer does not contain enough samples");

    int i=0,j=0,k=0,l=0;
    //int wavbuffer[N];
    double (*X)[2];                  /* pointer to frequency-domain samples */   
    double x[N][2];             
    double mag, min, max, mag_norm;                 
    X = malloc(2 * N * sizeof(double));  
    while (i<N) {
        x[i][1]=0;
        i++;
    }

<<<<<<< HEAD
    /* Execute arecord to start recording from the make with the separate-channels option */
    // char syscall[512];
    // sprintf(syscall, "arecord -I -f S16_LE -r44100 -D plughw:CARD=WEBCAM,DEV=0  | sender %s", serverIP);
    // system(syscall);
    
    //FILE * f = fopen("wavePipe", "r"); //opening the 2 channels wave file
    //if(!f) printf("Error reading from wavePipe");
=======
    mkfifo("wavePipe", S_IRWXO);
    system("arecord -d1 -r44100  -D plughw:CARD=Intel,DEV=0  wavePipe");
    // system("arecord -d1 -r4000 -D sysdefault:CARD=U0x46d0x80a  wavePipe");
    int lSize;
    FILE * f = fopen("wavePipe", "r"); //opening the 2 channels wave file
	//usleep(10000);
    //FILE * f = fopen("chirp10-500Hz.wav", "r"); //opening the 2 channels wave file
    if(!f) printf("Error reading from wavePipe");
>>>>>>> 140f706699ef0a828e155e200a2713712849775e

    // FILE * f2 = fopen ("chirpFFT.txt", "w"); // a text file that will have the left channel sound data,
    //just to see what we're dealing with
    //fseek (f , 0 , SEEK_END);
    int lSize = sizeof(float)*N; //position of the current stream (It's at the end because of fseek())
    //rewind (f);
	
    typedef struct wave {
        int chunkID;
        int chunkSize;
        int format;
        int subChunk1ID;
        int subChunk1size;
        int audio_num_ch;
        int sampleRate;
        int byteRate;
        int bckAli_bitPs;
        int subChunk2ID;
        int subChunk2size;
        int  data[(lSize-40)/4];
    } wave;
    wave * mywave;
    mywave=(wave *) malloc( lSize);

	memcpy(mywave, cb->fbuffer, lSize);	
	
    //fread(mywave,1,lSize,f);
    // printf("size ............. %d\n",lSize);
    // printf("size ......double....... %lu\n",(sizeof(double)*128));
    // printf("size ......double....... %lu\n",(sizeof(float)*128));

    int mask=65535;
    int  leftVa;
	while (i<N) {    
	//while (i<((lSize-40)/4)) {

        leftVa=(mywave->data[i+1]<<16) | (mywave->data[i] & mask);
        mywave->data[j]=leftVa;
		// x[k][0] = cb->fbuffer[i];
        /* LEFT CHANNEL DATA GOES HERE */
        x[k][0]=leftVa;
        //wavbuffer[k] = leftVa;
        j++;
        k++;
        //~ printf("k is %d and N is %d\n", k, N);
        if (k==N) {
            fft(N, x, X);
            min = 0;
            max = sqrt((X[0][0]*X[0][0])+(X[0][1]*X[0][1]));
            for(l=0; l<N; l++) {
                mag=sqrt((X[l][0]*X[l][0])+(X[l][1]*X[l][1]));
                if(mag > max) {
                    max = mag;
                }
                if(mag < min) {
                    min = mag;
                }
            }


            for(l=0; l<N; l++) {
                mag=sqrt((X[l][0]*X[l][0])+(X[l][1]*X[l][1]));

                mag_norm = (mag - min)/(max - min);
                cb->fbuffer[l] = mag_norm;

            }
            k=0;
        }
        i=i+2;
    }


    
   // for(i = 0; i < N; i++)
      //  printf("wavbuffer[%d] is %12d, cb->fbuffer[%d] is %12f\n", i, wavbuffer[i], i, cb->fbuffer[i]);


   // fwrite(mywave,1,(lSize/2)+22,f1);
    //fclose (f);
    // fclose (f2);
    free(X);
    return;
}

/*
int main(){
    float f[256];
    genfft(f, 256);
    int i;
    for(i = 0; i < 256; i++) 
        printf("f[%d] is %12f\n", i, f[i]);
}
*/
