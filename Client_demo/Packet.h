#ifndef PACKET_H
#define PACKET_H

#define TYPE_REG 1
#define TYPE_LOG 2
#define TYPE_MSG 3
#define TYPE_REG_ERROR 4
#define TYPE_REG_SUCCESS 5
#define TYPE_LOG_ERROR 6
#define TYPE_LOG_SUCCESS 7
#define TYPE_PORT 8
#define TYPE_PORTS 9
#define TYPE_CREATEROOM 10

typedef struct packet
{
    int type;
    char name[21];
    char passwd[11];
    char question[51];
    char answer[51];
    char message[101];
    char info[101];//携带执行结果
    unsigned short int port;
    //UDP 包的大小就应该是 1492 - IP头(20) - UDP头(8) = 1464(BYTES)
    //TCP 包的大小就应该是 1492 - IP头(20) - TCP头(20) = 1452(BYTES)
}Pack;

#endif // PACKET_H
