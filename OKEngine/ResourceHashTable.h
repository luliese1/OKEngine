#pragma once

class ResourceHashTable
{
public:
	static size_t GetHashKey(std::wstring str){ return m_Hasher(str); };

private:
	static std::hash<std::wstring> m_Hasher;

	//�ߺ� ���� ���� ������� �ʾҴ�.. ���߿� �����غ���

};

