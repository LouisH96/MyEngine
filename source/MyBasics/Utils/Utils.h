#pragma once

namespace MyEngine
{
class Utils
{
public:
	template<typename T>
	static void Delete(T*& p) { delete p;  p = nullptr; }
};
}