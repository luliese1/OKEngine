#pragma once

class ResourceHashTable
{
public:
	static size_t GetHashKey(std::wstring str){ return m_Hasher(str); };

private:
	static std::hash<std::wstring> m_Hasher;

	//중복 값에 대해 고려하지 않았다.. 나중에 생각해보자

};

