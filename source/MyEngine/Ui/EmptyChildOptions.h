#pragma once

namespace MyEngine
{
namespace Ui
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
