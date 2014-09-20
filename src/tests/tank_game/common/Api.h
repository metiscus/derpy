#ifndef API_H_
#define API_H_

#pragma pack(push, 1)

enum MessageType 
{
	StartSession,
};

struct MessageHeader {
	int type;
	int length;
};

const int MAX_MAP_WIDTH = 1000;

struct MsgStartSession
{
	int width;
	float data[MAX_MAP_WIDTH];
	float startX;
};


struct Message {
	MessageHeader header;
	union {
		MsgStartSession startSession;
	};
};


#pragma pack(pop)

#endif //API_H_