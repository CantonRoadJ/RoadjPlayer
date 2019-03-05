#include "readcommand.h"
#include "QDebug"
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/file.h>


ReadCommand::ReadCommand()
{
}
ReadCommand::~ReadCommand(){
}
static const struct key_code{
    const char *name;
    unsigned short code;
}
matrix_key_code[] = {
{"KEY_UP", 104},
{"KEY_DOWN", 109},
{"KEY_LEFT", 114},
{"KEY_RIGHT", 115},
{"KEY_PLAY", 207},
};

struct input_event ev;
int fd;
int rd;
//void ReadCommand::run(){
//    readUsbDev();
//    scanInputCommand();

//}
void ReadCommand::startReading(){
    if(openInputDev())
    {
        start();
    }
}
void ReadCommand::run(){
    //readUsbDev();
    scanInputCommand();


}
void ReadCommand::readInputCommand(){
    //qDebug()<<"print";
    // emit nextSongPress();
}
bool ReadCommand::openInputDev(){
    //Open Device
    if((fd = open("/dev/input/event0", O_RDONLY)) == -1)
    {
        qDebug("can not open input dev");
        return false;
    }else{
        qDebug("input dev has opened");
        return true;
    }
}

void ReadCommand::scanInputCommand(){
    int size = sizeof(matrix_key_code)/sizeof(matrix_key_code[0]);
    while(1)
    {

        memset((void *)&ev, 0, sizeof(ev));

        rd = read(fd, (void *)&ev, sizeof(ev));

        if(rd <= 0)
        {
            qDebug("rd: %d\n", rd);
        }

        if(ev.type == 1)
        {
            switch(ev.value)
            {
            case 0:
                //qDebug("Key release.\n");
                for(int i=0; i<size;i++){
                    if(ev.code == matrix_key_code[i].code){
                        emit keyRelease(matrix_key_code[i].code);
                    }
                }
                break;
            case 1:
                //qDebug("Key press.\n");
                for(int i=0; i<size;i++){
                    if(ev.code == matrix_key_code[i].code){
                        emit keyPress(matrix_key_code[i].code);
                    }//
                }
                break;
            case 2:
                // qDebug("Key hold.\n");
                for(int i=0; i<size;i++){
                    if(ev.code == matrix_key_code[i].code){
                        emit keyHold(matrix_key_code[i].code);
                    }
                }
                break;
            default:
                qDebug("Undifined value.\n");
                break;
            }
        }
    }
}
