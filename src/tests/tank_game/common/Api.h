#ifndef API_H_
#define API_H_

#pragma pack(push, 1)


enum MessageType 
{
	HeresMap,
	SetAction,
	DoRound,
};

struct MessageHeader {
	int type;
	int length;
};

struct HeresMapMsg
{
	MessageHeader hdr;
	int width;
	int player_1_pos;
	int player_2_pos;
	int youare;
};

struct SetActionMsg {
	MessageHeader hdr;
	float angle;
	float power;
};

struct DoRoundMsg {
	MessageHeader hdr;
	float player_1_angle;
	float player_1_power;
	float player_2_angle;
	float player_2_power;
};

const int message_max_size = 1024 * 10;

inline HeresMapMsg* build_HeresMapMsg( int width, int player_1_pos, int player_2_pos, char* data, int dataSize )
{
	HeresMapMsg *msg = (HeresMapMsg*)malloc(sizeof(HeresMapMsg) + dataSize);
	if(msg)
	{
		msg->hdr.type = HeresMap;
		msg->hdr.length = sizeof(HeresMapMsg) + dataSize;
		msg->width = width;
		msg->player_1_pos = player_1_pos;
		msg->player_2_pos = player_2_pos;

		char* dPtr = ((char*)msg) + sizeof(HeresMapMsg);
		memcpy(dPtr, data, dataSize);

		return msg;
	}

	return NULL;
}

#pragma pack(pop)

#endif //API_H_