#include <Scene\LAYER.h>
#include <type_traits>


LAYER begin(LAYER)
{
	return LAYER::BG;
}

LAYER end(LAYER)
{
	return LAYER::MAX;
}

LAYER operator*(LAYER layer)
{
	return layer;
}

LAYER operator++(LAYER & layer)
{
	return layer = static_cast<LAYER>(std::underlying_type<LAYER>::type(layer) + 1);
}
