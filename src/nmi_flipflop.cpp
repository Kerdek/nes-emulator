#include "nmi_flipflop.h"

namespace nes
{
	nmi_flipflop::nmi_flipflop() :
		flipflop{ true }
	{}
	bool nmi_flipflop::get() const
	{
		return flipflop;
	}
	void nmi_flipflop::set()
	{
		flipflop = true;
	}
	void nmi_flipflop::clear()
	{
		flipflop = false;
	}
}
