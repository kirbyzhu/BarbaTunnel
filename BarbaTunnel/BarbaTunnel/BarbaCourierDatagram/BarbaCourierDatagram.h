#pragma once
#include "BarbaBuffer.h"

class BarbaCourierDatagram
{
public:
	struct CreateStrcut
	{
		CreateStrcut() { SessionId=0; MaxPacketSize=0; MessageTimeout=10000;}
		DWORD SessionId;
		size_t MaxPacketSize;
		DWORD MessageTimeout;
	};

private:
	class Message
	{
	public:
		Message(DWORD id, DWORD totalParts);
		Message(DWORD id, BarbaBuffer* data, DWORD maxPartSize);
		Message(DWORD id, BYTE* data, size_t dataSize, DWORD maxPartSize);
		void AddPart(DWORD partIndex, BYTE* data, size_t dataSize);
		bool IsCompleted(); //return true if message contains all parts
		void GetData(BarbaBuffer* data); //merge all parts and return data
		~Message();

		DWORD Id; 
		DWORD LastUpdateTime;
		BarbaArray<BarbaBuffer*> Parts;

	private:
		void Construct(DWORD id, BYTE* data, size_t dataSize, DWORD maxPartSize);
		DWORD AddedPartCount;
	};

public:
	explicit BarbaCourierDatagram(CreateStrcut* cs);
	virtual ~BarbaCourierDatagram(void);

	void SendData(BarbaBuffer* data); //end-user send data with this method
	virtual void ReceiveData(BarbaBuffer* data)=0; //end-user receive data with this method
	virtual void Encrypt(BYTE* data, size_t dataSize, size_t index)=0;
	virtual void Decrypt(BYTE* data, size_t dataSize, size_t index)=0;
	CreateStrcut* GetCreateStruct() {return _CreateStruct;}

protected:
	DWORD GetNewMessageId();
	void SendChunkToInbound(BarbaBuffer* data); //Subclasser should called it when got new chunk
	virtual void SendChunkToOutbound(BarbaBuffer* chunk)=0; //Subclasser should send chunk

private:
	CreateStrcut* _CreateStruct;
	DWORD LastMessageId;
	BarbaArray<Message*> Messages;
	void RemoveMessage(int messageIndex);
	void RemoveTimeoutMessages();
	DWORD LastCleanTimeoutMessagesTime;
};

