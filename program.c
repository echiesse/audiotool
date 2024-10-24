#include<stdio.h>
#include<string.h>

#include<Windows.h>



int main()
{
    HWAVEOUT hwo;
    PDWORD pdwVolume;
    MMRESULT WINAPI waveOutGetVolume(hwo, pdwVolume);
    printf("%d\n",(int) pdwVolume);
    return 0;
}
