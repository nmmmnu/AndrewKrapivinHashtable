#ifndef FUNNEL_HASH_H_
#define FUNNEL_HASH_H_

#include <array>
#include <vector>
#include <iostream>

#include "murmur_hash_64a.h"

// Please provide:
// - uint64_t funnelHash(K s, uint64_t seed);
// -  bool funnelHashEmpty(K s)

namespace funnel_hash{
	uint64_t hash(std::string_view s, uint64_t seed){
		return murmur_hash64a(s, seed);
	}

	constexpr bool empty(std::string_view s){
		return s.empty();
	}
} // namespace funnel_hash

template<typename K, typename V, size_t N>
struct FunnelHash{
	size_t const TRIES = 3;

	enum class PushResult{
		ERROR		,
		INSERTED	,
		UPDATED
	};

	void clear(){
		storage_.clear();
	}

	template<typename UV>
	PushResult push(K const &k, UV &&v){
		size_t level = 0;

		while(true){
			if (level == storage_.size()){
				// create level
				storage_.push_back({});
			}

			auto &storage = storage_[level];

			for(size_t tries = 0; tries < TRIES; ++tries){
				auto const cell = hash__(k, tries);

				if (storage[cell].empty()){
					// insert here
					storage[cell].key = k;
					storage[cell].val = std::forward<UV>(v);

					return PushResult::INSERTED;
				}

				if (storage[cell].key == k){
					// already inserted, update
					storage[cell].val = std::forward<UV>(v);

					return PushResult::UPDATED;
				}
			}

			++level;
		}
	}

	bool remove(K const &k){
		size_t level = 0;

		while(true){
			if (level == storage_.size()){
				// no such level
				return false;
			}

			auto &storage = storage_[level];

			for(size_t tries = 0; tries < TRIES; ++tries){
				auto const cell = hash__(k, tries);

				if (storage[cell].empty()){
					// not found
					return false;
				}

				if (storage[cell].key == k){
					storage[cell] = {};

					// found
					return true;
				}
			}

			++level;
		}
	}

	template<bool Fast = false>
	auto const &get(K const &k, V const &def) const{
		auto *v = get<Fast>(k);

		return v ? *v : def;
	}

	template<bool Fast = false>
	const V *get(K const &k) const{
		size_t level = 0;

		while(true){
			if (level == storage_.size()){
				// no such level
				return nullptr;
			}

			auto &storage = storage_[level];

			for(size_t tries = 0; tries < TRIES; ++tries){
				auto const cell = hash__(k, tries);

				if constexpr(Fast){
					if (storage[cell].empty()){
						// not found
						return nullptr;
					}
				}

				if (storage[cell].key == k){
					// found
					return & storage[cell].val;
				}
			}

			++level;
		}
	}

	size_t levels() const{
		return storage_.size();
	}

	void print() const{
		for(auto &a : storage_){
			std::cout << "Level: {" << '\n';
			std::cout << '\t';
			for(auto &[k,v] : a){
				if (empty(k))
					std::cout << " {}, ";
				else
					std::cout << " { " << k << " : " << v << " }, ";
			}
			std::cout << '\n' << "}" << '\n';
		}
	}

private:
	static auto hash__(K const &k, size_t seed){
		return funnel_hash::hash(k, seed) % N;
	}

private:
	struct Pair{
		K key;
		V val;

		constexpr bool empty() const{
			return funnel_hash::empty(key);
		}
	};

	using Array = std::array<Pair, N>;

private:
	std::vector<Array> storage_;
};

#endif


