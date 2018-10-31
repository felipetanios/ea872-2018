struct NetworkMessage {
	char messageType;
	int objectId;
	int sequence;
	char rendererType;
	float x, y, z;
	float r, g, b;
	float width, height, depth;
};

enum MessageType {
	MessageType_NewObject = 1,
	MessageType_NewPosition = 2,
	MessageType_Destroy = 3,
	MessageType_PlaySound = 4,
};

enum RendererType {
	RendererType_Cube = 0,
	RendererType_Sphere = 1,
};
