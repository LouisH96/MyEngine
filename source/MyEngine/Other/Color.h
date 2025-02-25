#pragma once
#include <Math\Vectors.h>

namespace MyEngine
{
namespace Color
{
static const Float3 Red{ 1,0,0 };
static const Float3 Green{ 0,1,0 };
static const Float3 Blue{ 0,0,1 };

static const Float3 Yellow{ 1,1,0 };

namespace Chill
{
static constexpr float High{ .65f };
static constexpr float Low{ .25f };

static const Float3 Red{ High, Low, Low };
static const Float3 Green{ Low, High, Low };
static const Float3 Blue{ Low, Low, High };
static const Float3 Yellow{ High, High, Low };

} //end namespace Chill

}
}

