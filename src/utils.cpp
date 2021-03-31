#include <iostream>
#include <cstring>

#define MSGLEN strlen(newMsg->message)+1
#define PACKETSIZE sizeof(MSG)+MSGLEN

using namespace std;

typedef struct MSG
{
    int type;
    int priority;
    int sender;
    int size;
    char *message;
}MSG;

void serialize(MSG* msgPacket, char *data);
void deserialize(char *data, MSG* msgPacket);
void echoMsg(MSG* msgPacket);

int main(int argc, char * argv[])
{
    // serialize
    MSG* newMsg = new MSG;
    newMsg->type = 1;
    newMsg->priority = 10;
    newMsg->sender = 2;
    newMsg->message = (char *)malloc(strlen(argv[1]));
    strcpy(newMsg->message, argv[1]);
    int len = MSGLEN;
    newMsg->size = len;
    echoMsg(newMsg);

    // deserialize
    char data[PACKETSIZE];
    serialize(newMsg, data);
    //DEBUG
    /*
    printf("data:\n");
    for(int i=0; i< PACKETSIZE;i++){
        printf("0x%02x ", data[i]);
    }
    //*/
    MSG* temp = new MSG;
    deserialize(data, temp);
    echoMsg(temp);

    return 0;
}

void serialize(MSG* msgPacket, char *data)
{
    int *q = (int*)data;
    *q = msgPacket->type;       q++;
    *q = msgPacket->priority;   q++;
    *q = msgPacket->sender;     q++;
    *q = msgPacket->size;       q++;

    char *p = (char*)q;
    int i = 0;
    while (i < msgPacket->size)
    {
        *p = msgPacket->message[i];
        p++;
        i++;
    }
}

void deserialize(char *data, MSG* msgPacket)
{
    int *q = (int*)data;
    msgPacket->type = *q;       q++;
    msgPacket->priority = *q;   q++;
    msgPacket->sender = *q;     q++;
    msgPacket->size = *q;       q++;

    char *p = (char*)q;
    int i = 0;
    msgPacket->message = (char *)malloc(msgPacket->size);
    while (i < msgPacket->size)
    {
        msgPacket->message[i] = p[i];
        i++;
    }
}

void echoMsg(MSG* msgPacket)
{
    cout << msgPacket->type     << endl;
    cout << msgPacket->priority << endl;
    cout << msgPacket->sender   << endl;
    cout << msgPacket->size     << endl;
    cout << msgPacket->message  << endl;
    cout << "-----------------" << endl;
}
