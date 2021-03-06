#ifndef EA872_DATATYPES
#define EA872_DATATYPES

struct NetworkMessage {
	char messageType;
	int objectId;
	int sequence;
	int score = 0;
	char rendererType;
	float x, y, z;
	float r, g, b;
	float width, height, depth;
};

enum MessageType {
	MessageType_NewObject = 1,
	MessageType_NewPosition = 2,
	MessageType_Score = 3,
	MessageType_Destroy = 4,
	MessageType_PlaySound = 5,
	MessageType_EndOfGame = 6
};

enum RendererType {
	RendererType_Cube = 0,
	RendererType_Sphere = 1,
};

#endif