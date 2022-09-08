// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일

typedef unsigned __int64 ObjectGUID;

#include <windows.h>

#include <vector>
#include <map>
#include <queue>

#include <functional>
#include <unordered_map>

#include <string>

// 여기에 미리 컴파일하려는 헤더 추가
#include <memory>

//weakptr이 빈녀석인가
//expire보다 빠르다고함! https://stackoverflow.com/questions/45507041/how-to-check-if-weak-ptr-is-empty-non-assigned

template <typename T>
bool is_uninitialized(std::weak_ptr<T> const& weak) {
	using wt = std::weak_ptr<T>;
	return !weak.owner_before(wt{}) && !wt{}.owner_before(weak);
}

//나중에 커스텀 math library를 만들자.. 일단은 simple math 사용
#include <DirectXMath.h>
#include <SimpleMath.h>
#include <SimpleMath.inl>

using namespace DirectX::SimpleMath;

#include "AlignedAllocationPolicy.h"
#include "dllRenderStruct.h"

inline float DegToRad(float deg)
{
	float rad;
	rad = (3.141592f * deg) / 180.f;

	return rad;
}

#include "codecvt";

inline std::wstring s2ws(const std::string& txt)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(txt);
}
inline std::string ws2s(const std::wstring& txt)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(txt);
}


#include "DataStruct.h"

#endif //PCH_H
