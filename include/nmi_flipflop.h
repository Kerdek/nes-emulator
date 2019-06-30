#pragma once

namespace nes
{
	class nmi_flipflop
	{
		bool flipflop;

		nmi_flipflop(nmi_flipflop const &) = delete;
		nmi_flipflop(nmi_flipflop &&) = delete;
		nmi_flipflop & operator=(nmi_flipflop const &) = delete;
		nmi_flipflop & operator=(nmi_flipflop &&) = delete;

	public:
		nmi_flipflop();

		bool get() const;
		void set();
		void clear();
	};
}
