#pragma once

namespace MyEngine
{
namespace NewUi
{
class Elem;

struct EmptyChildOptions
{
	EmptyChildOptions() = default;
	EmptyChildOptions(Elem* pChild);

	Elem* pChild;
};
}
}
