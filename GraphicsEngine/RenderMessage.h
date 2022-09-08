#pragma once

class DXGraphicsEngine;
class RenderInfoManager;

class RenderMessage
{ 
protected:
	static std::weak_ptr<DXGraphicsEngine> m_GraphicsEngine;
	static std::weak_ptr<RenderInfoManager> m_RenderInfoManager;

	bool m_usingNow;
public:
	void static SetGraphicsEngine(std::shared_ptr<DXGraphicsEngine> engine) { m_GraphicsEngine = engine; }
	void static SetRenderInfoManager(std::shared_ptr<RenderInfoManager> infoManager) { m_RenderInfoManager = infoManager; }
	void Release();

	virtual void Execute() abstract;
};

class RenderMessage_BeginRender : public RenderMessage
{
public:
	virtual void Execute() override;
public:
	CameraInfo GetCameraInfo() const { return m_cameraInfo; }
	void SetCameraInfo(CameraInfo val) { m_cameraInfo = val; }
private:
	CameraInfo m_cameraInfo;
};

class RenderMessage_EndRender : public RenderMessage
{
public:
	virtual void Execute() override;
};

class RenderMessage_DrawObject : public RenderMessage
{
public:
	virtual void Execute() override;
	virtual void SetObject(size_t ObjecID, void* ConstantBuffer) {m_ObjectGUID = ObjecID; m_ConstantBuffer = ConstantBuffer;};
private:
	size_t m_ObjectGUID;
	void* m_ConstantBuffer;
};

//�굵 �ʿ��Ѱ�..
//DrawObject���� ���ָ� �� �� ������..
class RenderMessage_SetConstantBuffer : public RenderMessage
{
public:
	virtual void Execute() override;
private:

};


//������Ʈ�� ���ӿ������� �����Ǿ��� �� ȣ��
class RenderMessage_AddObject : public RenderMessage
{
public:
	virtual void Execute() override;

	MeshInfo GetObjectInfo() const { return m_ObjectInfo; }
	void SetObjectInfo(size_t ObjectID, MeshInfo val) { 
	m_ObjectGUID = ObjectID;
	m_ObjectInfo = val; }
private:
	size_t m_ObjectGUID;
	MeshInfo m_ObjectInfo;
};
//
////������Ʈ�� ���ӿ������� �����Ǿ��� �� ȣ��
//class RednerMessage_UpdateObject : public RenderMessage
//{
//public:
//	virtual void Execute() override;
//private:
//	MeshInfo m_ObjectInfo;
//};

//������Ʈ�� ���ӿ������� �����Ǿ��� �� ȣ��
class RenderMessage_DeleteObject : public RenderMessage
{
public:
	virtual void Execute() override;
	size_t GetObjectID() const { return m_ObjectID; }
	void SetObjectID(size_t val) { m_ObjectID = val; }
private:
	size_t m_ObjectID;
};

class RenderMessage_BindPass : public RenderMessage
{
public:
	virtual void Execute() override;
private:
};

class RenderMessage_SetPerObjectConstantBuffer : public RenderMessage
{
public:
	virtual void Execute() override;
private:
};

class RenderMessage_SetTexture : public RenderMessage
{
public:
	virtual void Execute() override;
private:
};

//����
class RenderMessage_Execute : public RenderMessage
{
public:
	virtual void Execute() override;
private:
};