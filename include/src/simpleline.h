#ifndef _CACHEPP_SIMPLELINE_H
#define _CACHEPP_SIMPLELINE_H

#include "src/globals.h"
#include "src/line.h"

namespace cachepp {
	/**
	 * simple concrete implementation of the Line interface
	 */
	class SimpleLine : public Line<bool> {
		public:
			SimpleLine(identifier id, bool is_corrupt);

		private:
			bool is_corrupt;

			virtual void aux_load();
			virtual void aux_unload();
			virtual bool calculate_checksum();
	};
}

#endif
