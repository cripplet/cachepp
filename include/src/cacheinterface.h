#ifndef _CACHEPP_CACHEINTERFACE_H
#define _CACHEPP_CACHEINTERFACE_H

#include <atomic>
#include <map>
#include <memory>

#include "src/globals.h"

namespace cachepp {
	/**
	 * This is the base Cache interface template
	 * 	this is NOT directly callable by the user
	 *	but is meant as a scaffold to quickly build other cache selection schemes
	 *
	 * typename C -- the internal cache type representation
	 * typename D -- the type of the cache class auxiliary data
	 * typename T -- the type of the cache line, subclassed from LineInterface in src/lineinterface.h
	 */
	template <typename C, typename D, typename T>
	class CacheInterface {
		public:
			CacheInterface(identifier size, bool is_thread_safe);

			/**
			 * read / write wrapper for the Line element
			 */
			std::vector<uint8_t> r(const std::shared_ptr<T>& arg, D aux = D());
			void w(const std::shared_ptr<T>& arg, std::vector<uint8_t> val, D aux = D());

			/**
			 * ensures the cache contains T -- that is, T is loaded
			 */
			virtual void acquire(const std::shared_ptr<T>& arg, D aux) = 0;

			/**
			 * mark that the cache line is no longer in active use
			 */
			virtual void release(const std::shared_ptr<T>& arg) = 0;

			/**
			 * clears the cache of all lines which existed at the beginning of the call
			 */
			virtual void clear() = 0;

			/**
			 * explicitly remove the line from the cache -- useful for when deleting the line from memory, etc.
			 */
			virtual void remove(const std::shared_ptr<T>& arg) = 0;

			/**
			 * reset miss rate counters
			 */
			virtual void reset();

			/**
			 * returns n_miss / n_acquire, or returns a RuntimeError if a divide-by-zero occurs
			 */
			double get_miss_rate();

			/**
			 * exposes the cache size to the outside world
			 */
			identifier get_size();

			/**
			 * exposes if the cache is safe for concurrent access
			 */
			bool get_is_thread_safe();

		protected:
			identifier size;
			bool is_thread_safe;

			/**
			 * optional performance tracker objects
			 */
			std::atomic<identifier> n_acquire;
			std::atomic<identifier> n_miss;

			C cache;

			/**
			 * updates cache internal tracker
			 */
			virtual void access(const std::shared_ptr<T>& arg, D aux) = 0;

			/**
			 * test for if the given arg is in the cache
			 */
			virtual bool in(const std::shared_ptr<T>& arg) = 0;

			/**
			 * allocates the input arg within the cache
			 */
			virtual void allocate(const std::shared_ptr<T>& arg) = 0;

			/**
			 * selection policy of the cache -- returns a line from the cache which MUST be evicted
			 */
			virtual std::shared_ptr<T> select() = 0;

			/**
			 * takes in a cache line and returns a recommendation on if the line should be evicted
			 *	return value of 0 indicates the line should NOT be evicted
			 *
			 * by default, not implemented and will need to be overridden
			 */
			virtual size_t heuristic(const std::shared_ptr<T>& arg) = 0;
	};
}

#include "src/templates/cacheinterface.template"

#endif
