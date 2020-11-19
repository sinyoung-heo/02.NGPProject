#pragma once
class CPacketMgr
{
	DECLARE_SINGLETON(CPacketMgr)

public:
	CPacketMgr() = default;
	~CPacketMgr() = default;

public:
	// Receive
	int		RecvPacketFromServer();
	void	ProcessData(char* net_buf, size_t io_byte);
	void	ProcessPacket(char* ptr);

	// Send
	void	SendPlayerMovePacket(unsigned char dir);
	void	SendPlayerSceneID(unsigned char scene_id);
	void	SendPlayerStance(unsigned char stance, unsigned char dir);

private:
	void	SendPacketToServer(void* packet);

};

